eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use strict;

if ($^O eq 'hpux' || $^O eq 'lynxos') {
    print "This test will not run properly on HP-UX or LynxOS.\n",
        "When one process uses up all of the file descriptors, no other\n",
        "processes run by the same user can start.\n";
    exit(0);
}

my $status = 0;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $iorbase = "server.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

my $logbase = 'server.log';
my $server_logfile = $server->LocalFile ($logbase);
$server->DeleteFile($logbase);

my $SV = $server->CreateProcess ("server", "-ORBAcceptErrorDelay 5 -ORBDebugLevel 1 -ORBLogFile $server_logfile -o $server_iorfile");
my $CL = $client->CreateProcess ("client", "-k file://$client_iorfile");

my $server_status = $SV->Spawn();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server->WaitForFileTimed ($iorbase,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

my $client_status = $CL->SpawnWaitKill(10 + $client->ProcessStartWaitInterval ());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV->WaitKill($server->ProcessStopWaitInterval ());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

if ($server->GetFile ($logbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_logfile>\n";
    exit 1;
}

if ($status == 0) {
    if (open(FH, $server_logfile)) {
        my $error_achieved = 0;
        my $reregister = 0;
        while(<FH>) {
            if (/TAO_Acceptor::handle_accept_error.+Too many files open/) {
                ++$error_achieved;
            }
            elsif (/TAO_Acceptor::handle_expiration.+registering\s+the\s+acceptor/) {
                ++$reregister;
            }
        }
        close(FH);

        if (!$error_achieved) {
            print "ERROR: The error situation was not achieved\n";
            ++$status;
        }
        if (!$reregister) {
            print "ERROR: The acceptor was not reregistered\n";
            ++$status;
        }
    }
    else {
        print "ERROR: Unable to read $server_logfile\n";
        ++$status;
    }
}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);
$server->DeleteFile($logbase);

exit($status);
