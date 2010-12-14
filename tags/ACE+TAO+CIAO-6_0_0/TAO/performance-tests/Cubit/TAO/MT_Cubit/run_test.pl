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

$usage = "run_tests.pl [-n iterations] [-r, for thread-per-rate] [-t low priority threads]\n";

$iterations = 1000;
$low_priority_threads = 1;
$thread_per_rate = '';

####
#### Process command line args.
####
while ($#ARGV >= $[  &&  $ARGV[0] =~ /^-/) {
    if ($ARGV[0] eq '-n') {
        if ($ARGV[1] =~ /^[\da-zA-Z]+$/) {
            $iterations = $ARGV[1]; shift;
        } else {
            print STDERR "$0:  must provide argument for -n option\n";
            die $usage;
        }
    } elsif ($ARGV[0] eq '-r') {
        $thread_per_rate = '-r';
    } elsif ($ARGV[0] eq '-t') {
        if ($ARGV[1] =~ /^[\da-zA-Z]+$/) {
            $low_priority_threads = $ARGV[1]; shift;
        } else {
            print STDERR "$0:  must provide argument for -n option\n";
            die $usage;
        }
    } elsif ($ARGV[0] eq '-?') {
        print "$usage";
        exit;
    } else {
        print STDERR "$0:  unknown option $ARGV[0]\n";
        die $usage;
    }
    shift;
}

$threads = $low_priority_threads + 1;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $iorbase = "mtcubit.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

$SV = $server->CreateProcess ("server", "-ORBdebuglevel $debug_level $thread_per_rate -f $server_iorfile -t $threads");
$CL = $client->CreateProcess ("client", "$thread_per_rate -f $client_iorfile -t $threads -n $iterations");

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server->WaitForFileTimed ($iorbase,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($server->GetFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($client->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 165);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV->Kill ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

exit $status;
