# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use Env (ACE_ROOT);
use lib "$ACE_ROOT/bin";
use PerlACE::Run_Test;

$iorfile = PerlACE::LocalFile ("ns.ior");
$ec1iorfile = PerlACE::LocalFile ("ec1.ior");
$ec2iorfile = PerlACE::LocalFile ("ec2.ior");
$arg_ns_ref = "-ORBInitRef NameService=file://$iorfile";

unlink $iorfile;
unlink $ec1iorfile;
unlink $ec2iorfile;

# start Naming Service
$NameService = "$ENV{TAO_ROOT}/orbsvcs/Naming_Service/Naming_Service";
$NS = new PerlACE::Process($NameService, "-o $iorfile");
$NS->Spawn();
if (PerlACE::waitforfile_timed ($iorfile, 10) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $NS->Kill(); 
    exit 1;
}

# start Supplier  
if ( -e "supplier.conf" ) 
{
   $supplier_conf_file = "supplier.conf";
}
else{
   $supplier_conf_file = "../supplier.conf";   
}
$args1 = "-ORBSvcConf $supplier_conf_file -ecname ec1 -gateway ec2 -iorfile $ec1iorfile";
$S1 = new PerlACE::Process("EchoEventSupplier", "$arg_ns_ref $args1");
$S1->Spawn();

$args2 = "-ORBSvcConf $supplier_conf_file -ecname ec2 -gateway ec1 -iorfile $ec2iorfile";
$S2 = new PerlACE::Process("EchoEventSupplier", "$arg_ns_ref $args2");
$S2->Spawn();

if ((PerlACE::waitforfile_timed ($ec1iorfile, 15) == -1) ||
    (PerlACE::waitforfile_timed ($ec2iorfile, 2) == -1)) {
    print STDERR "ERROR: cannot find files <$ec1iorfile> and <$ec2iorfile>\n";
    $NS->Kill(); 
    $S1->Kill();
    $S2->Kill();
    exit 1;
}

$args3 = "-ecname ec1";
$C1 = new PerlACE::Process("EchoEventConsumer", "$arg_ns_ref $args3");
$C1->Spawn();

$args4 = "-ecname ec2";
$C2 = new PerlACE::Process("EchoEventConsumer", "$arg_ns_ref $args4");
$C2->Spawn();

if ($C1->WaitKill(60) == -1) {
   print STDERR "consumer1 timedout \n";

   $C2->Kill();
   $S1->Kill();
   $S2->Kill();
   $NS->Kill();

   unlink $iorfile;

   exit 1;
}

if ($C2->WaitKill(10) == -1) {
   print STDERR "consumer2 timedout \n";

   $S1->Kill();
   $S2->Kill();
   $NS->Kill();

   unlink $iorfile;

   exit 1;
}
  
$NS->Kill();
$S1->Kill();
$S2->Kill();

unlink $iorfile;
unlink $ec1iorfile;
unlink $ec2iorfile;

exit 0;
