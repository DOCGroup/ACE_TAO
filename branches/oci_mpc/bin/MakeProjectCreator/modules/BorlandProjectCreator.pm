package BorlandProjectCreator;

# ************************************************************
# Description   : Not a complete implementation for Borland
# Author        : Chad Elliott
# Create Date   : 3/14/2002
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
  my($self) = shift;
  return 1;
}


sub file_sorter {
  my($self)  = shift;
  my($left)  = shift;
  my($right) = shift;
  return lc($left) cmp lc($right);
}


sub project_file_name {
  my($self) = shift;
  return $self->transform_file_name(
           "Makefile" . ($self->project_name() ne "" ? "." : "") .
           $self->project_name() . ".bor");
}


sub get_exe_template_input_file {
  my($self) = shift;
  return "borexe";
}
 
 
sub get_dll_template_input_file {
  my($self) = shift;
  return "bordll";
}
 
 
sub get_template {
  my($self) = shift;
  return "bor";  
}


1;
