package FileLocator;

# ************************************************************
# Description   : Use CVS to determine the list of modified files.
# Author        : Chad Elliott
# Create Date   : 6/18/2002
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;
use FileHandle;

# ************************************************************
# Subroutine Section
# ************************************************************

sub new {
  my($class)  = shift;
  my($self)   = bless {
                      }, $class;
  return $self;
}


sub locate {
  my($self)      = shift;
  my(@dirs)      = @_;
  my($fh)        = new FileHandle();
  my(@modified)  = ();
  my(@removed)   = ();
  my(@conflicts) = ();
  my(@unknown)   = ();
  my($cvsroot)   = $self->obtainCVSROOT();
  my($nul)       = ($^O eq 'MSWin32' ? 'nul' : '/dev/null');

  if (open($fh, 'cvs -q ' . (defined $cvsroot ? "-d $cvsroot " : '') .
                "-n update @dirs 2> $nul |")) {
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
      elsif ($line =~ /^[\?]\s+(.*)/) {
        push(@unknown, $1);
      }
    }
    close($fh);
  }
  return \@modified, \@removed, \@conflicts, \@unknown;
}


sub obtainCVSROOT {
  my($self)  = shift;
  my($fh)    = new FileHandle();
  my($croot) = $ENV{CVSROOT};

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

  return $croot;
}


1;
