eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

$usage = "run_tests.pl [-n iterations] [-r, for thread-per-rate] [-t low priority threads]\n";

use lib "../../../../../bin";
use PerlACE::Run_Test;

$iorfile = PerlACE::LocalFile ("mtcubit.ior");
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

# Make sure the file is gone, so we can wait on it.
unlink $iorfile;

$SV = new PerlACE::Process ("server", "$thread_per_rate -f $iorfile -t $threads");
$CL = new PerlACE::Process ("client", "$thread_per_rate -f $iorfile -t $threads -n $iterations");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 10) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (180);
$SV->Kill ();

unlink $iorfile;

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    exit 1;
}

exit 0;
