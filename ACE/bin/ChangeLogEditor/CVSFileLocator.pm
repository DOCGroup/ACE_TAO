package CVSFileLocator;

# ************************************************************
# Description   : Use CVS to determine the list of modified files.
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
  my($cvsroot)   = $self->obtainCVSROOT();
  my($err)       = $self->tmpnam('cle_cvs.err');

  if (open($fh, 'cvs -f -q ' . ($^O eq 'MSWin32' ? '-N ' : '') .
                (defined $cvsroot ? "-d $cvsroot " : '') .
                "-n update @dirs 2> $err |")) {
    while(<$fh>) {
      my($line) = $_;
      if ($line =~ /^[AM]\s+(.*)/) {
        push(@modified, $1);
      }
      elsif ($line =~ /^[R]\s+(.*)/) {
        push(@removed, $1);
      }
      elsif ($line =~ /^[C]\s+(.*)/) {
        push(@conflicts, $1);
      }
      elsif ($line =~ /^[\?]\s+(.*)/ && index($line, $err) == -1) {
        push(@unknown, $1);
      }
    }
    close($fh);

    $error = $self->process_errors($err);
  }
  else {
    $error = "Unable to run cvs with error redirection.";
  }

  return \@modified, \@removed, \@conflicts, \@unknown, $error;
}


sub obtainCVSROOT {
  my($self)  = shift;
  my($fh)    = new FileHandle();
  my($croot) = undef;

  if (open($fh, 'CVS/Root')) {
    while(<$fh>) {
      my($line) = $_;
      $line =~ s/\s+$//;
      if ($line =~ /^:pserver/ || $line =~ /^:ext/) {
        if (defined $ENV{CVSROOT} && $line eq $ENV{CVSROOT}) {
          last;
        }
        else {
          my($check) = $line;
          $check =~ s/:\w+\@/:\@/;
          $check =~ s/\.\w+\.\w+:/:/;
          my($clen) = length($check);
          foreach my $key (keys %ENV) {
            my($echeck) = $ENV{$key};
            $echeck =~ s/:\w+\@/:\@/;
            $echeck =~ s/\.\w+\.\w+:/:/;
            if ($check eq $echeck) {
              $croot = $ENV{$key};
              last;
            }
            else {
              my($len) = length($echeck);
              if ($len > 0 &&
                  substr($check, $clen - $len, $len) eq $echeck) {
                $croot = $ENV{$key};
                last;
              }
            }
          }
          if (defined $croot) {
            last;
          }
        }
        if (!defined $croot) {
          $croot = $line;
        }
      }
      else {
        $croot = $line;
        last;
      }
    }
    close($fh);
  }
  else {
    $croot = $ENV{CVSROOT};
  }

  return $croot;
}


1;
