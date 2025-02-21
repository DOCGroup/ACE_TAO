eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use strict;

my $debug_level = 0;
foreach my $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = 10;
    }
}

my $target = PerlACE::TestTarget::create_target(1) || die "Create target 1 failed\n";

my $iorbase = "server.ior";
my $server_iorfile = $target->LocalFile ($iorbase);
$target->DeleteFile ($iorbase);

my $status = 0;

print "Collocated\n\n";

my $shm = '-ORBSvcConf static_shmiop.conf -ORBListenEndpoints shmiop://';

my $SV = $target->CreateProcess ("shmiop_test", "-ORBDebugLevel $debug_level $shm " .
				 "-s $server_iorfile -c $server_iorfile");

my $server = $SV->SpawnWaitKill ($target->ProcessStartWaitInterval ());

if ($server != 0) {
    print STDERR "ERROR: Collocated test returned $server\n";
    $status = 1;
}

$target->DeleteFile ($iorbase);

exit $status;
