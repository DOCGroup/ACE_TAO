eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

$status = 0;

my $iorbase = "test.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
$server->DeleteFile($iorbase);

$SV = $server->CreateProcess ("server");

print STDERR "\n\n==== Running bug 2936 regression test\n";

$test = $SV->Spawn ();

if ($test != 0) {
    print STDERR "ERROR: test returned $test\n";
    exit 1;
}

$collocated = $SV->WaitKill (30);

if ($collocated != 0) {
    print STDERR "ERROR: Bug_2936_Regression returned $collocated\n";
    $status = 1;
}

$server->DeleteFile($iorbase);

exit $status;
