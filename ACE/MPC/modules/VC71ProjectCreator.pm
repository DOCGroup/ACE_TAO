package VC71ProjectCreator;

# ************************************************************
# Description   : A VC7.1 Project Creator
# Author        : Chad Elliott
# Create Date   : 4/17/2003
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use VC7ProjectCreator;

use vars qw(@ISA);
@ISA = qw(VC7ProjectCreator);

# ************************************************************
# Data Section
# ************************************************************

my %config = ('vcversion'    => '7.10',
              'forloopscope' => 'TRUE',
             );

# ************************************************************
# Subroutine Section
# ************************************************************

sub get_configurable {
  my($self, $name) = @_;
  return $config{$name};
}

1;
