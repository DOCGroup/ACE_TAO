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
  my($type) = shift;

  switch: {
    $type eq 'gnu'   && do { return new GNUObjectGenerator(); };
    $type eq 'nmake' && do { return new NMakeObjectGenerator(); };
    print STDERR "WARNING: Invalid object generator type: $type\n";
  }

  return new ObjectGenerator();
}


1;
