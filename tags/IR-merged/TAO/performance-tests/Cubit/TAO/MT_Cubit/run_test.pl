# $Id$
# -*- perl -*-
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

$usage = "run_tests.pl [-l suppress -ORBgioplite] [-n iterations] [-r, for thread-per-rate] [-t low priority threads]\n";

unshift @INC, '../../../../../bin';
require Process;

$server_port = 0;
$iorfile = "theior";
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

$SV = Process::Create ('.' . $DIR_SEPARATOR . "server" . $Process::EXE_EXT,
                       " -ORBport " . $server_port .
                       " $gioplite $thread_per_rate -f $iorfile -t $threads");

sleep $sleeptime;

$status = system ('.' . $DIR_SEPARATOR . "client" . $Process::EXE_EXT .
                  " $gioplite $thread_per_rate " .
                  "-f $iorfile -n $iterations -t $threads");


# @@ TODO change to Wait() once the -x option works.
$SV->Kill (); $SV->Wait ();

unlink $iorfile;

# @@ Capture any errors from the server too.
exit $status;
