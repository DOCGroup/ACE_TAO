eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';
$cdebug_level = '0';
foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
    if ($i eq '-cdebug') {
      $cdebug_level = '10';
    }
}

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $interpreter=$^X;
if($^O=~/mswin/i) {
	$interpreter=~s/\.exe$//i; 	# Remove '.exe' from end on Windows since on Windows CreateProcess() will append '.EXE'
}

$SV = $server->CreateProcess ($interpreter, "http_server.pl");
$CL = $client->CreateProcess ("client", "-ORBdebuglevel $cdebug_level");

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

# We need to kill web server process since it will not stop on its own until a certain timeout
# We are not interested in return code for web server
$SV->Kill ();


exit $status;
