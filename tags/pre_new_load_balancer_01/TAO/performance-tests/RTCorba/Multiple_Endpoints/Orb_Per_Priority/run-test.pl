eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../../../bin';
use PerlACE::Run_Test;

$client_conf = PerlACE::LocalFile ("client.conf");
$server_conf = PerlACE::LocalFile ("server.conf");
$iorfile = PerlACE::LocalFile ("ior");

$SV = new PerlACE::Process ("server",
                            " -ORBSvcConf $server_conf"
                            . " -t 1 -t 2 -t 3 -t 4 -t 5 -t 30 "
                            . " -o $iorfile");

$CL = new PerlACE::Process ("client",
                            " -ORBSvcConf $client_conf "
                            . " -i file://$iorfile "
                            . " -t 1 -t 2 -t 3 -t 4 -t 5 -t 30 -n 40000");
                       
print STDERR "================ ORB Per Priority Test\n";

unlink $iorfile."_1";
unlink $iorfile."_2";
unlink $iorfile."_3";
unlink $iorfile."_4";
unlink $iorfile."_5";
unlink $iorfile."_30";

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile."_1", 5) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (120);
$SV->Kill (10);

unlink $iorfile."_1";
unlink $iorfile."_2";
unlink $iorfile."_3";
unlink $iorfile."_4";
unlink $iorfile."_5";
unlink $iorfile."_30";

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    exit 1;
}

exit 0;
