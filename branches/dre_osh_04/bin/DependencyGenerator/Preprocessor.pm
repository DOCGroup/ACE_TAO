package Preprocessor;

# ************************************************************
# Description   : Invokes the preprocessor on the supplied file.
# Author        : Chad Elliott
# Create Date   : 2/10/2002
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;
use FileHandle;
use File::Spec;

# ************************************************************
# Subroutine Section
# ************************************************************

sub new {
  my($class)   = shift;
  my($cxx)     = shift;
  my($macros)  = shift;
  my($options) = shift;
  my($ipaths)  = shift;
  my($self)    = bless {'cxx'     => $cxx,
                        'macros'  => $macros,
                        'options' => $options,
                        'ipaths'  => $ipaths,
                        'fh'      => new FileHandle(),
                        'nul'     => File::Spec->devnull(),
                       }, $class;
  return $self;
}


sub process {
  my($self)  = shift;
  my($file)  = shift;
  my(@lines) = ();

  if (defined $file && -r $file) {
    my($cmd) = "$self->{'cxx'} -E";

    foreach my $macro (keys %{$self->{'macros'}}) {
      $cmd .= " -D$macro" . (defined $self->{'macros'}->{$macro} ?
                                   "=$self->{'macros'}->{$macro}" : '');
    }
    foreach my $option (@{$self->{'options'}}) {
      $cmd .= " $option";
    }
    foreach my $ipath (@{$self->{'ipaths'}}) {
      $cmd .= " -I$ipath";
    }
    $cmd .= " $file";

    my($fh)  = $self->{'fh'};
    if (open($fh, "$cmd 2> $self->{'nul'} |")) {
      while(<$fh>) {
        push(@lines, $_);
      }
      close($fh);
    }
    else {
      print STDERR "ERROR: Unable to run: $cmd\n";
    }
  }

  return \@lines;
}


1;
