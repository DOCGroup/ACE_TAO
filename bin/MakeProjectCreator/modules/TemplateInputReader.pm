package TemplateInputReader;

# ************************************************************
# Description   : Reads the template input and stores the values
# Author        : Chad Elliott
# Create Date   : 5/16/2002
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
  my($class) = shift;
  my($self)  = Parser::new($class);

  $self->{'values'}  = {};
  $self->{'cindex'}  = 0;
  my(@current) = ($self->{'values'});
  $self->{'current'} = \@current;


  return $self;
}


sub parse_line {
  my($self)        = shift;
  my($ih)          = shift;
  my($line)        = shift;
  my($status)      = 1;
  my($errorString) = "";
  my($current)     = $self->{'current'};

  if ($line eq "") {
  }
  elsif ($line =~ /^([\w\s]+)\s*{$/) {
    ## Entering a new scope
    my($name) = $1;
    $name =~ s/\s+$//;
    if (!defined $$current[$self->{'cindex'}]->{$name}) {
      $$current[$self->{'cindex'}]->{$name} = {};
    }
    push(@$current, $$current[$self->{'cindex'}]->{$name});
    $self->{'cindex'}++;
  }
  elsif ($line =~ /^}$/) {
    if ($self->{'cindex'} > 0) {
      pop(@$current);
      $self->{'cindex'}--;
    }
    else {
      $status = 0;
      $errorString = "ERROR: Unmatched curly brace";
    }
  }
  elsif ($line =~ /^(\w+)\s*=\s*(.*)?/) {
    my($name)  = $1;
    my($value) = $2;

    if (defined $value) {
      $value = $self->create_array($value);
    }
    else {
      $value = "";
    }

    if (!defined $$current[$self->{'cindex'}]->{$name}) {
      $$current[$self->{'cindex'}]->{$name} = $value;
    }
    else {
      $status = 0;
      $errorString = "ERROR: Redifinition of '$name'";
    }
  }
  else {
    $status = 0;
    $errorString = "ERROR: Unrecognized line: $line";
  }

  return $status, $errorString;
}


sub get_names {
  my($self) = shift;
  return keys %{$self->{'values'}};
}


sub get_value {
  my($self) = shift;
  my($tag)  = shift;
  return $self->{'values'}->{$tag};
}


1;
