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
  my($out)   = shift;
  my($in)    = shift;
  my($cwd)   = shift;
  my($chash) = $self->hash($cwd);
  my($nhash) = $self->hash($out);
  my($ihash) = $self->hash($in);
  my($val)   = 0xfeca1bad;

  return sprintf("%08X-%04X-%04X-%04X-%04X%08X",
                 $nhash, ($val >> 16) & 0xffff, ($val & 0xffff),
                 ($ihash >> 16) & 0xffff, $ihash & 0xffff, $chash);
}


sub hash {
  my($self)   = shift;
  my($str)    = shift;
  my($value)  = 0;

  if (defined $str) {
    my($length) = length($str);
    for(my $i = 0; $i < $length; $i++) {
      $value = ($value << 4) ^ ($value >> 28) ^ ord(substr($str, $i, 1));
    }
  }

  return $value;
}

1;
