package GNUACEProjectCreator;

# ************************************************************
# Description   : A GNU Project Creator for ACE
# Author        : Chad Elliott
# Create Date   : 3/13/2002
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;
use File::Basename;

use ProjectCreator;

use vars qw(@ISA);
@ISA = qw(ProjectCreator);

# ************************************************************
# Data Section
# ************************************************************

my(%compscript) = ('ACE_COMPONENTS'     => ['--ace',     '--set'],
                   'TAO_COMPONENTS'     => ['--tao',     '--set'],
                   'ORBSVCS_COMPONENTS' => ['--orbsvcs', '--append'],
                  );

# ************************************************************
# Subroutine Section
# ************************************************************

sub sort_files {
  #my($self) = shift;
  return 0;
}


sub convert_slashes {
  #my($self) = shift;
  return 0;
}


sub fill_value {
  my($self)  = shift;
  my($name)  = shift;
  my($value) = undef;
  my($crlf)  = $self->crlf();
  my($tag)   = 'source_files';
  my($names) = $self->{$tag};

  if ($name eq 'gnu_source_files') {
    my(%vpath) = ();
    $value = '';
    foreach my $name (keys %$names) {
      my($comps) = $$names{$name};

      foreach my $key (sort keys %$comps) {
        my($a)   = $$comps{$key};
        my(@arr) = @$a;
        my($cpy) = $key;

        $cpy =~ s/^\d+_//;
        $value .= "$crlf$crlf$cpy = \\";
        for(my $i = 0; $i <= $#arr; $i++) {
          my($item) = $arr[$i];
          my($dname) = dirname($item);

          $item =~ s/\.[^\.]+$//;
          if ($dname ne '.' && $dname !~ /^\.\.\//) {
            $vpath{$dname} = 1;
          }
          $value .= "$crlf  $item" . ($i != $#arr ? " \\" : '');
        }
      }
    }
    foreach my $name (keys %$names) {
      my($fname) = '';
      my($comps) = $$names{$name};
      foreach my $key (sort keys %$comps) {
        $fname = $key;
        $fname =~ s/^\d+_//;
        last;
      }

      if ($name ne 'default') {
        $fname = 'FILES';
        $value .= "$crlf$crlf" . "ifndef $name$crlf" .
                  "  $name = \\$crlf";
        my(@keys) = sort keys %$comps;
        for(my $i = 0; $i <= $#keys; $i++) {
          $keys[$i] =~ s/^\d+_//;
          my($key) = $keys[$i];
          $value .= "    $key" . ($i != $#keys ? " \\" : '') . $crlf;
        }
        $value .= "endif # $name";

        foreach my $key (@keys) {
          $value .= "$crlf$crlf" . "ifneq (,\$(findstring $key, \$($name)))$crlf" .
                    "  $fname += \$($key)$crlf" .
                    "endif # $key";
        }
      }
    }
    my(@vkeys) = sort keys %vpath;
    if ($#vkeys >= 0) {
      $value .= "$crlf$crlf" . 'VPATH = .';
      foreach my $key (@vkeys) {
        $value .= ":$key";
      }
    }
  }
  elsif ($name eq 'build') {
    foreach my $name (keys %$names) {
      if ($name ne 'default') {
        if (!defined $value) {
          $value = 'BUILD +=';
        }
        $value .= " $name";
      }
    }
  }
  elsif ($name eq 'comptarget') {
    foreach my $name (keys %$names) {
      if (defined $compscript{$name}) {
        if (!defined $value) {
          $value = '';
        }
        $value .= "$crlf.PHONY: $name$crlf" .
                  "$name:$crlf" .
                  "\t\@sh \$(ACE_ROOT)/bin/ace_components $compscript{$name}->[0] $compscript{$name}->[1] '\$($name)'$crlf$crlf" .
                  "compclean:$crlf" .
                  "\t\@sh \$(ACE_ROOT)/bin/ace_components $compscript{$name}->[0] --remove";
      }
    }
  }
  elsif ($name eq 'compclean') {
    foreach my $name (keys %$names) {
      if (defined $compscript{$name}) {
        $value = 'compclean';
        last;
      }
    }
  }
  elsif ($name eq 'notdirfiles') {
    $value = '$(notdir $(FILES))';
    foreach my $name (keys %$names) {
      my($comps) = $$names{$name};
      foreach my $key (keys %$comps) {
        my($arr) = $$comps{$key};
        foreach my $file (@$arr) {
          if ($file =~ /^\.\.\//) {
            $value = '$(FILES)';
            last;
          }
        }
      }
    }
  }
  elsif ($name eq 'tao') {
    my($incs) = $self->get_assignment('includes');
    my($libs) = $self->get_assignment('libpaths');
    if ((defined $incs && $incs =~ /tao/i) ||
        (defined $libs && $libs =~ /tao/i)) {
      $value = 1;
    }
  }

  return $value;
}


sub project_file_name {
  my($self) = shift;
  my($name) = shift;

  if (!defined $name) {
    $name = $self->project_name();
  }

  return 'Makefile' . ($name eq '' ? '' : ".$name");
}


sub get_dll_exe_template_input_file {
  #my($self) = shift;
  return 'gnuexe';
}


sub get_dll_template_input_file {
  #my($self) = shift;
  return 'gnudll';
}


sub get_template {
  #my($self) = shift;
  return 'gnu';
}

1;
