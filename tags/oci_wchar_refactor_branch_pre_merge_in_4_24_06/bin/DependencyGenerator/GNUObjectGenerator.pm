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
  my($noext)   = $_[1];
  my($objects) = [];
  $noext =~ s/\.[^\.]+$//o;

  foreach my $dirs (qw(VDIR VSHDIR)) {
    foreach my $ext (qw(SOEXT OBJEXT)) {
      push @{$objects}, '$('.$dirs.')'.$noext.'.$('.$ext.')';
    }
  }

  return $objects;
}


1;
