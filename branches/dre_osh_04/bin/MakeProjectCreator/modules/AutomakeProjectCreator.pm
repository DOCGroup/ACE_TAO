package AutomakeProjectCreator;

# ************************************************************
# Description   : A Automake Project Creator
# Author        : Chad Elliott
# Create Date   : 2/26/2003
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

  if ($name eq 'vpath') {
    my(%vpath) = ();
    my($names) = $self->{'source_files'};
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
      $value = 'VPATH = .:' . $str . $self->crlf();
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

  return 'Makefile' . ($name eq '' ? '' : ".$name") . '.am';
}


sub get_dll_exe_template_input_file {
  #my($self) = shift;
  return 'automakeexe';
}


sub get_dll_template_input_file {
  #my($self) = shift;
  return 'automakedll';
}


sub get_template {
  #my($self) = shift;
  return 'automake';
}


1;
