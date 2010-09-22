eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

sub usage() {
    print "Usage:\n";
    print "  run_test [-h] [-debug]\n\n";
    print "    -udp          -- Federate using udp\n";
    print "    -mcast        -- Federate using multicast (the default)\n";
    print "    -ipv6         -- Use an IPv6 mcast addr for federating\n";
    print "    -h            -- Prints this information\n";
    print "    -debug        -- Sets the debug flag for the test\n";
    exit;
}

my $udp = 0;
my $mcast_addr = "224.9.9.2";
my $debug = "";

my $i = 0;
while ($i <= $#ARGV) {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-help" || $ARGV[$i] eq "--help" || $ARGV[$i] eq "-?") {
        usage ();
    }
    elsif ($ARGV[$i] eq "-debug") {
        $debug .= "-ORBDebugLevel 10";
    }
    elsif ($ARGV[$i] eq "-udp") {
        $udp = 1;
    }
    elsif ($ARGV[$i] eq "-mcast") {
        $udp = 0;
    }
    elsif ($ARGV[$i] eq "-ipv6") {
        $mcast_addr = "FF05::255";
    }
    else {
        print "ERROR: Unknown Option: ".$ARGV[$i]."\n\n";
        usage ();
    }
    $i++;
}

$status = 0;

if ($udp) {
    print "Using UDP to link the event channels.\n\n";
}
else {
    print "Using multicast to link the event channels.\n\n";
}

my $nservice  = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $supplier1 = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $supplier2 = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $consumer1 = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";
my $consumer2 = PerlACE::TestTarget::create_target (5) || die "Create target 5 failed\n";

my $ns_port = $nservice->RandomPort();
my $ns_host = $nservice->HostName();
my $port1 = $supplier1->RandomPort();
my $port2 = $supplier1->RandomPort();
my $mport = $supplier1->RandomPort();

my $end_point = "-ORBEndpoint iiop://$ns_host";

my $ns_ior  = "ns.ior";
my $ec1_ior = "ec1.ior";
my $ec2_ior = "ec2.ior";

my $sp_conf = "supplier.conf";
if ( -e $sp_conf ) {
}
else {
   $sp_conf = "../"."$sp_conf";
}

my $supplier1_sp_conf = $supplier1->LocalFile($sp_conf);
my $supplier2_sp_conf = $supplier2->LocalFile($sp_conf);

my $ns_ns_ior = $nservice->LocalFile ($ns_ior);
my $supplier1_ns_ior = $supplier1->LocalFile ($ns_ior);
my $supplier2_ns_ior = $supplier2->LocalFile ($ns_ior);
my $consumer1_ns_ior = $consumer1->LocalFile ($ns_ior);
my $consumer2_ns_ior = $consumer2->LocalFile ($ns_ior);
my $supplier1_ec1_ior = $supplier1->LocalFile ($ec1_ior);
my $supplier2_ec2_ior = $supplier2->LocalFile ($ec2_ior);
$nservice->DeleteFile($ns_ior);
$supplier1->DeleteFile ($ns_ior);
$supplier2->DeleteFile ($ns_ior);
$consumer1->DeleteFile ($ns_ior);
$consumer2->DeleteFile ($ns_ior);
$supplier1->DeleteFile ($ec1_ior);
$supplier2->DeleteFile ($ec2_ior);

$NS = $nservice->CreateProcess("$ENV{TAO_ROOT}/orbsvcs/Naming_Service/tao_cosnaming",
                               "$debug -o $ns_ns_ior $end_point:$ns_port");
my $sp1_args = "$debug -ORBInitRef NameService=file://$supplier1_ns_ior ".
               "-ORBSvcConf $supplier1_sp_conf $end_point -iorfile $supplier1_ec1_ior";
my $sp2_args = "$debug -ORBInitRef NameService=file://$supplier2_ns_ior ".
               "-ORBSvcConf $supplier2_sp_conf $end_point -iorfile $supplier2_ec2_ior";

