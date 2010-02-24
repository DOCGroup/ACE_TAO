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

sub project_file_extension {
  #my $self = shift;
  return '.vcp';
}


sub get_dll_exe_template_input_file {
  #my $self = shift;
  return 'em3vcpdllexe';
}


sub get_lib_exe_template_input_file {
  #my $self = shift;
  return 'em3vcplibexe';
}


sub get_lib_template_input_file {
  #my $self = shift;
  return 'em3vcplib';
}


sub get_dll_template_input_file {
  #my $self = shift;
  return 'em3vcpdll';
}


1;
