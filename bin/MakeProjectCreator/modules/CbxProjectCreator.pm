package CbxProjectCreator;

# ************************************************************
# Description   : The Borland C++ BuilderX Project Creator
# Author        : Johnny Willemsen
# Create Date   : 10/12/2003
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

sub project_file_name {
  my($self) = shift;
  my($name) = shift;

  if (!defined $name) {
    $name = $self->project_name();
  }

  return $self->get_modified_project_file_name($name, '.cbx');
}


sub get_dll_exe_template_input_file {
  #my($self) = shift;
  return 'cbxexe';
}


sub get_dll_template_input_file {
  #my($self) = shift;
  return 'cbxdll';
}


sub get_template {
  #my($self) = shift;
  return 'cbx';
}


1;
