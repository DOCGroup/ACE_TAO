#!/bin/sh

export PS1='[\! \u@\h \W]\$ '

export ACE_ROOT=/proj/F6/ftciao/ACE
export TAO_ROOT=/proj/F6/ftciao/TAO
export CIAO_ROOT=/proj/F6/ftciao/CIAO
export DANCE_ROOT=/proj/F6/ftciao/CIAO/DAnCE
export CUTS_ROOT=/proj/F6/ftciao/CUTS
export MPC_ROOT=/proj/F6/common/MPC

export EXP_ROOT=/proj/F6/test
#export EXP_ROOT=/proj/F6/exp/FLARE-F6/test
export XERCESCROOT=/proj/F6/common/xerces-c-3.0.0
export BOOST_ROOT=/proj/F6/common/boost_1_37_0
export OSDDS_ROOT=/proj/F6/common/OpenSpliceDDS/V3.4.2

export
PATH=$OSDDS_ROOT/HDE/x86.linux2.6/bin:$OSDDS_ROOT/HDE/x86.linux2.6/bin:/usr/testbed/bin:/sbin:/bin:/usr/sbin:/usr/bin:/usr/games:/usr/local/sbin:/usr/local/bin:/usr/X11R6/bin:/users/wolff1/bin:$ACE_ROOT/bin:/proj/F6/common/MPC:$CUTS_ROOT/bin

export
LD_LIBRARY_PATH=$OSDDS_ROOT/HDE/x86.linux2.6/lib:$ACE_ROOT/lib:$CUTS_ROOT/lib:$BOOST_ROOT/lib:$XERCESCROOT/lib
#:$TAO_ROOT/orbsvcs/orbsvcs/LWFT:$TAO_ROOT/tao:$TAO_ROOT/orbsvcs/orbsvcs

alias rm='rm -i'

