eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a Perl script that runs the client and all the other servers that
# are needed

use lib  '../../../../../bin';
use PerlACE::Run_Test;

$status = 0;

$ec_st_conf = PerlACE::LocalFile ("ec.st$PerlACE::svcconf_ext");

sub RunTest ($$$)
{
    my $message = shift;
    my $program = shift;
    my $arguments = shift;

    my $TEST = new PerlACE::Process ($program, $arguments);

    print STDERR "\n\n$message\n";
    
    my $test = $TEST->SpawnWaitKill (60);

    if ($test != 0) {
        print STDERR "ERROR: Test returned $test\n";
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
