package MPC;

# ******************************************************************
# Description : Instantiate a Driver and run it
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
  my($class) = shift;
  my($self)  = bless {'creators' => [ 'GNUACEProjectCreator',
                                      'NMakeProjectCreator',
                                      'VC6ProjectCreator',
                                      'VC7ProjectCreator',
                                      'VC71ProjectCreator',
                                      'BorlandProjectCreator',
                                      'CbxProjectCreator',
                                      'GHSProjectCreator',
                                      'EM3ProjectCreator',
                                      'VA4ProjectCreator',
                                      'MakeProjectCreator',
                                      'AutomakeProjectCreator',
                                      'BMakeProjectCreator',
                                      'HTMLProjectCreator',
                                      'SLEProjectCreator',
                                    ],
                     }, $class;
  return $self;
}


sub getCreatorList {
  my($self) = shift;
  return $self->{'creators'};
}


sub execute {
  my($self)   = shift;
  my($base)   = shift;
  my($name)   = shift;
  my($args)   = shift;
  my($driver) = new Driver($base, $name, @{$self->{'creators'}});
  return $driver->run(@$args);
}


1;
