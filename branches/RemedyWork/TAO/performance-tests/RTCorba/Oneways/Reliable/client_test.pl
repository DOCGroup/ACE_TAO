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

$iterations = 4000;
$bufsize = 4000;
$work = 10;
$type = "";
$all = 1;
$other = "";
$status = 0;

# Parse the arguments

@types = ("none", "transport", "server", "target", "twoway -x");

@bufsizes = (10, 40, 100, 400, 1000, 2000);

for ($i = 0; $i <= $#ARGV; $i++) {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?") {
        print "Run_Test Perl script for TAO Reliable Oneways Test\n\n";
        print "run_test [-t test type] [-i iterations] [-m buffer size]\n";
        print "\n";
        print "-t test type      -- runs only one type of oneway test\n";
        print "-i iterations     -- number of calls in each test\n";
        print "-m buffer size    -- queue size for buffered oneways\n";
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
    else {
        $other .= $ARGV[$i];
    }
}

my $client = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

my $iorbase = "test.ior";
my $client_iorfile = $client->LocalFile ($iorbase);
$client->DeleteFile($iorbase);

$CL = $client->CreateProcess ("client", "");

if ($all == 1) {
    foreach $bufsize (@bufsizes) {
        print STDERR "\n***************** STARTING TEST ******************\n";
        $CL->Arguments ("-k file://$client_iorfile -ORBNodelay 0 -t none ".
                        "-i $iterations -m $bufsize");

        my $client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 45);
        if ($client_status != 0) {
            print STDERR "ERROR: client SpawWaiKill returned $client_status\n";
            $status = 1;
        }
    }
}

foreach $type (@types) {
    my $transport = "";
    my $shutdown = "";

    if ($type == "none" || $type == "transport") {
        $transport = "-ORBNodelay 0";
    }

    if ($all == 0) {
        $shutdown = "-x";
    }

    print STDERR "\n***************** STARTING TEST ******************\n";

    $CL->Arguments ("-k file://$client_iorfile $transport -t $type -i $iterations ".
                    "-m $bufsize -w $work $shutdown");

    my $client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 45);
    if ($client_status != 0) {
        print STDERR "ERROR: client SpawWaiKill returned $client_status\n";
        $status = 1;
    }
}

$client->DeleteFile($iorbase);

exit $status;
