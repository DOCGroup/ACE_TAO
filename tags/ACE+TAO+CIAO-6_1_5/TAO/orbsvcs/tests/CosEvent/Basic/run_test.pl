eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

# This is a Perl script that runs the client and all the other servers that
# are needed

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $test = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

$test->AddLibPath ('../lib');

my $svc_pull_conf = "svc.pull$PerlACE::svcconf_ext";
my $test_svc_pull_conf = $test->LocalFile ("svc.pull$PerlACE::svcconf_ext");

sub RunTest ($$$)
{
    my $message = shift;
    my $program = shift;
    my $arguments = shift;

    my $T = $test->CreateProcess ($program, $arguments);

    print STDERR "\n\n$message\n";

    my $process_status = $T->SpawnWaitKill ($test->ProcessStartWaitInterval() + 225);

    if ($process_status != 0) {
        print STDERR "ERROR: Test returned $process_status\n";
        $status = 1;
    }
}

RunTest ("\n\nShutdown EC with clients still attached\n",
         "Shutdown",
         "");

RunTest ("\n\nDisconnect callbacks test\n",
         "Disconnect",
         "");

RunTest ("\n\nMT Disconnects test\n",
         "MT_Disconnect",
         "");

RunTest ("\n\nPush Events\n",
         "Push_Event",
         "");

RunTest ("\n\nPull-Push Events\n",
         "Pull_Push_Event",
         "-ORBSvcConf $svc_pull_conf");

RunTest ("\n\nRandom\n",
         "Random",
         "");

exit $status;
