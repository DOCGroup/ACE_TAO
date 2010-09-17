# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

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
my $s1 = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $s2 = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $c1 = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";
my $c2 = PerlACE::TestTarget::create_target (5) || die "Create target 5 failed\n";

my $nsiorfile = "ns.ior";
my $ec1iorfile = "ec1.ior";
my $ec2iorfile = "ec2.ior";
my $supplier_conf_file = "";
if ( -e "supplier.conf" ) {
   $supplier_conf_file = "supplier.conf";
}
else{
   $supplier_conf_file = "../supplier.conf";
}

my $ns_nsiorfile = $ns->LocalFile ($nsiorfile);
my $s1_ec1iorfile = $s1->LocalFile ($ec1iorfile);
my $s2_ec2iorfile = $s2->LocalFile ($ec2iorfile);
my $s1_nsiorfile = $s1->LocalFile ($nsiorfile);
my $s2_nsiorfile = $s2->LocalFile ($nsiorfile);
my $c1_nsiorfile = $c1->LocalFile ($nsiorfile);
my $c2_nsiorfile = $c2->LocalFile ($nsiorfile);
$ns->DeleteFile ($nsiorfile);
$s1->DeleteFile ($ec1iorfile);
$s2->DeleteFile ($ec2iorfile);
$s1->DeleteFile ($nsiorfile);
$s2->DeleteFile ($nsiorfile);
$c1->DeleteFile ($nsiorfile);
$c2->DeleteFile ($nsiorfile);

$NameService = "$ENV{TAO_ROOT}/orbsvcs/Naming_Service/Naming_Service";
$NS = $ns->CreateProcess ($NameService, "-ORBdebuglevel $debug_level ".
                                        " -o $ns_nsiorfile");
$args1 = "-ORBSvcConf $supplier_conf_file -ecname ec1 -gateway ec2";
$S1 = $s1->CreateProcess ("EchoEventSupplier", "-ORBInitRef NameService=file://$s1_nsiorfile ".
                                               "$args1 ".
                                               "-iorfile $s1_ec1iorfile");
$args2 = "-ORBSvcConf $supplier_conf_file -ecname ec2 -gateway ec1";
$S2 = $s2->CreateProcess ("EchoEventSupplier", "-ORBInitRef NameService=file://$s2_nsiorfile ".
                                               "$args2 ".
                                               "-iorfile $s2_ec2iorfile");
$args3 = "-ecname ec1";
$C1 = $c1->CreateProcess ("EchoEventConsumer", "-ORBInitRef NameService=file://$c1_nsiorfile ".
                                               "$args3");
$args4 = "-ecname ec2";
$C2 = $c2->CreateProcess ("EchoEventConsumer", "-ORBInitRef NameService=file://$c2_nsiorfile ".
                                               "$args4");


# start Naming Service
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
if ($s1->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$s1_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($s2->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$s2_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($c1->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$c1_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($c2->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$c2_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

# start Supplier
$S1_status = $S1->Spawn ();

if ($S1_status != 0) {
    print STDERR "ERROR: Supplier1 returned $S1_status\n";
    exit 1;
}

$S2_status = $S2->Spawn ();

if ($S2_status != 0) {
    print STDERR "ERROR: Supplier2 returned $S2_status\n";
    exit 1;
}

if ($s1->WaitForFileTimed ($ec1iorfile, $s1->ProcessStartWaitInterval()+60) == -1) {
    print STDERR "ERROR: cannot find file <$s1_ec1iorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    $S1->Kill (); $S1->TimedWait (1);
    exit 1;
}

if ($s2->WaitForFileTimed ($ec2iorfile, $s2->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$s2_ec2iorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    $S1->Kill (); $S1->TimedWait (1);
    $S2->Kill (); $S2->TimedWait (1);
    exit 1;
}

$C1_status = $C1->Spawn ();

if ($C1_status != 0) {
    print STDERR "ERROR: Consumer returned $C1_status\n";
    exit 1;
}

$C2_status = $C2->Spawn ();

if ($C2_status != 0) {
    print STDERR "ERROR: Consumer returned $C2_status\n";
    exit 1;
}

$C1_status = $C1->WaitKill ($c1->ProcessStopWaitInterval()+45);

if ($C1_status != 0) {
    print STDERR "ERROR: Consumer1 returned $C1_status\n";
    $status = 1;
}

$C2_status = $C2->WaitKill ($c2->ProcessStopWaitInterval());

if ($C2_status != 0) {
    print STDERR "ERROR: Consumer2 returned $C2_status\n";
    $status = 1;
}

$NS_status = $NS->TerminateWaitKill ($ns->ProcessStopWaitInterval());

if ($NS_status != 0) {
    print STDERR "ERROR: Name Service returned $NS_status\n";
    $status = 1;
}

$S1_status = $S1->TerminateWaitKill ($s1->ProcessStopWaitInterval());

if ($S1_status != 0) {
    print STDERR "ERROR: Supplier1 returned $S1_status\n";
    $status = 1;
}

$S2_status = $S2->TerminateWaitKill ($s2->ProcessStopWaitInterval());

if ($S2_status != 0) {
    print STDERR "ERROR: Supplier2 returned $S2_status\n";
    $status = 1;
}

$ns->DeleteFile ($nsiorfile);
$s1->DeleteFile ($ec1iorfile);
$s2->DeleteFile ($ec2iorfile);
$s1->DeleteFile ($nsiorfile);
$s2->DeleteFile ($nsiorfile);
$c1->DeleteFile ($nsiorfile);
$c2->DeleteFile ($nsiorfile);

exit $status;
