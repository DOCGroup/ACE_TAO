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
  my($self)   = shift;
  my($file)   = shift;
  my($ifiles) = $self->{'ifiles'};

  foreach my $inc (@{$self->{'files'}->{$file}}) {
    if (!defined $$ifiles{$inc}) {
      $$ifiles{$inc} = 1;
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

  if (open($fh, $file)) {
    my($ifcount) = 0;
    my(@zero)    = ();
    my($files)   = $self->{'files'};
    my($recurse) = ++$self->{'recurse'};

    $$files{$file} = [];
    while(<$fh>) {
      ## As an optimization, use a very simple regular expression on the
      ## outside that all of the inner regular expressions have in
      ## common.  That way we go down the path of if elsif only if it is
      ## even possible due to the outside regular expression.
      if (/#/) {
        ## Remove c++ comments inside this if statement.
        ## This saves about 5% off of processing the ace directory
        ## and we only need to strip comments if we are actually
        ## going to look at the string.
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
            if (!$noinline ||
                ($recurse == 1 || $inc !~ /\.i(nl)?$/)) {
              push(@{$$files{$file}}, $inc);
              if (!defined $$files{$inc}) {
                ## Process this file, but do not return the include files
                $self->process($inc, $noinline, 1);
              }
            }
          }
        }
      }
    }
    close($fh);

    --$self->{'recurse'};

    if (!$noincs) {
      $self->{'ifiles'} = {};
      $self->getFiles($file);
      my(@incs) = keys %{$self->{'ifiles'}};
      return \@incs;
    }
  }
}


1;
