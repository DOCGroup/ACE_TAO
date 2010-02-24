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

sub generate {
  my($out, $in, $cwd) = @_;
  my $chash = GUID::hash($cwd);
  my $nhash = GUID::hash($out);
  my $ihash = GUID::hash($in);
  my $val   = 0xfeca1bad;

  return sprintf("%08X-%04X-%04X-%04X-%04X%08X",
                 $nhash & 0xffffffff, ($val >> 16) & 0xffff,
                 ($val & 0xffff), ($ihash >> 16) & 0xffff,
                 $ihash & 0xffff, $chash & 0xffffffff);
}


sub hash {
  my $str   = shift;
  my $value = 0;

  if (defined $str) {
    my $length = length($str);
    for(my $i = 0; $i < $length; $i++) {
      $value = (($value << 4) & 0xffffffff) ^ ($value >> 28)
        ^ ord(substr($str, $i, 1));
    }
  }

  return $value;
}

1;
