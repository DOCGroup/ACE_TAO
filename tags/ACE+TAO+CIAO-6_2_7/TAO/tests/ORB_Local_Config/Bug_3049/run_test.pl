eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
$server->AddLibPath ('../lib');

my $conffilebase = "a.conf";
my $conffile = $server->LocalFile ($conffilebase);

# copy the configuation file
if ($server->PutFile ($conffilebase) == -1) {
    print STDERR "ERROR: cannot set file <$conffile>\n";
    return 1;
}

$SV = $server->CreateProcess ("Test");

$test = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval());

if ($test != 0) {
    print STDERR "ERROR: test returned $test\n";
    exit 1;
}

exit 0;
