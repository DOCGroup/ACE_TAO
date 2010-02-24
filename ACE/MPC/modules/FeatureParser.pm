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
  my $class    = shift;
  my $features = shift;
  my @files    = @_;
  my $self     = $class->SUPER::new();

  ## Set the values associative array
  $self->{'values'} = {};

  ## Process each feature file
  foreach my $f (@files) {
    if (defined $f) {
      my($status, $warn) = $self->read_file($f);
      if (!$status) {
        ## We only want to warn the user about problems
        ## with the feature file.
        my $lnumber = $self->get_line_number();
        $self->warning($self->mpc_basename($f) . ": line $lnumber: $warn");
      }
    }
  }

  ## Process each feature definition
  foreach my $feature (@$features) {
    my($status, $warn) = $self->parse_line(undef, $feature);
    ## We only want to warn the user about problems
    ## with the -feature option.
    $self->warning("-features parameter: $warn") if (!$status);
  }

  return $self;
}


sub parse_line {
  my($self, $if, $line) = @_;
  my $error;

  if ($line eq '') {
  }
  elsif ($line =~ /^(\w+)\s*=\s*(\d+)$/) {
    ## This is a valid value, so we can store it.
    $self->{'values'}->{lc($1)} = $2;
  }
  else {
    $error = "Unrecognized line: $line";
  }

  return (defined $error ? 0 : 1), $error;
}


sub get_names {
  my @names = sort keys %{$_[0]->{'values'}};
  return \@names;
}


sub get_value {
  ## All feature names are case-insensitive.
  my($self, $tag) = @_;
  return $self->{'values'}->{lc($tag)};
}


1;
