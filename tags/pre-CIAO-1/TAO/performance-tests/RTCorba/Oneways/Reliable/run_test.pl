eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../../bin";
use PerlACE::Run_Test;

$iterations = 4000;
$bufsize = 4000;
$work = 10;
$type = "";
$level = "object";
$shutdown = "";
$all = 1;
$other = "";

$status = 0;

$iorfile = PerlACE::LocalFile ("test.ior");
unlink $iorfile;

# Parse the arguments

@types = ("none", "transport", "server", "target", "twoway -x");

@bufsizes = (10, 40, 100, 400, 1000, 2000);

for ($i = 0; $i <= $#ARGV; $i++) {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?") {
        print "Run_Test Perl script for TAO Reliable Oneways Test\n\n";
        print "run_test [-t test type] [-i iterations] [-m buffer size]\n";
        print "\n";
        print "-t test type        -- runs only one type of oneway test\n";
        print "-i iterations       -- number of calls in each test\n";
        print "-l level      -- level at which policy is set\n";
        print "-m buffer size      -- queue size for buffered oneways\n";
        print "-w servant work   -- number of loops of 1000 by servant\n";
        exit 0;
    }
    elsif ($ARGV[$i] eq "-i") {
        $iterations = $ARGV[$i + 1];
        $i++;
    }
    elsif ($ARGV[$i] eq "-m") {
        $bufsize = $ARGV[$i + 1];
        $i++;
        last SWITCH;
    }
    elsif ($ARGV[$i] eq "-w") {
        $work = $ARGV[$i + 1];
        $i++;
        last SWITCH;
    }
    elsif ($ARGV[$i] eq "-t") {
        @types = split (',', $ARGV[$i + 1]);
        $all = 0;
        $i++;
    }
    elsif ($ARGV[$i] eq "-l") {
        $level = $ARGV[$i + 1];
        $i++;
    }
    else {
        $other .= $ARGV[$i];
    }
}

###############################################################################

unlink $iorfile;

$SV = new PerlACE::Process ("server");
$CL = new PerlACE::Process ("client");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 5) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); 
    exit 1;
}

###############################################################################

if ($all == 1) {
    foreach $bufsize (@bufsizes) {
        print STDERR "\n***************** STARTING TEST ******************\n";
        
        $CL->Arguments ("-ORBNodelay 0 -l $level -t none -i $iterations -m $bufsize");

        my $client = $CL->SpawnWaitKill (60);
        if ($client != 0) {
            print STDERR "ERROR: client returned $client\n";
            $status = 1;
        }
    }
}

###############################################################################

print STDERR "\nSyncScope policy set at the $level level\n";

foreach $type (@types) {
    print STDERR "\n***************** STARTING TEST ******************\n";

    my $transport = "";
    
    if ($type == "none" || $type == "transport") {
        $transport = "-ORBNodelay 0";
    }

    if ($all == 0) {
        $shutdown = "-x";
    }

    $CL->Arguments (" $transport -t $type -i $iterations -l $level -m $bufsize -w $work $shutdown ");

    my $client = $CL->SpawnWaitKill (60);
    if ($client != 0) {
        print STDERR "ERROR: client returned $client\n";
        $status = 1;
    }
}

###############################################################################

$server = $SV->WaitKill (5);
if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
