eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../../../bin';
use PerlACE::Run_Test;

$status = 0;
$client_conf = PerlACE::LocalFile ("client$PerlACE::svcconf_ext");
$server_conf = PerlACE::LocalFile ("server$PerlACE::svcconf_ext");
$iorfile = PerlACE::LocalFile ("test.ior");

$SV = new PerlACE::Process ("server",
                            " -ORBSvcConf $server_conf"
                            . " -o $iorfile");

# Run client with 5 threads of low priority and 1 thread of high priority.

$CL = new PerlACE::Process ("client",
                            " -ORBSvcConf $client_conf "
                            . " -i file://$iorfile "
                            . " -n 10000 "
                            . " -t 1 -t 2 -t 3 -t 4 -t 5 -t 30");


print STDERR "================ Single Endpoint Test\n";

unlink $iorfile;

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 5) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); 
    exit 1;
}

$client = $CL->SpawnWaitKill (60);
$server = $SV->WaitKill (10);

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
