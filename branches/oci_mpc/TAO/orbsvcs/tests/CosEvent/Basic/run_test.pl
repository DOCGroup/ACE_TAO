eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a Perl script that runs the client and all the other servers that
# are needed

use lib '../../../../../bin';
use PerlACE::Run_Test;

$status = 0;

$svc_pull_conf = PerlACE::LocalFile ("svc.pull.conf");

sub RunTest ($$$)
{
    my $message = shift;
    my $program = shift;
    my $arguments = shift;

    my $TEST = new PerlACE::Process ($program, $arguments);

    print STDERR "\n\n$message\n";
    
    my $test = $TEST->SpawnWaitKill (240);

    if ($test != 0) {
        print STDERR "ERROR: Test returned $test\n";
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
