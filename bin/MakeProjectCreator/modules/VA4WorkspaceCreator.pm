package VA4WorkspaceCreator;

# ************************************************************
# Description   : A VA4 Workspace Creator
# Author        : Chad Elliott
# Create Date   : 5/13/2002
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use VA4ProjectCreator;
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
  return $self->get_workspace_name() . ".icp";
}


sub pre_workspace {
  my($self) = shift;
  my($fh)   = shift;
  my($crlf) = $self->crlf();

  print $fh "// Visual Age C++ 4 workspace file$crlf$crlf";
}


sub write_comps {
  my($self)     = shift;
  my($fh)       = shift;
  my($projects) = $self->get_projects();
  my($pjs)      = $self->get_project_info();
  my(@list)     = $self->sort_dependencies($projects, $pjs);
  my($crlf)     = $self->crlf();

  foreach my $project (@list) {
    my($base) = $project;
    $base =~ s/\.[^\.]+//;
    my($ics)  = "$base.ics";

    print $fh "subproject $base icc \"$project\", ics \"$ics\"$crlf" .
              "{$crlf" .
              "}$crlf";
  }

  print $fh "build buildAll$crlf" .
            "{$crlf";
  foreach my $project (@list) {
    my($base) = $project;
    $base =~ s/\.[^\.]+//;
    print $fh "        use $base$crlf";
  }
  print $fh "}$crlf$crlf";
}


1;
