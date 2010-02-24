package SLEWorkspaceCreator;

# ************************************************************
# Description   : The SLE Workspace Creator
# Author        : Johnny Willemsen
# Create Date   : 3/23/2004
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use SLEProjectCreator;
use WorkspaceCreator;

use vars qw(@ISA);
@ISA = qw(WorkspaceCreator);

# ************************************************************
# Subroutine Section
# ************************************************************


sub compare_output {
  #my $self = shift;
  return 1;
}


sub workspace_file_extension {
  #my $self = shift;
  return '.vpw';
}


sub pre_workspace {
  my($self, $fh) = @_;
  my $crlf = $self->crlf();

  print $fh "<!DOCTYPE Workspace SYSTEM \"http://www.slickedit.com/dtd/vse/8.1/vpw.dtd\">$crlf" .
            "<Workspace Version=\"8.1\" VendorName=\"SlickEdit\">$crlf";
}


sub write_comps {
  my($self, $fh) = @_;
  my $crlf = $self->crlf();

  print $fh "\t<Projects>$crlf";
  foreach my $project ($self->sort_dependencies($self->get_projects(), 0)) {
    print $fh "\t\t<Project File=\"$project\"/>$crlf";
  }
  print $fh "\t</Projects>$crlf";
}


sub post_workspace {
  my($self, $fh) = @_;
  print $fh '</Workspace>' . $self->crlf();
}


1;
