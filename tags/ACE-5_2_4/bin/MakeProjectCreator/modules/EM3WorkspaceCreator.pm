package EM3WorkspaceCreator;

# ************************************************************
# Description   : An eMbedded v3 Workspace Creator
# Author        : Chad Elliott
# Create Date   : 7/3/2002
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use EM3ProjectCreator;
use VC6WorkspaceCreator;

use vars qw(@ISA);
@ISA = qw(VC6WorkspaceCreator);

# ************************************************************
# Subroutine Section
# ************************************************************


sub workspace_file_name {
  my($self) = shift;
  return $self->get_workspace_name() . ".vcw";
}


sub pre_workspace {
  my($self) = shift;
  my($fh)   = shift;
  my($crlf) = $self->crlf();

  print $fh "Microsoft eMbedded Visual Tools Workspace File, Format Version 3.00$crlf" .
            "# WARNING: DO NOT EDIT OR DELETE THIS WORKSPACE FILE!$crlf" .
            "$crlf";
}


1;
