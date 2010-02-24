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

use ObjectGenerator;

# ************************************************************
# Data Section
# ************************************************************

my $generators = {};

# ************************************************************
# Subroutine Section
# ************************************************************

sub register {
  $generators = shift;
}


sub create {
  return $$generators{$_[0]}->new() if (defined $$generators{$_[0]});

  print STDERR "WARNING: Invalid object generator type: $_[0]\n";
  return new ObjectGenerator();
}


1;
