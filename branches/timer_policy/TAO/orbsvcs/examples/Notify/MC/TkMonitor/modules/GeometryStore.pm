package GeometryStore;

# ******************************************************************
# Description : Store and retrieve geometry
# Author      : Chad Elliott
# Create Date : 1/5/2006
# $Id$
# ******************************************************************

# ******************************************************************
# Pragma Section
# ******************************************************************

use strict;
use Errno qw(ENOLCK);
use FileHandle;
use Fcntl qw(:DEFAULT :flock);

# ************************************************************
# Data Section
# ************************************************************

our($VERSION) = '1.0.2';
my($file) = (defined $ENV{HOME} ? $ENV{HOME} : '' ) . '/.geometry';

# ************************************************************
# Subroutine Section
# ************************************************************

sub store {
  my($name) = shift;
  my($geom) = shift;
  my($fh)   = new FileHandle();

  if (open($fh, "+<$file")) {
    my($locked) = flock($fh, LOCK_EX | LOCK_NB);
    ## If we were able to lock the file or locking is not
    ## supported by the OS, then we will go ahead and write
    if ($locked || $! == ENOLCK) {
      my(@lines) = ();
      my($re)    = $name;
      $re =~ s/([\+\-\\\$\[\]\(\)\.])/\\$1/g;
      while(<$fh>) {
        if ($_ !~ /^$re:/) {
          push(@lines, $_);
        }
      }

      seek($fh, 0, 0);
      truncate($fh, 0);
      foreach my $line (@lines) {
        print $fh $line;
      }
      print $fh "$name: $geom\n";
      close($fh);
    }
  }
}


sub retrieve {
  my($name) = shift;
  my($fh)   = new FileHandle();
  if (open($fh, $file)) {
    $name =~ s/([\+\-\\\$\[\]\(\)\.])/\\$1/g;
    while(<$fh>) {
      if (/^\s*$name:\s+(\d+x\d+)(\+[\-]?\d+\+[\-]?\d+)/) {
        return $1, $2;
      }
    }
    close($fh);
  }
  return '', '';
}


1;
