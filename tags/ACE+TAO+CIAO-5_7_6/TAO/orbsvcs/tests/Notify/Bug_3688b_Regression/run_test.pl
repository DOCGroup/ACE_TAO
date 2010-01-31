eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $iorbase = "test.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
$server->DeleteFile($iorbase);

$SV = $server->CreateProcess ("server", "");

$collocated = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval());

if ($collocated != 0) {
    print STDERR "ERROR: server returned $collocated\n";
    $status = 1;
}

$server->DeleteFile($iorbase);

exit $status;
