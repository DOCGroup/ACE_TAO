eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{'ACE_ROOT'}/bin";
use PerlACE::TestTarget;

$CIAO_ROOT = "$ENV{'CIAO_ROOT'}";

$tg_sender = PerlACE::TestTarget::create_target (1) || die "Create target for EM failed\n";
$tg_receiver = PerlACE::TestTarget::create_target (1) || die "Create target for EM failed\n";

$tg_sender->AddLibPath ('../lib');
$tg_receiver->AddLibPath ('../lib');

print "Start receiver\n";
$R = $tg_receiver->CreateProcess ("$CIAO_ROOT/connectors/dds4ccm/performance-tests/DDSLatency/DDS_Receiver/DDS_receiver", "");
$R->Spawn();
sleep (5);
print "Start sender\n";
$S = $tg_sender->CreateProcess ("$CIAO_ROOT/connectors/dds4ccm/performance-tests/DDSLatency/DDS_Sender/DDS_Sender", "");
$S->SpawnWaitKill ($tg_sender->ProcessStartWaitInterval () + 180);
$R->Kill ();
exit 0;
