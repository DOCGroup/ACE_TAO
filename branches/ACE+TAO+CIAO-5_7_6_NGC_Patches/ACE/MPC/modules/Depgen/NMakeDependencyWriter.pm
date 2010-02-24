package NMakeDependencyWriter;

# ************************************************************
# Description   : Generates NMake dependencies.
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
# Subroutine Section
# ************************************************************

sub process {
  my $sources = $_[1];
  my $files   = $_[2];
  my $total   = 0;

  $$sources[0] =~ s/\//\\/g;
  $$sources[0] =~ s/\\\\/\\/g;
  my $dep = "$$sources[0] :\\\n";

  ## Sort the dependencies to make them reproducible
  foreach my $file (sort @$files) {
    $file =~ s/\//\\/g;
    $file =~ s/\\\\/\\/g;
    if ($file ne $$sources[0]) {
      $dep .= "\t\"$file\"\\\n";
      ++$total;
    }
  }

  if ($total == 0) {
    $dep = '';
  }
  else {
    $dep .= "\n\n";
  }

  return $dep;
}


1;
