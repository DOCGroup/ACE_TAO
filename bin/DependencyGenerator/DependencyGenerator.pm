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
  my($macros)  = shift;
  my($options) = shift;
  my($ipaths)  = shift;
  my($replace) = shift;
  my($type)    = shift;
  my($self)    = bless {'pre'     => new Preprocessor($macros,
                                                      $options, $ipaths),
                        'replace' => $replace,
                        'dwrite'  => DependencyWriterFactory::create($type),
                        'cwd'     => undef,
                       }, $class;

  ## Set the current working directory, but
  ## escape regular expression special characters
  $self->{'cwd'} = $self->escape_regex_special(Cwd::getcwd()) . '/';

  return $self;
}


sub escape_regex_special {
  my($self) = shift;
  my($name) = shift;

  $name =~ s/([\+\-\\\$\[\]\(\)\.])/\\$1/g;
  return $name;
}


sub process {
  my($self)    = shift;
  my($file)    = shift;
  my($objects) = shift;
  my($replace) = $self->{'replace'};
  my(@repkeys) = keys %$replace;
  my($cwd)     = $self->{'cwd'};
  my(@files)   = @{$self->{'pre'}->process($file)};

  ## Go through each file
  foreach my $finc (@files) {
    ## Remove the current working directory from the front of the
    ## file.  This will help reduce the size of the dependency file.
    $finc =~ s/^$cwd//;

    ## Modify those that have elements for replacement
    foreach my $rep (@repkeys) {
      if ($finc =~ /^$rep/) {
        $finc =~ s/^$rep/$$replace{$rep}/;
        last;
      }
    }

    ## Remove extra slashes
    $finc =~ s/\/\//\\/g;
  }

  ## Sort the dependencies to make them reproducible
  @files = sort @files;

  ## Generate the dependency string
  return $self->{'dwrite'}->process($objects, \@files);
}


1;
