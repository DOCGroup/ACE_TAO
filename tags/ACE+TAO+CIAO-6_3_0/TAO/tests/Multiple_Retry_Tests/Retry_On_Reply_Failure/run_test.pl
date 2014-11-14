eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

# Current supported forward once exceptions:
#      EF_OBJECT_NOT_EXIST = 0x1,
#      EF_COMM_FAILURE     = 0x2,
#      EF_TRANSIENT        = 0x4,
#      EF_INV_OBJREF       = 0x8,
@configurations = ({
         description => "case 1: retry on OBJECT_NOT_EXIST exceptions until no failure",
         orb_invocation_retry_opts => "-ORBForwardOnObjectNotExistLimit 10 -ORBForwardDelay 50",
         client_expect_ex_kind => "0",
	 server_raise_ex_kind => "1",
	 num_exceptions_to_throw => 2,
     },{
         description => "case 2: retry on OBJECT_NOT_EXIST exceptions and give up",
         orb_invocation_retry_opts => "-ORBForwardOnObjectNotExistLimit 1 -ORBForwardDelay 50",
         client_expect_ex_kind => "1",
	 server_raise_ex_kind => "1",
	 num_exceptions_to_throw => 2,
     },{
         description => "case 3: retry on COMM_FAILURE exceptions until no failure",
         orb_invocation_retry_opts => "-ORBForwardOnCommFailureLimit 10 -ORBForwardDelay 50",
         client_expect_ex_kind => "0",
	 server_raise_ex_kind => "2",
	 num_exceptions_to_throw => 2,
     },{
         description => "case 4: retry on COMM_FAILURE exceptions and give up",
         orb_invocation_retry_opts => "-ORBForwardOnCommFailureLimit 1 -ORBForwardDelay 50",
         client_expect_ex_kind => "2",
	 server_raise_ex_kind => "2",
	 num_exceptions_to_throw => 2,
     },{
         description => "case 5: retry on TRANSIENT exceptions until no failure",
         orb_invocation_retry_opts => "-ORBForwardOnTransientLimit 10 -ORBForwardDelay 50",
         client_expect_ex_kind => "0",
	 server_raise_ex_kind => "4",
	 num_exceptions_to_throw => 2,
     },{
         description => "case 6: retry on TRANSIENT exceptions and give up",
         orb_invocation_retry_opts => "-ORBForwardOnTransientLimit 1 -ORBForwardDelay 50",
         client_expect_ex_kind => "4",
	 server_raise_ex_kind => "4",
	 num_exceptions_to_throw => 2,
     },{
         description => "case 7: retry on INV_OBJREF exceptions until no failure",
         orb_invocation_retry_opts => "-ORBForwardOnInvObjrefLimit 10 -ORBForwardDelay 50",
         client_expect_ex_kind => "0",
	 server_raise_ex_kind => "8",
	 num_exceptions_to_throw => 2,
     },{
         description => "case 8: retry on INV_OBJREF exceptions and give up",
         orb_invocation_retry_opts => "-ORBForwardOnInvObjrefLimit 1 -ORBForwardDelay 50",
         client_expect_ex_kind => "8",
	 server_raise_ex_kind => "8",
	 num_exceptions_to_throw => 2,
     },



    );

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $iorbase = "test.ior";
my $host = $server->HostName();
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);

for $test (@configurations) {
    print STDERR "\n==== Running $test->{description} \n\n";

    my $port = $server->RandomPort();
    $server->DeleteFile($iorbase);
    $client->DeleteFile($iorbase);

    $SV = $server->CreateProcess ("server", "-ORBdebuglevel $debug_level ".
                                            "-o $server_iorfile ".
                                            "-ORBEndpoint iiop://$host:$port ".
                                            "-e $test->{server_raise_ex_kind} ".
	                                    "-n $test->{num_exceptions_to_throw}");

    $CL = $client->CreateProcess ("client", "-k corbaloc::$host:$port/Simple_Server ".
                                            "-e $test->{client_expect_ex_kind} ".
                                            "$test->{orb_invocation_retry_opts}");

    print STDERR $SV->CommandLine () . "\n";
    print STDERR $CL->CommandLine () . "\n";

    $server_status = $SV->Spawn ();

    if ($server_status != 0) {
        print STDERR "ERROR: server returned $server_status\n";
        exit 1;
    }

    if ($server->WaitForFileTimed ($iorbase,
                                   $server->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$server_iorfile>\n";
        $SV->Kill (); $SV->TimedWait (1);
        exit 1;
    }

    if ($server->GetFile ($iorbase) == -1) {
        print STDERR "ERROR: cannot retrieve file <$server_iorfile>\n";
        $SV->Kill (); $SV->TimedWait (1);
        exit 1;
    }

    if ($client->PutFile ($iorbase) == -1) {
        print STDERR "ERROR: cannot set file <$client_iorfile>\n";
        $SV->Kill (); $SV->TimedWait (1);
        exit 1;
    }

    $client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 45);

    if ($client_status != 0) {
        print STDERR "ERROR: client returned $client_status\n";
        $status = 1;
    }

    $server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

    if ($server_status != 0) {
        print STDERR "ERROR: server returned $server_status\n";
        $status = 1;
    }

}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

exit $status;
