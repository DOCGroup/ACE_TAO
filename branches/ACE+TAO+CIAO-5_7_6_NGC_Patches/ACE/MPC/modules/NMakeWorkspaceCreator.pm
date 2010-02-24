package NMakeWorkspaceCreator;

# ************************************************************
# Description   : A NMake Workspace (Makefile) creator
# Author        : Chad Elliott
# Create Date   : 6/10/2002
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use NMakeProjectCreator;
use MakeWorkspaceBase;
use WinWorkspaceBase;
use WorkspaceCreator;

use vars qw(@ISA);
@ISA = qw(MakeWorkspaceBase WinWorkspaceBase WorkspaceCreator);

# ************************************************************
# Data Section
# ************************************************************

my $targets = 'clean depend generated realclean $(CUSTOM_TARGETS)';

# ************************************************************
# Subroutine Section
# ************************************************************

sub workspace_file_extension {
  #my $self = shift;
  return '.mak';
}


sub pre_workspace {
  my($self, $fh) = @_;
  $self->workspace_preamble($fh, $self->crlf(), 'NMAKE Workspace',
                            '$Id$');
}


sub write_project_targets {
  my($self, $fh, $crlf, $target, $list) = @_;
  my $cwd = $self->getcwd();

  ## Print out a make command for each project
  foreach my $project (@$list) {
    my $dir   = $self->mpc_dirname($project);
    my $chdir = ($dir ne '.');

    print $fh ($chdir ? "\t\@cd $dir$crlf\t\@echo Directory: $dir$crlf" : ''),
              "\t\@echo Project: ", $self->mpc_basename($project), $crlf,
              "\t\$(MAKE) /\$(MAKEFLAGS) /f ", $self->mpc_basename($project),
              " $target$crlf",
              ($chdir ? "\t\@cd \$(MAKEDIR)$crlf" : '');
  }
}


sub write_comps {
  my($self, $fh) = @_;
  my %targnum;
  my $pjs     = $self->get_project_info();
  my @list    = $self->number_target_deps($self->get_projects(), $pjs,
                                          \%targnum, 0);
  my $crlf    = $self->crlf();
  my $default = 'Win32 Debug';

  ## Determine the default configuration.  We want to get the Debug
  ## configuration (if available).  It just so happens that Debug comes
  ## before Release so sorting the configurations works in our favor.
  foreach my $project (keys %$pjs) {
    my($name, $deps, $pguid, $lang, $custom_only, $nocross, $managed, @cfgs) = @{$pjs->{$project}};
    @cfgs = sort @cfgs;
    if (defined $cfgs[0]) {
      $default = $cfgs[0];

      ## The configuration comes out in the form that is usable to Visual
      ## Studio.  We need it to be in the form that was chosen for the
      ## nmake configuration. So, we just swap the parts and remove the
      ## '|' character.
      $default =~ s/(.*)\|(.*)/$2 $1/;
      last;
    }
  }

  ## Print out the content
  print $fh '!IF "$(CFG)" == ""', $crlf,
            'CFG=', $default, $crlf,
            '!MESSAGE No configuration specified. ',
            'Defaulting to ', $default, '.', $crlf,
            '!ENDIF', $crlf, $crlf,
            '!IF "$(CUSTOM_TARGETS)" == ""', $crlf,
            'CUSTOM_TARGETS=_EMPTY_TARGET_', $crlf,
            '!ENDIF', $crlf;

  ## Send all the information to our base class method
  $self->write_named_targets($fh, $crlf, \%targnum, \@list,
                             $targets, 'CFG="$(CFG)" ', '',
                             $self->project_target_translation());
}


1;
