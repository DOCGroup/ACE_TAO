package VXTestWorkspaceCreator;

# ************************************************************
# Description   : VxTest Workspace create
# Author        : Johnny Willemsen
# Create Date   : 28/03/2008
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use VXTestProjectCreator;
use WorkspaceCreator;

use vars qw(@ISA);
@ISA = qw(WorkspaceCreator);

# ************************************************************
# Subroutine Section
# ************************************************************

sub write_workspace {
  return 1;
}

1;
