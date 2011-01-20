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

my $ns = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $nfs = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $test = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

$test->AddLibPath ('../lib');

PerlACE::check_privilege_group();

$experiment_timeout = 60;
$startup_timeout = 60;

$nsiorfile = "naming.ior";
$nfsiorfile = "notify.ior";
my $svcconffile = "adminproperties.conf";

my $ns_nsiorfile = $ns->LocalFile ($nsiorfile);
my $nfs_nsiorfile = $nfs->LocalFile ($nsiorfile);
my $test_nsiorfile = $test->LocalFile ($nsiorfile);
my $nfs_nfsiorfile = $nfs->LocalFile ($nfsiorfile);
my $test_svcconffile = $test->LocalFile ($svcconffile);
$ns->DeleteFile ($nsiorfile);
$nfs->DeleteFile ($nsiorfile);
$test->DeleteFile ($nsiorfile);
$nfs->DeleteFile ($nfsiorfile);

@tests = ( {
    name => "AdminProperties",
    args => "-ORBSvcConf $test_svcconffile -ORBConnectIPV6Only 1",
   }, {
    name => "ConnectDisconnect",
    args => "-ORBConnectIPV6Only 1",
    extra => 300,
   }, {
    name => "Events",
    args => "-ORBConnectIPV6Only 1",
   }, {
    name => "IdAssignment",
    args => "-ORBConnectIPV6Only 1",
   }, {
    name => "LifeCycle",
    args => "-ORBConnectIPV6Only 1",
   }, {
    name => "Simple",
    args => "-ORBConnectIPV6Only 1",
   }, {
    name => "MultiTypes",
    args => "-ORBConnectIPV6Only 1",
   }, {
    name => "Filter",
    args => "-ORBConnectIPV6Only 1",
   }, {
    name => "Updates",
    args => "-ORBConnectIPV6Only 1",
   }, {
    name => "Sequence",
    args => "-ORBConnectIPV6Only 1",
   }, );

@default_test_configs = (
   "notify.rt.conf",
   "notify.reactive.conf",
   "notify.mt.conf"
);

if ($#ARGV == -1) {
    @test_configs = @default_test_configs;
}
else {
    @test_configs = @ARGV;
}

$NS = $ns->CreateProcess ("../../../Naming_Service/tao_cosnaming",
                              "-o $ns_nsiorfile");

$NS_status = $NS->Spawn ();
if ($NS_status != 0) {
    print STDERR "ERROR: Name Service returned $NS_status\n";
    exit 1;
}
if ($ns->WaitForFileTimed ($nsiorfile,$ns->ProcessStartWaitInterval()+$startup_timeout) == -1) {
    print STDERR "ERROR: cannot find file <$ns_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($ns->GetFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$ns_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($nfs->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$nfs_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($test->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$test_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

for $dispatch_opt ("", "-UseSeparateDispatchingOrb 1") {

for $config (@test_configs) {
    if ($dispatch_opt =~ /UseSeparateDispatchingORB 1/i
        && ($config =~ /\.(reactive|rt)\./)) {
        print STDERR "\nSkipping $config;  not supported with $dispatch_opt\n\n";
        next;
      }

    print STDERR "\nTesting Notification Service with config file = $config ....\n\n";

    my $nfs_config = $nfs->LocalFile ($config);

    $NFS = $nfs->CreateProcess ("../../../Notify_Service/tao_cosnotification",
                                ' '.$dispatch_opt.' '.
                                "-ORBInitRef NameService=file://$nfs_nsiorfile " .
                                "-IORoutput $nfs_nfsiorfile " .
                                "-ORBSvcConf $nfs_config ".
                                "-ORBListenEndpoints iiop://1.2@[::1]");
    $nfs->DeleteFile ($nfsiorfile);
    $NFS_status = $NFS->Spawn ();
    if ($NFS_status != 0) {
        print STDERR "ERROR: Notify Service returned $NFS_status\n";
        $NS->Kill (); $NS->TimedWait (1);
        $NFS->Kill (); $NFS->TimedWait (1);
        exit 1;
    }
    if ($nfs->WaitForFileTimed ($nfsiorfile,$nfs->ProcessStartWaitInterval()+$startup_timeout) == -1) {
        print STDERR "ERROR: cannot find file <$nfs_nfsiorfile>\n";
        $NS->Kill (); $NS->TimedWait (1);
        $NFS->Kill (); $NFS->TimedWait (1);
        exit 1;
    }

    for $name (@tests) {
        $status = 0;
        ## The MaxQueueLength and MaxEventsPerConsumer are not supported in the Reactive
        ## configuration, so we skip this test for now.
        ## The Notification should actually throw an exception for the property not supported.
        if ($name->{name} eq "AdminProperties"
            && ($config eq "notify.reactive.conf" || $config eq "notify.rt.conf")) {
            next;
          }

        print STDERR "\nTesting $name->{name}....\n\n";
        $TEST = $test->CreateProcess ("./$name->{name}",
                                      "-ORBInitRef NameService=file://$test_nsiorfile " .
                                      "$name->{args} ");
        $TEST_status = $TEST->Spawn ();
        if ($TEST_status != 0) {
            print STDERR "ERROR: Test returned $TEST_status\n";
            $TEST->Kill (); $TEST->TimedWait (1);
            $NFS->Kill (); $NFS->TimedWait (1);
            $NS->Kill (); $NS->TimedWait (1);
            exit 1;
        }

        $TEST_status = $TEST->WaitKill ($test->ProcessStartWaitInterval()+$experiment_timeout +
                                       (defined $name->{extra} ?
                                                $name->{extra} : 0));
        if ($TEST_status != 0) {
            print STDERR "ERROR: $name->{name} returned $TEST_status\n";
            $TEST->Kill (); $TEST->TimedWait (1);
        }
    }

    $NFS_status = $NFS->Kill ($nfs->ProcessStopWaitInterval());
    if ($NFS_status != 0) {
        print STDERR "ERROR: Notify Service returned $NFS_status\n";
        $NFS->Kill (); $NFS->TimedWait (1);
        $NS->Kill (); $NS->TimedWait (1);
        exit 1;
    }

}
}

$NS_status = $NS->TerminateWaitKill ($ns->ProcessStopWaitInterval());
if ($NS_status != 0) {
    print STDERR "ERROR: Name Service returned $NS_status\n";
    $status = 1;
}

$ns->DeleteFile ($nsiorfile);
$nfs->DeleteFile ($nsiorfile);
$test->DeleteFile ($nsiorfile);
$nfs->DeleteFile ($nfsiorfile);

exit $status;
