package BCB2007WorkspaceCreator;

# ************************************************************
# Description   : A BCB2007 Workspace Creator
# Author        : Johnny Willemsen
# Create Date   : 14/12/2005
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use BCB2007ProjectCreator;
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
  return '.groupproj';
}


sub pre_workspace {
  my($self, $fh) = @_;
  my $crlf = $self->crlf();

  ## This identifies it as a Borland C++Builder 2007 file
  print $fh '﻿<Project xmlns="http://schemas.microsoft.com/developer/msbuild/2003">', $crlf;

  ## Optionally print the workspace comment
#  $self->print_workspace_comment($fh,
#            '<!-- $Id$ -->', $crlf,
#            '<!-- MPC Command -->', $crlf,
#            '<!-- ', $self->create_command_line_string($0, @ARGV), ' -->',
#            $crlf);
}


sub write_comps {
  my($self, $fh) = @_;
  my $crlf = $self->crlf();
  my $project_info = $self->get_project_info();
  my @projects = $self->sort_dependencies($self->get_projects(), 0);

  ## Print GUID and personality information
  print $fh '  <PropertyGroup>', $crlf,
            '    <ProjectGuid>{1946f85e-487f-46b6-8e41-159cd446db35}</ProjectGuid>', $crlf,
            '  </PropertyGroup>', $crlf,
            '  <ItemGroup />', $crlf,
            '  <ItemGroup />', $crlf,
            '  <ProjectExtensions>', $crlf,
            '    <Borland.Personality>Default.Personality</Borland.Personality>', $crlf,
            '    <Borland.ProjectType />', $crlf,
            '    <BorlandProject>', $crlf,
            '  <BorlandProject xmlns=""> <Default.Personality> </Default.Personality> </BorlandProject></BorlandProject>', $crlf,
            '  </ProjectExtensions>', $crlf;

  ## Print the project targets
  foreach my $project (@projects) {
    my $name = $$project_info{$project}->[0];
    print $fh '  <Target Name="', $name, '">', $crlf,
              '    <MSBuild Projects="', $self->mpc_basename($project), '" Targets="" />', $crlf,
              '  </Target>', $crlf,
              '  <Target Name="', $name, ':Make">', $crlf,
              '    <MSBuild Projects="', $self->mpc_basename($project), '" Targets="Make" />', $crlf,
              '  </Target>', $crlf,
              '  <Target Name="', $name, ':Clean">', $crlf,
              '    <MSBuild Projects="', $self->mpc_basename($project), '" Targets="Clean" />', $crlf,
              '  </Target>', $crlf;
  }

  ## Print the target build order
  print $fh '  <Target Name="Build">', $crlf,
            '    <CallTarget Targets="';
  foreach my $project (@projects) {
    print $fh $$project_info{$project}->[0], ';';
  }

  ## Print the target make order
  print $fh '" />', $crlf,
            '  </Target>', $crlf,
            '  <Target Name="Make">', $crlf,
            '    <CallTarget Targets="';
  foreach my $project (@projects) {
    print $fh $$project_info{$project}->[0], ':Make;';
  }

  ## Print the target clean order
  print $fh '" />', $crlf,
            '  </Target>', $crlf,
            '  <Target Name="Clean">', $crlf,
            '    <CallTarget Targets="';
  foreach my $project (@projects) {
    print $fh $$project_info{$project}->[0], ':Clean;';
  }
  print $fh '" />', $crlf,
            '  </Target>', $crlf,
            '</Project>', $crlf;
}


1;
