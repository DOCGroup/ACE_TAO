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

my $client = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

my $iorbase = "oc.ior";
my $client_iorfile = $client->LocalFile ($iorbase);

$client_conf_base = "oc_svc$PerlACE::svcconf_ext";
my $client_conf = $client->LocalFile ($client_conf_base);
if ($client->PutFile ($client_conf_base) == -1) {
    print STDERR "ERROR: cannot set file <$client_conf>\n";
    exit 1;
}

$CL = $client->CreateProcess ("client", "-k file://$client_iorfile ".
                                        "-orbsvcconf $client_conf ".
                                        "-ORBDebugLevel $debug_level");

my $client_status = $CL->Spawn();

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    exit 1;
}

if ($client->WaitForFileTimed ($iorbase,
                           $client->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$client_iorfile>\n";
    exit 1;
}

$client_status = $CL->WaitKill ($client->ProcessStartWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

exit $status;
