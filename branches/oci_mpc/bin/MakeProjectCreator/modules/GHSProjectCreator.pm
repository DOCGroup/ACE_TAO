package GHSProjectCreator;

# ************************************************************
# Description   : Not a complete implementation for GHS
# Author        : Chad Elliott
# Create Date   : 4/19/2002
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

sub project_file_name {
  my($self) = shift;
  return $self->transform_file_name("ghs/" .
                                    $self->project_name() . ".bld");
}


sub get_dll_exe_template_input_file {
  my($self) = shift;
  return "ghsexe";  
}
 
 
sub get_dll_template_input_file {
  my($self) = shift;
  return "ghsdll";  
}
 
 
sub get_template {
  my($self) = shift;
  return "ghs";
}


1;
