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
# Subroutine Section
# ************************************************************

sub dollar_special {
  #my($self) = shift;
  return 1;
}


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
  my($names) = $self->{'source_files'};

  if ($name eq 'vpath') {
    my(%vpath) = ();
    foreach my $name (keys %$names) {
      my($comps) = $$names{$name};
      foreach my $key (keys %$comps) {
        foreach my $item (@{$$comps{$key}}) {
          my($dname) = $self->relative(dirname($item));
          if ($dname ne '.' && $dname !~ /^\.\.\//) {
            $vpath{$dname} = 1;
          }
        }
      }
    }
    my($str) = join(':', sort keys %vpath);
    if ($str ne '') {
      $value = 'VPATH = .:' . $str . $self->crlf();
    }
  }
  elsif ($name eq 'tao') {
    my($incs) = $self->get_assignment('includes');
    my($libs) = $self->get_assignment('libpaths');
    $value = ((defined $incs && $incs =~ /tao/i) ||
              (defined $libs && $libs =~ /tao/i));
  }

  return $value;
}


sub project_file_name {
  my($self) = shift;
  my($name) = shift;

  if (!defined $name) {
    $name = $self->project_name();
  }

  return $self->get_modified_project_file_name('GNUmakefile' .
                                               ($name eq '' ? '' : ".$name"),
                                               '');
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
