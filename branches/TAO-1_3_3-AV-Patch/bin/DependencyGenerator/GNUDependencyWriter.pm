package GNUDependencyWriter;

# ************************************************************
# Description   : Generates GNU Makefile dependencies.
# Author        : Chad Elliott
# Create Date   : 2/10/2002
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;
use DependencyWriter;

use vars qw(@ISA);
@ISA = qw(DependencyWriter);

# ************************************************************
# Subroutine Section
# ************************************************************

sub process {
  my($self)    = shift;
  my($objects) = shift;
  my($files)   = shift;
  my($dep)     = "@$objects: \\\n";
  my($count)   = scalar(@$files) - 1;

  for(my $i = 0; $i <= $count; ++$i) {
    $dep .= "  $$files[$i]" . ($i != $count ? " \\\n" : "\n");
  }

  return $dep;
}


1;
