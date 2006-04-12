package SVNFileLocator;

# ************************************************************
# Description   : Use SVN to determine the list of modified files.
# Author        : Chad Elliott
# Create Date   : 11/29/2005
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;
use FileHandle;

use FileLocator;

use vars qw(@ISA);
@ISA = qw(FileLocator);

# ************************************************************
# Subroutine Section
# ************************************************************

sub locate {
  my($self)      = shift;
  my(@dirs)      = @_;
  my($fh)        = new FileHandle();
  my(@modified)  = ();
  my(@removed)   = ();
  my(@conflicts) = ();
  my(@unknown)   = ();
  my($nul)       = ($^O eq 'MSWin32' ? 'nul' : '/dev/null');

  if (open($fh, "svn diff @dirs 2> $nul |")) {
    while(<$fh>) {
      my($line) = $_;
      if ($line =~ /^Index:\s+(.*)/) {
        if (-r $1) {
          push(@modified, $1);
        }
        else {
          push(@removed, $1);
        }
      }
    }
    close($fh);
  }
  return \@modified, \@removed, \@conflicts, \@unknown;
}


1;
