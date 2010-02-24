package BMakeWorkspaceCreator;

# ************************************************************
# Description   : A Borland Make Workspace (Makefile) creator
# Author        : Chad Elliott
# Create Date   : 2/03/2004
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use BMakeProjectCreator;
use MakeWorkspaceBase;
use WinWorkspaceBase;
use WorkspaceCreator;

use vars qw(@ISA);
@ISA = qw(MakeWorkspaceBase WinWorkspaceBase WorkspaceCreator);

# ************************************************************
# Data Section
# ************************************************************

## Borland Make's maximum line length
my $max_line_length = 32767;
my $targets = 'clean generated realclean $(CUSTOM_TARGETS)';

# ************************************************************
# Subroutine Section
# ************************************************************

sub workspace_file_extension {
  #my $self = shift;
  return '.bmak';
}


sub workspace_file_name {
  my $self = shift;
  return $self->get_modified_workspace_name('Makefile', '.bmak');
}


sub pre_workspace {
  my($self, $fh) = @_;
  $self->workspace_preamble($fh, $self->crlf(),
                            'Borland Workspace Makefile',
                            '$Id$');
}


sub write_project_targets {
  my($self, $fh, $crlf, $target, $list, $and) = @_;
  my $cwd = $self->getcwd();

  ## Print out a make command for each project
  foreach my $project (@$list) {
    my $dir = $self->mpc_dirname($project);
    $dir =~ s/\//\\/g;
    my $chdir = ($dir ne '.');

    print $fh "\t", ($chdir ? "\$(COMSPEC) /c \"cd $dir $and " : ''),
              "\$(MAKE) -\$(MAKEFLAGS) -f ",
              $self->mpc_basename($project), " $target",
              ($chdir ? '"' : ''), $crlf;
  }
}


sub write_comps {
  my($self, $fh, $creator) = @_;
  my %targnum;
  my $pjs  = $self->get_project_info();
  my @list = $self->number_target_deps($self->get_projects(), $pjs,
                                       \%targnum, 0);
  my $crlf = $self->crlf();

  ## Set up the custom targets
  print $fh '!ifndef CUSTOM_TARGETS', $crlf,
            'CUSTOM_TARGETS=_EMPTY_TARGET_', $crlf,
            '!endif', $crlf;

  ## Translate each project name
  my %trans;
  foreach my $project (@list) {
    $trans{$project} = $$pjs{$project}->[0];
  }

  ## Send all the information to our base class method
  $self->write_named_targets($fh, $crlf, \%targnum, \@list,
                             $targets, '', '', \%trans, undef,
                             $creator->get_and_symbol(), $max_line_length);
}


1;
