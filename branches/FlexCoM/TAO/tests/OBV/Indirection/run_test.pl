# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use Env (ACE_ROOT);
use lib "$ACE_ROOT/bin";
use PerlACE::TestTarget;

$iorfile = "server.ior";

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

$server->DeleteFile($iorfile);

my $server_iorfile = $server->LocalFile ($iorfile);

$S = $server->CreateProcess("MessengerServer");
$S->Spawn();

if ($server->WaitForFileTimed ($iorfile, $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile>\n";
    $S->Kill();
    exit 1;
}

$C = $client->CreateProcess("MessengerClient");
$C->Spawn();

$CRET = $C->WaitKill($client->ProcessStartWaitInterval());
$S->Kill();

# clean-up

$server->DeleteFile($iorfile);

if ($CRET != 0) {
    print STDERR "ERROR: Client returned <$CRET>\n";
    exit 1 ;
}

exit 0;







