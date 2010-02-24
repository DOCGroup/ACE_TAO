package BDS4ProjectCreator;

# ************************************************************
# Description   : The Borland Developer Studio 4 Project Creator
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
  return '.bdsproj';
}


sub get_dll_exe_template_input_file {
  #my $self = shift;
  return 'bds4exe';
}


sub get_dll_template_input_file {
  #my $self = shift;
  return 'bds4dll';
}


1;
