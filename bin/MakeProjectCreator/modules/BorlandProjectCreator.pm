package BorlandProjectCreator;

# ************************************************************
# Description   : A Borland Project Creator
# Author        : Chad Elliott
# Create Date   : 3/14/2002
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use ProjectCreator;
use File::Basename;

use vars qw(@ISA);
@ISA = qw(ProjectCreator);

# ************************************************************
# Subroutine Section
# ************************************************************

sub fill_value {
  my($self)  = shift;
  my($name)  = shift;
  my($value) = undef;

  if ($name eq 'cppdir') {
    my(%dirnames) = ();
    foreach my $file ($self->get_component_list('source_files')) {
      my($dirname) = dirname($file);
      if ($dirname eq '') {
        $dirname = '.';
      }
      $dirnames{$dirname} = 1;
    }
    $value = join(';', sort keys %dirnames);
  }

  return $value;
}


sub project_file_name {
  my($self) = shift;
  my($name) = shift;

  if (!defined $name) {
    $name = $self->project_name();
  }

  return 'Makefile' . ($name eq '' ? '' : ".$name") . '.bor';
}


sub get_dll_exe_template_input_file {
  #my($self) = shift;
  return 'borexe';
}


sub get_dll_template_input_file {
  #my($self) = shift;
  return 'bordll';
}


sub get_template {
  #my($self) = shift;
  return 'bor';
}


1;
