package Parser;

# ************************************************************
# Description   : A basic parser that requires a parse_line override
# Author        : Chad Elliott
# Create Date   : 5/16/2002
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;
use FileHandle;

# ************************************************************
# Subroutine Section
# ************************************************************

sub new {
  my($class)  = shift;
  my($self)   = bless {'line_number' => 0,
                      }, $class;
  return $self;
}


sub strip_line {
  my($self) = shift;
  my($line) = shift;

  $self->{'line_number'}++;
  $line =~ s/\/\/.*//;
  $line =~ s/^\s+//;
  $line =~ s/\s+$//;

  return $line;
}


sub process_special {
  my($self)   = shift;
  my($line)   = shift;
  my($length) = length($line);

  for(my $i = 0; $i < $length; $i++) {
    my($ch) = substr($line, $i, 1);
    if ($ch eq "\\" && $i + 1 < $length) {
      substr($line, $i, 1) = "";
      $length--;
    }
    elsif ($ch eq '"') {
      substr($line, $i, 1) = "";
      $length--;
      $i--;
    }
  }
  return $line;
}


sub read_file {
  my($self)        = shift;
  my($input)       = shift;
  my($ih)          = new FileHandle();
  my($status)      = 1;
  my($errorString) = "";

  $self->{'line_number'} = 0;
  if (open($ih, $input)) {
    while(<$ih>) {
      my($line) = $self->strip_line($_);

      ($status, $errorString) = $self->parse_line($ih, $line);

      if (!$status) {
        last;
      }
    }
    close($ih);
  }
  else {
    $errorString = "ERROR: Unable to open for reading";
    $status = 0;
  }

  return $status, $errorString;
}


sub line_number {
  my($self)   = shift;
  my($number) = shift;

  if (defined $number) {
    $self->{'line_number'} = $number;
  }

  return $self->{'line_number'};
}


sub create_array {
  my($self)   = shift;
  my($line)   = shift;
  my(@array)  = ();
  my($length) = length($line);
  my($prev)   = 0;
  my($double) = 0;

  for(my $i = 0; $i <= $length; $i++) {
    my($ch) = substr($line, $i, 1);
    if (!$double && ($ch eq "" || $ch =~ /\s/)) {
      my($val) = substr($line, $prev, $i - $prev);
      $val =~ s/^\s+//;
      $val =~ s/\s+$//;
      if ($val =~ /^\"(.*)\"$/) {
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
      substr($line, $i, 1) = "";
      $length--;
    }
    elsif ($ch eq '"') {
      $double ^= 1;
    }
  }
  return \@array;
}


# ************************************************************
# Virtual Methods To Be Overridden
# ************************************************************

sub parse_line {
  my($self) = shift;
  my($ih)   = shift;
  my($line) = shift;
}


1;
