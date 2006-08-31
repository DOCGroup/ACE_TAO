# $Id$
# -*- perl -*-

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# import module and libraries
use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

# get the CIAO_ROOT and WLG_ROOT
$CIAO_ROOT = $ENV{'CIAO_ROOT'};

# parse the command line arguments
while ($arg = shift)
{
    $arg eq "-i" and $datfile = shift;
    $arg eq "--help" and print_manager_help () and exit;
}

# spawn a new Execution_Manager
$em = "$CIAO_ROOT/DAnCE/ExecutionManager/Execution_Manager";
$em_args = "-o EM.ior -i $datfile";

$manager = new PerlACE::Process ($em, $em_args);
$manager->SpawnWaitKill (999999999);
1;

sub print_manager_help
{
  $syntax = "wlg_manager.pl [-i dat_file]";
  %help =
  (
      'i'     => "specify dat file; default = wlg.dat",
      '-help' => "display this help message"
  );

  print "SYNTAX: $syntax\n\n";

  while (($key, $value) = each %help)
  {
    print "  -$key       $value\n";
  }
  1;
}

