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

use StringProcessor;

use vars qw(@ISA);
@ISA = qw(StringProcessor);

# ************************************************************
# Data Section
# ************************************************************

my($cwd) = Cwd::getcwd();

# ************************************************************
# Subroutine Section
# ************************************************************

sub new {
  my($class) = shift;
  my($inc)   = shift;
  my($self)  = $class->SUPER::new();

  $self->{'line_number'} = 0;
  $self->{'include'}     = $inc;

  return $self;
}


sub cd {
  my($self)   = shift;
  my($dir)    = shift;
  my($status) = chdir($dir);

  if ($status && $dir ne '.') {
    ## First strip out any /./ or ./ or /.
    $dir =~ s/\/\.\//\//g;
    $dir =~ s/^\.\///;
    $dir =~ s/\/\.$//;

    ## If the new directory contains a relative directory
    ## then we just get the real working directory
    if ($dir =~ /\.\./) {
      $cwd = Cwd::getcwd();
    }
    else {
      if ($dir =~ /^\// || $dir =~ /^[A-Za-z]:/) {
        $cwd = $dir;
      }
      else {
        $cwd .= "/$dir";
      }
    }
  }
  return $status;
}


sub getcwd {
  #my($self) = shift;
  return $cwd;
}


sub strip_line {
  my($self) = shift;
  my($line) = shift;

  ++$self->{'line_number'};
  $line =~ s/\/\/.*//;
  $line =~ s/^\s+//;
  $line =~ s/\s+$//;

  return $line;
}


sub collect_line {
  my($self) = shift;
  my($fh)   = shift;
  my($lref) = shift;
  my($line) = shift;

  $$lref = $self->strip_line($line);
  return $self->parse_line($fh, $$lref);
}


sub read_file {
  my($self)        = shift;
  my($input)       = shift;
  my($ih)          = new FileHandle();
  my($status)      = 1;
  my($errorString) = '';

  $self->{'line_number'} = 0;
  if (open($ih, $input)) {
    my($line) = "";
    while(<$ih>) {
      ($status, $errorString) = $self->collect_line($ih, \$line, $_);

      if (!$status) {
        last;
      }
    }
    close($ih);
  }
  else {
    $errorString = 'ERROR: Unable to open for reading';
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


sub slash_to_backslash {
  my($self) = shift;
  my($file) = shift;
  $file =~ s/\//\\/g;
  return $file;
}


sub get_include_path {
  my($self) = shift;
  return $self->{'include'};
}


sub search_include_path {
  my($self)  = shift;
  my($file)  = shift;
  my($found) = undef;

  foreach my $include (@{$self->{'include'}}, '.') {
    if (-r "$include/$file") {
      $found = "$include/$file";
      last;
    }
  }
  return $found;
}


# ************************************************************
# Virtual Methods To Be Overridden
# ************************************************************

sub convert_slashes {
  #my($self) = shift;
  return 1;
}


sub parse_line {
  #my($self) = shift;
  #my($ih)   = shift;
  #my($line) = shift;
}


1;
