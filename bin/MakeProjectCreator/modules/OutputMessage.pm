package OutputMessage;

# ************************************************************
# Description   : Prints information, warnings and errors.
# Author        : Chad Elliott
# Create Date   : 2/02/2004
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

# ************************************************************
# Data Section
# ************************************************************

my($information) = 'INFORMATION: ';
my($warning)     = 'WARNING: ';
my($error)       = 'ERROR: ';

# ************************************************************
# Subroutine Section
# ************************************************************

sub new {
  my($class) = shift;
  my($info)  = shift;
  my($warn)  = shift;
  my($self)  = bless {'information' => $info,
                      'warnings'    => $warn,
                     }, $class;
  return $self;
}


sub split_message {
  my($self) = shift;
  my($msg)  = shift;
  my($spc)  = shift;

  $msg =~ s/\.\s+/.\n$spc/g;
  return $msg . "\n";
}


sub information {
  my($self) = shift;
  my($msg)  = shift;

  if ($self->{'information'}) {
    print $information . $self->split_message($msg, ' ' x
                                              length($information));
  }
}


sub warning {
  my($self) = shift;
  my($msg)  = shift;

  if ($self->{'warnings'}) {
    print $warning . $self->split_message($msg, ' ' x
                                          length($warning));
  }
}


sub error {
  my($self) = shift;
  my($msg)  = shift;

  print $error . $self->split_message($msg, ' ' x
                                      length($error));
}


1;
