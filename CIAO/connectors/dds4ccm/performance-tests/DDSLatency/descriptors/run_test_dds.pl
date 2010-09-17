eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{'ACE_ROOT'}/bin";
use PerlACE::TestTarget;

foreach $i (@ARGV) {
    if ($i eq '-b') {
        $rw_extra = '-b:';
    }
}

$nr_runs = 2;
@profiles = ("Latency_Library#UDPv4QoS", "Latency_Library#SharedMemQos");

$CIAO_ROOT = "$ENV{'CIAO_ROOT'}";
$DDS4CCM_ROOT = "$CIAO_ROOT/connectors/dds4ccm";

$tg_sender = PerlACE::TestTarget::create_target (1) || die "Create target for EM failed\n";
$tg_receiver = PerlACE::TestTarget::create_target (1) || die "Create target for EM failed\n";

$tg_sender->AddLibPath ('../lib');
$tg_receiver->AddLibPath ('../lib');

sub run_tests {
    for ($i = 0; $i < $nr_runs; ++$i) {
        $qos = $profiles[$i];

        print "Start receiver with QoS profile <$qos>\n";
        $R = $tg_receiver->CreateProcess ("$DDS4CCM_ROOT/performance-tests/DDSLatency/DDS_Receiver/DDS_receiver", "-q $qos $rw_extra");

        $R->Spawn();

        print "Start sender with QoS profile <$qos>\n";
        $S = $tg_sender->CreateProcess ("$DDS4CCM_ROOT/performance-tests/DDSLatency/DDS_Sender/DDS_Sender", "-q $qos $rw_extra");

        $S->SpawnWaitKill ($tg_sender->ProcessStartWaitInterval () + 180);
        $R->Kill ();
    }
}

run_tests ();

exit 0;
