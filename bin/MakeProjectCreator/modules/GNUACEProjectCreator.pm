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
  my($dcomp) = $self->get_default_component_name();

  if ($name eq 'vpath') {
    my(%vpath) = ();
    foreach my $name (keys %$names) {
      my($comps) = $$names{$name};
      foreach my $key (sort keys %$comps) {
        foreach my $item (@{$$comps{$key}}) {
          my($dname) = dirname($item);
          if ($dname ne '.' && $dname !~ /^\.\.\//) {
            $vpath{$dname} = 1;
          }
        }
      }
    }
    my($str) = join(':', keys %vpath);
    if ($str ne '') {
      $value = 'VPATH = .:' . $str . $crlf;
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
