package VC7WorkspaceCreator;

# ************************************************************
# Description   : A VC7 Workspace Creator
# Author        : Chad Elliott
# Create Date   : 5/14/2002
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use VC7ProjectCreator;
use WorkspaceCreator;

use vars qw(@ISA);
@ISA = qw(WorkspaceCreator);

# ************************************************************
# Subroutine Section
# ************************************************************

sub allow_duplicates {
  my($self) = shift;
  return 0;
}

sub crlf {
  my($self) = shift;
  return $self->windows_crlf();
}


sub workspace_file_name {
  my($self) = shift;
  return $self->get_modified_workspace_name($self->get_workspace_name(),
                                            '.sln');
}


sub pre_workspace {
  my($self) = shift;
  my($fh)   = shift;
  my($crlf) = $self->crlf();

  print $fh "Microsoft Visual Studio Solution File, Format Version 7.00$crlf";
}


sub print_inner_project {
  #my($self)  = shift;
  #my($fh)    = shift;
  #my($gen)   = shift;
  #my($pguid) = shift;
  #my($deps)  = shift;
  #my($name)  = shift;
}


sub print_configs {
  my($self)    = shift;
  my($fh)      = shift;
  my($configs) = shift;
  my($crlf)    = $self->crlf();
  my($count)   = 0;

  foreach my $key (sort keys %$configs) {
    print $fh "\t\tConfigName.$count = $key$crlf";
    $count++;
  }
}


sub print_dependencies {
  my($self) = shift;
  my($fh)   = shift;
  my($gen)  = shift;
  my($list) = shift;
  my($pjs)  = shift;
  my($crlf) = $self->crlf();

  ## I hate to add yet another loop through all the projects, but 
  ## we must have some way to map plain project names to guids.
  my(%name_to_guid_map) = ();
  foreach my $project(@$list) {
    my($name, $deps, $guid) = @{$$pjs{$project}};
    $name_to_guid_map{$name} = $guid;
  }

  ## Project Dependencies
  print $fh "\tGlobalSection(ProjectDependencies) = postSolution$crlf";
  foreach my $project (@$list) {
    my($name, $rawdeps, $project_guid) = @{$$pjs{$project}};
    my($deps) = $self->get_validated_ordering($project);
    if (defined $deps && $deps ne '') {
      my($darr) = $self->create_array($deps);
      my($i)    = 0;
      foreach my $dep (@$darr) {
        my($guid) = $name_to_guid_map{$dep};
        if (defined $guid && $guid ne $project_guid) {
          print $fh "\t\t{$project_guid}.$i = {$guid}$crlf";
          $i++;
        }
      }
    }
  }
  print $fh "\tEndGlobalSection$crlf";
}


sub write_comps {
  my($self)     = shift;
  my($fh)       = shift;
  my($gen)      = shift;
  my($projects) = $self->get_projects();
  my($vc7guid)     = '8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942';
  my($pjs)      = $self->get_project_info();
  my(@list)     = $self->sort_dependencies($projects, $pjs);
  my($crlf)     = $self->crlf();

  ## I hate to add yet another loop through all the projects, but 
  ## we must have some way to map plain project names to guids.
  my(%name_to_guid_map) = ();
  foreach my $project(sort @list) {
    my($name, $deps, $guid) = @{$$pjs{$project}};
    $name_to_guid_map{$name} = $guid;
  }

  ## Project Information
  foreach my $project (sort @list) {
    my($name, $rawdeps, $guid) = @{$$pjs{$project}};
    my($deps) = $self->get_validated_ordering($project);
    ## Convert all /'s to \
    my($cpy) = $self->slash_to_backslash($project);
    print $fh "Project(\"{$vc7guid}\") = \"$name\", \"$cpy\", \"{$guid}\"$crlf";
    $self->print_inner_project($fh, $gen, $guid, $deps, $name, \%name_to_guid_map);
    print $fh "EndProject$crlf";
  }

  ## Project Configurations
  print $fh "Global$crlf" .
            "\tGlobalSection(SolutionConfiguration) = preSolution$crlf";

  my(%configs) = ();
  foreach my $project (@list) {
    my($name, $deps, $pguid, @cfgs) = @{$$pjs{$project}};
    foreach my $cfg (@cfgs) {
      $cfg =~ s/\|.*//;
      $configs{$cfg} = 1;
    }
  }
  $self->print_configs($fh, \%configs);
  print $fh "\tEndGlobalSection$crlf";

  ## Print dependencies if there are any
  $self->print_dependencies($fh, $gen, \@list, $pjs);

  ## Project Configuration Names
  print $fh "\tGlobalSection(ProjectConfiguration) = postSolution$crlf";
  foreach my $project (sort @list) {
    my($name, $deps, $pguid, @cfgs) = @{$$pjs{$project}};
    foreach my $cfg (sort @cfgs) {
      my($c) = $cfg;
      $c =~ s/\|.*//;
      print $fh "\t\t{$pguid}.$c.ActiveCfg = $cfg$crlf" .
                "\t\t{$pguid}.$c.Build.0 = $cfg$crlf";
    }
  }
  print $fh "\tEndGlobalSection$crlf" .
            "\tGlobalSection(ExtensibilityGlobals) = postSolution$crlf" .
            "\tEndGlobalSection$crlf" .
            "\tGlobalSection(ExtensibilityAddIns) = postSolution$crlf" .
            "\tEndGlobalSection$crlf" .
            "EndGlobal$crlf";
}


1;
