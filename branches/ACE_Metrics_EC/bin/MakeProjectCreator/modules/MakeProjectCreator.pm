package MakeProjectCreator;

# ************************************************************
# Description   : A Generic Make Project Creator
# Author        : Chad Elliott
# Create Date   : 2/18/2003
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

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
  return 'makeexe';
}


sub get_dll_template_input_file {
  #my($self) = shift;
  return 'makedll';
}


sub get_template {
  #my($self) = shift;
  return 'make';
}

1;
