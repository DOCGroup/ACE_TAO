package WixWorkspaceCreator;

# ************************************************************
# Description   : A Wix Workspace creator
# Author        : James H. Hill
# Create Date   : 6/23/2009
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;
use WixProjectCreator;
use WorkspaceCreator;

use vars qw(@ISA);
@ISA = qw(WorkspaceCreator);

# ************************************************************
# Data Section
# ************************************************************


# ************************************************************
# Subroutine Section
# ************************************************************

sub workspace_file_extension {
  return '.wxs';
}

sub workspace_file_name {
  my $self = shift;
  return $self->get_modified_workspace_name($self->get_workspace_name(),
                                            '.wxi');
}

sub pre_workspace {
  my($self, $fh) = @_;
  my $crlf = $self->crlf();
  my $name = $self->get_workspace_name();

  ## Begin the project definition for the workspace
  print $fh '<?xml version="1.0" encoding="utf-8" standalone="yes"?>', $crlf,
            '<Include>', $crlf;
}

sub write_comps {
  my($self, $fh) = @_;
  my $crlf = $self->crlf();


  # print the target for clean
  foreach my $project ($self->sort_dependencies($self->get_projects())) {
    print $fh "  <?include $project ?>", $crlf;
  }
}

sub post_workspace {
  my($self, $fh) = @_;
  my $info = $self->get_project_info();
  my $crlf = $self->crlf();

  # Create a component group consisting of all the projects.
  print $fh $crlf,
            '  <Fragment>', $crlf,
            '    <ComponentGroup Id="',
            $self->get_workspace_name(), '">', $crlf;

  foreach my $project ($self->sort_dependencies($self->get_projects())) {
    print $fh '      <ComponentRef Id="',
              $$info{$project}->[0], '" />', $crlf;
  }

  print $fh '    </ComponentGroup>', $crlf,
            '  </Fragment>', $crlf,
            '</Include>', $crlf;
}

1;
