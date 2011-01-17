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

$synch_with_server_option = "";

if ($ARGV[0] eq 'synch_with_server') {
    $synch_with_server_option = "-s";
}
elsif ($ARGV[0] eq '') {
#synch with transport
}
else {
    print STDERR "ERROR: invalid parameter $ARGV[0] \n";
    exit 1;
}

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

$SV = $server->CreateProcess ("server_main", "-ORBdebuglevel $debug_level $synch_with_server_option");

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval() + 45);

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

exit $status;
