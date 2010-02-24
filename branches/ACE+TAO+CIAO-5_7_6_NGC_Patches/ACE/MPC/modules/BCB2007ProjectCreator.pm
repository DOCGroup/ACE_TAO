package BCB2007ProjectCreator;

# ************************************************************
# Description   : The Borland C++ Builder 2007 Project Creator
# Author        : Johnny Willemsen
# Create Date   : 14/12/2005
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use ProjectCreator;
use BorlandProjectBase;
use XMLProjectBase;

use vars qw(@ISA);
@ISA = qw(XMLProjectBase BorlandProjectBase ProjectCreator);

# ************************************************************
# Subroutine Section
# ************************************************************

sub crlf {
  #my $self = shift;
  return "\n";
}


sub project_file_extension {
  #my $self = shift;
  return '.cbproj';
}


sub get_dll_exe_template_input_file {
  #my $self = shift;
  return 'bcb2007exe';
}


sub get_dll_template_input_file {
  #my $self = shift;
  return 'bcb2007dll';
}


1;
