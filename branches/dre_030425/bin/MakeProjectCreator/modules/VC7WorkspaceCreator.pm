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


sub crlf {
  my($self) = shift;
  return $self->windows_crlf();
}


sub workspace_file_name {
  my($self) = shift;
  return $self->get_workspace_name() . '.sln';
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

  ## Project Dependencies
  print $fh "\tGlobalSection(ProjectDependencies) = postSolution$crlf";
  foreach my $project (@$list) {
    my($name, $deps, $pguid) = @{$$pjs{$project}};
    if (defined $deps && $deps ne '') {
      my($darr) = $self->create_array($deps);
      my($i)    = 0;
      foreach my $dep (@$darr) {
        my($val) = $gen->specific_lookup($dep);
        if (defined $val && $pguid ne $val) {
          print $fh "\t\t{$pguid}.$i = {$val}$crlf";
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
  my($guid)     = '8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942';
  my($pjs)      = $self->get_project_info();
  my(@list)     = $self->sort_dependencies($projects, $pjs);
  my($crlf)     = $self->crlf();

  ## $guid above is the VC7 Project GUID.  It should not change.

  ## Project Information
  foreach my $project (sort @list) {
    my($name, $deps, $pguid) = @{$$pjs{$project}};

    ## Convert all /'s to \
    my($cpy) = $self->slash_to_backslash($project);
    print $fh "Project(\"{$guid}\") = \"$name\", \"$cpy\", \"{$pguid}\"$crlf";
    $self->print_inner_project($fh, $gen, $pguid, $deps);
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
