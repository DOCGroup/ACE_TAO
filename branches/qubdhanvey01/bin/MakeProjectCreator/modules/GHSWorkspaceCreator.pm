package GHSWorkspaceCreator;

# ************************************************************
# Description   : An incomplete GHS Workspace creator
# Author        : Chad Elliott
# Create Date   : 7/3/2002
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;
use File::Basename;

use GHSProjectCreator;
use WorkspaceCreator;

use vars qw(@ISA);
@ISA = qw(WorkspaceCreator);

# ************************************************************
# Subroutine Section
# ************************************************************

sub workspace_file_name {
  my($self) = shift;
  return "ghs/default.bld";
}


sub pre_workspace {
  my($self) = shift;
  my($fh)   = shift;
  my($crlf) = $self->crlf();

  print $fh "#!build$crlf" .
            "default:$crlf";
}


sub write_comps {
  my($self)     = shift;
  my($fh)       = shift;
  my($gen)      = shift;
  my($projects) = $self->get_projects();
  my($pjs)      = $self->get_project_info();
  my(@list)     = $self->sort_dependencies($projects, $pjs);
  my($crlf)     = $self->crlf();

  ## Print out the projet
  print $fh "\tnobuild$crlf" .
            "\t:cx_option=noexceptions$crlf" .
            "\t:cx_option=std_namespaces$crlf" .
            "\t:cx_template_option=noautomatic$crlf" .
            "\t:language=cxx$crlf" .
            "\t:cx_mode=ansi$crlf" .
            "\t:cx_lib=scnoe$crlf";

  foreach my $project (@list) {
    ## Convert all /'s to \
    $project =~ s/\//\\/g;
            
    print $fh "..\\$project$crlf";
    if ($gen->exe_target()) {
      print $fh "\tprogram$crlf";
    }
    else {
      print $fh "\tlibrary$crlf";
    }
  }
}



1;
