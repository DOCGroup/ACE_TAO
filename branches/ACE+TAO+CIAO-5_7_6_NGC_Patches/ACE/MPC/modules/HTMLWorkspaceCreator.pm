package HTMLWorkspaceCreator;

# ************************************************************
# Description   : An html workspace creator
# Author        : Justin Michel
# Create Date   : 8/25/2003
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use HTMLProjectCreator;
use WorkspaceCreator;

use vars qw(@ISA);
@ISA = qw(WorkspaceCreator);

# ************************************************************
# Subroutine Section
# ************************************************************

sub workspace_file_extension {
  #my $self = shift;
  return '_workspace.html';
}


sub pre_workspace {
  my($self, $fh) = @_;
  my $crlf = $self->crlf();

  ## Print the header
  print $fh '<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">', $crlf,
            '<html>', $crlf;

  ## Next, goes the workspace comment
  $self->print_workspace_comment($fh,
            '<!-- $Id$ -->', $crlf,
            '<!-- MPC Command: -->', $crlf,
            '<!-- ', $self->create_command_line_string($0, @ARGV),' -->', $crlf);

  ## Then, comes the title and the CSS settings.
  print $fh '<head>', $crlf,
            '<title>', $self->get_workspace_name(), '</title>', $crlf,
            '  <style type="text/css">', $crlf,
            '    a {font: 12pt bold verdana, lucida; color: white; padding: 3px;}', $crlf,
            '    td {font: 12pt bold verdana, lucida; color: white; padding: 3px; background-color: cadetblue;}', $crlf,
            '    thead tr td {font: 18pt "trebuchet ms", helvetica; color: white; padding: 3px; background-color: teal;}', $crlf,
            '  </style>', $crlf,
            '</head>', $crlf,
            '<body>', $crlf;
}


sub write_comps {
  my($self, $fh, $creator) = @_;
  my $crlf = $self->crlf();

  ## Start the table for all of the projects
  print $fh "<table style=\"table-layout:fixed\" width=\"400\" " .
            "summary=\"MPC Projects\">$crlf" .
            "<col style=\"background-color: darkcyan;\">$crlf" .
            "<thead>$crlf" .
            "<tr><td>Projects In Build Order</td></tr>$crlf" .
            "</thead>$crlf" .
            "<tbody>$crlf";

  ## Sort the projects in build order instead of alphabetical order
  my $project_info = $self->get_project_info();
  foreach my $project ($self->sort_dependencies($self->get_projects(), 0)) {
    print $fh "<tr><td>" .
              "<a href='$project'>$$project_info{$project}->[0]</a>" .
              "</td></tr>$crlf";
  }

  ## End the table
  print $fh "</tbody></table>";
}


sub post_workspace {
  my($self, $fh) = @_;
  print $fh "</body></html>" . $self->crlf();
}


1;
