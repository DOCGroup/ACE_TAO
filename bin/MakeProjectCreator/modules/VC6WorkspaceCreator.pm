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


sub workspace_file_name {
  my($self) = shift;
  return $self->get_workspace_name() . ".dsw";
}


sub pre_workspace {
  my($self) = shift;
  my($fh)   = shift;

  print $fh "Microsoft Developer Studio Workspace File, Format Version 6.00\r\n" .
            "# WARNING: DO NOT EDIT OR DELETE THIS WORKSPACE FILE!\r\n" .
            "\r\n";
}


sub write_comps {
  my($self)     = shift;
  my($fh)       = shift;
  my($projects) = $self->get_projects();
  my($pjs)      = $self->get_project_info();

  foreach my $project (@$projects) {
    my($pi) = $$pjs{$project};
    my($name, $deps) = @$pi;

    ## Convert all /'s to \
    $project =~ s/\//\\/g;

    print $fh "###############################################################################\r\n" .
              "\r\n" .
              "Project: \"$name\"=$project - Package Owner=<4>\r\n" .
              "\r\n" .
              "Package=<5>\r\n" .
              "{{{\r\n" .
              "}}}\r\n" .
              "\r\n" .
              "Package=<4>\r\n" .
              "{{{\r\n";

    if (defined $deps && $deps ne "") {
      my($darr) = $self->create_array($deps);
      foreach my $dep (@$darr) {
        print $fh "    Begin Project Dependency\r\n" .
                  "    Project_Dep_Name $dep\r\n" .
                  "    End Project Dependency\r\n";
      }
    }

    print $fh "}}}\r\n" .
              "\r\n";
  }
}


sub post_workspace {
  my($self) = shift;
  my($fh)   = shift;

  print $fh "###############################################################################\r\n" .
            "\r\n" .
            "Global:\r\n" .
            "\r\n" .
            "Package=<5>\r\n" .
            "{{{\r\n" .
            "}}}\r\n" .
            "\r\n" .
            "Package=<3>\r\n" .
            "{{{\r\n" .
            "}}}\r\n" .
            "\r\n" .
            "###############################################################################\r\n" .
            "\r\n";
}


sub project_creator {
  my($self) = shift;
  return new VC6ProjectCreator($self->get_global_cfg(),
                               $self->get_include_path(),
                               $self->get_template_override(),
                               $self->get_ti_override());
}


1;
