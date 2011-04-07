eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$tg = PerlACE::TestTarget::create_target (1) || die "Create target for ns failed\n";

$status = 0;
$iorbase1 = "NodeApp1.ior";
$iorbase2 = "NodeApp2.ior";
$iorbase3 = "NodeApp3.ior";
$iorbase4 = "NodeApp4.ior";
$iorbase5 = "NodeApp5.ior";

$iorfile1 = $tg->LocalFile ("NodeApp1.ior");
$iorfile2 = $tg->LocalFile ("NodeApp2.ior");
$iorfile3 = $tg->LocalFile ("NodeApp3.ior");
$iorfile4 = $tg->LocalFile ("NodeApp4.ior");
$iorfile5 = $tg->LocalFile ("NodeApp5.ior");

$tg->DeleteFile ("NodeApp1.ior");
$tg->DeleteFile ("NodeApp2.ior");
$tg->DeleteFile ("NodeApp3.ior");
$tg->DeleteFile ("NodeApp4.ior");
$tg->DeleteFile ("NodeApp5.ior");

$CIAO_ROOT=$ENV{'CIAO_ROOT'};

$SV1 = $tg->CreateProcess ("$DANCE_ROOT/bin/dance_node_manager",
                             "-ORBEndpoint iiop://localhost:10000 -s $DANCE_ROOT/bin/dance_locality_manager");

$SV2 = $tg->CreateProcess ("$DANCE_ROOT/bin/dance_node_manager",
                             "-ORBEndpoint iiop://localhost:20000  -s $DANCE_ROOT/bin/dance_locality_manager");

$SV3 = $tg->CreateProcess ("$DANCE_ROOT/bin/dance_node_manager",
                             "-ORBEndpoint iiop://localhost:30000  -s $DANCE_ROOT/bin/dance_locality_manager");

$SV4 = $tg->CreateProcess ("$DANCE_ROOT/bin/dance_node_manager",
                             "-ORBEndpoint iiop://localhost:40000  -s $DANCE_ROOT/bin/dance_locality_manager");

$SV5 = $tg->CreateProcess ("$DANCE_ROOT/bin/dance_node_manager",
                             "-ORBEndpoint iiop://localhost:50000  -s $DANCE_ROOT/bin/dance_locality_manager");
$SV1->Spawn ();
$SV2->Spawn ();
$SV3->Spawn ();
$SV4->Spawn ();
$SV5->Spawn ();

sleep (99999999999);
