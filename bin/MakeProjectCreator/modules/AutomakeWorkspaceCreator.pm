package AutomakeWorkspaceCreator;

# ************************************************************
# Description   : A Automake Workspace (Makefile) creator
# Author        : Chad Elliott
# Create Date   : 5/13/2002
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;
use File::Basename;

use AutomakeProjectCreator;
use WorkspaceCreator;

use vars qw(@ISA);
@ISA = qw(WorkspaceCreator);

# ************************************************************
# Subroutine Section
# ************************************************************

sub workspace_file_name {
  my($self) = shift;
  return $self->get_modified_workspace_name('Makefile', '.am');
}


sub pre_workspace {
  my($self) = shift;
  my($fh)   = shift;
  my($crlf) = $self->crlf();

  print $fh "##$crlf" .
            "##  Process this file with automake$crlf" .
            "##$crlf" .
            "$crlf" .
            "## The number in AUTOMAKE_OPTIONS is the minimum required version automake$crlf" .
            "## needed to process this file.$crlf" .
            "AUTOMAKE_OPTIONS = 1.4$crlf$crlf";
}


sub write_comps {
  my($self)     = shift;
  my($fh)       = shift;
  my($projects) = $self->get_projects();
  my($pjs)      = $self->get_project_info();
  my(@list)     = $self->sort_dependencies($projects, $pjs);
  my($crlf)     = $self->crlf();
  my(%unique)   = ();
  my(@dirs)     = ();

  ## Get a unique list of directories while
  ## preserving the order of the original @list
  foreach my $dep (@list) {
    my($dir) = dirname($dep);
    if (!defined $unique{$dir}) {
      $unique{$dir} = 1;
      unshift(@dirs, $dir);
    }
  }

  ## Print out the subdirectories
  print $fh 'SUBDIRS =';
  foreach my $dir (@dirs) {
    print $fh " \\$crlf        $dir";
  }
  print $fh $crlf;
}


1;
