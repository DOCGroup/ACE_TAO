eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

$usage = "run_tests.pl [-l suppress -ORBgioplite] [-n iterations] [-r, for thread-per-rate] [-t low priority threads]\n";

use lib "../../../../../bin";
require ACEutils;

$iorfile = "mtcubit.ior";
$sleeptime = 3;
$gioplite = '-ORBgioplite';
$iterations = 1000;
$low_priority_threads = 1;
$thread_per_rate = '';

####
#### Process command line args.
####
while ($#ARGV >= $[  &&  $ARGV[0] =~ /^-/) {
    if ($ARGV[0] eq '-l') {
        $gioplite = '';
    } elsif ($ARGV[0] eq '-n') {
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

$SV = Process::Create ('.' . $DIR_SEPARATOR . "server" . $EXE_EXT,
                       " $gioplite $thread_per_rate -f $iorfile -t $threads");

sleep $sleeptime;

if (ACE::waitforfile_timed ($iorfile, 10) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

$CL = Process::Create ('.' . $DIR_SEPARATOR . "client" . $EXE_EXT .
                  " $gioplite $thread_per_rate " .
                  "-f $iorfile -n $iterations -t $threads");

$client = $CL->TimedWait (60);
if ($client == -1) {
  print STDERR "ERROR: client timedout\n";
  $CL->Kill (); $CL->TimedWait (1);
}

$SV->Terminate (); $server = $SV->TimedWait (10);
if ($server == -1) {
  print STDERR "ERROR: server could not be terminated\n";
  $SV->Kill (); $SV->TimedWait (1);
}

unlink $iorfile;

exit $client == 0  ?  0  :  1;
