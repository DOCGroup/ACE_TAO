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
                        'recurse' => 0,
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
  my($self)     = shift;
  my($file)     = shift;
  my($noinline) = shift;
  my($noincs)   = shift;
  my($fh)       = new FileHandle();
  my(@incs)     = ();

  if (open($fh, $file)) {
    my($ifcount) = 0;
    my(@zero)    = ();

    ++$self->{'recurse'};
    $self->{'files'}->{$file} = [];
    while(<$fh>) {
      $_ =~ s/\/\/.*//;

      ## As an optimization, use a very simple regular expression no the
      ## outside that all of the inner regular expressions have in
      ## common.  That way we go down the path of if elsif only if it is
      ## even possible due to the outside regular expression.
      if (/#/) {
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
            if (!$noinline ||
                ($inc !~ /\.i(nl)?$/ || $self->{'recurse'} == 1)) {
              push(@{$self->{'files'}->{$file}}, $inc);
              if (!defined $self->{'files'}->{$inc}) {
                ## Process this file, but do not return the include files
                $self->process($inc, $noinline, 1);
              }
            }
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
    --$self->{'recurse'};
  }
  return \@incs;
}


1;
