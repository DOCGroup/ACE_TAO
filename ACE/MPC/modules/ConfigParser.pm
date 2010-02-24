package ConfigParser;

# ************************************************************
# Description   : Reads a generic config file and store the values
# Author        : Chad Elliott
# Create Date   : 6/12/2006
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
  my($class, $valid) = @_;
  my $self = $class->SUPER::new();

  ## Set up the internal data members
  $self->{'values'} = {};
  $self->{'clean'}  = {};
  $self->{'valid'}  = $valid;
  $self->{'warned'} = {};

  return $self;
}


sub parse_line {
  my($self, $if, $line) = @_;
  my $error;

  if ($line eq '') {
  }
  elsif ($line =~ /^([^=]+)\s*=\s*(.*)$/) {
    ## Save the name, removing any trailing white space, and the value
    ## too.
    my $name  = $1;
    my $clean = $2;
    $name =~ s/\s+$//;

    ## Pre-process the name and value
    my $value = $self->preprocess($clean);
    $name = $self->preprocess($name);
    $name =~ s/\\/\//g;

    ## Store the name value pair
    if (!defined $self->{'valid'}) {
      ## There are no valid names, so all names are valid, except an
      ## empty name.
      if ($name ne '') {
        $self->{'values'}->{$name} = $value;
        $self->{'clean'}->{$name} = $clean;
      }
    }
    elsif (defined $self->{'valid'}->{lc($name)}) {
      ## This is a valid value, so we can store it.
      $self->{'values'}->{lc($name)} = $value;
      $self->{'clean'}->{lc($name)} = $clean;
    }
    else {
      $error = "Invalid keyword: $name";
    }
  }
  else {
    $error = "Unrecognized line: $line";
  }

  return (defined $error ? 0 : 1), $error;
}


sub get_names {
  my @names = keys %{$_[0]->{'values'}};
  return \@names;
}


sub get_value {
  ## Try the tag first and if that doesn't work make it all lower-case.
  my($self, $tag) = @_;
  return $self->{'values'}->{$tag} || $self->{'values'}->{lc($tag)};
}


sub get_unprocessed {
  ## Try the tag first and if that doesn't work make it all lower-case.
  my($self, $tag) = @_;
  return $self->{'clean'}->{$tag} || $self->{'clean'}->{lc($tag)};
}


sub preprocess {
  my($self, $str) = @_;

  ## We need to replace $(...) with the equivalent environment variable
  ## value.
  while($str =~ /\$([\(\w\)]+)/) {
    my $name = $1;
    $name =~ s/[\(\)]//g;
    my $val = $ENV{$name};

    ## If the environment variable is not set, we will end up removing
    ## the reference, but we need to warn the user that we're doing so.
    if (!defined $val) {
      $val = '';
      if (!defined $self->{'warned'}->{$name}) {
        $self->diagnostic("$name was used in the configuration file, " .
                          "but was not defined.");
        $self->{'warned'}->{$name} = 1;
      }
    }

    ## Do the replacement
    $str =~ s/\$([\(\w\)]+)/$val/;
  }
  return $str;
}

1;
