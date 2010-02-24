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

my $debugtag = 'DEBUG: ';
my $infotag  = 'INFORMATION: ';
my $warntag  = 'WARNING: ';
my $errortag = 'ERROR: ';

my $debug       = 0;
my $information = 0;
my $warnings    = 1;
my $diagnostic  = 1;
my $details     = 1;

# ************************************************************
# Subroutine Section
# ************************************************************

sub new {
  my $class = shift;
  return bless {}, $class;
}


sub set_levels {
  my $str = shift;

  if (defined $str) {
    $debug       = ($str =~ /debug\s*=\s*(\d+)/i ? $1 : 0);
    $details     = ($str =~ /detail(s)?\s*=\s*(\d+)/i ? $2 : 0);
    $diagnostic  = ($str =~ /diag(nostic)?\s*=\s*(\d+)/i ? $2 : 0);
    $information = ($str =~ /info(rmation)?\s*=\s*(\d+)/i ? $2 : 0);
    $warnings    = ($str =~ /warn(ing)?\s*=\s*(\d+)/i ? $2 : 0);
  }
}

sub split_message {
  my($self, $msg, $spc) = @_;
  $msg =~ s/\.\s+/.\n$spc/g;
  return $msg . "\n";
}


sub details {
  if ($details) {
    #my($self, $msg) = @_;
    print "$_[1]\n";
  }
}


sub diagnostic {
  if ($diagnostic) {
    #my($self, $msg) = @_;
    print "$_[1]\n";
  }
}


sub debug {
  if ($debug) {
    #my($self, $msg) = @_;
    print "$debugtag$_[1]\n";
  }
}


sub information {
  if ($information) {
    #my($self, $msg) = @_;
    print $infotag, $_[0]->split_message($_[1], ' ' x length($infotag));
  }
}


sub warning {
  if ($warnings) {
    #my($self, $msg) = @_;
    print $warntag, $_[0]->split_message($_[1], ' ' x length($warntag));
  }
}


sub error {
  my($self, $msg, $pre) = @_;
  print STDERR '', (defined $pre ? "$pre\n" : ''), $errortag,
               $self->split_message($msg, ' ' x length($errortag));
}


1;
