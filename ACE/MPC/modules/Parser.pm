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

use OutputMessage;
use StringProcessor;
use DirectoryManager;

use vars qw(@ISA);
@ISA = qw(OutputMessage StringProcessor DirectoryManager);

# ************************************************************
# Data Section
# ************************************************************

my %filecache;

# ************************************************************
# Subroutine Section
# ************************************************************

sub new {
  my($class, $inc) = @_;
  my $self = $class->SUPER::new();

  ## Set up the internal data members.
  $self->{'line_number'} = 0;
  $self->{'include'}     = $inc;

  return $self;
}


sub strip_line {
  my($self, $line) = @_;

  ## Keep track of our line number
  ++$self->{'line_number'};

  ## Remove comments and leading and trailing white-space.
  $line =~ s/\/\/.*//;
  $line =~ s/^\s+//;
  $line =~ s/\s+$//;

  return $line;
}


sub preprocess_line {
  #my $self = shift;
  #my $fh   = shift;
  #my $line = shift;
  return $_[0]->strip_line($_[2]);
}


sub read_file {
  my($self, $input, $cache) = @_;
  my $ih = new FileHandle();
  my $status = 1;
  my $errorString;

  $self->{'line_number'} = 0;
  if (open($ih, $input)) {
    $self->debug("Open $input");
    if ($cache) {
      ## If we don't have an array for this file, then start one
      $filecache{$input} = [] if (!defined $filecache{$input});

      while(<$ih>) {
        ## Preprocess the line
        my $line = $self->preprocess_line($ih, $_);

        ## Push the line onto the array for this file
        push(@{$filecache{$input}}, $line);

        ## Parse the line
        ($status, $errorString) = $self->parse_line($ih, $line);

        ## Stop reading the file if we've encountered an error
        last if (!$status);
      }
    }
    else {
      ## We're not caching, so we just preprocess and parse in one call.
      while(<$ih>) {
        ($status, $errorString) = $self->parse_line(
                                    $ih, $self->preprocess_line($ih, $_));

        ## Stop reading the file if we've encountered an error
        last if (!$status);
      }
    }
    $self->debug("Close $input");
    close($ih);
  }
  else {
    $errorString = "Unable to open \"$input\" for reading";
    $status = 0;
  }

  return $status, $errorString;
}


sub cached_file_read {
  my($self, $input) = @_;
  my $lines = $filecache{$input};

  if (defined $lines) {
    my $status = 1;
    my $error;
    $self->{'line_number'} = 0;
    foreach my $line (@$lines) {
      ++$self->{'line_number'};
      ## Since we're "reading" a cached file, we must pass undef as the
      ## file handle to parse_line().
      ($status, $error) = $self->parse_line(undef, $line);

      ## Stop "reading" the file if we've encountered an error
      last if (!$status);
    }
    return $status, $error;
  }

  ## We haven't cached this file yet, read it and cache it.
  return $self->read_file($input, 1);
}


sub get_line_number {
  return $_[0]->{'line_number'};
}


sub set_line_number {
  my($self, $number) = @_;
  $self->{'line_number'} = $number;
}


sub slash_to_backslash {
  ## This method is here solely for convenience.  It's used to make the
  ## calling code look cleaner.
  my($self, $file) = @_;
  $file =~ s/\//\\/g;
  return $file;
}


sub get_include_path {
  return $_[0]->{'include'};
}


sub search_include_path {
  my($self, $file) = @_;

  foreach my $include ('.', @{$self->{'include'}}) {
    return "$include/$file" if (-r "$include/$file");
  }

  return undef;
}


sub escape_regex_special {
  my($self, $name) = @_;
  $name =~ s/([\+\-\\\$\[\]\(\)\.])/\\$1/g;
  return $name;
}


# ************************************************************
# Virtual Methods To Be Overridden
# ************************************************************

sub parse_line {
  #my $self = shift;
  #my $ih   = shift;
  #my $line = shift;
}


1;