if ($udp) {
    $sp1_args .= " -udp -ecname ec1 -port $port1 -listenport $port2";
    $sp2_args .= " -udp -ecname ec2 -port $port2 -listenport $port1";
}
else {
    $sp1_args .= " -ecname ec1 -address $mcast_addr -port $mport";
    $sp2_args .= " -ecname ec2 -address $mcast_addr -port $mport";
}

$S1 = $supplier1->CreateProcess("EchoEventSupplier", $sp1_args);
$S2 = $supplier2->CreateProcess("EchoEventSupplier", $sp2_args);
$C1 = $consumer1->CreateProcess("EchoEventConsumer", "$debug -ORBInitRef NameService=file://$consumer1_ns_ior ".
                                                     "-ecname ec2 $end_point");
$C2 = $consumer2->CreateProcess("EchoEventConsumer", "$debug -ORBInitRef NameService=file://$consumer2_ns_ior ".
                                                     "-ecname ec2 $end_point");

# start Naming Service
$process_status = $NS->Spawn ();

if ($process_status != 0) {
    print STDERR "ERROR: naming service returned $process_status\n";
    exit 1;
}

if ($nservice->WaitForFileTimed ($ns_ior,
                                 $nservice->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$ns_ior>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

if ($nservice->GetFile ($ns_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$ns_ns_ior>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

if ($supplier1->PutFile ($ns_ior) == -1) {
    print STDERR "ERROR: supplier 2 cannot set file <$supplier1_ns_ior>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

if ($supplier2->PutFile ($ns_ior) == -1) {
    print STDERR "ERROR: supplier 2 cannot set file <$supplier2_ns_ior>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

if ($consumer1->PutFile ($ns_ior) == -1) {
    print STDERR "ERROR: consumer 1 cannot set file <$consumer1_ns_ior>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

if ($consumer2->PutFile ($ns_ior) == -1) {
    print STDERR "ERROR: consumer 2 cannot set file <$consumer2_ns_ior>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

# start EchoEventSupplier
print "starting supplier 1\n";

$process_status = $S1->Spawn ();

if ($process_status != 0) {
    print STDERR "ERROR: supplier 1 returned $process_status\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

print "starting supplier 2\n";

$process_status = $S2->Spawn ();

if ($process_status != 0) {
    print STDERR "ERROR: supplier 2 returned $process_status\n";
    $S1->Kill (); $S1->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

if ($supplier1->WaitForFileTimed ($ec1_ior,
                                  $supplier1->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$supplier1_ec1_ior>\n";
    $S1->Kill (); $S1->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

if ($supplier2->WaitForFileTimed ($ec2_ior,
                                  $supplier2->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$supplier2_ec2_ior>\n";
    $S2->Kill (); $S2->TimedWait (1);
    $S1->Kill (); $S1->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

$process_status = $C1->Spawn ();

if ($process_status != 0) {
    print STDERR "ERROR: consumer 1 returned $process_status\n";
    $S2->Kill (); $S2->TimedWait (1);
    $S1->Kill (); $S1->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

$process_status = $C2->Spawn ();

if ($process_status != 0) {
    print STDERR "ERROR: consumer 2 returned $process_status\n";
    $S2->Kill (); $S2->TimedWait (1);
    $S1->Kill (); $S1->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

$process_status = $C1->WaitKill ($consumer1->ProcessStopWaitInterval() + 15);

if ($process_status != 0) {
    print STDERR "ERROR: consumer 1 returned $process_status\n";
    $status = 1;
}

$process_status = $C2->WaitKill ($consumer2->ProcessStopWaitInterval());

if ($process_status != 0) {
    print STDERR "ERROR: consumer 2 returned $process_status\n";
    $status = 1;
}

$NS->Kill();
$S1->Kill();
$S2->Kill();

$nservice->DeleteFile($ns_ior);
$supplier1->DeleteFile ($ns_ior);
$supplier2->DeleteFile ($ns_ior);
$consumer1->DeleteFile ($ns_ior);
$consumer2->DeleteFile ($ns_ior);
$supplier1->DeleteFile ($ec1_ior);
$supplier2->DeleteFile ($ec2_ior);

exit $status;
