package DependencyWriterFactory;

# ************************************************************
# Description   : Create DependencyWriter objects.
# Author        : Chad Elliott
# Create Date   : 5/23/2003
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use GNUDependencyWriter;
use NMakeDependencyWriter;

# ************************************************************
# Subroutine Section
# ************************************************************

sub create {
  my($type) = shift;

  switch: {
    $type eq 'gnu'   && do { return new GNUDependencyWriter(); };
    $type eq 'nmake' && do { return new NMakeDependencyWriter(); };
    print STDERR "WARNING: Invalid dependency writer type: $type\n";
  }

  return new DependencyWriter();
}


1;
