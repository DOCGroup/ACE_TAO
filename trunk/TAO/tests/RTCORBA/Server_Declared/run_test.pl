eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../../bin';
use PerlACE::Run_Test;

$iorfile1 = PerlACE::LocalFile ("test1.ior");
$iorfile2 = PerlACE::LocalFile ("test2.ior");

unlink $iorfile1;
unlink $iorfile2;
$status = 0;

$server_conf = PerlACE::LocalFile ("server.conf");

# CORBA priorities 65, 70 and 75 are for the SCHED_OTHER class on
# Solaris.  May need to use different values for other platforms
# depending on their native priorities scheme, i.e., based on the
# available range.

$server_args =
    "-p $iorfile1 -o $iorfile2 -a 65 -b 75 -c 70 -ORBSvcConf $server_conf "
    ."-ORBendpoint iiop://$TARGETHOSTNAME:0/priority=65 "
    ."-ORBendpoint iiop://$TARGETHOSTNAME:0/priority=75 "
    ."-ORBendpoint iiop://$TARGETHOSTNAME:0/priority=73 ";

print STDERR "Value is " . $^O;

if ($^O eq "MSWin32") {
    $server_args =
        "-p $iorfile1 -o $iorfile2 -a 3 -b 5 -c 2 -ORBSvcConf $server_conf "
        ."-ORBendpoint iiop://$TARGETHOSTNAME:0/priority=3 "
        ."-ORBendpoint iiop://$TARGETHOSTNAME:0/priority=5 "
        ."-ORBendpoint iiop://$TARGETHOSTNAME:0/priority=1 ";
}
elsif ($^O eq "dec_osf") {
    $server_args =
        "-p $iorfile1 -o $iorfile2 -a 20 -b 30 -c 25 -ORBSvcConf $server_conf "
        ."-ORBendpoint iiop://$TARGETHOSTNAME:0/priority=20 "
        ."-ORBendpoint iiop://$TARGETHOSTNAME:0/priority=30 "
        ."-ORBendpoint iiop://$TARGETHOSTNAME:0/priority=28 ";
}
elsif ($^O eq "hpux") {
    $server_args =
        "-p $iorfile1 -o $iorfile2 -a 17 -b 29 -c 24 -ORBSvcConf $server_conf "
        ."-ORBendpoint iiop://$TARGETHOSTNAME:0/priority=17 "
        ."-ORBendpoint iiop://$TARGETHOSTNAME:0/priority=29 "
        ."-ORBendpoint iiop://$TARGETHOSTNAME:0/priority=27 ";
}

$client_args = "-p file://$iorfile1 -o file://$iorfile2";
print STDERR "server $server_args\n";
$SV = new PerlACE::Process ("server", $server_args);
$CL = new PerlACE::Process ("client", $client_args);

print STDERR "\n********** RTCORBA SERVER_DECLARED Priority Unit Test\n\n";


$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile2, 10) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile2>\n";
    $SV->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (60);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile1;
unlink $iorfile2;

exit $status;
