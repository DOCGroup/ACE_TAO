eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../bin';
use PerlACE::Run_Test;

$client_conf = PerlACE::LocalFile ("client.conf");
$server_conf = PerlACE::LocalFile ("server.conf");
$iorfile = PerlACE::LocalFile ("test.ior");

$threads='2';
$status = 0;

unlink $iorfile;

$SV = new PerlACE::Process ("server",
                            "-ORBSvcConf $server_conf"
                            . " -ORBEndPoint iiop://localhost:0/priority=0 "
                            . " -ORBEndPoint iiop://localhost:0/priority=1 "
                            . " -ORBEndPoint iiop://localhost:0/priority=2 "
                            . " -ORBEndPoint iiop://localhost:0/priority=3 "
                            . " -ORBEndPoint iiop://localhost:0/priority=4 "
                            . " -o $iorfile -n $threads");

$CL = new PerlACE::Process ("client",
                            "-ORBSvcConf $client_conf "
                            . " -k file://$iorfile "
                            . " -t 0 -t 1 -i 1000");

print STDERR "================ Multi-threaded test\n";

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 10) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); 
    exit 1;
}


$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (60);

if ($server == -1) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
