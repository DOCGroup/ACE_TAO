package SLEWorkspaceCreator;

# ************************************************************
# Description   : A Sle Workspace Creator
# Author        : Johnny Willemsen
# Create Date   : 3/23/2004
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use SleProjectCreator;
use WorkspaceCreator;

use vars qw(@ISA);
@ISA = qw(WorkspaceCreator);

# ************************************************************
# Subroutine Section
# ************************************************************


sub compare_output {
  #my($self) = shift;
  return 1;
}


sub workspace_file_name {
  my($self) = shift;
  return $self->get_modified_workspace_name($self->get_workspace_name(),
                                            '.vpw');
}


sub pre_workspace {
  my($self) = shift;
  my($fh)   = shift;
  my($crlf) = $self->crlf();

  print $fh "<!DOCTYPE Workspace SYSTEM \"http://www.slickedit.com/dtd/vse/8.1/vpw.dtd\">$crlf" .
            "<Workspace Version=\"8.1\" VendorName=\"SlickEdit\">$crlf";
}


sub write_comps {
  my($self)     = shift;
  my($fh)       = shift;
  my($projects) = $self->get_projects();
  my($pjs)      = $self->get_project_info();
  my(@list)     = $self->sort_dependencies($projects, $pjs);
  my($crlf)     = $self->crlf();

  print $fh "<Projects>$crlf";
  foreach my $project (@list) {
    print $fh "  <Project File=\"$project\"/>$crlf";
  }
  print $fh "</Projects>$crlf";
  print $fh "</Workspace>$crlf";
}


1;
