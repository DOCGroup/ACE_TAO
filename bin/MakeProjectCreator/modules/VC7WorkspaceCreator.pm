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
  if ($^O eq 'MSWin32') {
    return "\n";
  }
  else {
    return "\r\n";
  }
}


sub workspace_file_name {
  my($self) = shift;
  return $self->get_workspace_name() . ".sln";
}


sub pre_workspace {
  my($self) = shift;
  my($fh)   = shift;
  my($crlf) = $self->crlf();

  print $fh "Microsoft Visual Studio Solution File, Format Version 7.00$crlf";
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
  foreach my $project (@list) {
    my($pi) = $$pjs{$project};
    my($name, $deps, $pguid) = @$pi;

    ## Convert all /'s to \
    my($cpy) = $project;
    $cpy =~ s/\//\\/g;
    print $fh "Project(\"{$guid}\") = \"$name\", \"$cpy\", \"{$pguid}\"$crlf" .
              "EndProject$crlf";
  }

  ## Project Configurations
  print $fh "Global$crlf" .
            "\tGlobalSection(SolutionConfiguration) = preSolution$crlf";

  my(%configs) = ();
  foreach my $project (@list) {
    my($pi) = $$pjs{$project};
    my($name, $deps, $pguid, @cfgs) = @$pi;
    foreach my $cfg (@cfgs) {
      $cfg =~ s/\|.*//;
      $configs{$cfg} = 1;
    }
  }
  my($count) = 0;
  foreach my $key (sort keys %configs) {
    print $fh "\t\tConfigName.$count = $key$crlf";
    $count++;
  }

  ## Project Dependencies
  print $fh "\tEndGlobalSection$crlf" .
            "\tGlobalSection(ProjectDependencies) = postSolution$crlf";
  foreach my $project (@list) {
    my($pi) = $$pjs{$project};
    my($name, $deps, $pguid) = @$pi;
    if (defined $deps && $deps ne "") {
      my($darr) = $self->create_array($deps);
      my($i)    = 0;
      foreach my $dep (@$darr) {
        my($val) = $gen->specific_lookup($dep);
        if (!defined $val) {
          $val = $dep;
        }
        if ($pguid ne $val) {
          print $fh "\t\t{$pguid}.$i = {$val}$crlf";
          $i++;
        }
      }
    }
  }
  print $fh "\tEndGlobalSection$crlf" .
            "\tGlobalSection(ProjectConfiguration) = postSolution$crlf";

  ## Project Configuration Names
  foreach my $project (@list) {
    my($pi) = $$pjs{$project};
    my($name, $deps, $pguid, @cfgs) = @$pi;
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


sub project_creator {
  my($self) = shift;
  return new VC7ProjectCreator($self->get_global_cfg(),
                               $self->get_include_path(),
                               $self->get_template_override(),
                               $self->get_ti_override(),
                               $self->get_dynamic(),
                               $self->get_static());
}


1;
