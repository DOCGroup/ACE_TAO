package VC6WorkspaceCreator;

# ************************************************************
# Description   : A VC6 Workspace Creator
# Author        : Chad Elliott
# Create Date   : 5/13/2002
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use VC6ProjectCreator;
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
  return $self->get_modified_workspace_name($self->get_workspace_name(),
                                            '.dsw');
}


sub pre_workspace {
  my($self) = shift;
  my($fh)   = shift;
  my($crlf) = $self->crlf();

  print $fh "Microsoft Developer Studio Workspace File, Format Version 6.00$crlf" .
            "# WARNING: DO NOT EDIT OR DELETE THIS WORKSPACE FILE!$crlf" .
            $crlf;
}


sub allow_duplicates {
  #my($self) = shift;
  return 0;
}


sub write_comps {
  my($self)     = shift;
  my($fh)       = shift;
  my($projects) = $self->get_projects();
  my($pjs)      = $self->get_project_info();
  my($crlf)     = $self->crlf();

  foreach my $project (@$projects) {
    my($name) = $$pjs{$project}->[0];
    my($deps) = $self->get_validated_ordering($project);

    print $fh "###############################################################################$crlf" .
              $crlf .
              "Project: \"$name\"=" . $self->slash_to_backslash($project) .
              " - Package Owner=<4>$crlf" .
              $crlf .
              "Package=<5>$crlf" .
              "{{{$crlf" .
              "}}}$crlf" .
              $crlf .
              "Package=<4>$crlf" .
              "{{{$crlf";

    if (defined $deps && $deps ne '') {
      my($darr) = $self->create_array($deps);
      foreach my $dep (@$darr) {
        ## Avoid cirular dependencies
        if ($name ne $dep) {
          print $fh "    Begin Project Dependency$crlf" .
                    "    Project_Dep_Name $dep$crlf" .
                    "    End Project Dependency$crlf";
        }
      }
    }

    print $fh "}}}$crlf$crlf";
  }
}


sub post_workspace {
  my($self) = shift;
  my($fh)   = shift;
  my($crlf) = $self->crlf();

  print $fh "###############################################################################$crlf" .
            $crlf .
            "Global:$crlf" .
            $crlf .
            "Package=<5>$crlf" .
            "{{{$crlf" .
            "}}}$crlf" .
            $crlf .
            "Package=<3>$crlf" .
            "{{{$crlf" .
            "}}}$crlf" .
            $crlf .
            "###############################################################################$crlf" .
            $crlf;
}


1;
