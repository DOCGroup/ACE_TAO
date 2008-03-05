eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
& eval 'exec perl -S $0 $argv:q'
if 0;
	
# $Id$
# -*- perl -*-
	
use lib "$ENV{'ACE_ROOT'}/bin";
use PerlACE::Run_Test;

#delete IOR files
if (-e em.ior) {
    unlink "em.ior"
}
if (-e ns.ior) {
    unlink "ns.ior"
}

$ns_status = 0;
$mt_status = 0;
$pl_status = 0;
$em_status = 0;

if (PerlACE::is_vxworks_test()) {
    $ns = new PerlACE::ProcessVX ("$ENV{'ACE_ROOT'}/TAO/orbsvcs/Naming_Service/Naming_Service", "-o ns.ior");
}
else {
    $ns = new PerlACE::Process ("$ENV{'ACE_ROOT'}/TAO/orbsvcs/Naming_Service/Naming_Service", "-o ns.ior");
}

$ns_status = $ns->Spawn();
sleep 10;

#read NameService IOR
$data_file = "ns.ior";
open(DAT, $data_file) || die("ERROR: Could not open file!");
$ior = <DAT>;
close(DAT);

if (PerlACE::is_vxworks_test()) {
    $mt = new PerlACE::ProcessVX ("./MockTest", "-ORBInitRef NameService=file://ns.ior");
}
else {
    $mt = new PerlACE::Process ("./MockTest", "-ORBInitRef NameService=file://ns.ior");
}

$mt_status = $mt->Spawn();
sleep 10;

if (PerlACE::is_vxworks_test()) {
    $em = new PerlACE::ProcessVX ("$ENV{'ACE_ROOT'}/bin/dance", "--log-level 1 -eem.ior --node-mgr MyHost  -ORBListenEndpoints iiop://localhost:12345 -ORBInitRef ProcessDestinationNC=file://ns.ior");    
    $pl = new PerlACE::ProcessVX ("$ENV{'ACE_ROOT'}/bin/dance", "--log-level 1 -x MockTest.cdp -k file://em.ior");
}
else {
    $em = new PerlACE::Process ("$ENV{'ACE_ROOT'}/bin/dance", "--log-level 1 -eem.ior --node-mgr MyHost -ORBListenEndpoints iiop://localhost:12345 -ORBInitRef ProcessDestinationNC=file://ns.ior");    
#open (OLD_STDIN, "<STDIN");
#open (STDIN, "")
    $pl = new PerlACE::Process ("$ENV{'ACE_ROOT'}/bin/dance", "--log-level 1 -x MockTest.cdp -k file://em.ior");
}

$em_status = $em->Spawn();
sleep 30;
#print STDIN "c\n";
#open (OLD_STDIN, "<STDIN");
#open (kid, "|-");
open (STDIN, "<./plCommand.In");
$pl_status = $pl->Spawn();
sleep 10;
#print kid "c\n";

#print STDIN "c\n";
sleep 10;


$pl_status = $pl->Kill();
close (STDIN);
$em_status = $em->Kill();

$ns_status = $ns->Kill();

$mt_status = $mt->Kill();

#delete IOR files
if (-e em.ior) {
    unlink "em.ior"
}
if (-e ns.ior) {
    unlink "ns.ior"
}

if ($pl_status != 0)
{
    print "\n ERROR: test returned $pl_status \n";
}

exit $pl_status;
														