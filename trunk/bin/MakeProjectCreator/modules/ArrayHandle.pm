package ArrayHandle;

# ************************************************************
# Description   : Similar to a FileHandle only with an array
# Author        : Chad Elliott
# Create Date   : 6/19/2003
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

# ************************************************************
# Subroutine Section
# ************************************************************

sub new {
  my($class) = shift;
  my($array) = shift;
  my($self)  = bless {'array' => $array,
                      'index' => 0,
                     }, $class;
  return $self;
}


sub getline {
  my($self) = shift;
  return $self->{'array'}->[$self->{'index'}++];
}


1;
