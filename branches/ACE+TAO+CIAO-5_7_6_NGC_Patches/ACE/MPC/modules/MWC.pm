package MWC;

# ******************************************************************
# Description : Instantiate a Driver and run it.  This is here to
#               maintain backward compatibility.
# Author      : Chad Elliott
# Create Date : 1/30/2004
# ******************************************************************

# ******************************************************************
# Pragma Section
# ******************************************************************

use strict;
use Driver;

# ************************************************************
# Subroutine Section
# ************************************************************

sub new {
  my $class = shift;
  my $self = bless {'creators' => [],
                   }, $class;
  return $self;
}


sub getCreatorList {
  return $_[0]->{'creators'};
}


sub execute {
  my($self, $base, $name, $args) = @_;
  my $driver = new Driver($base, $name, @{$self->{'creators'}});
  return $driver->run(@$args);
}


1;
