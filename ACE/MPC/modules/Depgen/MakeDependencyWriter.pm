package MakeDependencyWriter;

# ************************************************************
# Description   : Generates generic Makefile dependencies.
# Author        : Chad Elliott
# Create Date   : 2/10/2002
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;
use DependencyWriter;

use vars qw(@ISA);
@ISA = qw(DependencyWriter);

# ************************************************************
# Data Section
# ************************************************************

my $cygwin = (defined $ENV{OS} && $ENV{OS} =~ /windows/i);

# ************************************************************
# Subroutine Section
# ************************************************************

sub process {
  ## Replace whitespace with escaped whitespace.
  map(s/(\s)/\\$1/g, @{$_[2]});

  ## Replace <drive letter>: with /cygdrive/<drive letter>.  The user may
  ## or may not be using Cygwin, but leaving the colon in there will
  ## cause make to fail catastrophically on the next invocation.
  map(s/([A-Z]):/\/cygdrive\/$1/gi, @{$_[2]}) if ($cygwin);

  ## Sort the dependencies to make them reproducible.
  return "@{$_[1]}: \\\n  " . join(" \\\n  ", sort @{$_[2]}) . "\n";
}


1;
