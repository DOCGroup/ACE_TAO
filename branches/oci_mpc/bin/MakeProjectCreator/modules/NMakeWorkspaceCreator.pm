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
use File::Basename;

use NMakeProjectCreator;
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

  print $fh "# Microsoft Developer Studio Generated NMAKE File\r\n\r\n";
}


sub write_comps {
  my($self)     = shift;
  my($fh)       = shift;
  my($projects) = $self->get_projects();
  my($pjs)      = $self->get_project_info();
  my(@list)     = $self->sort_dependencies($projects, $pjs);

  print $fh "ALL:\r\n";
  foreach my $project (@list) {
    my($dir)    = dirname($project);
    my($chdir)  = 0;
    my($back)   = 1;

    ## If the directory isn't "." then we need
    ## to figure out how to get back to our starting point
    if ($dir ne ".") {
      $chdir = 1;
      my($length) = length($dir);
      for(my $i = 0; $i < $length; $i++) {
        if (substr($dir, $i, 1) eq "/") {
          $back++;
        }
      }
    }

    ## These commands will work.  In practicality, only the
    ## default configuration can be built at the top level.
    print $fh ($chdir ? "\tcd $dir\r\n" : "") .
              "\t\$(MAKE) /f " . basename($project) . " CFG=\"\$(CFG)\"\r\n" .
              ($chdir ? "\tcd " . ("../" x $back) : "");
  }
}



sub project_creator {
  my($self) = shift;
  return new NMakeProjectCreator($self->get_global_cfg(),
                                 $self->get_include_path(),
                                 $self->get_template_override(),
                                 $self->get_ti_override());
}


1;
