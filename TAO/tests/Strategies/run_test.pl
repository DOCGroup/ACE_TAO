eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../bin';
use PerlACE::Run_Test;

$status = 0;
$iorfile    = PerlACE::LocalFile ("test.ior");
$svcconf    = PerlACE::LocalFile ("svc$PerlACE::svcconf_ext");
$advsvcconf = PerlACE::LocalFile ("advanced_svc$PerlACE::svcconf_ext");

print "$svcconf\n";
unlink $iorfile;


# Run with a static configuration

$SV = new PerlACE::Process ("server", "-o $iorfile -ORBSvcConf $svcconf");
$CL = new PerlACE::Process ("client", "-k file://$iorfile -x -ORBSvcConf $svcconf");

print STDERR "\nrunning Strategies test with static config\n\n";

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 5) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (60);

if ($client == -1) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (10);

if ($server == -1) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;


# Run using dynamic configuration (and the Advanced_Resource_Factory)
# This will fail for builds lacking dynamic libraries.

$SV2 = new PerlACE::Process ("server",
                             "-o $iorfile -ORBSvcConf $advsvcconf");
$CL2 = new PerlACE::Process ("client",
                             "-k file://$iorfile -x -ORBSvcConf $advsvcconf");

print STDERR "\nrunning Strategies test with dynamic config\n\n";

$SV2->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 5) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV2->Kill ();
    exit 1;
}

$client = $CL2->SpawnWaitKill (60);

if ($client == -1) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV2->WaitKill (10);

if ($server == -1) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
