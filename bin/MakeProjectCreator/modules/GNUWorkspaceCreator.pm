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

  print $fh "#----------------------------------------------------------------------------\n" .
            "#       GNU Workspace\n" .
            "#----------------------------------------------------------------------------\n" .
            "\n";
}


sub write_comps {
  my($self)     = shift;
  my($fh)       = shift;
  my($projects) = $self->get_projects();
  my($pjs)      = $self->get_project_info();
  my(@list)     = $self->sort_dependencies($projects, $pjs);

  ## Print out the projet Makefile
  print $fh "include \$(ACE_ROOT)/include/makeinclude/macros.GNU\n" .
            "TARGETS_NESTED := \$(TARGETS_NESTED:.nested=)\n" .
            "\n" .
            "\$(TARGETS_NESTED):\n" .
            "ifeq (Windows,\$(findstring Windows,\$(OS)))\n";
  foreach my $project (@list) {
    print $fh "\t\@cmd /c \"\$(MAKE) -f " . basename($project) . " -C " . dirname($project) . " \$(\@)\"\n";
  }
  print $fh "else\n";
  foreach my $project (@list) {
    print $fh "\t\@\$(MAKE) -f " . basename($project) . " -C " . dirname($project) . " \$(\@);\n";
  }
  print $fh "endif\n";
}



sub project_creator {
  my($self) = shift;
  return new GNUProjectCreator($self->get_global_cfg(),
                               $self->get_include_path(),
                               $self->get_template_override(),
                               $self->get_ti_override());
}


1;
