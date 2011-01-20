# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;

if (!defined $ENV{TAO_ROOT}) {
   $ENV{TAO_ROOT} = "$ENV{ACE_ROOT}/TAO";
}

sub usage() {
   print "Usage:\n";
   print "  run_test [-h] [-debug]\n\n";
   print "    -udp          -- Federate using udp\n";
   print "    -mcast        -- Federate using multicast (the default)\n";
   print "    -h            -- Prints this information\n";
   print "    -debug        -- Sets the debug flag for the test\n";
   exit;
}

my $udp = 0;
my $i = 0;
my $flags = "";
while ($i <= $#ARGV) {
   if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-help" ||
       $ARGV[$i] eq "--help" || $ARGV[$i] eq "-?") {
     usage ();
   } elsif ($ARGV[$i] eq "-debug") {
     $flags .= " -ORBDebugLevel 10 ";
   } elsif ($ARGV[$i] eq "-udp") {
     $udp = 1;
   } elsif ($ARGV[$i] eq "-mcast") {
     $udp = 0;
   } else {
     print "ERROR: Unknown Option: ".$ARGV[$i]."\n\n";
     usage ();
   }
   $i++;
}

if ($udp) {
   print "Using UDP to link the event channels.\n\n";
} else {
   print "Using multicast to link the event channels.\n\n";
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

my $ns_hostname = $ns->HostName();
my $s1_hostname = $s1->HostName();
my $s2_hostname = $s2->HostName();
my $c1_hostname = $c1->HostName();
my $c2_hostname = $c2->HostName();
my $ns_port = $ns->RandomPort();

$NameService = "$ENV{TAO_ROOT}/orbsvcs/Naming_Service/tao_cosnaming";
$NS = $ns->CreateProcess ($NameService, "$flags ".
                                        " -o $ns_nsiorfile ".
                                        "-ORBListenEndpoints iiop://$ns_hostname:$ns_port");

my($port1) = $s1->RandomPort() ;
my($port2) = $s1->RandomPort() ;
my($mport) = $s1->RandomPort() ;

$args1 = "$flags -ORBInitRef NameService=file://$s1_nsiorfile -ORBSvcConf $supplier_conf_file -ORBListenEndpoints iiop://$s1_hostname -iorfile $s1_ec1iorfile";
if ($udp) {
   $args1 .= " -udp -ecname ec1 -port $port1 -listenport $port2 ";
} else {
   $args1 .= " -ecname ec1 -address 224.9.9.2 -port $mport ";
}
$S1 = $s1->CreateProcess ("EchoEventSupplier", "$args1");


$args2 = "$flags -ORBInitRef NameService=file://$s2_nsiorfile -ORBSvcConf $supplier_conf_file -ORBListenEndpoints iiop://$s2_hostname  -iorfile $s2_ec2iorfile";
if ($udp) {
   $args2 .= " -udp -ecname ec2 -port $port2 -listenport $port1 ";
} else {
   $args2 .= " -ecname ec2 -address 224.9.9.2 -port $mport ";
}
$S2 = $s2->CreateProcess ("EchoEventSupplier", "$args2");

$args3 = "$flags -ORBInitRef NameService=file://$c1_nsiorfile -ecname ec1 -ORBListenEndpoints iiop://$c1_hostname";
$C1 = $c1->CreateProcess ("EchoEventConsumer", "$args3");


$args4 = "$flags -ORBInitRef NameService=file://$c2_nsiorfile -ecname ec2 -ORBListenEndpoints iiop://$c2_hostname";
$C2 = $c2->CreateProcess ("EchoEventConsumer", "$args4");

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

# start EchoEventSupplier
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

$C1_status = $C1->WaitKill ($c1->ProcessStopWaitInterval()+30);

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
