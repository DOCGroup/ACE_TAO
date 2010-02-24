package SLEProjectCreator;

# ************************************************************
# Description   : The SLE Project Creator
# Author        : Johnny Willemsen
# Create Date   : 3/23/2003
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use ProjectCreator;
use XMLProjectBase;

use vars qw(@ISA);
@ISA = qw(XMLProjectBase ProjectCreator);

# ************************************************************
# Subroutine Section
# ************************************************************

sub project_file_extension {
  #my $self = shift;
  return '.vpj';
}


sub get_dll_exe_template_input_file {
  #my $self = shift;
  return 'sleexe';
}


sub get_dll_template_input_file {
  #my $self = shift;
  return 'sledll';
}


1;
