eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

my $svcconf = "svc.conf.csd";
my $server_svcconf = $server->LocalFile ($svcconf);
if ($server->PutFile ($svcconf) == -1) {
    print STDERR "ERROR: cannot set file <$server_svcconf>\n";
    exit 1;
}


$SV = $server->CreateProcess ("Collocation",  "-ORBSvcConf $server_svcconf");

$test = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval());

if ($test != 0) {
    print STDERR "ERROR: test returned $test\n";
    exit 1;
}

exit 0;
