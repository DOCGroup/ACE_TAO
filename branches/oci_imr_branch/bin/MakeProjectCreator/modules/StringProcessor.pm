package StringProcessor;

# ************************************************************
# Description   : Perform various algorithms on strings
# Author        : Chad Elliott
# Create Date   : 3/07/2003
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

# ************************************************************
# Subroutine Section
# ************************************************************

sub extractType {
  my($self) = shift;
  my($name) = shift;
  my($type) = $name;

  if ($name =~ /(.*)(Project|Workspace)Creator/) {
    $type = $1;
  }

  return lc($type);
}


sub process_special {
  my($self) = shift;
  my($line) = shift;

  ## Replace all escaped double quotes and escaped backslashes
  ## with special characters
  my($escaped) = ($line =~ s/\\\\/\01/g);
  $escaped |= ($line =~ s/\\"/\02/g);

  ## Un-escape all other characters
  $line =~ s/\\(.)/$1/g;

  ## Remove any non-escaped double quotes
  $line =~ s/"//g;
                  
  ## Put the escaped double quotes and backslashes back in
  if ($escaped) {
    $line =~ s/\02/"/g;
    $line =~ s/\01/\\/g;
  }

  return $line;
}


sub create_array {
  my($self)  = shift;
  my($line)  = shift;
  my(@array) = ();

  ## Replace all escaped double and single quotes with special characters
  my($escaped) = ($line =~ s/\\\"/\01/g);
  $escaped |= ($line =~ s/\\\'/\02/g);

  foreach my $part (grep(!/^\s*$/,
                         split(/(\"[^\"]+\"|\'[^\']+\'|\s+)/, $line))) {
    ## Remove enclosing double and single quotes
    $part =~ s/^"(.*)"$/$1/;
    $part =~ s/^'(.*)'$/$1/;

    ## Put any escaped double or single quotes back into the string.
    if ($escaped) {
      $part =~ s/\01/\"/g;
      $part =~ s/\02/\'/g;
    }

    ## Push it onto the array
    push(@array, $part);
  }

  return \@array;
}


1;
