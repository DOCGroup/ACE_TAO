eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../bin";
require PerlACE::Run_Test;

$poa_file = PerlACE::LocalFile ("POA.cfg");
$obj_file = PerlACE::LocalFile ("Object.cfg");

$SV = new PerlACE::Process ("server", "-ORBSvcConf server.conf"
                                      . " -ORBendpoint iiop://localhost:0/priority=5"
                                      . " -ORBendpoint iiop://localhost:0/priority=15"
                                      . " -POAConfigFile $poa_file"
                                      . " -ObjectConfigFile $obj_file");

$CL = new PerlACE::Process ("client", "-POAConfigFile $poa_file -ObjectConfigFile $obj_file");

$status = 0;

$SV->Spawn ();

sleep (5);

if ($CL->SpawnWaitKill (60) == -1) {
  print STDERR "ERROR: client timedout\n";
  $CL->Kill (); 
  exit 1;
}

$SV->Kill ();

exit 0;
