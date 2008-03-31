eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# run_test.pl,v 1.5 2006/01/31 21:54:08 wotte Exp
# -*- perl -*-

use lib "$ENV{'ACE_ROOT'}/bin";
use PerlACE::Run_Test;

$CIAO_ROOT = "$ENV{'CIAO_ROOT'}";
$TAO_ROOT = "$ENV{'TAO_ROOT'}";
$DAnCE = "$ENV{'CIAO_ROOT'}/DAnCE";
$ns_running = 0;
$nsior = PerlACE::LocalFile ("ns.ior");

unlink $nsior;

# Invoke naming service

$NS = new PerlACE::Process ("$TAO_ROOT/orbsvcs/Naming_Service/Naming_Service", "-m 0 -ORBEndpoint iiop://roon.dre.vanderbilt.edu:40000 -o ns.ior");

$NS->SpawnWaitKill (9999999);

 print STDERR "Starting Naming Service\n";

 if (PerlACE::waitforfile_timed ($nsior, 10) == -1)
 {
     print STDERR "ERROR: cannot find naming service IOR file\n";
     $NS->Kill ();
     exit 1;
 }

exit $status;
