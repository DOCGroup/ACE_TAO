package VC71WorkspaceCreator;

# ************************************************************
# Description   : A VC7.1 Workspace Creator
# Author        : Chad Elliott
# Create Date   : 4/17/2003
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use VC71ProjectCreator;
use VC7WorkspaceCreator;

use vars qw(@ISA);
@ISA = qw(VC7WorkspaceCreator);

# ************************************************************
# Subroutine Section
# ************************************************************


sub pre_workspace {
  my($self) = shift;
  my($fh)   = shift;

  print $fh "Microsoft Visual Studio Solution File, Format Version 8.00" .
            $self->crlf();
}


sub print_inner_project {
  my($self)  = shift;
  my($fh)    = shift;
  my($gen)   = shift;
  my($pguid) = shift;
  my($deps)  = shift;
  my($project_name) = shift;
  my($name_to_guid_map) = shift;

  my($crlf) = $self->crlf();
  print $fh "\tProjectSection(ProjectDependencies) = postProject$crlf";
  my($darr) = $self->create_array($deps);
  foreach my $dep (@$darr) {
    ## Avoid cirular dependencies
    if ($project_name ne $dep) {
      my($guid) = $name_to_guid_map->{$dep};
      if (defined $guid) {
        print $fh "\t\t{$guid} = {$guid}$crlf";
      }
    }
  }
  print $fh "\tEndProjectSection$crlf";
}


sub print_configs {
  my($self)    = shift;
  my($fh)      = shift;
  my($configs) = shift;
  my($crlf)    = $self->crlf();
  foreach my $key (sort keys %$configs) {
    print $fh "\t\t$key = $key$crlf";
  }
}


sub print_dependencies {
  ## These are done in the print_inner_project method
}


1;
