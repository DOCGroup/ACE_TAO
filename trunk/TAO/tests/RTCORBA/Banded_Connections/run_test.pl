eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../../bin';
use PerlACE::Run_Test;

$status = 0;

$iorfile1 = PerlACE::LocalFile ("test1.ior");
$iorfile2 = PerlACE::LocalFile ("test2.ior");
$server_conf = PerlACE::LocalFile ("server.conf");

unlink $iorfile1;
unlink $iorfile2;

print STDERR "\n********** RTCORBA Priority Banded Connections Unit Test\n";


# CORBA priorities 66, 75 and 80, etc. are for the SCHED_OTHER class on
# Solaris.  May need to use different values for other platforms
# depending on their native priorities scheme, i.e., based on the
# available range.

$server_args =
    "-n $iorfile1 -o $iorfile2 -b bands.unix -ORBSvcConf $server_conf "
    ."-p 67 -w 78 "
    ."-ORBendpoint iiop://$TARGETHOSTNAME:0/priority=69 "
    ."-ORBendpoint iiop://$TARGETHOSTNAME:0/priority=76 "
    ."-ORBendpoint iiop://$TARGETHOSTNAME:0/priority=80 ";

$client_args =
    "-n file://$iorfile1 -o file://$iorfile2 "
    ."-a 67 -b 75 -c 90";

if ($^O eq "MSWin32") {
    $server_args =
        "-n $iorfile1 -o $iorfile2 -b bands.nt -ORBSvcConf $server_conf "
            ."-p 1 -w 7 "
            ."-ORBendpoint iiop://$TARGETHOSTNAME:0/priority=2 "
                ."-ORBendpoint iiop://$TARGETHOSTNAME:0/priority=4 "
                    ."-ORBendpoint iiop://$TARGETHOSTNAME:0/priority=6 ";

    $client_args =
        "-n file://$iorfile1 -o file://$iorfile2 "
            ."-a 2 -b 4 -c 6 ";
}

$SV = new PerlACE::Process ("server", $server_args);
$CL = new PerlACE::Process ("client", $client_args);

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

$server = $SV->WaitKill (30);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile1;
unlink $iorfile2;

# Clean up SHMIOP files
PerlACE::check_n_cleanup_files ("server_shmiop_*");

exit $status;
