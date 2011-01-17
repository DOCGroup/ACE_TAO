eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

########################
#command line options
#set defaults:
my($status) = 0;

my($verbose) = 0;         # 1: report perl actions before executing them
my($debug_builds) = 0;    # 0: use exes from Release directories

foreach $i (@ARGV) {
    if ($i eq "--debug_build"){
        $debug_builds = 1;
    }
    elsif ($i eq "-v"){
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

my $server1 = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $server2 = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $client = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

######## Define files for targets

my $factory1_ior = "factory1.ior";
my $factory2_ior = "factory2.ior";
my $replica1_ior = "replica1.ior";
my $replica2_ior = "replica2.ior";

my $data_file = "persistent.dat";

# Files which used by server1
my $server1_factory1_ior = $server1->LocalFile ($factory1_ior);
my $server1_replica1_ior = $server1->LocalFile ($replica1_ior);
my $server1_data_file = $server1->LocalFile ($data_file);

# Files which used by server2
my $server2_factory2_ior = $server2->LocalFile ($factory2_ior);
my $server2_replica2_ior = $server2->LocalFile ($replica2_ior);
my $server2_data_file = $server2->LocalFile ($data_file);

# Files which used by client
my $client_replica1_ior = $client->LocalFile ($replica1_ior);
my $client_replica2_ior = $client->LocalFile ($replica2_ior);

# remove all files
$server1->DeleteFile ($factory1_ior);
$server1->DeleteFile ($replica1_ior);
$server1->DeleteFile ($data_file);
$server2->DeleteFile ($factory2_ior);
$server2->DeleteFile ($replica2_ior);
$server2->DeleteFile ($data_file);
$client->DeleteFile ($replica1_ior);
$client->DeleteFile ($replica2_ior);

### Create process
$SV1 = $server1->CreateProcess ("./$build_directory/ft_replica",
                              "-o $server1_factory1_ior -t $server1_replica1_ior -q -f none -p $server1_data_file");

$SV2 = $server2->CreateProcess ("./$build_directory/ft_replica",
                              "-o $server2_factory2_ior -t $server2_replica2_ior -q -f none -p $server2_data_file");

$CL = $client->CreateProcess ("./$build_directory/ft_client",
                              "-f file://$client_replica1_ior -f file://$client_replica2_ior -c testscript");



print "\nTest: Starting replica 1 " . $SV1->CommandLine . "\n" if ($verbose);

$server_status = $SV1->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

print "waiting for replica 1's IOR\n" if ($verbose);

if ($server1->WaitForFileTimed ($replica1_ior,
                               $server1->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server1_replica1_ior>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

if ($server1->GetFile ($replica1_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server1_replica1_ior>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}
if ($client->PutFile ($replica1_ior) == -1) {
    print STDERR "ERROR: cannot set file <$client_replica1_ior>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}


print "\nTest: Starting replica 2 " . $SV2->CommandLine . "\n" if ($verbose);

$server_status = $SV2->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

print "waiting for replica 2's IOR\n" if ($verbose);

if ($server2->WaitForFileTimed ($replica2_ior,
                               $server2->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server2_replica2_ior>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
    exit 1;
}
if ($server2->GetFile ($replica2_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server2_replica2_ior>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
    exit 1;
}
if ($client->PutFile ($replica2_ior) == -1) {
    print STDERR "ERROR: cannot set file <$client_replica2_ior>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
    exit 1;
}

print "\nTest: Starting client " . $CL->CommandLine . "\n" if ($verbose);

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 45);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

print "wait for server 1.\n" if ($verbose);
$server_status = $SV1->WaitKill ($server1->ProcessStopWaitInterval() + 50);

if ($server_status != 0) {
    print STDERR "TEST ERROR: server returned $server_status\n";
    $status = 1;
}

print "wait for server 2.\n" if ($verbose);
$server_status = $SV2->WaitKill ($server2->ProcessStopWaitInterval() + 50);

if ($server_status != 0) {
    print STDERR "TEST ERROR: server returned $server_status\n";
    $status = 1;
}

print "Clean up scratch files\n" if ($verbose);

# remove all files
$server1->DeleteFile ($factory1_ior);
$server1->DeleteFile ($replica1_ior);
$server1->DeleteFile ($data_file);
$server2->DeleteFile ($factory2_ior);
$server2->DeleteFile ($replica2_ior);
$server2->DeleteFile ($data_file);
$client->DeleteFile ($replica1_ior);
$client->DeleteFile ($replica2_ior);

exit $status;
