package GNUAutobuildWorkspaceCreator;

# ************************************************************
# Description   : A GNU Workspace (GNUmakefile) creator that
#                 collates build results for a single makefile
#                 before outputting to stdout.
# Author        : Chad Elliott, minor modifications by Will Otte
# Create Date   : 3/22/07
# $Id$
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use GNUACEWorkspaceCreator;
use GNUAutobuildProjectCreator;
use MakeWorkspaceBase;
use WorkspaceCreator;

use vars qw(@ISA);
@ISA = qw(GNUACEWorkspaceCreator MakeWorkspaceBase WorkspaceCreator);

# ************************************************************
# Data Section
# ************************************************************

my($base) = 'GNUmakefile';

# ************************************************************
# Subroutine Section
# ************************************************************
sub extractType {
    return 'gnuace';
}

sub write_project_targets {
  my($self)   = shift;
  my($fh)     = shift;
  my($crlf)   = shift;
  my($target) = shift;
  my($list)   = shift;

  foreach my $project (@$list) {
      my($dname) = $self->mpc_dirname($project);
      my($chdir) = ($dname ne '.');
      my($output_project) = ($chdir ? $self->mpc_basename($project) : $project);

      print $fh "\t\@",
            ($chdir ? "cd $dname && " : ''),
            "\$(MAKE) -f ",
            $output_project,
            " $target &> $output_project.log ; cat $output_project.log ; rm $output_project.log $crlf";
  }
}

1;
