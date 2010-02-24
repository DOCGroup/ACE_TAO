package WinVersionTranslator;

# ************************************************************
# Description   : Translate the version value for Windows.
#                 Windows can not handle letters in the version
#                 and truncates anything after \d+\.\d+.  We
#                 will convert letters to numbers, retain
#                 trailing numbers and everything else will be
#                 converted to a zero.
#
#                 ex. 1.4.3  => 1.403
#                     1.4a.5 => 1.4005
#                     1.4b.4 => 1.4104
#
# Author        : Chad Elliott
# Create Date   : 10/7/2004
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

# ************************************************************
# Subroutine Section
# ************************************************************

sub translate {
  my $version = shift;

  ## See if the version contains something other than numbers followed by
  ## a decimal point and numbers.
  if ($version =~ /^(\d+\.\d+)([^\d].*)$/) {
    $version = $1;
    my $post = $2;
    my $length = length($post);

    ## Convert the non-conforming value to all numbers
    for(my $i = 0; $i < $length; ++$i) {
      my $ch = substr($post, $i, 1);
      if ($ch =~ /[a-z]/i) {
        my $digit = ord(lc($ch)) - ord('a');
        $version .= $digit;
      }
      elsif ($ch =~ /\d/) {
        $version .= $ch;
      }
      else {
        $version .= '0';
      }
    }

    ## If we have a good version number we need to make sure that the
    ## minor version number does not exceed the value of a short unsigned
    ## integer.
    if ($version =~ /(\d+)\.(\d+)/) {
      my($major, $minor) = ($1, $2);
      $minor =~ s/^\d+\.//;
      while($minor > 65535) {
        $minor = substr($minor, 0, length($minor) - 1);
      }
      $version = $major . '.' . $minor;
    }
  }
  return $version;
}

1;
