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
my $ce = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $co = PerlACE::TestTarget::create_target (5) || die "Create target 5 failed\n";
my $su = PerlACE::TestTarget::create_target (7) || die "Create target 7 failed\n";

$con_count = 10;
$sup_count = 10;
$ev_count = 8;

$nsiorfile = "ns.ior";

my $ns_nsiorfile = $ns->LocalFile ($nsiorfile);
my $es_nsiorfile = $es->LocalFile ($nsiorfile);
my $ce_nsiorfile = $ce->LocalFile ($nsiorfile);
my $co_nsiorfile = $co->LocalFile ($nsiorfile);
my $su_nsiorfile = $su->LocalFile ($nsiorfile);

$ns->DeleteFile ($nsiorfile);
$es->DeleteFile ($nsiorfile);
$ce->DeleteFile ($nsiorfile);
$co->DeleteFile ($nsiorfile);
$su->DeleteFile ($nsiorfile);

# Parse the arguments

for ($i = 0; $i <= $#ARGV; $i++) {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?") {
        print "usage: run_test2.pl -e event_count -c consumers -s suppliers -h help\n";
        exit;
    }
    elsif ($ARGV[$i] eq "-e") {
        $ev_count = $ARGV[$i + 1];
        $i++;
    }
    elsif ($ARGV[$i] eq "-c") {
        $con_count = $ARGV[$i + 1];
        $i++;
    }
    elsif ($ARGV[$i] eq "-s") {
        $sup_count = $ARGV[$i + 1];
        $i++;
    }
}

$NS = $ns->CreateProcess ($ENV{"TAO_ROOT"}."/orbsvcs/Naming_Service/tao_cosnaming",
                          "-o $ns_nsiorfile");

$ES = $es->CreateProcess ($ENV{"TAO_ROOT"}."/orbsvcs/Event_Service/tao_rtevent",
                          "-t new -ORBInitRef NameService=file://$es_nsiorfile");

$CE = $ce->CreateProcess ("../../bin/RtEC_Based_CosEC",
                          "-ORBInitRef NameService=file://$ce_nsiorfile");

for ($cntr1 = 0; $cntr1 < $con_count ; ++$cntr1) {
    push @CONS, ($co->CreateProcess ("Consumer", "-ORBInitRef NameService=file://$co_nsiorfile -c $ev_count"));
}

for ($cntr2 = 0; $cntr2 < $con_count ; ++$cntr2) {
    push @SUPS, ($su->CreateProcess ("Supplier", "-ORBInitRef NameService=file://$su_nsiorfile -c $ev_count"));
}

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
if ($ce->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$ce_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($co->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$co_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($su->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$su_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

sleep 10;

# now start the Rt EC..
$ES_status = $ES->Spawn ();

if ($ES_status != 0) {
    print STDERR "ERROR: Event Service returned $ES_status\n";
    exit 1;
}

sleep 10;

# now start the CosEC1..
$CE_status = $CE->Spawn ();

if ($CE_status != 0) {
    print STDERR "ERROR: RtEC_Based_CosEC returned $CE_status\n";
    exit 1;
}

sleep 10;

# now start the consumers
foreach $CO (@CONS) {
    print "creating consumer# $cntr1\n";
    $CO_status = $CO->Spawn ();
    if ($CO_status != 0) {
        print STDERR "ERROR: Consumer $cntr1 returned $CO_status\n";
        exit 1;
    }
    sleep 10;
}

# now start the suppliers
foreach $SU (@SUPS) {
    print "creating supplier# $cntr2\n";
    $SU_status = $SU->Spawn ();
    if ($SU_status != 0) {
        print STDERR "ERROR: Supplier $cntr2 returned $SU_status\n";
        exit 1;
    }
    sleep 10;
}

print "waiting for the suppliers to finish\n";
foreach $SU (@SUPS) {
    $SU_status = $SU->WaitKill ($su->ProcessStopWaitInterval()+45);
    if ($SU_status != 0) {
        print STDERR "ERROR: A Supplier returned $SU_status\n";
        $status = 1;
    }
}

print "waiting for the last consumer to finish\n";
foreach $CO (@CONS) {
    $CO_status = $CO->WaitKill ($co->ProcessStopWaitInterval()+45);

    if ($CO_status != 0) {
        print STDERR "ERROR: A Consumer returned $CO_status\n";
        $status = 1;
    }
}

print "cleanup...\n";
$CE_status = $CE->TerminateWaitKill ($ce->ProcessStopWaitInterval());

if ($CE_status != 0) {
    print STDERR "ERROR: RtEC_Basic_CosEC returned $CE_status\n";
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
print "done!.\n";

$ns->DeleteFile ($nsiorfile);
$es->DeleteFile ($nsiorfile);
$ce->DeleteFile ($nsiorfile);
$co->DeleteFile ($nsiorfile);
$su->DeleteFile ($nsiorfile);

exit $status;
