package SVNFileLocator;

# ************************************************************
# Description   : Use SVN to determine the list of modified files.
# Author        : Chad Elliott
# Create Date   : 11/29/2005
# $Id$
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
  my($error)     = undef;
  my($err)       = $self->tmpnam('cle_svn.err');

  if (open($fh, "svn status @dirs 2> $err |")) {
    while(<$fh>) {
      my($line) = $_;
      if ($line =~ /^([A-Z\s\?])([A-Z\s])[A-Z\s][\+\*\s][A-Z\s][A-Z\s]\s+(.*)$/) {
        my($content)  = $1;
        my($property) = $2;
        my($file)     = $3;

        ## Subversion differs from CVS in that it will print paths with
        ## windows style back-slashes instead of forward slashes.
        $file =~ s!\\!/!g if ($^O eq 'MSWin32');

        if ($property eq 'M' ||
            $content eq 'M'  || $content eq 'A' || $content eq 'R') {
          push(@modified, $file);
        }
        elsif ($content eq 'D') {
          push(@removed, $file);
        }
        elsif ($content eq 'C' || $property eq 'C') {
          push(@conflicts, $file);
        }
        elsif ($content eq '?' && index($line, $err) == -1) {
          push(@unknown, $file);
        }
      }
    }
    close($fh);

    $error = $self->process_errors($err);
  }
  else {
    $error = "Unable to run svn with error redirection.";
  }

  return \@modified, \@removed, \@conflicts, \@unknown, $error;
}


1;
