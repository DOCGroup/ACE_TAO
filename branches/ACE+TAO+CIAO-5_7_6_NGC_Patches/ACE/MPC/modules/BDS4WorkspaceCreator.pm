package BDS4WorkspaceCreator;

# ************************************************************
# Description   : A BDS 4 Workspace Creator
# Author        : Johnny Willemsen
# Create Date   : 14/12/2005
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use BDS4ProjectCreator;
use WinWorkspaceBase;
use WorkspaceCreator;

use vars qw(@ISA);
@ISA = qw(WinWorkspaceBase WorkspaceCreator);

# ************************************************************
# Subroutine Section
# ************************************************************

sub crlf {
  #my $self = shift;
  return "\n";
}


sub compare_output {
  #my $self = shift;
  return 1;
}


sub workspace_file_extension {
  #my $self = shift;
  return '.bdsgroup';
}


sub pre_workspace {
  my($self, $fh) = @_;
  my $crlf = $self->crlf();

  ## This identifies it as a Borland Developer Studio 2006 file
  print $fh '﻿<?xml version="1.0" encoding="utf-8"?>', $crlf;

  ## Optionally print the workspace comment
  $self->print_workspace_comment($fh,
            '<!-- $Id$ -->', $crlf,
            '<!-- MPC Command -->', $crlf,
            '<!-- ', $self->create_command_line_string($0, @ARGV), ' -->',
            $crlf);
}


sub write_comps {
  my($self, $fh) = @_;
  my $crlf = $self->crlf();

  ## Print out the guid and the personality information
  print $fh '<BorlandProject>', $crlf,
            '  <PersonalityInfo>', $crlf,
            '    <Option>', $crlf,
            '      <Option Name="Personality">Default.Personality</Option>', $crlf,
            '      <Option Name="ProjectType"></Option>', $crlf,
            '      <Option Name="Version">1.0</Option>', $crlf,
            '      <Option Name="GUID">{93D77FAD-C603-4FB1-95AB-34E0B6FBF615}</Option>', $crlf,
            '    </Option>', $crlf,
            '  </PersonalityInfo>', $crlf,
            '  <Default.Personality>', $crlf,
            '    <Projects>', $crlf;

  ## Print out the projects in the correct build order
  foreach my $project ($self->sort_dependencies($self->get_projects(), 0)) {
    print $fh '      <Projects Name="$project">$project</Projects>', $crlf;
  }

  print $fh '    </Projects>', $crlf,
            '    <Dependencies/>', $crlf,
            '  </Default.Personality>', $crlf,
            '</BorlandProject>', $crlf;
}


1;
