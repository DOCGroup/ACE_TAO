eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{'ACE_ROOT'}/bin";
use PerlACE::Run_Test;

$CIAO_ROOT = "$ENV{'CIAO_ROOT'}";
$TAO_ROOT = "$ENV{'TAO_ROOT'}";
$DAnCE = "$ENV{'DANCE_ROOT'}";

print "Start receiver\n";
$R = new PerlACE::Process ("$CIAO_ROOT/connectors/dds4ccm/examples/Hello/DDS_Receiver/DDS_receiver", "");
$R->Spawn();
print "Start sender\n";
$S = new PerlACE::Process ("$CIAO_ROOT/connectors/dds4ccm/examples/Hello/DDS_Sender/DDS_Sender",
                        "-i 10000 -t -s \"Hello, World\"");
$S->SpawnWaitKill (30);
$R->Kill ();

exit 0;
