eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $iorbase1 = "test1.ior";
my $iorbase2 = "test2.ior";
my $client_iorfile1 = $client->LocalFile ("$iorbase1");
my $server_iorfile1 = $server->LocalFile ("$iorbase1");

my $client_iorfile2 = $client->LocalFile ("$iorbase2");
my $server_iorfile2 = $server->LocalFile ("$iorbase2");

$server->DeleteFile($iorbase1);
$client->DeleteFile($iorbase1);

$server->DeleteFile($iorbase2);
$client->DeleteFile($iorbase2);

$status = 0;

$server_args =
    "-a 45 -b 55";

print STDERR "Value is " . $^O;

if ($^O eq "MSWin32") {
    $server_args =
        "-a 1 -b 2";
}
elsif ($^O eq "dec_osf") {
    $server_args =
        "-a 20 -b 30";
}
elsif ($^O eq "hpux") {
    $continuous = 1;
    $server_args =
        "-a 17 -b 29";
}

if ($continuous) {
    $server_args .= " -ORBSvcConf continuous$PerlACE::svcconf_ext";
}

$SV = $server->CreateProcess ("server", "-p $server_iorfile1 -o $server_iorfile2 $server_args");
$CL = $client->CreateProcess ("client", "-p file://$client_iorfile1 -o file://$client_iorfile2");

print STDERR "\n********** RTCORBA SERVER_DECLARED Priority Unit Test\n\n";

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server->WaitForFileTimed ($iorbase2,
                               $server->ProcessStartWaitInterval ()) == -1) {
    $server_status = $SV->TimedWait (1);
    if ($server_status == 2) {
        # Mark as no longer running to avoid errors on exit.
        $SV->{RUNNING} = 0;
        exit $status;
    }
    else {
        print STDERR "ERROR: cannot find file <$server_iorfile2>\n";
        $SV->Kill ();
        exit 1;
    }
}

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval ());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval ());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($iorbase1);
$client->DeleteFile($iorbase1);

$server->DeleteFile($iorbase2);
$client->DeleteFile($iorbase2);

exit $status;
