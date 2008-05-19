eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$iorfile = PerlACE::LocalFile ("server.ior");
unlink $iorfile;
$status = 0;

my $cl_port = PerlACE::random_port();
my $cl_opts = " -k file://$iorfile -ORBDottedDecimalAddresses 1 " .
    "-ORBEndpoint iiop://:$cl_port";

# By default use RW wait startegy
my $sr_svc_opt = "-ORBSvcConf rw.conf";
if ($ARGV[0] eq 'default') {
    $sr_svc_opt = "";
}

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server", "-o server.ior");
}
else {
    $SV = new PerlACE::Process ("server", "-o $iorfile $sr_svc_opt");
}
$CL = new PerlACE::Process ("client", $cl_opts);
$CL2 = new PerlACE::Process ("client", " -s $cl_opts");

print $SV->CommandLine() . "\n";
$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

print $CL->CommandLine() . "\n";
$client = $CL->SpawnWaitKill (60);

# The client crashes, therefore it normally exists with status != 0,
# but a status of -1 would be a serious error.
if ($client == -1) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

print $CL2->CommandLine() . "\n";
$client2 = $CL2->SpawnWaitKill (60);

# The client crashes, therefore it normally exists with status != 0,
# but a status of -1 would be a serious error.
if ($client2 == -1) {
    print STDERR "ERROR: client2 returned $client2\n";
    $status = 1;
}

$server = $SV->WaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
