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

  ## Sort the dependencies to make them reproducible
  foreach my $file (sort @$files) {
    $dep .= "  $file \\\n";
  }
  $dep =~ s/ \\$//;

  return $dep;
}


1;
