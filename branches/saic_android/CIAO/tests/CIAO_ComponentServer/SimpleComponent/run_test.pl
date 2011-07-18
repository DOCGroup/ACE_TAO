eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
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
my $host = new PerlACE::TestTarget;

if (PerlACE::is_vxworks_test()) {
    $SV1 = new PerlACE::ProcessVX ("client", "-ORBDebuglevel $debug_level -s $ciao_root/bin/ciao_componentserver -d 120");
}
else {
    $SV1 = $target->CreateProcess ("client", "-ORBDebuglevel $debug_level -s $ciao_root/bin/ciao_componentserver -d 120");
}
    
$server1 = $SV1->SpawnWaitKill (300);

if ($server1 != 0) {
    print STDERR "ERROR: server1 returned $server\n";
    exit 1;
}

$target->GetStderrLog();

$target->DeleteFile($iorfile);
$host->DeleteFile ($iorbase);

exit $status;
