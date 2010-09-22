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

my $ns = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $es = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $ce1 = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $ce2 = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";
my $co1 = PerlACE::TestTarget::create_target (5) || die "Create target 5 failed\n";
my $co2 = PerlACE::TestTarget::create_target (6) || die "Create target 6 failed\n";
my $su1 = PerlACE::TestTarget::create_target (7) || die "Create target 7 failed\n";
my $su2 = PerlACE::TestTarget::create_target (8) || die "Create target 8 failed\n";

#event count
$ev_count = 20;
$status = 0;

# setup CosEC params such that..
# cos event service name = "cosec1"
# for ConsumerQOS: EventID = 21, SourceID = 6
# for SupplierQOS: EventID = 20, SourceID = 5.
$CosEC1_params = "-n cosec1 -e 21 -o 6 -p \"5 20\"";

# setup another CosEC params such that..
# cos event service name = "cosec2"
# for ConsumerQOS: EventID = 20, SourceID = 5
# for SupplierQOS: EventID = 21, SourceID = 6.
$CosEC2_params = "-n cosec2 -e 20 -o 5 -p \"6 21\"";

$nsiorfile = "ns.ior";

my $ns_nsiorfile = $ns->LocalFile ($nsiorfile);
my $es_nsiorfile = $es->LocalFile ($nsiorfile);
my $ce1_nsiorfile = $ce1->LocalFile ($nsiorfile);
my $ce2_nsiorfile = $ce2->LocalFile ($nsiorfile);
my $co1_nsiorfile = $co1->LocalFile ($nsiorfile);
my $co2_nsiorfile = $co2->LocalFile ($nsiorfile);
my $su1_nsiorfile = $su1->LocalFile ($nsiorfile);
my $su2_nsiorfile = $su2->LocalFile ($nsiorfile);

$ns->DeleteFile ($nsiorfile);
$es->DeleteFile ($nsiorfile);
$ce1->DeleteFile ($nsiorfile);
$ce2->DeleteFile ($nsiorfile);
$co1->DeleteFile ($nsiorfile);
$co2->DeleteFile ($nsiorfile);
$su1->DeleteFile ($nsiorfile);
$su2->DeleteFile ($nsiorfile);

# Parse the arguments

for ($i = 0; $i <= $#ARGV; $i++) {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?") {
        print "usage: run_test.pl -e event_count -h help\n";
        exit;
    }
    elsif ($ARGV[$i] eq "-e") {
        $ev_count = $ARGV[$i + 1];
        $i++;
    }
}

$NS = $ns->CreateProcess ($ENV{"TAO_ROOT"}."/orbsvcs/Naming_Service/tao_cosnaming",
                          "-o $ns_nsiorfile");

$ES = $es->CreateProcess ($ENV{"TAO_ROOT"}."/orbsvcs/Event_Service/Event_Service",
                          "-t new -ORBInitRef NameService=file://$es_nsiorfile");

$CE1 = $ce1->CreateProcess ("../../bin/RtEC_Based_CosEC",
                          "-ORBInitRef NameService=file://$ce1_nsiorfile $CosEC1_params");

$CE2 = $ce2->CreateProcess ("../../bin/RtEC_Based_CosEC",
                          "-ORBInitRef NameService=file://$ce2_nsiorfile $CosEC2_params");

$CO1 = $co1->CreateProcess ("Consumer",
                          "-ORBInitRef NameService=file://$co1_nsiorfile -n cosec1 -c $ev_count");

$CO2 = $co2->CreateProcess ("Consumer",
                          "-ORBInitRef NameService=file://$co2_nsiorfile -n cosec2 -c $ev_count");

$SU1 = $su1->CreateProcess ("Supplier",
                          "-ORBInitRef NameService=file://$su1_nsiorfile -n cosec2 -c $ev_count");

$SU2 = $su2->CreateProcess ("Supplier",
                          "-ORBInitRef NameService=file://$su2_nsiorfile -n cosec1 -c $ev_count");

# first start the Naming service..
$NS_status = $NS->Spawn ();

if ($NS_status != 0) {
    print STDERR "ERROR: Name Service returned $NS_status\n";
    exit 1;
}

