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
# Data Section
# ************************************************************

my $mpt = 'mpt';

# ************************************************************
# Subroutine Section
# ************************************************************

sub new {
  my($class, $inc) = @_;
  my $self = Parser::new($class, $inc);

  ## Set up the internal data members
  $self->{'values'}    = {};
  $self->{'cindex'}    = 0;
  $self->{'current'}   = [ $self->{'values'} ];
  $self->{'realnames'} = {};

  return $self;
}


sub parse_line {
  my($self, $ih, $line) = @_;
  my $status = 1;
  my $errorString;
  my $current = $self->{'current'};

  if ($line eq '') {
  }
  elsif ($line =~ /^([\w\s\(\)\.]+)\s*{$/) {
    ## Entering a new scope, we need to save the real name so that it can
    ## be accessed at a later time.
    my $rname = $1;
    $rname =~ s/\s+$//;
    my $name = lc($rname);
    $self->{'realnames'}->{$name} = $rname;

    ## Scopes are reentrant, so we only create a new map when we haven't
    ## got one.
    if (!defined $$current[$self->{'cindex'}]->{$name}) {
      $$current[$self->{'cindex'}]->{$name} = {};
    }

    ## Keep track of the current scope
    push(@$current, $$current[$self->{'cindex'}]->{$name});
    $self->{'cindex'}++;
  }
  elsif ($line =~ /^}$/) {
    ## Maintain the scope and make sure there aren't any unmatched
    ## braces.
    if ($self->{'cindex'} > 0) {
      pop(@$current);
      $self->{'cindex'}--;
    }
    else {
      $status = 0;
      $errorString = 'Unmatched curly brace';
    }
  }
  elsif ($line =~ /^(\w+)\s*(\+=|=)\s*(.*)?/) {
    ## Save the name, operation type and value.
    my $name  = lc($1);
    my $op    = $2;
    my $value = $3;

    ## Turn the value into an array
    if (defined $value) {
      $value = $self->create_array($value);
    }
    else {
      $value = [];
    }

    ## Store the value
    if ($op eq '+=' && defined $$current[$self->{'cindex'}]->{$name}) {
      push(@{$$current[$self->{'cindex'}]->{$name}}, @$value);
    }
    else {
      $$current[$self->{'cindex'}]->{$name} = $value;
    }
  }
  elsif ($line =~ /^conditional_include\s+"([\w\s\-\+\/\\\.]+)"$/) {
    ## Search for the include template file.  If it does not exist, we
    ## don't complain.  It's likely that these sort of files won't exist.
    my $file = $self->search_include_path("$1.$mpt");
    if (defined $file) {
      ## Process the file making sure to restore the line number seting
      ## when we get done.
      my $ol = $self->get_line_number();
      ($status, $errorString) = $self->read_file($file);
      $self->set_line_number($ol);
    }
  }
  else {
    $status = 0;
    $errorString = "Unrecognized line: $line";
  }

  return $status, $errorString;
}


sub get_value {
  ## All template names are case-insensitive.
  my($self, $tag) = @_;
  return $self->{'values'}->{lc($tag)};
}


sub get_realname {
  ## Sometimes, we need to get back to the name retaining the case so we
  ## access the hash map containing them.
  my($self, $tag) = @_;
  return $self->{'realnames'}->{lc($tag)};
}


1;
