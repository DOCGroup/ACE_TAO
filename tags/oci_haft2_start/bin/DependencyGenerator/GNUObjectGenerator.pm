package GNUObjectGenerator;

# ************************************************************
# Description   : Generates object files for GNU Makefiles.
# Author        : Chad Elliott
# Create Date   : 5/23/2003
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;
use ObjectGenerator;

use vars qw(@ISA);
@ISA = qw(ObjectGenerator);

# ************************************************************
# Subroutine Section
# ************************************************************

sub process {
  my($self)    = shift;
  my($file)    = shift;
  my(@objects) = ();
  my(@exts)    = ('o');
  my(@dirs)    = (defined $ENV{VDIR} ? $ENV{VDIR} : '');
  my($noext)   = $file;
  $noext =~ s/\.[^\.]+$//;

  if (defined $ENV{SOEXT}) {
    push(@exts, $ENV{SOEXT});
  }
  if (defined $ENV{VSHDIR}) {
    push(@dirs, $ENV{VSHDIR});
  }

  foreach my $dirs (@dirs) {
    foreach my $ext (@exts) {
      push(@objects, "$dirs$noext.$ext");
    }
  }

  return @objects;
}


1;
