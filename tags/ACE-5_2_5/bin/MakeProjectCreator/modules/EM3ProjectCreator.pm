package EM3ProjectCreator;

# ************************************************************
# Description   : An eMbedded Visual C++ 3.00 Project Creator
# Author        : Chad Elliott
# Create Date   : 7/3/2002
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use VC6ProjectCreator;

use vars qw(@ISA);
@ISA = qw(VC6ProjectCreator);

# ************************************************************
# Subroutine Section
# ************************************************************

sub fill_value {
  my($self)  = shift;
  my($name)  = shift;
  my($value) = undef;

  if ($name eq "make_file_name") {
    $value = $self->base_project_name() . ".vcn";
  }

  return $value;
}


sub project_file_name {
  my($self) = shift;
  return $self->project_name() . ".vcp";
}


sub static_project_file_name {
  my($self) = shift;
  return $self->project_name() . $self->get_static_append() . ".vcp";
}


sub get_dll_exe_template_input_file {
  my($self) = shift;
  return "em3vcpdllexe";
}


sub get_lib_exe_template_input_file {
  my($self) = shift;
  return "em3vcplibexe";
}


sub get_lib_template_input_file {
  my($self) = shift;
  return "em3vcplib";
}


sub get_dll_template_input_file {
  my($self) = shift;
  return "em3vcpdll";
}


sub get_template {
  my($self) = shift;
  return "em3vcp";
}


1;
