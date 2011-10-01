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
         description => "case 1: not forward upon OBJECT_NOT_EXIST exception",
         orb_exception_forward_opt => "-ORBForwardOnceOnObjectNotExist 0",
         num_requests => "1",
         client_expect_num_ex => "1",
         client_expect_ex_kind => "1",
         server_raise_ex_kind => "1",
         servant_calls => "1",
     },{
         description => "case 2: default Fault Tolerant always forward upon COMM_FAILURE exception",
         orb_exception_forward_opt => "-ORBForwardOnceOnCommFailure 0",
         num_requests => "1",
         client_expect_num_ex => "0",
         client_expect_ex_kind => "0",
         server_raise_ex_kind => "2",
         servant_calls => "0",  # use 0 to indicate request looping case
     },{
         description => "case 3: default Fault Tolerant always forward upon TRANSIENT exception",
         orb_exception_forward_opt => "-ORBForwardOnceOnTransient 0",
         num_requests => "1",
         client_expect_num_ex => "0",
         client_expect_ex_kind => "0",
         server_raise_ex_kind => "4",
         servant_calls => "0",  # use 0 to indicate request looping case
     },{
         description => "case 4: not forward upon INV_OBJREF exception",
         orb_exception_forward_opt => "-ORBForwardOnceOnInvObjref 0",
         num_requests => "1",
         client_expect_num_ex => "1",
         client_expect_ex_kind => "8",
         server_raise_ex_kind => "8",
         servant_calls => "1",
     },{
         description => "case 5: forward once upon OBJECT_NOT_EXIST exception",
         orb_exception_forward_opt => "-ORBForwardOnceOnObjectNotExist 1",
         num_requests => "4",
         client_expect_num_ex => "4",
         client_expect_ex_kind => "1",
         server_raise_ex_kind => "1",
         servant_calls => "5", # 4 requests + 1 forward
     },{
         description => "case 6: forward once upon COMM_FAILURE exception",
         orb_exception_forward_opt => "-ORBForwardOnceOnCommFailure 1",
         num_requests => "4",
         client_expect_num_ex => "4",
         client_expect_ex_kind => "2",
         server_raise_ex_kind => "2",
         servant_calls => "5", # 4 requests + 1 forward
     },{
         description => "case 7: forward once upon TRANSIENT exception",
         orb_exception_forward_opt => "-ORBForwardOnceOnTransient 1",
         num_requests => "4",
         client_expect_num_ex => "4",
         client_expect_ex_kind => "4",
         server_raise_ex_kind => "4",
         servant_calls => "5", # 4 requests + 1 forward
     },{
         description => "case 8: forward once upon INV_OBJREF exception",
         orb_exception_forward_opt => "-ORBForwardOnceOnInvObjref 1",
         num_requests => "4",
         client_expect_num_ex => "4",
         client_expect_ex_kind => "8",
         server_raise_ex_kind => "8",
         servant_calls => "5", # 4 requests + 1 forward
     },
    );

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $iorbase = "test.ior";
my $host = $server->HostName(); #"localhost";
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
                                            "-r $test->{servant_calls}");

    $CL = $client->CreateProcess ("client", "-k corbaloc::$host:$port/Simple_Server -x ".
                                            "-e $test->{client_expect_ex_kind} " .
                                            "-n $test->{client_expect_num_ex} ".
                                            "-i $test->{num_requests} $test->{orb_exception_forward_opt}");

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
