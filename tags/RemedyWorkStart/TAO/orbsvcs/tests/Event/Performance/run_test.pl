eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a Perl script that runs the client and all the other servers that
# are needed

use lib  "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;

my $test = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

$test->AddLibPath ('../lib');

$ec_st_conf = $test->LocalFile ("ec.st$PerlACE::svcconf_ext");

sub RunTest ($$$)
{
    my $message = shift;
    my $program = shift;
    my $arguments = shift;

    my $T = $test->CreateProcess ($program, $arguments);

    print STDERR "\n\n$message\n";

    my $test_status = $T->SpawnWaitKill ($test->ProcessStartWaitInterval() + 45);

    if ($test_status != 0) {
        print STDERR "ERROR: Test returned $test_status\n";
        $status = 1;
    }
}

RunTest ("\n\nThroughput/Latency single threaded configuration\n",
         "Throughput",
         "-ORBsvcconf $ec_st_conf -burstsize 2000 -burstcount 1");

RunTest ("\n\nThroughput/Latency MT-safe configuration\n",
         "Throughput",
         "-burstsize 2000 -burstcount 1");

RunTest ("\n\nThroughput/Latency MT-safe configuration, 4 consumers\n",
         "Throughput",
         "-burstsize 2000 -burstcount 1 -consumers 4");

RunTest ("\n\nThroughput/Latency MT-safe configuration, 4 consumers 4 suppliers\n",
         "Throughput",
         "-burstsize 2000 -burstcount 1 -consumers 4 -suppliers 4");

RunTest ("\n\nThroughput/Latency MT-safe configuration, 4 consumers 4 suppliers\n",
         "Throughput",
         "-burstsize 2000 -burstcount 1 -consumers 4 -suppliers 4"
         . " -consumers_tshift 0 -suppliers_tshift 0");

RunTest ("\n\nConnection and disconnection time, 100 consumers 100 suppliers\n",
         "Connect",
         "-consumers 100 -suppliers 100 -connection_order interleaved");

RunTest ("\n\nConnection and disconnection time, 500 consumers 500 suppliers\n",
         "Connect",
         "-consumers 500 -suppliers 500 -connection_order interleaved");

exit $status;
