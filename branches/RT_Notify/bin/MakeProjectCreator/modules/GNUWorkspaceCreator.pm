package GNUWorkspaceCreator;

# ************************************************************
# Description   : A GNU Workspace (Makefile) creator
# Author        : Chad Elliott
# Create Date   : 5/13/2002
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;
use File::Basename;

use GNUProjectCreator;
use WorkspaceCreator;

use vars qw(@ISA);
@ISA = qw(WorkspaceCreator);

# ************************************************************
# Subroutine Section
# ************************************************************

sub workspace_file_name {
  my($self) = shift;
  return "Makefile";
}


sub pre_workspace {
  my($self) = shift;
  my($fh)   = shift;
  my($crlf) = $self->crlf();

  print $fh "#----------------------------------------------------------------------------$crlf" .
            "#       GNU Workspace$crlf" .
            "#----------------------------------------------------------------------------$crlf" .
            "$crlf";
}


sub write_comps {
  my($self)     = shift;
  my($fh)       = shift;
  my($projects) = $self->get_projects();
  my($pjs)      = $self->get_project_info();
  my(@list)     = $self->sort_dependencies($projects, $pjs);
  my($crlf)     = $self->crlf();

  ## Print out the projet Makefile
  print $fh "include \$(ACE_ROOT)/include/makeinclude/macros.GNU$crlf" .
            "TARGETS_NESTED := \$(TARGETS_NESTED:.nested=)$crlf" .
            "MFILES = \\$crlf";
  for(my $i = 0; $i <= $#list; $i++) {
    print $fh "         $list[$i]";
    if ($i != $#list) {
      print $fh " \\";
    }
    print $fh $crlf;
  }

  print $fh "$crlf" .
            "\$(TARGETS_NESTED):$crlf" .
            "ifneq (Windows,\$(findstring Windows,\$(OS)))$crlf" .
            "\t\@for file in \$(MFILES); do \\$crlf" .
            "\t\$(MAKE) -f `basename \$\$file` -C `dirname \$\$file` \$(\@); \\$crlf" .
            "\tdone$crlf" .
            "else$crlf";
  foreach my $project (@list) {
    print $fh "\t-\@cmd /c \"\$(MAKE) -f " . basename($project) . " -C " . dirname($project) . " \$(\@)\"$crlf";
  }
  print $fh "endif$crlf";
}



1;
