#!/usr/bin/perl -w
#
# $Id$
#

use English;

sub load_file {
  local $filename = shift;
  local %R = ();

  open (XF, $filename)
    || die "Cannot open $filename\n";
  while (<XF>) {
    chomp;
    local @f = split(/\s+/, $_);
    $R{$f[0]} = $f[1];
  }
  close(XF);

  return %R;
}

sub interpolate {
  local %T = @_;
  local %R = ();

  local $last_i;
  foreach my $i (sort keys %T) {
    if (!defined $last_i) {
      $last_i = $i;
      $R{$i} = $T{$i};
      next;
    }
    local $si = $last_i + 1;
    while ($si < $i) {
      $R{$si} = $T{$last_i} + ($si - $last_i) * ($T{$i} - $T{$last_i}) / ($i - $last_i);
      $si++;
    }
    $last_i = $i;
    $R{$i} = $T{$i};
  }
  return %R;
}

sub range {
  local @values = @_;
  local ($min_i, $max_i);
  foreach my $i (@values) {
    if (!defined $min_i
        || $i < $min_i) {
      $min_i = $i;
    }
    if (!defined $max_i
        || $i > $max_i) {
      $max_i = $i;
    }
  }
  return ($min_i, $max_i);
}

local %BX = load_file($ARGV[0]);
print STDERR "X loaded\n";
local %BY = load_file($ARGV[1]);
print STDERR "Y loaded\n";

local %X = %BX; interpolate (%BX);
local %Y = %BY; interpolate (%BY);

local ($min_i, $max_i) = range(keys %X);
local ($min_j, $max_j) = range(keys %Y);

print STDERR "RANGE[X] = $min_i, $max_i\n";
print STDERR "RANGE[Y] = $min_j, $max_j\n";

local $total = 0;
foreach my $i (($min_i - $max_j)..($max_i - $min_j)) {
  local $z = 0;
  foreach my $j (-$max_j..-$min_j) {
    local $f = 0;
    local $g = 0;
    if (defined $X{$i - $j}) {
      $f = $X{$i - $j};
    }
    if (defined $Y{-$j}) {
      $g = $Y{-$j};
    }
    $z += $f * $g;
  }
  print $i, " ", $z, "\n";
  $total += $z;
}

print STDERR "TOTAL = $total\n";
