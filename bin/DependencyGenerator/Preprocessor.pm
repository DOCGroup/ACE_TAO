package Preprocessor;

# ************************************************************
# Description   : Preprocesses the supplied file.
# Author        : Chad Elliott
# Create Date   : 2/10/2002
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;
use FileHandle;

# ************************************************************
# Subroutine Section
# ************************************************************

sub new {
  my($class)   = shift;
  my($macros)  = shift;
  my($options) = shift;
  my($ipaths)  = shift;
  my($self)    = bless {'macros'  => $macros,
                        'options' => $options,
                        'ipaths'  => $ipaths,
                        'files'   => {},
                       }, $class;
  return $self;
}


sub locateFile {
  my($self) = shift;
  my($file) = shift;

  foreach my $dir ('.', @{$self->{'ipaths'}}) {
    if (-r "$dir/$file") {
      return "$dir/$file";
    }
  }

  return undef;
}


sub getFiles {
  my($self) = shift;
  my($file) = shift;

  foreach my $inc (@{$self->{'files'}->{$file}}) {
    if (!defined $self->{'ifiles'}->{$inc}) {
      $self->{'ifiles'}->{$inc} = 1;
      $self->getFiles($inc);
    }
  }
}


sub process {
  my($self)   = shift;
  my($file)   = shift;
  my($noincs) = shift;
  my($fh)     = new FileHandle();
  my(@incs)   = ();

  if (open($fh, $file)) {
    my($ifcount) = 0;
    my(@zero)    = ();

    $self->{'files'}->{$file} = [];
    while(<$fh>) {
      $_ =~ s/\/\/.*//;

      if (/#\s*if\s+0/) {
        push(@zero, $ifcount);
        ++$ifcount;
      }
      elsif (/#\s*if/) {
        ++$ifcount;
      }
      elsif (/#\s*endif/) {
        --$ifcount;
        if (defined $zero[0] && $ifcount == $zero[$#zero]) {
          pop(@zero);
        }
      }
      elsif (!defined $zero[0] &&
             /#\s*include\s+(\/\*\*\/\s*)?[<"]([^">]+)[">]/) {
        my($inc) = $self->locateFile($2);
        if (defined $inc) {
          $inc =~ s/\\/\//g;
          push(@{$self->{'files'}->{$file}}, $inc);
          if (!defined $self->{'files'}->{$inc}) {
            ## Process this file, but do not return the include files
            $self->process($inc, 1);
          }
        }
      }
    }
    close($fh);

    if (!$noincs) {
      $self->{'ifiles'} = {};
      $self->getFiles($file);
      @incs = keys %{$self->{'ifiles'}};
    }
  }

  return \@incs;
}


1;
