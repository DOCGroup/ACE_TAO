eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../bin";
use PerlACE::Run_Test;

$status = 0;

$threads = '4';

$iorfile = PerlACE::LocalFile ("test.ior");
$server_conf = PerlACE::LocalFile ("server.conf");
$client_conf = PerlACE::LocalFile ("client.conf");
$st_server_conf = PerlACE::LocalFile ("st_server.conf");
$st_client_conf = PerlACE::LocalFile ("st_client.conf");
$client_st_muxed_conf = PerlACE::LocalFile ("client-st-muxed.conf");

unlink $iorfile;

print STDERR "================ Multi-threaded test\n";

$SV = new PerlACE::Process ("server",
                            " -ORBSvcConf $server_conf -o $iorfile"
                            . " -n $threads");
$CL = new PerlACE::Process ("client",
                            " -ORBSvcConf $client_conf -k file://$iorfile "
                            . " -n $threads -i 1000");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 5) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); 
    exit 1;
}

$client = $CL->SpawnWaitKill (60);
$server = $SV->WaitKill (5);

unlink $iorfile;

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

print STDERR "================ Single-threaded test\n";

unlink $iorfile;

$SV = new PerlACE::Process ("st_server",
                            " -ORBSvcConf $st_server_conf"
                            . " -o $iorfile");
$CL = new PerlACE::Process ("st_client",
                            " -ORBSvcConf $st_client_conf "
                            . " -k file://$iorfile "
                            . " -i 1000");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 5) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); 
    exit 1;
}

$client = $CL->SpawnWaitKill (60);
$server = $SV->WaitKill (5);

unlink $iorfile;

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

print STDERR "================ Deferred synchronous test\n";

unlink $iorfile;

$SV = new PerlACE::Process ("st_server",
                            " -ORBSvcConf $st_server_conf"
                            . " -o $iorfile");
$CL = new PerlACE::Process ("deferred_synch_client",
                            " -ORBSvcConf $client_st_muxed_conf "
                            . " -k file://$iorfile "
                            . " -i 1000");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 5) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); 
    exit 1;
}

$client = $CL->SpawnWaitKill (60);
$server = $SV->WaitKill (5);

unlink $iorfile;

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

exit $status;
