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
  my($projects) = $self->get_projects();
  my($pjs)      = $self->get_project_info();
  my(@list)     = $self->sort_dependencies($projects, $pjs);
  my($crlf)     = $self->crlf();

  ## Only use the list if there is more than one project
  if ($#list > 0) {
    print $fh "MFILES = \\$crlf";
    for(my $i = 0; $i <= $#list; $i++) {
      print $fh "         $list[$i]";
      if ($i != $#list) {
        print $fh " \\";
      }
      print $fh $crlf;
    }
  }

  ## Print out the projet Makefile
  print $fh $crlf .
            "all clean realclean:$crlf";

  ## If there is more than one project, use a for loop
  if ($#list > 0) {
    print $fh "\t\@for file in \$(MFILES); do \\$crlf" .
              "\told=`pwd`; \\$crlf" .
              "\tcd `dirname \$\$file`; \\$crlf" .
              "\t\$(MAKE) -f `basename \$\$file` \$(\@); \\$crlf" .
              "\tcd \$\$old; \\$crlf" .
              "\tdone$crlf";
  }
  else {
    ## Otherwise, just list the call to make without a for loop
    print $fh "\t\@";
    my($dname) = dirname($list[0]);
    if ($dname ne '.') {
      print $fh "cd $dname && ";
    }
    print $fh "\$(MAKE) -f " .
              ($dname eq '.' ? $list[0] : basename($list[0])) .
              " \$(\@);$crlf";
  }
}



1;
