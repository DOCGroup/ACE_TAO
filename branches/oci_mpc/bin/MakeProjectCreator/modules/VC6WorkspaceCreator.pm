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
  if ($^O eq 'MSWin32') {
    return "\n";
  }
  else {
    return "\r\n";
  }
}


sub workspace_file_name {
  my($self) = shift;
  return $self->get_workspace_name() . ".dsw";
}


sub pre_workspace {
  my($self) = shift;
  my($fh)   = shift;
  my($crlf) = $self->crlf();

  print $fh "Microsoft Developer Studio Workspace File, Format Version 6.00$crlf" .
            "# WARNING: DO NOT EDIT OR DELETE THIS WORKSPACE FILE!$crlf" .
            "$crlf";
}


sub write_comps {
  my($self)     = shift;
  my($fh)       = shift;
  my($projects) = $self->get_projects();
  my($pjs)      = $self->get_project_info();
  my($crlf)     = $self->crlf();

  foreach my $project (@$projects) {
    my($pi) = $$pjs{$project};
    my($name, $deps) = @$pi;

    ## Convert all /'s to \
    $project =~ s/\//\\/g;

    print $fh "###############################################################################$crlf" .
              "$crlf" .
              "Project: \"$name\"=$project - Package Owner=<4>$crlf" .
              "$crlf" .
              "Package=<5>$crlf" .
              "{{{$crlf" .
              "}}}$crlf" .
              "$crlf" .
              "Package=<4>$crlf" .
              "{{{$crlf";

    if (defined $deps && $deps ne "") {
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

    print $fh "}}}$crlf" .
              "$crlf";
  }
}


sub post_workspace {
  my($self) = shift;
  my($fh)   = shift;
  my($crlf) = $self->crlf();

  print $fh "###############################################################################$crlf" .
            "$crlf" .
            "Global:$crlf" .
            "$crlf" .
            "Package=<5>$crlf" .
            "{{{$crlf" .
            "}}}$crlf" .
            "$crlf" .
            "Package=<3>$crlf" .
            "{{{$crlf" .
            "}}}$crlf" .
            "$crlf" .
            "###############################################################################$crlf" .
            "$crlf";
}


sub project_creator {
  my($self) = shift;
  return new VC6ProjectCreator($self->get_global_cfg(),
                               $self->get_include_path(),
                               $self->get_template_override(),
                               $self->get_ti_override(),
                               $self->get_dynamic(),
                               $self->get_static());
}


1;
