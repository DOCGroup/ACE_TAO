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


sub crlf {
  my($self) = shift;
  return $self->windows_crlf();
}


sub workspace_file_name {
  my($self) = shift;
  return "Makefile";
}


sub pre_workspace {
  my($self) = shift;
  my($fh)   = shift;
  my($crlf) = $self->crlf();

  print $fh "# Microsoft Developer Studio Generated NMAKE File$crlf$crlf";
}


sub write_project_targets {
  my($self)     = shift;
  my($fh)       = shift;
  my($target)   = shift;
  my($list)     = shift;
  my($crlf)     = $self->crlf();

  foreach my $project (@$list) {
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

    print $fh ($chdir ? "\tcd $dir$crlf" : "") .
              "\t\$(MAKE) /f " . basename($project) . " $target$crlf" .
              ($chdir ? "\tcd " . ("../" x $back) . $crlf : "");
  }
}


sub write_comps {
  my($self)     = shift;
  my($fh)       = shift;
  my($projects) = $self->get_projects();
  my($pjs)      = $self->get_project_info();
  my(@list)     = $self->sort_dependencies($projects, $pjs);
  my($crlf)     = $self->crlf();

  print $fh "ALL:$crlf";
  $self->write_project_targets($fh, "CFG=\"\$(CFG)\"", \@list);

  print $fh "$crlf" .
            "CLEAN\tREALCLEAN:$crlf";
  $self->write_project_targets($fh, "CLEAN", \@list);
}



1;
