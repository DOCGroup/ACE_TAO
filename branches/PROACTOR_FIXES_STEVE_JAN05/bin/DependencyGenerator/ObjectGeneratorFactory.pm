package ObjectGeneratorFactory;

# ************************************************************
# Description   : Create ObjectGenerator objects.
# Author        : Chad Elliott
# Create Date   : 5/23/2003
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use GNUObjectGenerator;
use NMakeObjectGenerator;

# ************************************************************
# Subroutine Section
# ************************************************************

sub create {
  switch: {
    $_[0] eq 'gnu'   && do { return new GNUObjectGenerator(); };
    $_[0] eq 'nmake' && do { return new NMakeObjectGenerator(); };
    print STDERR "WARNING: Invalid object generator type: $_[0]\n";
  }

  return new ObjectGenerator();
}


1;
