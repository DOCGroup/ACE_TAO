eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $logger = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

my $iorbase = "Messenger.ior";
my $iorlogger = "Logger.ior";

my $server_iorfile_base = $server->LocalFile ($iorbase);
my $client_iorfile_base = $client->LocalFile ($iorbase);
my $logger_iorfile_logger = $logger->LocalFile ($iorlogger);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);
$logger->DeleteFile($iorlogger);

$SV = $server->CreateProcess ("MessengerServer", "-ORBdebuglevel $debug_level ".
                                                 "-o $server_iorfile_base");
$CL = $client->CreateProcess ("MessengerClient", "-k file://$client_iorfile_base");
$LR = $logger->CreateProcess ("LoggerServer", "-ORBdebuglevel $debug_level ".
                                              "-o $logger_iorfile_logger");
$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server->WaitForFileTimed ($iorbase,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile_base>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($server->GetFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_iorfile_base>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($client->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile_base>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$logger_status = $LR->Spawn ();

if ($logger_status != 0) {
    print STDERR "ERROR: logger returned $logger_status\n";
    exit 1;
}

if ($logger->WaitForFileTimed ($iorlogger,
                               $logger->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$logger_iorfile_logger>\n";
    $LR->Kill (); $LR->TimedWait (1);
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV->Kill ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$logger_status = $LR->Kill ();

if ($logger_status != 0) {
    print STDERR "ERROR: logger returned $logger_status\n";
    $status = 1;
}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);
$logger->DeleteFile($iorlogger);

exit $status;
