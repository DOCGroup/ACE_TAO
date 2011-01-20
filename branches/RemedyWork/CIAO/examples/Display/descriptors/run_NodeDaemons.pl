eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$tg = PerlACE::TestTarget::create_target (1) || die "Create target for ns failed\n";

$iorbase1 = "NodeApp1.ior";
$iorfile1 = $tg->LocalFile ($iorbase1);
$iorbase2 = "NodeApp2.ior";
$iorfile2 = $tg->LocalFile ($iorbase2);

$iorfile1 = $tg->DeleteFile ($iorbase1);
$iorfile2 = $tg->DeleteFile ($iorbase2);

$CIAO_ROOT=$ENV{'CIAO_ROOT'};

$SV1 = $tg->CreateProcess ("$DANCE_ROOT/bin/dance_node_manager",
                             "-ORBEndpoint iiop://localhost:10001 -s $DANCE_ROOT/bin/dance_locality_manager");

$SV2 = $tg->CreateProcess ("$DANCE_ROOT/bin/dance_node_manager",
                             "-ORBEndpoint iiop://localhost:20001 -s $DANCE_ROOT/bin/dance_locality_manager");

$SV1->Spawn ();
$SV2->Spawn ();

sleep (99999999999);
