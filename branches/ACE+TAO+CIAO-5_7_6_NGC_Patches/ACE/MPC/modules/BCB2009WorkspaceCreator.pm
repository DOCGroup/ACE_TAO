package BCB2009WorkspaceCreator;

# ************************************************************
# Description   : A BCB2009 Workspace Creator
# Author        : Johnny Willemsen
# Create Date   : 17/10/2008
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use BCB2007WorkspaceCreator;
use BCB2009ProjectCreator;

use vars qw(@ISA);
@ISA = qw(BCB2007WorkspaceCreator);

sub pre_workspace {
  my($self, $fh) = @_;
  my $crlf = $self->crlf();

  ## This identifies it as a Borland C++Builder 2009 file
  print $fh '﻿<Project xmlns="http://schemas.microsoft.com/developer/msbuild/2003">', $crlf;

  ## Optionally print the workspace comment
#  $self->print_workspace_comment($fh,
#            '<!-- $Id$ -->', $crlf,
#            '<!-- MPC Command -->', $crlf,
#            '<!-- ', $self->create_command_line_string($0, @ARGV), ' -->',
#            $crlf);
}

1;
