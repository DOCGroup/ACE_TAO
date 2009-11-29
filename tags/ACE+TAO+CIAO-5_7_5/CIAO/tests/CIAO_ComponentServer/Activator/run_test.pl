eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';
$ciao_root = "$ENV{CIAO_ROOT}";

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $target = PerlACE::TestTarget::create_target ($PerlACE::TestConfig);

$SV1 = $target->CreateProcess ("./client", "-s $ciao_root/bin/ciao_componentserver");

$server1 = $SV1->SpawnWaitKill (60);

if ($server1 != 0) {
    $target->GetStderrLog();
    print STDERR "ERROR: server1 returned $server\n";
    exit 1;
}

$target->GetStderrLog();

exit $status;
