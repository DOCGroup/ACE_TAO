eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use Sys::Hostname;

my $status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $host = hostname();

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";


$SV = $server->CreateProcess ("server",
                              "-ORBdebuglevel $debug_level " .
                              "-ORBUseSharedProfiles 0 " .
                              "-ORBEndpoint iiop://localhost " .
                              "-ORBEndpoint iiop://${host}");
    
$server_status = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

exit $status;
