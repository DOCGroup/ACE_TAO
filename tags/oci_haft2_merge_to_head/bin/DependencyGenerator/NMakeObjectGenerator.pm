package NMakeObjectGenerator;

# ************************************************************
# Description   : Generates object files for NMake Makefiles.
# Author        : Chad Elliott
# Create Date   : 5/23/2003
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;
use ObjectGenerator;

use vars qw(@ISA);
@ISA = qw(ObjectGenerator);

# ************************************************************
# Subroutine Section
# ************************************************************

sub process {
  my($self) = shift;
  my($file) = shift;
  return $file;
}


1;
