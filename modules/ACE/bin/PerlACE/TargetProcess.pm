package PerlACE::TargetProcess;

# ******************************************************************
# Description : Creates the target process object
# Author      : Chad Elliott
# Create Date : 6/06/2008
#          $Id$
# ******************************************************************

# ******************************************************************
# Pragma Section
# ******************************************************************

use strict;

use PerlACE::Process;

# ******************************************************************
# Data Section
# ******************************************************************

my $roles_reversed;
foreach my $arg (@ARGV) {
  if ($arg eq '-reverse_roles') {
    $roles_reversed = 1;
    last;
  }
}

# ******************************************************************
# Subroutine Section
# ******************************************************************

sub new {
  my $class = shift;

  if (PerlACE::is_vxworks_test() && !$roles_reversed) {
    return new PerlACE::ProcessVX(@_);
  }

  return new PerlACE::Process(@_);
}

sub roles_reversed {
  return $roles_reversed;
}

1;
