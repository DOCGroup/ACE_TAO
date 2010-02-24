package CCWorkspaceCreator;

# ************************************************************
# Description   : A Code Composer Workspace creator
# Author        : Chad Elliott
# Create Date   : 9/18/2006
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use CCProjectCreator;
use WinWorkspaceBase;
use WorkspaceCreator;

use vars qw(@ISA);
@ISA = qw(WinWorkspaceBase WorkspaceCreator);

# ************************************************************
# Subroutine Section
# ************************************************************

sub compare_output {
  #my $self = shift;
  return 1;
}


sub workspace_file_extension {
  #my $self = shift;
  return '.code_composer';
}


sub write_comps {
  my($self, $fh, $creator) = @_;
  my $crlf = $self->crlf();

  ## Workspace only consists of the name of the project.  Really, Code
  ## Composer doesn't use a workspace.  Each project contains the
  ## dependencies.
  foreach my $project ($self->sort_dependencies($self->get_projects(), 0)) {
    print $fh "$project$crlf";
    $self->add_dependencies($creator, $project);
  }
}


sub add_dependencies {
  my($self, $creator, $proj) = @_;
  my $fh     = new FileHandle();
  my $outdir = $self->get_outdir();
  $outdir    = $self->getcwd() if ($outdir eq '.');

  if (open($fh, "$outdir/$proj")) {
    my @read;
    my $write;
    my $cwd = $self->getcwd();
    while(<$fh>) {
      ## This is a comment found in cc.mpd if the project contains the
      ## 'after' keyword setting.
      if (/MPC\s+ADD\s+DEPENDENCIES/) {
        my @projs;
        my $crlf = $self->crlf();
        my $deps = $self->get_validated_ordering($proj);
        foreach my $dep (@$deps) {
          my $relative = $self->get_relative_dep_file($creator,
                                                      "$cwd/$proj", $dep);
          if (defined $relative) {
            if (!$write) {
              ## Indicate that we need to re-write the file and add in
              ## the start of the project dependencies section
              $write = 1;
              push(@read, "[Project Dependencies]$crlf");
            }

            ## Add in the dependency and save the project name for later.
            push(@read, "Source=\"$relative\"$crlf");
            push(@projs, $relative);
          }
        }
        if ($write) {
          ## Finish off the dependency information for the current
          ## project.
          push(@read, $crlf);
          foreach my $proj (@projs) {
            push(@read, "[\"$proj\" Settings]$crlf",
                        "MatchConfigName=true$crlf", $crlf);
          }
        }
        else {
          ## We don't need to re-write the file, so we can stop reading
          ## it.
          last;
        }
      }
      else {
        ## Save the line to possibly be written out at the end.
        push(@read, $_);
      }
    }
    close($fh);

    ## If we need to re-write the file, then do so
    if ($write && open($fh, ">$outdir/$proj")) {
      foreach my $line (@read) {
        print $fh $line;
      }
      close($fh);
    }
  }
}

1;
