package FileLocator;

# ************************************************************
# Description   : Base class for file locators.
# Author        : Chad Elliott
# Create Date   : 6/18/2002
# $Id$
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


sub tmpnam {
  my($self) = shift;
  my($file) = shift;
  my(@def)  = ("/tmp", ".");

  foreach my $possible ($ENV{TMPDIR}, $ENV{TEMP}, $ENV{TMP}, @def) {
    if (defined $possible && -d $possible && -w $possible) {
      $possible =~ s!\\!/!g;
      return $possible . '/' . $$ . '_' . $> . '_' . $file;;
    }
  }

  return $file;
}


sub process_errors {
  my($self)  = shift;
  my($file)  = shift;
  my($error) = undef;

  if (-s $file != 0) {
    my($fh) = new FileHandle();
    if (open($fh, $file)) {
      $error = '';
      while(<$fh>) {
        $error .= $_;
      }
      close($fh);
      $error =~ s/\s+$//;
    }
  }
  unlink($file);

  return $error;
}


sub locate {
  my($self)      = shift;
  my(@dirs)      = @_;
  my(@modified)  = ();
  my(@removed)   = ();
  my(@conflicts) = ();
  my(@unknown)   = ();
  return \@modified, \@removed, \@conflicts, \@unknown;
}


1;
