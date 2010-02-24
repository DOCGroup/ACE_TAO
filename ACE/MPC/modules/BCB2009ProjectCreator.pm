package BCB2009ProjectCreator;

# ************************************************************
# Description   : The Borland C++ Builder 2009 Project Creator
# Author        : Johnny Willemsen
# Create Date   : 17/10/2008
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use BCB2007ProjectCreator;

use vars qw(@ISA);
@ISA = qw(BCB2007ProjectCreator);

# ************************************************************
# Subroutine Section
# ************************************************************

sub get_dll_exe_template_input_file {
  #my $self = shift;
  return 'bcb2009exe';
}


sub get_dll_template_input_file {
  #my $self = shift;
  return 'bcb2009dll';
}


1;
