package FeatureParser;

# ************************************************************
# Description   : Reads the feature files and store the values
# Author        : Chad Elliott
# Create Date   : 5/21/2003
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use Parser;

use vars qw(@ISA);
@ISA = qw(Parser);

# ************************************************************
# Subroutine Section
# ************************************************************

sub new {
  my($class)       = shift;
  my($global_file) = shift;
  my($file)        = shift;
  my($self)        = $class->SUPER::new();

  ## Set the values associative array
  $self->{'values'} = {};

  ## Process each feature file
  foreach my $f ($global_file, $file) {
    if (defined $f) {
      my($status, $warn) = $self->read_file($f);
      if (!$status) {
        ## We only want to warn the user about problems
        ## with the feature file.
        my($lnumber) = $self->line_number();
        $warn =~ s/ERROR/WARNING/;
        print "$f: line $lnumber:\n$warn\n";
      }
    }
  }

  return $self;
}


sub parse_line {
  my($self)   = shift;
  my($if)     = shift;
  my($line)   = shift;
  my($status) = 1;
  my($error)  = '';

  if ($line eq '') {
  }
  elsif ($line =~ /^(\w+)\s*=\s*(\d+)$/) {
    $self->{'values'}->{$1} = $2;
  }
  else {
    $status = 0;
    $error  = "ERROR: Unrecognized line: $line";
  }

  return $status, $error;
}


sub get_value {
  my($self) = shift;
  my($tag)  = shift;
  return $self->{'values'}->{$tag};
}


1;
