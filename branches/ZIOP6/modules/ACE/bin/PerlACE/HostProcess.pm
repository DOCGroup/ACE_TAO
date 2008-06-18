package PerlACE::HostProcess;

# ******************************************************************
# Description : Creates the host process object
# Author      : Chad Elliott
# Create Date : 6/06/2008
#          $Id$
# ******************************************************************

# ******************************************************************
# Pragma Section
# ******************************************************************

use strict;

use PerlACE::TargetProcess;

# ******************************************************************
# Subroutine Section
# ******************************************************************

sub new {
  my $class = shift;

  if (PerlACE::is_vxworks_test() &&
      PerlACE::TargetProcess::roles_reversed()) {
    return new PerlACE::ProcessVX(@_);
  }

  return new PerlACE::Process(@_);
}

1;
