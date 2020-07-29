eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

foreach my $protocol ("iiop", "diop") {
    $port = $server->RandomPort ();
    $endpoint = "$protocol://:$port/portspan=2";
    print STDOUT "Creating three $protocol servers with endpoint $endpoint...\n";

    $SV1 = $server->CreateProcess ("server", "-ORBEndpoint $endpoint");
    $SV2 = $server->CreateProcess ("server", "-ORBEndpoint $endpoint");
    $SV3 = $server->CreateProcess ("server", "-ORBEndpoint $endpoint");

    print STDOUT "Starting server 1\n";
    $server_status = $SV1->Spawn ();
    if ($server_status != 0) {
        print STDERR "ERROR: server failed to start: $server_status\n";
        exit 1;
    }
    $server_status = $SV1->Wait (1);
    if ($server_status != -1) {
        print STDERR "ERROR: server exited prematurely: $server_status\n";
        exit 1;
    }
    print STDOUT "Started server 1\n";

    print STDOUT "Starting server 2\n";
    $server_status = $SV2->Spawn ();
    if ($server_status != 0) {
        print STDERR "ERROR: server failed to start: $server_status\n";
        $SV1->Kill ();
        exit 1;
    }
    $server_status = $SV2->Wait (1);
    if ($server_status != -1) {
        print STDERR "ERROR: server exited prematurely: $server_status\n";
        exit 1;
    }
    print STDOUT "Started server 2\n";

    print STDOUT "Starting server 3\n";
    $server_status = $SV3->Spawn ();
    if ($server_status == 0) {
        $server_status = $SV3->Wait (1);
        if ($server_status == -1) {
            print STDERR "ERROR: Last server didn't fail, still running!\n";
            $SV1->Kill ();
            $SV2->Kill ();
            $SV3->Kill ();
            exit 1;
        }
    }
    print STDOUT "Success: server 3 failed to start, stopping...\n";

    $SV1->Kill();
    $SV2->Kill();
    $SV3->Kill();

    print STDOUT "Portspan $protocol test completed successfully.\n\n";
}

exit $status;
