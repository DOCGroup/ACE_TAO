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
  my($ipaths)  = shift;
  return bless {'macros'  => $macros,
                'ipaths'  => $ipaths,
                'files'   => {},
                'ifound'  => {},
                'recurse' => 0,
               }, $class;
}


sub locateFile {
  my($self) = shift;
  my($file) = shift;

  if (exists $self->{'ifound'}->{$file}) {
    return $self->{'ifound'}->{$file};
  }
  else {
    foreach my $dir ('.', @{$self->{'ipaths'}}) {
      if (-r "$dir/$file") {
        $self->{'ifound'}->{$file} = "$dir/$file";
        return $self->{'ifound'}->{$file};
      }
    }
  }

  $self->{'ifound'}->{$file} = undef;
  return undef;
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
        ## Remove c++ and same line c comments inside this if statement.
        ## This saves about 5% off of processing the ace directory
        ## and we only need to strip comments if we are actually
        ## going to look at the string.
        $_ =~ s/\/\/.*//o;
        $_ =~ s/\/\*.*\*\///o;

        if (/#\s*endif/) {
          --$ifcount;
          if (defined $zero[0] && $ifcount == $zero[$#zero]) {
            pop(@zero);
          }
        }
        elsif (/#\s*if\s+0/) {
          push(@zero, $ifcount);
          ++$ifcount;
        }
        elsif (/#\s*if/) {
          ++$ifcount;
        }
        elsif (!defined $zero[0] &&
               /#\s*include\s+[<"]([^">]+)[">]/o) {
          my($inc) = $self->locateFile($1);
          if (defined $inc) {
            $inc =~ s/\\/\//go;
            if (!$noinline ||
                ($recurse == 1 || $inc !~ /\.i(nl)?$/o)) {
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
  }

  ## This has to be outside the if (open(...
  ## If the last file to be processed isn't accessable then
  ## we still need to return the array reference of includes.
  if (!$noincs) {
    my(@files)  = ($file);
    my(%ifiles) = ();

    for(my $i = 0; $i <= $#files; ++$i) {
      foreach my $inc (@{$self->{'files'}->{$files[$i]}}) {
        if (!defined $ifiles{$inc}) {
          $ifiles{$inc} = 1;
          push(@files, $inc);
        }
      }
    }
    shift(@files);
    return \@files;
  }
}


1;
