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
  return $self->get_modified_workspace_name($self->get_workspace_name(),
                                            '.mak');
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
    my($back)   = 1;

    ## If the directory isn't '.' then we need
    ## to figure out how to get back to our starting point
    if ($dir ne '.') {
      $chdir = 1;
      my($length) = length($dir);
      for(my $i = 0; $i < $length; $i++) {
        if (substr($dir, $i, 1) eq '/') {
          $back++;
        }
      }
    }

    print $fh ($chdir ? "\tcd $dir$crlf" : '') .
              "\t\$(MAKE) /f " . basename($project) . " $target$crlf" .
              ($chdir ? "\tcd " . ('../' x $back) . $crlf : '');
  }
}


sub write_comps {
  my($self)     = shift;
  my($fh)       = shift;
  my($projects) = $self->get_projects();
  my($pjs)      = $self->get_project_info();
  my(@list)     = $self->sort_dependencies($projects, $pjs);
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

  foreach my $target ('ALL', 'DEPEND', 'IDL_STUBS', 'CLEAN', 'REALCLEAN') {
    print $fh $crlf .
              $target . ':' . $crlf;
    $self->write_project_targets($fh, 'CFG="$(CFG)" ' . $target, \@list);
  }
}



1;
