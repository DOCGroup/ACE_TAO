package FileLocator;

# ************************************************************
# Description   : Base class for file locators.
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
  my(@modified)  = ();
  my(@removed)   = ();
  my(@conflicts) = ();
  my(@unknown)   = ();
  return \@modified, \@removed, \@conflicts, \@unknown;
}


1;
