eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{'ACE_ROOT'}/bin";
use PerlACE::TestTarget;

$tg_sender = PerlACE::TestTarget::create_target (1) || die "Create target for EM failed\n";
$tg_receiver = PerlACE::TestTarget::create_target (1) || die "Create target for EM failed\n";

$domain_id = "$ENV{'DDS4CCM_DEFAULT_DOMAIN_ID'}";

print "Start receiver\n";
$R = $tg_receiver->CreateProcess ("./DDS_ObO_receiver", "$domain_id");
$R->Spawn();

sleep (1);

print "Start sender\n";
$S = $tg_sender->CreateProcess ("./DDS_ObO_Sender", "$domain_id");
$S->Spawn ();


sleep (90);

$R->Kill ();
$S->Kill ();

exit 0;
