eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

########################
#command line options
#set defaults:
my($verbose) = 0;         # 1: report perl actions before executing them
my($debug_builds) = 0;    # 0: use exes from Release directories

foreach $i (@ARGV) {
  if ($i eq "--debug_build")
  {
    $debug_builds = 1;
  }
  elsif ($i eq "-v")
  {
    $verbose += 1;
  }
}

my($build_directory) = "/Release";
if ( $debug_builds ) {
  $build_directory = "";
}

if ( $verbose > 1) {
  print "verbose: $verbose\n";
  print "debug_builds: $debug_builds -> $build_directory\n";
}

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

my($SV1) = new PerlACE::Process ("./$build_directory/ft_replica", "-o $factory1_ior -t $replica1_ior -q -f none");
my($SV2) = new PerlACE::Process ("./$build_directory/ft_replica", "-o $factory2_ior -t $replica2_ior -q -f none");
my($CL) = new PerlACE::Process ("./$build_directory/ft_client", "-f file://$replica1_ior -f file://$replica2_ior -c testscript");

print "\nTest: Starting replica 1 " . $SV1->CommandLine . "\n" if ($verbose);
$SV1->Spawn ();

print "waiting for replica 1's IOR\n" if ($verbose);

if (PerlACE::waitforfile_timed ($replica1_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "TEST ERROR: cannot find replica 1 file <$replica1_ior>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

print "\nTest: Starting replica 2 " . $SV2->CommandLine . "\n" if ($verbose);
$SV2->Spawn ();

print "waiting for replica 2's IOR\n" if ($verbose);
if (PerlACE::waitforfile_timed ($replica2_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "TEST ERROR: cannot find replica 2 file <$replica2_ior>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
    exit 1;
}

print "\nTest: Starting client " . $CL->CommandLine . "\n" if ($verbose);

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "TEST ERROR: client returned $client\n";
    $status = 1;
}

print "wait for server 1.\n" if ($verbose);
$server = $SV1->WaitKill (60);

print "wait for server 2.\n" if ($verbose);
$server = $SV2->WaitKill (60);

if ($server != 0) {
    print STDERR "TEST ERROR: server returned $server\n";
    $status = 1;
}

if ($status == 0) {
print "Clean up scratch files\n" if ($verbose);

unlink $factory1_ior;
unlink $factory2_ior;
unlink $replica1_ior;
unlink $replica2_ior;
unlink $data_file;
}

exit $status;
