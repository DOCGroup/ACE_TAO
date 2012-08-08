eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{'ACE_ROOT'}/bin";
use PerlACE::TestTarget;

$status = 0;

my $server_box = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client_box = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $iorbase = "server.ior";
my $server_iorfile = $server_box->LocalFile ($iorbase);
my $client_iorfile = $client_box->LocalFile ($iorbase);
$server_box->DeleteFile($iorbase);
$client_box->DeleteFile($iorbase);

$port = $server_box->RandomPort ();

## NOTE we duplicate the server's CORBALOC in the client to test for CORBALOC processing of -ORBPreferredInterfaces
$SV = $server_box->CreateProcess ("server", "-o $server_iorfile -u corbaloc:miop:1.0\@1.0-foo-1/[FF01:0:0:0:0:0:25:334]:$port -ORBSvcConf miop_svc_ipv6.conf");
$CL = $client_box->CreateProcess ("client", "-ORBPreferredInterfaces FF01:*=lo -ORBEnforcePreferredInterfaces=1 -ORBSvcConf miop_svc_ipv6.conf -ORBIPMulticastLoop 1 -k corbaloc:miop:1.0\@1.0-foo-1/[FF01:0:0:0:0:0:25:334]:$port");
$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server_box->WaitForFileTimed ($iorbase,
                               $server_box->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($server_box->GetFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}
if ($client_box->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$client_status = $CL->SpawnWaitKill ($client_box->ProcessStartWaitInterval());

if ($^O ne "MSWin32") {
    # This should have failed (on linux). Using IPv6 with a Null interface and a link local mcast
    # should (or at least has been observed to) return a transport error on the send which is swallowed quietly for some reason.
    # See "Error returned from transport" in TAO_UIPMC_Transport<CONNECTION_HANDLER>::send

    if ($client_status != 0) {
        print STDERR "ERROR: Client returned $client_status. We expected this client to 'fail' but to ignore the error silently.\n";
        $status = 1;
    }

    if ($SV->TimedWait(1) != -1)
    {
        print STDERR "ERROR: server has exited. This means the server received the 1st client message. This is unexpected on this platform but may not be wrong\n";
    }

    # Now we run the client again to close down the server cleanly. We specify the 'new' form of preferred interface which seems to be needed
    # and we use the server writen ior file here to test the normal cdr -ORBPreferredInterfaces processing.
    $CL->Arguments("-ORBPreferredInterfaces FF01:*=eth0 -ORBEnforcePreferredInterfaces=1 -ORBIPMulticastLoop 1 -ORBSvcConf miop_svc_ipv6.conf -k file://$client_iorfile");

    $client_status = $CL->SpawnWaitKill ($client_box->ProcessStartWaitInterval());
}

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status trying to shutdown server\n";
    $status = 1;
}

$server_status = $SV->WaitKill ($server_box->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: REGRESSION Server had to be killed and returned $server_status. This means the client message isn't getting through.\n";
    $status = 1;
}

$server_box->DeleteFile($iorbase);
$client_box->DeleteFile($iorbase);

exit $status;
