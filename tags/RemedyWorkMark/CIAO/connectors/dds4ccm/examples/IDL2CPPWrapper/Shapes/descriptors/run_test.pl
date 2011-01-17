eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{'ACE_ROOT'}/bin";
use PerlACE::TestTarget;

$tg_sender = PerlACE::TestTarget::create_target (1) || die "Create target for EM failed\n";
$tg_receiver = PerlACE::TestTarget::create_target (1) || die "Create target for EM failed\n";
$tg_sender->AddLibPath ('../lib');
$tg_receiver->AddLibPath ('../lib');


print "Start receiver\n";
$R = $tg_receiver->CreateProcess ("../lib/I2C_Shapes_Receiver", "");
$R->Spawn();

sleep (1);

print "Start sender\n";
$S = $tg_sender->CreateProcess ("../lib/I2C_Shapes_Sender", "");
$S->Spawn ();


sleep (80);

$R->Kill ();
$S->Kill ();

exit 0;
