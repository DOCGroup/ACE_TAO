package DependencyGenerator;

# ************************************************************
# Description   : Runs the correct dependency generator on the file.
# Author        : Chad Elliott
# Create Date   : 2/10/2002
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use Preprocessor;
use DependencyWriterFactory;

# ************************************************************
# Subroutine Section
# ************************************************************

sub new {
  my($class)    = shift;
  my($macros)   = shift;
  my($ipaths)   = shift;
  my($replace)  = shift;
  my($type)     = shift;
  my($noinline) = shift;
  my($self)     = bless {'pre'      => new Preprocessor($macros, $ipaths),
                         'replace'  => $replace,
                         'dwrite'   => DependencyWriterFactory::create($type),
                         'noinline' => $noinline,
                        }, $class;

  ## Set the current working directory, but
  ## escape regular expression special characters
  $self->{'cwd'} = Cwd::getcwd() . '/';
  $self->{'cwd'} =~ s/([\+\-\\\$\[\]\(\)\.])/\\$1/g;

  ## Sort the replace keys to get the longest key first.  This way
  ## when we are replacing portions of the file path, we replace the
  ## most we can.
  my(@repkeys) = sort { length($b) <=> length($a) } keys %$replace;
  $self->{'repkeys'} = \@repkeys;

  return $self;
}


sub process {
  my($self)    = shift;
  my($file)    = shift;
  my($objects) = shift;
  my($replace) = $self->{'replace'};
  my($cwd)     = $self->{'cwd'};
  my($files)   = $self->{'pre'}->process($file, $self->{'noinline'});

  ## Go through each file
  foreach my $finc (@$files) {
    ## If we can remove the current working directory fromm the file
    ## then we do not need to check the repkeys array and that cuts
    ## the processing time for the ace directory almost in half.
    if ($finc =~ s/^$cwd//o) {
    }
    else {
      ## Modify those that have elements for replacement
      foreach my $rep (@{$self->{'repkeys'}}) {
        if ($finc =~ s/^$rep/$$replace{$rep}/) {
          last;
        }
      }
    }
  }

  ## Generate the dependency string
  return $self->{'dwrite'}->process($objects, $files);
}


1;
