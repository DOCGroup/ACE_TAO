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

sub new {
  my($class)  = shift;
  my($self)   = bless {
                      }, $class;
  return $self;
}


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
  my($self)   = shift;
  my($line)   = shift;
  my($length) = length($line);

  for(my $i = 0; $i < $length; $i++) {
    my($ch) = substr($line, $i, 1);
    if ($ch eq "\\" && $i + 1 < $length) {
      substr($line, $i, 1) = '';
      $length--;
    }
    elsif ($ch eq '"') {
      substr($line, $i, 1) = '';
      $length--;
      $i--;
    }
  }
  return $line;
}


sub create_array {
  my($self)   = shift;
  my($line)   = shift;
  my(@array)  = ();
  my($length) = length($line);
  my($prev)   = 0;
  my($double) = 0;
  my($single) = 0;

  for(my $i = 0; $i <= $length; $i++) {
    my($ch) = substr($line, $i, 1);
    if (!$double && !$single && ($ch eq '' || $ch =~ /\s/)) {
      my($val) = substr($line, $prev, $i - $prev);
      $val =~ s/^\s+//;
      $val =~ s/\s+$//;
      if ($val =~ /^\"(.*)\"$/) {
        $val = $1;
      }
      elsif ($val =~ /^\'(.*)\'$/) {
        $val = $1;
      }
      push(@array, $val);
      for(; $i < $length; $i++) {
        if (substr($line, $i, 1) !~ /\s/) {
          $i--;
          last;
        }
      }
      $prev = $i + 1;
    }
    elsif ($double && $ch eq "\\" && $i + 1 < $length) {
      substr($line, $i, 1) = '';
      $length--;
    }
    elsif ($ch eq '"') {
      $double ^= 1;
    }
    elsif ($ch eq "'") {
      $single ^= 1;
    }
  }
  return \@array;
}


1;
