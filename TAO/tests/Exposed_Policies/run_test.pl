eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../bin";
require PerlACE::Run_Test;

$poa_file = PerlACE::LocalFile ("POA.cfg");
$obj_file = PerlACE::LocalFile ("Object.cfg");
$server_conf = PerlACE::LocalFile ("server.conf");

$SV = new PerlACE::Process ("server", "-ORBSvcConf $server_conf"
                                      . " -ORBendpoint iiop://localhost:0/priority=1"
                                      . " -ORBendpoint iiop://localhost:0/priority=4"
                                      . " -POAConfigFile $poa_file"
                                      . " -ObjectConfigFile $obj_file");

$CL = new PerlACE::Process ("client", "-POAConfigFile $poa_file -ObjectConfigFile $obj_file");

$status = 0;

$SV->Spawn ();

sleep (5);

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->TerminateWaitKill (10);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

exit $status;
