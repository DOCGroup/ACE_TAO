eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use PerlACE::TestTarget;

$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

#my $target = PerlACE::TestTarget::create_target ($PerlACE::TestConfig);
my $server = PerlACE::TestTarget::create_target (1);
#my $client = new PerlACE::TestTarget;
my $client = PerlACE::TestTarget::create_target (2);
if (!defined $server || !defined $client) {
    exit 1;
}

my $iorbase = "server.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
$server->DeleteFile($server_iorfile);
$client->DeleteFile($client_iorfile);

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("Bug_3499_Regression", "-ORBGestalt CURRENT");
}
else {
    $SV = $server->CreateProcess ("Bug_3499_Regression", "-ORBGestalt CURRENT");
}

$SV->SpawnWaitKill (1000);

exit 0;
