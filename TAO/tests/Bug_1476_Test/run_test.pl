eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';
$use_file = 0;

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
    elsif ($i eq '-file') {
        $use_file = 1;
    }
}

my $client = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

my $iorbase = "test.ior";
my $client_iorfile = $client->LocalFile ($iorbase);
my $ior_arg = ($use_file == 0) ? "corbaloc::1.2\@192.100.0.0:49440/no_object" : "file://$client_iorfile";

$CL = $client->CreateProcess ("client");

my @synchs = ("none","delayed");
my @levels = ("obj", "orb", "thread");

foreach $synch (@synchs) {
    foreach $level (@levels) {
        $CL->Arguments ("-ORBDebuglevel $debug_level -k $ior_arg -s$synch -l$level");

        $client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 285);

        if ($client_status != 0) {
            print STDERR "ERROR: client returned $client_status\n";
            $status = 1;
        }

        print STDOUT "----------\n";
    }
}

exit $status;
