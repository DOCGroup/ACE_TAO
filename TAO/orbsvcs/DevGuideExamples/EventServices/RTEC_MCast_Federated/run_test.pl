# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use Env (ACE_ROOT);
use lib "$ACE_ROOT/bin";
use PerlACE::Run_Test;

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

$nsiorfile = PerlACE::LocalFile ("ns.ior");
$ec1iorfile = PerlACE::LocalFile ("ec1.ior");
$ec2iorfile = PerlACE::LocalFile ("ec2.ior");

$arg_ns_ref = "-ORBInitRef NameService=file://$nsiorfile";
$end_point = "-ORBEndpoint iiop://localhost";
$ns_port = PerlACE::random_port();

unlink $nsiorfile;
unlink $ec1iorfile;
unlink $ec2iorfile;

# start Naming Service

$NameService = "$ENV{TAO_ROOT}/orbsvcs/Naming_Service/Naming_Service";
$NS = new PerlACE::Process($NameService, "$flags -o $nsiorfile $end_point:$ns_port");
$NS->Spawn();
if (PerlACE::waitforfile_timed ($nsiorfile, 5) == -1) {
    print STDERR "ERROR: cannot find file <$nsiorfile>\n";
    $NS->Kill();
    exit 1;
}

# start EchoEventSupplier
my($port1) = 10001 + PerlACE::uniqueid() ;
my($port2) = 10001 + PerlACE::uniqueid() ;
my($mport) = 10001 + PerlACE::uniqueid() ;
if ( -e "supplier.conf" )
{
   $supplier_conf_file = "supplier.conf";
}
else{
   $supplier_conf_file = "../supplier.conf";
}

$args1 = "$flags $arg_ns_ref -ORBSvcConf $supplier_conf_file $end_point -iorfile $ec1iorfile";
if ($udp) {
  $args1 .= " -udp -ecname ec1 -port $port1 -listenport $port2 ";
} else {
  $args1 .= " -ecname ec1 -address 224.9.9.2 -port $mport ";
}
$S1 = new PerlACE::Process("EchoEventSupplier", $args1);
$S1->Spawn();

$args2 = "$flags $arg_ns_ref -ORBSvcConf $supplier_conf_file $end_point -iorfile $ec2iorfile";
if ($udp) {
  $args2 .= " -udp -ecname ec2 -port $port2 -listenport $port1 ";
} else {
  $args2 .= " -ecname ec2 -address 224.9.9.2 -port $mport ";
}
$S2 = new PerlACE::Process("EchoEventSupplier", $args2);
$S2->Spawn();

if ((PerlACE::waitforfile_timed ($ec1iorfile, 15) == -1) ||
    (PerlACE::waitforfile_timed ($ec2iorfile, 2) == -1)) {
    print STDERR "ERROR: cannot find files <$ec1iorfile> and <$ec2iorfile>\n";
    $NS->Kill();
    $S1->Kill();
    $S2->Kill();
    exit 1;
}

$args3 = "$flags $arg_ns_ref -ecname ec1 $end_point";
$C1 = new PerlACE::Process("EchoEventConsumer", $args3);
$C1->Spawn();


$args4 = "$flags $arg_ns_ref -ecname ec2 $end_point";
$C2 = new PerlACE::Process("EchoEventConsumer", $args4);
$C2->Spawn();

if ($C1->WaitKill(30) == -1) {
	$S1->Kill();
	$S2->Kill();
	$NS->Kill();
	$C2->Kill();

    exit 1;
}

if ($C2->WaitKill(5) == -1) {
	$S1->Kill();
	$S2->Kill();
	$NS->Kill();
	exit 1;
}

$NS->Kill();
$S1->Kill();
$S2->Kill();

unlink $nsiorfile;
unlink $ec1iorfile;
unlink $ec2iorfile;

exit 0;
