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
use FileHandle;

use Preprocessor;
use DependencyWriterFactory;

# ************************************************************
# Subroutine Section
# ************************************************************

sub new {
  my($class)   = shift;
  my($pre)     = shift;
  my($macros)  = shift;
  my($options) = shift;
  my($ipaths)  = shift;
  my($self)    = bless {'pre' => new Preprocessor($pre, $macros,
                                                  $options, $ipaths),
                       }, $class;
  return $self;
}


sub process {
  my($self)     = shift;
  my($file)     = shift;
  my($objects)  = shift;
  my($exclude)  = shift;
  my($replace)  = shift;
  my($type)     = shift;
  my(%split)    = ();
  my(@files)    = ();

  ## Preprocess the file
  foreach my $line (@{$self->{'pre'}->process($file)}) {
    if ($line =~ /^#\s*(line\s+)?\d+\s+\"([\w\\\/:\s\.\-\+\~]+)/) {
      $split{$2} = 1;
    }
  }

  ## Go through each file and exclude those that need exclusion
  ## and modify those that have elements for replacement
  my($skip) = 0;
  foreach my $file (keys %split) {
    foreach my $exc (@$exclude) {
      if ($file =~ /^$exc/i) {
        $skip = 1;
        last;
      }
    }
    if ($skip) {
      $skip = 0;
    }
    else {
      foreach my $rep (keys %$replace) {
        if ($file =~ /^$rep/) {
          $file =~ s/^$rep/$$replace{$rep}/;
          last;
        }
      }
      $file =~ s/\\\\/\\/g;
      $file =~ s/\/\//\\/g;
      push(@files, $file);
    }
  }

  ## Sort the dependencies to make them reproducible
  @files = sort @files;

  ## Allocate the correct type of dependency writer
  ## and generate the dependency string
  my($dwrite) = DependencyWriterFactory::create($type);
  return $dwrite->process($objects, \@files);
}


1;
