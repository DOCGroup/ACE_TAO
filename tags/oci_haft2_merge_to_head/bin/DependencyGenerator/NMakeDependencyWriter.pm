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
  my($self)    = shift;
  my($sources) = shift;
  my($files)   = shift;
  my($count)   = scalar(@$files);
  my($total)   = 0;

  $$sources[0] =~ s/\//\\/g;
  $$sources[0] =~ s/\\\\/\\/g;
  my($dep) = "$$sources[0] :\\\n";

  for(my $i = 0; $i < $count; ++$i) {
    $$files[$i] =~ s/\//\\/g;
    $$files[$i] =~ s/\\\\/\\/g;
    if ($$files[$i] ne $$sources[0]) {
      $dep .= "\t\"$$files[$i]\"\\\n";
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
