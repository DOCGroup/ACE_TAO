package BorlandProjectBase;

# ************************************************************
# Description   : A Borland base module for Borland Project Creators
# Author        : Chad Elliott
# Create Date   : 5/5/2009
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;
use WinProjectBase;

our @ISA = qw(WinProjectBase);

# ************************************************************
# Subroutine Section
# ************************************************************

sub get_properties {
  my $self = shift;

  ## Get the base class properties and add the properties that we
  ## support.
  my $props = $self->WinProjectBase::get_properties();

  ## All projects that use this base class are for Borland compilers.
  $$props{'borland'} = 1;

  return $props;
}


1;
