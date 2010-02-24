package MakeWorkspaceCreator;

# ************************************************************
# Description   : A Generic Workspace (Makefile) creator
# Author        : Chad Elliott
# Create Date   : 2/18/2003
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use MakeProjectCreator;
use MakeWorkspaceBase;
use WorkspaceCreator;

use vars qw(@ISA);
@ISA = qw(MakeWorkspaceBase WorkspaceCreator);

# ************************************************************
# Data Section
# ************************************************************

my $targets = 'clean depend generated realclean $(CUSTOM_TARGETS)';

# ************************************************************
# Subroutine Section
# ************************************************************

sub write_project_targets {
  my($self, $fh, $crlf, $target, $list) = @_;

  ## Print out a make command for each project
  foreach my $project (@$list) {
    my $dname = $self->mpc_dirname($project);
    my $chdir = ($dname ne '.');
    print $fh "\t\@",
              ($chdir ? "cd $dname && " : ''),
              "\$(MAKE) -f ",
              ($chdir ? $self->mpc_basename($project) : $project),
              " $target$crlf";
  }
}

sub pre_workspace {
  my($self, $fh) = @_;
  $self->workspace_preamble($fh, $self->crlf(), 'Make Workspace',
                            '$Id$');
}


sub write_comps {
  my($self, $fh) = @_;
  my %targnum;
  my @list = $self->number_target_deps($self->get_projects(),
                                       $self->get_project_info(),
                                       \%targnum, 0);

  ## Send all the information to our base class method
  $self->write_named_targets($fh, $self->crlf(), \%targnum, \@list,
                             ($self->languageIs(Creator::csharp) ?
                               'bundle ' : '') . $targets, '', 'generated ',
                             $self->project_target_translation(1), 1);
}




1;
