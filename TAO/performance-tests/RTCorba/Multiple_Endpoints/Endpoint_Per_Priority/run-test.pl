eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../../../bin';
use PerlACE::Run_Test;

$client_conf = PerlACE::LocalFile ("client.conf");
$server_conf = PerlACE::LocalFile ("server.conf");
$iorfile = PerlACE::LocalFile ("test.ior");

$SV = new PerlACE::Process ("server",
                            " -ORBSvcConf $server_conf "
                            . " -ORBEndPoint iiop://localhost:0/priority=1 "
                            . " -ORBEndPoint iiop://localhost:0/priority=2 "
                            . " -ORBEndPoint iiop://localhost:0/priority=3 "
                            . " -ORBEndPoint iiop://localhost:0/priority=4 "
                            . " -ORBEndPoint iiop://localhost:0/priority=5 "
                            . " -o $iorfile");
$CL = new PerlACE::Process ("client",
                            " -ORBSvcConf $client_conf "
                            . " -i file://$iorfile "
                            . " -t 1 -t 2 -t 3 -t 4 -t 5 -n 4000");
                             
print STDERR "================ Endpoint Per Priority Test\n";

unlink $iorfile;

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 5) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); 
    exit 1;
}

$client = $CL->SpawnWaitKill(60);
$SV->Kill ();

unlink $iorfile;

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    exit 1;
}

exit 0;
