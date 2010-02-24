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
use ObjectGeneratorFactory;

# ************************************************************
# Subroutine Section
# ************************************************************

sub new {
  my($class, $macros, $ipaths, $replace, $type, $noinline, $exclude) = @_;
  my $self     = bless {'pre'      => new Preprocessor($macros,
                                                       $ipaths, $exclude),
                        'replace'  => $replace,
                        'dwrite'   => DependencyWriterFactory::create($type),
                        'objgen'   => ObjectGeneratorFactory::create($type),
                        'noinline' => $noinline,
                       }, $class;

  ## Set the current working directory, but
  ## escape regular expression special characters
  $self->{'cwd'} = Cwd::getcwd() . '/';
  $self->{'cwd'} =~ s/([\+\-\\\$\[\]\(\)\.])/\\$1/g;

  ## Sort the replace keys to get the longest key first.  This way
  ## when we are replacing portions of the file path, we replace the
  ## most we can.
  my @repkeys = sort { length($b) <=> length($a) } keys %$replace;
  $self->{'repkeys'} = \@repkeys;

  return $self;
}


sub process {
  my($self, $file) = @_;

  ## Generate the dependency string
  my $depstr = $self->{'dwrite'}->process(
                  $self->{'objgen'}->process($file),
                  $self->{'pre'}->process($file, $self->{'noinline'}));

  ## Perform the replacements on the dependency string
  $depstr =~ s/$self->{'cwd'}//go;
  my $replace = $self->{'replace'};
  foreach my $rep (@{$self->{'repkeys'}}) {
    $depstr =~ s/$rep/$$replace{$rep}/g;
  }

  return $depstr;
}


1;
