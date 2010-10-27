eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;
$iorfile1 = PerlACE::LocalFile ("NodeApp1.ior");
$iorfile2 = PerlACE::LocalFile ("NodeApp2.ior");
$iorfile3 = PerlACE::LocalFile ("NodeApp3.ior");
$iorfile4 = PerlACE::LocalFile ("NodeApp4.ior");
$iorfile5 = PerlACE::LocalFile ("NodeApp5.ior");

#for ($iter = 0; $iter <= $#ARGV; $iter++) {
#    if ($ARGV[$iter] eq "-h" || $ARGV[$iter] eq "-?") {
#      print "Run_Test Perl script for NodeApplicationTest \n\n";
#      print "run_test \n";
#      print "\n";
#      print "-h                  -- prints this information\n";
#      exit 0;
#  }
#}


unlink $iorfile1;
unlink $iorfile2;
unlink $iorfile3;
unlink $iorfile4;
unlink $iorfile5;

$CIAO_ROOT=$ENV{'CIAO_ROOT'};

$SV1 = new PerlACE::Process ("$DANCE_ROOT/bin/dance_node_manager",
                             "-ORBEndpoint iiop://localhost:10000 -s
$CIAO_ROOT/DAnCE/NodeApplication/NodeApplication");

$SV2 = new PerlACE::Process ("$DANCE_ROOT/bin/dance_node_manager",
                             "-ORBEndpoint iiop://localhost:20000  -s
$CIAO_ROOT/DAnCE/NodeApplication/NodeApplication");

$SV3 = new PerlACE::Process ("$DANCE_ROOT/bin/dance_node_manager",
                             "-ORBEndpoint iiop://localhost:30000  -s
$CIAO_ROOT/DAnCE/NodeApplication/NodeApplication");

$SV4 = new PerlACE::Process ("$DANCE_ROOT/bin/dance_node_manager",
                             "-ORBEndpoint iiop://localhost:40000  -s
$CIAO_ROOT/DAnCE/NodeApplication/NodeApplication");

$SV5 = new PerlACE::Process ("$DANCE_ROOT/bin/dance_node_manager",
                             "-ORBEndpoint iiop://localhost:50000  -s
$CIAO_ROOT/DAnCE/NodeApplication/NodeApplication");
$SV1->Spawn ();
$SV2->Spawn ();
$SV3->Spawn ();
$SV4->Spawn ();
$SV5->Spawn ();

sleep (99999999999);
