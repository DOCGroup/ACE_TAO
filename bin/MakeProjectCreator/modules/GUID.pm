package GUID;

# ************************************************************
# Description   : Generate GUID's for VC7 projects and workspaces
# Author        : Chad Elliott
# Create Date   : 5/14/2002
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
  my($self)  = bless {
                     }, $class;
  return $self;
}


sub generate {
  my($self)  = shift;
  my($str)   = shift;
  my($time)  = time();               ## 32 bits
  my($hash)  = 0;                    ## 32 bits
  my($rand1) = int(rand(0xffff));    ## 16 bits
  my($rand2) = int(rand(0xffff));    ## 16 bits
  my($rand3) = int(rand(0xffff));    ## 16 bits
  my($rand4) = $$ + int(rand(0xff)); ## 16 bits


  ## Hash the input string
  my($length) = length($str);
  for(my $i = 0; $i < $length; $i++) {
    my($high) = $hash & 0xf8000000;
    $hash <<= 5;
    $hash ^= ($high >> 27);
    $hash ^= ord(substr($str, $i, 1));
  }

  return sprintf("%08X-%04X-%04X-%04X-%04X%08X",
                 $hash, $rand1, $rand2, $rand3, $rand4, $time);
}


1;
