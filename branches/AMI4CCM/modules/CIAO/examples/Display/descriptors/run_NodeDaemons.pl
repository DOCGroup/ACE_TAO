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

$CIAO_ROOT=$ENV{'CIAO_ROOT'};

$SV1 = new PerlACE::Process ("$DANCE_ROOT/bin/dance_node_manager",
                             "-ORBEndpoint iiop://localhost:10001 -s $CIAO_ROOT/DAnCE/NodeApplication/NodeApplication");

$SV2 = new PerlACE::Process ("$DANCE_ROOT/bin/dance_node_manager",
                             "-ORBEndpoint iiop://localhost:20001 -s $CIAO_ROOT/DAnCE/NodeApplication/NodeApplication");

$SV1->Spawn ();
$SV2->Spawn ();

sleep (99999999999);
