# $Id$
# -*- perl -*-

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use warnings;
use strict;

# Import the necessary modules/libraries
use Win32::TieRegistry;

my ($input) = 'install.dat';

# Get the command-line arguments
while (my ($arg) = @ARGV)
{
  $arg eq "-f" and $input = shift;
  $arg eq "--help" and show_help () and exit 0;
}

# Verify we can locate the $COSMIC_ROOT environment variable.
!exists $ENV{'COSMIC_ROOT'} and die "error: COSMIC_ROOT environment variable not found\n";
my ($cosmic) = $ENV{'COSMIC_ROOT'};

$Registry->Delimiter("/");
my ($gme) = $Registry->{"HKEY_CURRENT_USER/Software/GME/Components/"}
            or die "Cannot find GME component registry information: $^E\n";

my ($hkcr) = $Registry->{"HKEY_CLASSES_ROOT/"}
            or die "Cannot find CLSID information: $^E\n";

open INPUT, 'install.dat' or die $!;

while (<INPUT>)
{
  # This should be replace w/ tagged regular expression
  chomp;

  my (@values) = split;
  my ($count) = scalar (@values);

  if ($count != 5)
  {
    print "error: invalid line [$_]\n";
  }
  else
  {
    register_component ($gme,
                        $hkcr,
                        $values[1],
                        $values[2],
                        $values[3],
                        $values[0],
                        "$cosmic\\bin\\$values[4]");
  }
}

close INPUT;


sub register_component
{
  my ($gme) =  shift;
  my ($hkcr) = shift;
  my ($name) = shift;
  my ($guid) = shift;
  my ($paradigm) = shift;
  my ($type) = shift;
  my ($server) = shift;

  # Create the GME registry entry.
  my ($entry) = {'Paradigm'       => $paradigm,
                 'Description'    => "$name GME component",
                 'Icon'           => ',IDI_COMPICON',
                 'Type'           => [pack('L', 1), 'REG_DWORD'],
                 'Associated/'    =>
                    {"/$paradigm" => ''}};

  $gme->{"$name/"} = $entry;

  # Create the ProgID entry for the component.
  $entry = {'/'       => $name,
            'CLSID/'  =>
              {'/'    => $guid}};

  $hkcr->{"$name/"} = $entry;

  # Create the CLSID entry for the component.
  my($clsid) = $hkcr->{'CLSID/'};

  $entry = {'/'               => $name,
            'InProcServer32/' => {'/' => $server},
            'ProgID/'         => {'/' => $name}};
  $clsid->{"$guid/"} = $entry;
}

1;

sub show_help
{
  print "USAGE: install.pl [OPTIONS]\n\n" .
        "Installs the CoSMIC components for GME.\n\n" .
        "OPTIONS\n" .
        "    -f                  input file (default is install.dat)\n" .
        "    --help              show this help screen\n\n";
}
