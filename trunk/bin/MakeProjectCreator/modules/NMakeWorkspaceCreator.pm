package NMakeWorkspaceCreator;

# ************************************************************
# Description   : A NMake Workspace (Makefile) creator
# Author        : Chad Elliott
# Create Date   : 6/10/2002
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;
use File::Basename;

use NMakeProjectCreator;
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
  if ($self->make_coexistence()) {
    return $self->get_modified_workspace_name($self->get_workspace_name(),
                                              '.nmake');
  }
  else {
    return $self->get_modified_workspace_name('Makefile', '');
  }
}


sub pre_workspace {
  my($self) = shift;
  my($fh)   = shift;
  my($crlf) = $self->crlf();

  print $fh "# Microsoft Developer Studio Generated NMAKE File$crlf$crlf";
}


sub write_project_targets {
  my($self)     = shift;
  my($fh)       = shift;
  my($target)   = shift;
  my($list)     = shift;
  my($crlf)     = $self->crlf();

  foreach my $project (@$list) {
    my($dir)    = dirname($project);
    my($chdir)  = 0;
    my($back)   = '';

    ## If the directory isn't '.' then we need
    ## to figure out how to get back to our starting point
    if ($dir ne '.') {
      $chdir = 1;
      my($count) = ($dir =~ tr/\///);
      if ($dir =~ /^\.\.\//) {
        $back = ('../' x $count) . basename($self->getcwd());
      }
      else {
        $back = ('../' x ($count + 1));
      }
    }

    print $fh ($chdir ? "\tcd $dir$crlf" : '') .
              "\t\$(MAKE) /f " . basename($project) . " $target$crlf" .
              ($chdir ? "\tcd $back$crlf" : '');
  }
}


sub write_comps {
  my($self)     = shift;
  my($fh)       = shift;
  my($projects) = $self->get_projects();
  my($pjs)      = $self->get_project_info();
  my(%targnum)  = ();
  my(@list)     = $self->number_target_deps($projects, $pjs, \%targnum);
  my($crlf)     = $self->crlf();
  my($default)  = 'Win32 Debug';

  ## Determine the default configuration
  foreach my $project (keys %$pjs) {
    my($name, $deps, $pguid, @cfgs) = @{$pjs->{$project}};
    @cfgs = sort @cfgs;
    if (defined $cfgs[0]) {
      $default = $cfgs[0];
      $default =~ s/(.*)\|(.*)/$2 $1/;
      last;
    }
  }

  ## Print out the content
  print $fh '!IF "$(CFG)" == ""' . $crlf .
            'CFG=' . $default . $crlf .
            '!MESSAGE No configuration specified. ' .
            'Defaulting to ' . $default . '.' . $crlf .
            '!ENDIF' . $crlf;

  ## Print out the "all" target
  print $fh $crlf . 'ALL:';
  foreach my $project (@list) {
    print $fh " $$pjs{$project}->[0]";
  }
  print $fh $crlf;

  ## Print out all other targets here
  foreach my $target ('DEPEND', 'CLEAN', 'REALCLEAN') {
    print $fh $crlf .
              $target . ':' . $crlf;
    $self->write_project_targets($fh, 'CFG="$(CFG)" ' . $target, \@list);
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

    print $fh $crlf;
    $self->write_project_targets($fh, 'CFG="$(CFG)" ' . 'ALL', [ $project ]);
  }

  ## Print out the project_name_list target
  print $fh $crlf . "project_name_list:$crlf";
  foreach my $project (sort @list) {
    print $fh "\t\@echo $$pjs{$project}->[0]$crlf";
  }
}



1;
