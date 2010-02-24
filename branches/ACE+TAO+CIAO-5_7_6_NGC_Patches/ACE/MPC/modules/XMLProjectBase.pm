package XMLProjectBase;

# ************************************************************
# Description   : An XML base module for Project Creators
# Author        : Chad Elliott
# Create Date   : 1/30/2006
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

# ************************************************************
# Subroutine Section
# ************************************************************

sub get_quote_symbol {
  #my $self = shift;
  return '&quot;';
}


sub get_escaped_quote_symbol {
  #my $self = shift;
  return '\\&quot;';
}


sub get_gt_symbol {
  #my $self = shift;
  return '&gt;';
}


sub get_lt_symbol {
  #my $self = shift;
  return '&lt;';
}


sub get_and_symbol {
  #my $self = shift;
  return '&amp;&amp;';
}


1;
