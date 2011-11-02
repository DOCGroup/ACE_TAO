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
$level = "object";
$shutdown = "";
$all = 1;
$other = "";

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
my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $iorbase = "test.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

$SV = $server->CreateProcess ("server", "-ORBdebuglevel $debug_level -o $server_iorfile");
$CL = $client->CreateProcess ("client", "");

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

# -*- perl -*-
###############################################################################

if ($all == 1) {
    foreach $bufsize (@bufsizes) {
        print STDERR "\n***************** STARTING TEST ******************\n";

        $CL->Arguments ("-k file://$client_iorfile -ORBNodelay 0 -l $level ".
                        "-t none -i $iterations -m $bufsize");

        my $client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 45);

        if ($client_status != 0) {
            print STDERR "ERROR: client returned $client_status\n";
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

    $CL->Arguments ("-k file://$client_iorfile $transport -t $type -i $iterations ".
                    "-l $level -m $bufsize -w $work $shutdown ");

    my $client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 45);

    if ($client_status != 0) {
        print STDERR "ERROR: client returned $client_status\n";
        $status = 1;
    }
}

###############################################################################

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

exit $status;