if ($ns->WaitForFileTimed ($nsiorfile,$ns->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$ns_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

if ($ns->GetFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$ns_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($es->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$es_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($ce1->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$ce1_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($ce2->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$ce2_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($co1->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$co1_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($co2->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$co2_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($su1->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$su1_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($su2->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$su2_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

# now start the Rt EC..
$ES_status = $ES->Spawn ();

if ($ES_status != 0) {
    print STDERR "ERROR: Event Service returned $ES_status\n";
    exit 1;
}

sleep 10;
# now start the CosEC1..
$CE1_status = $CE1->Spawn ();

if ($CE1_status != 0) {
    print STDERR "ERROR: RtEC_Based_CosEC 1 returned $CE1_status\n";
    exit 1;
}

sleep 10;

# now start the CosEC2..
$CE2_status = $CE2->Spawn ();

if ($CE2_status != 0) {
    print STDERR "ERROR: RtEC_Based_CosEC 2 returned $CE2_status\n";
    exit 1;
}

sleep 10;

# start 1 consumer that uses CosEC1 to receive events
$CO1_status = $CO1->Spawn ();

if ($CO1_status != 0) {
    print STDERR "ERROR: Consumer 1 returned $CO1_status\n";
    exit 1;
}

sleep 10;

# start 1 supplier  that uses CosEC2 to send events
$SU1_status = $SU1->Spawn ();

if ($SU1_status != 0) {
    print STDERR "ERROR: Supplier 1 returned $SU1_status\n";
    exit 1;
}

sleep 10;

# wait for the supplier to finish
$SU1_status = $SU1->WaitKill ($su1->ProcessStopWaitInterval()+45);

if ($SU1_status != 0) {
    print STDERR "ERROR: Supplier 1 returned $SU1_status\n";
    $status = 1;
}

# wait for the consumer to finish
$CO1_status = $CO1->WaitKill ($co1->ProcessStopWaitInterval()+45);

if ($CO1_status != 0) {
    print STDERR "ERROR: Consumer 1 returned $CO1_status\n";
    $status = 1;
}

#----------

# start 1 consumer that uses CosEC1 to receive events
$CO2_status = $CO2->Spawn ();

if ($CO2_status != 0) {
    print STDERR "ERROR: Consumer 2 returned $CO2_status\n";
    exit 1;
}

sleep 10;

# start 1 supplier  that uses CosEC2 to send events
$SU2_status = $SU2->Spawn ();

if ($SU2_status != 0) {
    print STDERR "ERROR: Supplier 2 returned $SU2_status\n";
    exit 1;
}

sleep 10;

# wait for the supplier to finish
$SU2_status = $SU2->WaitKill ($su2->ProcessStopWaitInterval()+45);

if ($SU2_status != 0) {
    print STDERR "ERROR: Supplier 2 returned $SU2_status\n";
    $status = 1;
}

# wait for the consumer to finish
$CO2_status = $CO2->WaitKill ($co2->ProcessStopWaitInterval()+45);

if ($CO2_status != 0) {
    print STDERR "ERROR: Consumer 2 returned $CO2_status\n";
    $status = 1;
}

#----------

# cleanup..
$CE2_status = $CE2->TerminateWaitKill ($ce2->ProcessStopWaitInterval());

if ($CE2_status != 0) {
    print STDERR "ERROR: RtEC_Basic_CosEC 2  returned $CE2_status\n";
    $status = 1;
}

$CE1_status = $CE1->TerminateWaitKill ($ce1->ProcessStopWaitInterval());

if ($CE1_status != 0) {
    print STDERR "ERROR: RtEC_Basic_CosEC 1 returned $CE1_status\n";
    $status = 1;
}

$ES_status = $ES->TerminateWaitKill ($es->ProcessStopWaitInterval());

if ($ES_status != 0) {
    print STDERR "ERROR: Event Service returned $ES_status\n";
    $status = 1;
}

$NS_status = $NS->TerminateWaitKill ($ns->ProcessStopWaitInterval());

if ($NS_status != 0) {
    print STDERR "ERROR: Name Service returned $NS_status\n";
    $status = 1;
}

$ns->DeleteFile ($nsiorfile);
$es->DeleteFile ($nsiorfile);
$ce1->DeleteFile ($nsiorfile);
$ce2->DeleteFile ($nsiorfile);
$co1->DeleteFile ($nsiorfile);
$co2->DeleteFile ($nsiorfile);
$su1->DeleteFile ($nsiorfile);
$su2->DeleteFile ($nsiorfile);

exit $status;
