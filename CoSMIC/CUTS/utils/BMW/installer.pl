# $Id$
# -*- perl -*-

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# PERL settings
use warnings;
use strict;

# Import PerlACE modules/libraries
use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use Cwd;

# Func. prototype
sub show_help ();

my ($server) = 'iis';
my ($alias)  = 'CUTS';
my ($install) = 1;
my ($root) = 'C:/Inetpub';
my ($arg);

# Parse the command line arguments
while ($arg = shift)
{
  $arg eq '--install' and $install = 1;
  $arg eq '--uninstall' and $install = 0;
  $arg eq '--root' and $root = shift;
  $arg eq '--help' and show_help () and exit 0;
}

if ($server eq 'iis')
{
  # Verify Microsoft IIS is indeed installed on this computer.
  if (!-d "$root/AdminScripts")
  {
    print "Microsoft IIS is not installed on at $root\n";
    exit 1;
  }

  my ($cmd) = 'C:/WINDOWS/System32/CScript';
  my ($installer) = new PerlACE::Process ($cmd);

  if ($install)
  {
    # Install CUTS into the Microsoft IIS server.
    my ($cwd) = cwd;
    my ($website) = 'Default Web Site';

    # Convert forward slashes to backslashes
    $cwd =~ s/\//\\/g;

    # Create the VirtualDir for CUTS.
    my ($args) = "$root/AdminScripts/mkwebdir.vbs //Nologo " .
                 "--computer localhost -w \"$website\" -v \"$alias,$cwd\"";
    $installer->Arguments ($args);
    $installer->SpawnWaitKill ();

    # Perform the final adminstration steps.
    my ($adsutil) = "$root/AdminScripts/adsutil.vbs //Nologo";
    my (@commands) = ('APPCREATEPOOLPROC W3SVC/1/Root/CUTS',
                      'SET W3SVC/1/Root/CUTS/AppFriendlyName "CUTS"',
                      'SET W3SVC/1/Root/CUTS/AppIsolated 2',
                      'SET W3SVC/1/Root/CUTS/AccessScript True');

    my ($command);
    for $command (@commands)
    {
      $installer->Arguments ($adsutil . ' ' . $command);
      $installer->SpawnWaitKill ();
    }
  }
  else
  {
    # Uninstall CUTS from the Microsoft IIS server.
    my ($args) = "$root/AdminScripts/adsutil.vbs //Nologo DELETE W3SVC/1/Root/CUTS";

    $installer->Arguments ($args);
    $installer->SpawnWaitKill ();
  }
}

#
# show_help
#
sub show_help ()
{
  print "USAGE: installer.pl [--install|--uninstall] [OPTIONS]\n\n" .
        "  --install          install the BMW website\n" .
        "  --uninstall        uninstall the BMW website\n\n" .
        "OPTIONS\n" .
        "  --root             root location of the server\n\n" .
        "The default behavior is to install the BWM website if no arguments\n" .
        "are specified on the command-line.\n";
}

1;
