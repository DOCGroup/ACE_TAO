package MakeWorkspaceCreator;

# ************************************************************
# Description   : A Generic Workspace (Makefile) creator
# Author        : Chad Elliott
# Create Date   : 2/18/2003
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;
use File::Basename;

use MakeProjectCreator;
use WorkspaceCreator;

use vars qw(@ISA);
@ISA = qw(WorkspaceCreator);

# ************************************************************
# Data Section
# ************************************************************

my(@targets) = ('clean', 'depend', 'realclean');

# ************************************************************
# Subroutine Section
# ************************************************************

sub workspace_file_name {
  my($self) = shift;
  return $self->get_modified_workspace_name('Makefile', '');
}


sub workspace_per_project {
  #my($self) = shift;
  return 1;
}


sub pre_workspace {
  my($self) = shift;
  my($fh)   = shift;
  my($crlf) = $self->crlf();

  print $fh "#----------------------------------------------------------------------------$crlf" .
            "#       Make Workspace$crlf" .
            "#----------------------------------------------------------------------------$crlf" .
            $crlf;
}


sub write_comps {
  my($self)     = shift;
  my($fh)       = shift;
  my($crlf)     = $self->crlf();
  my($projects) = $self->get_projects();
  my(%targnum)  = ();
  my($pjs)      = $self->get_project_info();
  my(@list)     = $self->number_target_deps($projects, $pjs, \%targnum);

  ## Print out the "all" target
  print $fh $crlf . 'all:';
  foreach my $project (@list) {
    print $fh " $$pjs{$project}->[0]";
  }

  ## Print out all other targets here
  print $fh "$crlf$crlf@targets:$crlf";
  foreach my $project (@list) {
    my($dname) = dirname($project);
    print $fh "\t\@" .
              ($dname ne '.' ? "cd $dname; " : '') .
              "\$(MAKE) PWD=`pwd` -f " .
              ($dname eq '.' ? $project : basename($project)) .
              " \$(\@)$crlf";
  }

  ## Print out each target separately
  foreach my $project (@list) {
    my($dname) = dirname($project);
    print $fh $crlf . $$pjs{$project}->[0] . ':';
    if (defined $targnum{$project}) {
      foreach my $number (@{$targnum{$project}}) {
        print $fh " $$pjs{$list[$number]}->[0]";
      }
    }

    print $fh $crlf .
              "\t\@" .
              ($dname ne '.' ? "cd $dname; " : '') .
              "\$(MAKE) PWD=`pwd` -f " .
              ($dname eq '.' ? $project : basename($project)) .
              $crlf;
  }

  ## Print out the project_name_list target
  print $fh $crlf . "project_name_list:$crlf";
  foreach my $project (@list) {
    print $fh "\t\@echo $$pjs{$project}->[0]$crlf";
  }
}



1;
