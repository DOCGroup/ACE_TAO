eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$tg_deamons = PerlACE::TestTarget::create_target (1) || die "Create target for ns failed\n";

$ior_base1 = "NodeApp1.ior";
$ior_file1 = $tg_deamons->LocalFile ($ior_base1);
$ior_base2 = "NodeApp2.ior";
$ior_file2 = $tg_deamons->LocalFile ($ior_base2);

#for ($iter = 0; $iter <= $#ARGV; $iter++) {
#    if ($ARGV[$iter] eq "-h" || $ARGV[$iter] eq "-?") {
#      print "Run_Test Perl script for NodeApplicationTest \n\n";
#      print "run_test \n";
#      print "\n";
#      print "-h                  -- prints this information\n";
#      exit 0;
#  }
#}

$ior_file1 = $tg_deamons->DeleteFile ($ior_base1);
$ior_file2 = $tg_deamons->DeleteFile ($ior_base2);

$CIAO_ROOT=$ENV{'CIAO_ROOT'};
$DANCE_ROOT=$ENV{'DANCE_ROOT'};

$SV1 = $tg_deamons->CreateProcess ("$DANCE_ROOT/bin/dance_node_manager",
                                   "-ORBEndpoint iiop://localhost:30000 -s $DANCE_ROOT/DAnCE/bin/dance_locality_manager -d 30");

$SV2 = $tg_deamons->CreateProcess ("$DANCE_ROOT/bin/dance_node_manager",
                                   "-ORBEndpoint iiop://localhost:40000 -s $DANCE_ROOT/bin/dance_locality_manager -d 30");

$SV1->Spawn ();
$SV2->Spawn ();

sleep (99999999999);
