eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../../bin';
use PerlACE::Run_Test;

my($debug) = 1;


my($factory1_ior) = PerlACE::LocalFile ("factory1.ior");
my($factory2_ior) = PerlACE::LocalFile ("factory2.ior");
my($replica1_ior) = PerlACE::LocalFile ("replica1.ior");
my($replica2_ior) = PerlACE::LocalFile ("replica2.ior");
my($data_file) = PerlACE::LocalFile ("persistent.dat");

unlink $factory1_ior;
unlink $factory2_ior;
unlink $replica1_ior;
unlink $replica2_ior;
unlink $data_file;
my($status) = 0;

my($SV1) = new PerlACE::Process ("release/ft_replica", "-o $factory1_ior -t $replica1_ior -q -ORBInitRef ReplicationManager=NULL");
my($SV2) = new PerlACE::Process ("release/ft_replica", "-o $factory2_ior -t $replica2_ior -q -ORBInitRef ReplicationManager=NULL");
my($CL) = new PerlACE::Process ("release/ft_client", "-f file://$replica1_ior -f file://$replica2_ior -c testscript");
#my($CL) = new PerlACE::Process ("release/ft_client", "-f file://$replica1_iogr -c testscript");

print "\nTest: Starting replica 1 " . $SV1->CommandLine . "\n" if ($debug);
$SV1->Spawn ();

print "waiting for replica 1's IOR\n" if ($debug);

if (PerlACE::waitforfile_timed ($replica1_ior, 5) == -1) {
    print STDERR "ERROR: cannot find replica 1 file <$replica1_ior>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

print "\nTest: Starting replica 2 " . $SV2->CommandLine . "\n" if ($debug);
$SV2->Spawn ();

print "waiting for replica 2's IOR\n" if ($debug);
if (PerlACE::waitforfile_timed ($replica2_ior, 5) == -1) {
    print STDERR "ERROR: cannot find replica 2 file <$replica2_ior>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
    exit 1;
}

print "\nTest: Starting client " . $CL->CommandLine . "\n" if ($debug);

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

print "wait for server 1.\n" if ($debug);
$server = $SV1->WaitKill (60);

print "wait for server 2.\n" if ($debug);
$server = $SV2->WaitKill (60);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

if ($status == 0) {
print "Clean up scratch files\n" if ($debug);

unlink $factory1_ior;
unlink $factory2_ior;
unlink $replica1_ior;
unlink $replica2_ior;
unlink $data_file;
}

exit $status;
