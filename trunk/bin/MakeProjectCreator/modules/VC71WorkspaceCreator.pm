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
  my($crlf) = $self->crlf();

  print $fh "Microsoft Visual Studio Solution File, Format Version 8.00$crlf";
}


sub print_inner_project {
  my($self)  = shift;
  my($fh)    = shift;
  my($gen)   = shift;
  my($pguid) = shift;
  my($deps)  = shift;

  if (defined $deps && $deps ne '') {
    my($crlf) = $self->crlf();
    print $fh "\tProjectSection(ProjectDependencies) = postProject$crlf";
    my($darr) = $self->create_array($deps);
    foreach my $dep (@$darr) {
      my($val) = $gen->specific_lookup($dep);
      if (defined $val && $pguid ne $val) {
        print $fh "\t\t{$val} = {$val}$crlf";
      }
    }
    print $fh "\tEndProjectSection$crlf";
  }
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
  #my($self) = shift;
  #my($fh)   = shift;
  #my($gen)  = shift;
  #my($list) = shift;
  #my($pjs)  = shift;

  ## These are done in the print_inner_project method
}


1;
