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

use vars qw(@ISA);
@ISA = qw(OutputMessage StringProcessor);

# ************************************************************
# Data Section
# ************************************************************

my($cwd) = Cwd::getcwd();
if ($^O eq 'cygwin' && $cwd !~ /[A-Za-z]:/) {
   my($cyg) = `cygpath -w $cwd`;
   if (defined $cyg) {
     $cyg =~ s/\\/\//g;
     chop($cwd = $cyg);
   }
 }

# ************************************************************
# Subroutine Section
# ************************************************************

sub new {
  my($class) = shift;
  my($inc)   = shift;
  my($info)  = (defined $ENV{MPC_SILENT} ||
                !defined $ENV{MPC_INFORMATION} ? 0 : 1);
  my($warn)  = (defined $ENV{MPC_SILENT} ? 0 : 1);
  my($self)  = $class->SUPER::new($info, $warn);

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
      if ($^O eq 'cygwin' && $cwd !~ /[A-Za-z]:/) {
         my($cyg) = `cygpath -w $cwd`;
         if (defined $cyg) {
           $cyg =~ s/\\/\//g;
           chop($cwd = $cyg);
         }
       }
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
    my($line) = '';
    while(<$ih>) {
      ($status, $errorString) = $self->collect_line($ih, \$line, $_);

      if (!$status) {
        last;
      }
    }
    close($ih);
  }
  else {
    $errorString = 'Unable to open for reading';
    $status = 0;
  }

  return $status, $errorString;
}


sub get_line_number {
  my($self) = shift;
  return $self->{'line_number'};
}


sub set_line_number {
  my($self)   = shift;
  my($number) = shift;
  $self->{'line_number'} = $number;
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

  foreach my $include ('.', @{$self->{'include'}}) {
    if (-r "$include/$file") {
      return "$include/$file";
    }
  }

  return undef;
}


sub escape_regex_special {
  my($self) = shift;
  my($name) = shift;

  $name =~ s/([\+\-\\\$\[\]\(\)\.])/\\$1/g;
  return $name;
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
