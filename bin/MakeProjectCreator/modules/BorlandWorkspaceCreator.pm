package BorlandWorkspaceCreator;

# ************************************************************
# Description   : A Borland Workspace (Makefile.bor) creator
# Author        : Chad Elliott
# Create Date   : 7/02/2002
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;
use File::Basename;

use BorlandProjectCreator;
use WorkspaceCreator;

use vars qw(@ISA);
@ISA = qw(WorkspaceCreator);

# ************************************************************
# Subroutine Section
# ************************************************************

sub workspace_file_name {
  my($self) = shift;
  return $self->get_modified_workspace_name('Makefile', '.bor');
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
            "#       Borland Workspace$crlf" .
            "#----------------------------------------------------------------------------$crlf" .
            $crlf;
}


sub write_comps {
  my($self)     = shift;
  my($fh)       = shift;
  my($projects) = $self->get_projects();
  my($pjs)      = $self->get_project_info();
  my(@list)     = $self->sort_dependencies($projects, $pjs);
  my($crlf)     = $self->crlf();

  print $fh "!include <\$(ACE_ROOT)\\include\\makeinclude\\make_flags.bor>$crlf";

  foreach my $target ('all', 'clean', 'realclean', 'install') {
    print $fh $crlf .
              "$target\:$crlf";
    foreach my $project (@list) {
      my($dir)    = dirname($project);
      my($chdir)  = 0;
      my($back)   = '';

      ## If the directory isn't "." then we need
      ## to figure out how to get back to our starting point
      if ($dir ne '.') {
        $chdir = 1;
        my($count)  = 0;
        my($length) = length($dir);
        for(my $i = 0; $i < $length; $i++) {
          if (substr($dir, $i, 1) eq '/') {
            $count++;
          }
        }
        if ($dir =~ /^\.\.\//) {
          $back = ('../' x $count) . basename($self->getcwd());
        }
        else {
          $back = ('../' x ($count + 1));
        }
      }

      print $fh ($chdir ? "\t\@cd $dir$crlf" : '') .
                "\t\$(MAKE) -\$(MAKEFLAGS) \$(MAKE_FLAGS) -f " . basename($project) . " $target$crlf" .
                ($chdir ? "\t\@cd $back$crlf" : '');
    }
  }

  # Generate a convenient rule for regenerating the workspace.
  print $fh $crlf .
            "regenerate:$crlf" .
            "\t$^X $0 @ARGV$crlf";
}



1;
