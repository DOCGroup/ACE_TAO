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
  $arg eq "-p" and $plan = shift;
  $arg eq "--startup" and $startup = 1;
  $arg eq "--shutdown" and $startup = 0;
  $arg eq "--help" and print_help_2() and exit;
}

!defined($plan) and die "*** syntax error: you must specify a plan\n";

# spawn the an instance of the executor
$planner = "$CIAO_ROOT/DAnCE/Plan_Launcher/plan_launcher";
$planner_args = "-p $plan -k file://EM.ior -o DAM.ior";

if (defined ($startup) and $startup == 0)
{
  $planner_args = "-p $plan -k file://EM.ior -i file://DAM.ior";
}

$planner_daemon = new PerlACE::Process ($planner, $planner_args);

if (defined ($startup))
{
  $planner_daemon->Spawn();
}
else
{
  $planner_daemon->SpawnWaitKill(999999999);
}

#
# print_help
#
sub print_help_2
{
  print "  SYNTAX: wlg_planner.pl -p <deployment.cdp> [--startup | --shutdown]\n";
  return 1;
}

1;
