eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# ******************************************************************
# Pragma Section
# ******************************************************************

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $nfs = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $sup = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $con = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

$sup->AddLibPath ('../lib');
$con->AddLibPath ('../lib');


# ******************************************************************
# Data Section
# ******************************************************************

my $eventType = '-any'; # your choice of -any -structured or -sequence
my $notify_port = $nfs->RandomPort ();
my $notify_host = $nfs->HostName ();

my $st_topo = "ns_st_topo.conf";
my $st_both = "ns_st_both.conf";
# File used to detect notification service startup
$nfsiorfile = "notify.ior";
# Hard coded file name in Consumer.cpp (for now)
$conidsfile = "consumer.ids";
 # Hard coded file name in Supplier.cpp (for now)
$supidsfile = "supplier.ids";
# File used to communicate channel # from consumer to supplier
$channel_id = "channel_id";
# File used to save configuration for reconnect test
$save_xml = "reconnect_test.xml";
# File names comes from svc.conf (+.xml & .000)
$save_000 = "reconnect_test.000";
# File names comes from svc.conf (+.xml & .001)
$save_001 = "reconnect_test.001";
# Event persistent database file (from svc.conf)
$eventpersist = "event_persist.db";
# Hardcode filename written by the Supplier when it pauses.
$supplier_pause = "Supplier.paused";

my $nfs_nfsiorfile = $nfs->LocalFile ($nfsiorfile);
my $con_conidsfile = $con->LocalFile ($conidsfile);
my $sup_supidsfile = $sup->LocalFile ($supidsfile);
my $con_channel_id = $con->LocalFile ($channel_id);
my $sup_channel_id = $sup->LocalFile ($channel_id);
my $nfs_save_xml = $nfs->LocalFile ($save_xml);
my $nfs_save_000 = $nfs->LocalFile ($save_000);
my $nfs_save_001 = $nfs->LocalFile ($save_001);
my $nfs_eventpersist = $nfs->LocalFile ($eventpersist);
my $sup_supplier_pause = $sup->LocalFile ($supplier_pause);
my $nfs_svcconf = $nfs->LocalFile ($st_topo);
my $nfs_st_both = $nfs->LocalFile ($st_both);
$nfs->DeleteFile ($nfsiorfile);
$con->DeleteFile ($conidsfile);
$sup->DeleteFile ($supidsfile);
$con->DeleteFile ($channel_id);
$sup->DeleteFile ($channel_id);
$nfs->DeleteFile ($save_xml);
$nfs->DeleteFile ($save_000);
$nfs->DeleteFile ($save_001);
$nfs->DeleteFile ($eventpersist);
$sup->DeleteFile ($supplier_pause);

# ******************************************************************
# Main Section
# ******************************************************************

my $verbose = "";
my $discon  = "";

# Process command line arguments
foreach my $i (@ARGV) {
    if ($i eq '-any') {
        $eventType = '-any';
    }
    elsif ($i eq '-str' or $i eq '-structured') {
        $eventType = '-structured';
    }
    elsif ($i eq '-seq' or $i eq '-sequence') {
        $eventType = '-sequence';
    }
    elsif ($i eq '-mt') {
        $svcconf = 'ns_mt_topo.conf';
        $nfs_svcconf = $nfs->LocalFile ($svcconf);
    }
    elsif ($i eq '-disconnect') {
        $discon= '-disconnect';
    }
    elsif ($i eq '-v' or $i eq '-verbose') {
        $verbose = '-v';
    }
    else {
        print "TEST SCRIPT: unknown: $i\n";
        print "TEST SCRIPT: usage: [-any|-str|-seq] -mt -v\n";
        exit(-4);
    }
}

my $client_args = "$eventType $verbose -NoNameSvc -ORBInitRef " .
                   "NotifyEventChannelFactory=corbaloc::" .
                   "$notify_host:$notify_port/NotifyEventChannelFactory ";
my $ns_base_args = "-ORBObjRefStyle url -NoNameSvc -Boot " .
                    "-IORoutput $nfs_nfsiorfile " .
                    "-ORBEndpoint iiop://:$notify_port ";
my $ns_args = "-ORBSvcConf $nfs_svcconf $ns_base_args";

# Delete the all of the files used in the test

# Define the processes first
my $NFS = $nfs->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/Notify_Service/tao_cosnotification");
my $CON = $con->CreateProcess ("Consumer");
my $SUP = $sup->CreateProcess ("Supplier");

################
# begin test 1
################

if ($verbose eq '-v') {
    print "TEST SCRIPT: Starting Notify Service on port $notify_port\n";
}
$NFS->Arguments($ns_args);
if ($verbose eq '-v') {print "TEST SCRIPT: " . $NFS->CommandLine() . "\n";}
$NFS_status = $NFS->Spawn ();
if ($NFS_status != 0) {
    print STDERR "ERROR: Notify Service returned $NFS_status\n";
    exit 1;
}
if ($nfs->WaitForFileTimed ($nfsiorfile,$nfs->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$nfs_nfsiorfile>\n";
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

# start the consumer,
# write the channel number to channel.id for use by Supplier
# expect to receive 20 events of type any
#
$CON->Arguments("-channel $con_channel_id " .
                "-expect 20 $discon $client_args");
print "TEST SCRIPT: ", $CON->CommandLine(), "\n" if ($verbose eq '-v');
$CON_status = $CON->Spawn ();
if ($CON_status != 0) {
    print STDERR "ERROR: Consumer returned $CON_status\n";
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($con->WaitForFileTimed ($channel_id,$con->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$con_channel_id>\n";
    $CON->Kill (); $CON->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($con->GetFile ($channel_id) == -1) {
    print STDERR "ERROR: cannot retrieve file <$con_channel_id>\n";
    $CON->Kill (); $CON->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($sup->PutFile ($channel_id) == -1) {
    print STDERR "ERROR: cannot set file <$sup_channel_id>\n";
    $CON->Kill (); $CON->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

# send 10 Any's, picking up the channel# from channel.id
$SUP->Arguments("-channel $sup_channel_id -send 10 $client_args");
print "TEST SCRIPT: ", $SUP->CommandLine(), "\n" if ($verbose eq '-v');
$SUP_status = $SUP->SpawnWaitKill ($sup->ProcessStartWaitInterval()+45);
if ($SUP_status != 0) {
    print STDERR "ERROR: Supplier returned $SUP_status\n";
    $CON->Kill (); $CON->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

# forget the channel id, depend on the reconnect information
$con->DeleteFile ($channel_id);
$sup->DeleteFile ($channel_id);

$SUP->Arguments("-send 10 -serial_number 10 $discon $client_args");
if ($verbose eq '-v') {print "TEST SCRIPT: " . $SUP->CommandLine() . "\n";}
$SUP_status = $SUP->SpawnWaitKill ($sup->ProcessStartWaitInterval()+45);
if ($SUP_status != 0) {
    print STDERR "ERROR: Supplier returned $SUP_status\n";
    $CON->Kill (); $CON->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

$CON_status = $CON->WaitKill ($con->ProcessStopWaitInterval()+45);
if ($CON_status != 0) {
    print STDERR "ERROR: Consumer returned $CON_status\n";
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

print "TEST SCRIPT: ****Passed: Supplier reconnect test.\n";

################
# end of test 1
################

################
# begin test 2
################
# Now start the consumer again.  Depending on reconnect this time
# Let it write the channel.id file so we know when it's up.

if ($verbose eq '-v') {print "TEST SCRIPT: " . $CON->CommandLine() . "\n";}
$CON_status = $CON->Spawn ();
if ($CON_status != 0) {
    print STDERR "ERROR: Consumer returned $CON_status\n";
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($con->WaitForFileTimed ($channel_id,$con->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$con_channel_id>\n";
    $CON->Kill (); $CON->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($con->GetFile ($channel_id) == -1) {
    print STDERR "ERROR: cannot retrieve file <$con_channel_id>\n";
    $CON->Kill (); $CON->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($sup->PutFile ($channel_id) == -1) {
    print STDERR "ERROR: cannot set file <$sup_channel_id>\n";
    $CON->Kill (); $CON->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

# forget the channel id, depend on the supplier reconnect information
$con->DeleteFile ($channel_id);
$sup->DeleteFile ($channel_id);

$SUP->Arguments("-send 20 -serial_number 0 $client_args");
if ($verbose eq '-v') {print "TEST SCRIPT: " . $SUP->CommandLine() . "\n";}
$SUP_status = $SUP->SpawnWaitKill ($sup->ProcessStartWaitInterval()+45);
if ($SUP_status != 0) {
    print STDERR "ERROR: Supplier returned $SUP_status\n";
    $CON->Kill (); $CON->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

$CON_status = $CON->WaitKill ($con->ProcessStopWaitInterval()+45);
if ($CON_status != 0) {
    print STDERR "ERROR: Consumer returned $CON_status\n";
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

print "TEST SCRIPT: ****Passed: Consumer reconnect test.\n";

################
# end of test 2
################

################
# begin test 3
################

if ($verbose eq '-v') {print "TEST SCRIPT: Stop the Notification Service\n";}
$NFS_status = $NFS->Kill ($nfs->ProcessStopWaitInterval());
if ($NFS_status != 0) {
    print STDERR "ERROR: Notify Service returned $NFS_status\n";
    exit 1;
}
$nfs->DeleteFile ($nfsiorfile);

if ($verbose eq '-v') {
    print "TEST SCRIPT: Restarting Notify Service on port $notify_port\n",
          "TEST SCRIPT: It should load topology from $nfs_save_xml\n";
}

# sleep to avoid socket-related problems
sleep(10 * $nfs->ProcessStartWaitInterval());
$NFS->Arguments($ns_args);
if ($verbose eq '-v') {print "TEST SCRIPT: " . $NFS->CommandLine() . "\n";}
$NFS_status = $NFS->Spawn ();
if ($NFS_status != 0) {
    print STDERR "ERROR: Notify Service returned $NFS_status\n";
    exit 1;
}
if ($nfs->WaitForFileTimed ($nfsiorfile,$nfs->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$nfs_nfsiorfile>\n";
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

# Now start the consumer again.  It will use the reconnect info.
# Let it write the channel.id file so we know when it's up.
$con->DeleteFile ($channel_id);
$sup->DeleteFile ($channel_id);

if ($verbose eq '-v') {print "TEST SCRIPT: " . $CON->CommandLine() . "\n";}
$CON_status = $CON->Spawn ();
if ($CON_status != 0) {
    print STDERR "ERROR: Consumer returned $CON_status\n";
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($con->WaitForFileTimed ($channel_id,$con->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$con_channel_id>\n";
    $CON->Kill (); $CON->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($con->GetFile ($channel_id) == -1) {
    print STDERR "ERROR: cannot retrieve file <$con_channel_id>\n";
    $CON->Kill (); $CON->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($sup->PutFile ($channel_id) == -1) {
    print STDERR "ERROR: cannot set file <$sup_channel_id>\n";
    $CON->Kill (); $CON->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

# forget the channel id, depend on the supplier reconnect information
$con->DeleteFile ($channel_id);
$sup->DeleteFile ($channel_id);

$SUP->Arguments("-send 20 -serial_number 0 $client_args");
if ($verbose eq '-v') {print "TEST SCRIPT: " . $SUP->CommandLine() . "\n";}
$SUP_status = $SUP->SpawnWaitKill ($sup->ProcessStartWaitInterval()+45);
if ($SUP_status != 0) {
    print STDERR "ERROR: Supplier returned $SUP_status\n";
    $CON->Kill (); $CON->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

$CON_status = $CON->WaitKill ($con->ProcessStopWaitInterval()+45);
if ($CON_status != 0) {
    print STDERR "ERROR: Consumer returned $CON_status\n";
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

print "TEST SCRIPT: ****Passed: Topology Persistence Test.\n";

################
# end of test 3
################

################
# begin test 4
################

# Now start the consumer one more time
# Let it write the channel.id file so we know when it's up.
$con->DeleteFile ($channel_id);
$sup->DeleteFile ($channel_id);

if ($verbose eq '-v') {print "TEST SCRIPT: " . $CON->CommandLine() . "\n";}
$CON_status = $CON->Spawn ();
if ($CON_status != 0) {
    print STDERR "ERROR: Consumer returned $CON_status\n";
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($con->WaitForFileTimed ($channel_id,$con->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$con_channel_id>\n";
    $CON->Kill (); $CON->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($con->GetFile ($channel_id) == -1) {
    print STDERR "ERROR: cannot retrieve file <$con_channel_id>\n";
    $CON->Kill (); $CON->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($sup->PutFile ($channel_id) == -1) {
    print STDERR "ERROR: cannot set file <$sup_channel_id>\n";
    $CON->Kill (); $CON->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

# forget the channel id, depend on the supplier reconnect information
$con->DeleteFile ($channel_id);
$sup->DeleteFile ($channel_id);

# Start the Supplier, tell it to send 10 messages,
# pause until it is reconnected then send another 10.
$SUP->Arguments("-send 20 -pause 10 -serial_number 0 $client_args");
if ($verbose eq '-v') {print "TEST SCRIPT: " . $SUP->CommandLine() . "\n";}
$SUP_status = $SUP->Spawn ();
if ($SUP_status != 0) {
    print STDERR "ERROR: Supplier returned $SUP_status\n";
    $CON->Kill (); $CON->TimedWait (1);
    $SUP->Kill (); $SUP->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($sup->WaitForFileTimed ($supplier_pause,$sup->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$sup_supplier_pause>\n";
    $CON->Kill (); $CON->TimedWait (1);
    $SUP->Kill (); $SUP->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

if ($verbose eq '-v') {print "TEST SCRIPT: Stop the Notification Service\n";}
$NFS_status = $NFS->Kill ($nfs->ProcessStopWaitInterval());
if ($NFS_status != 0) {
    print STDERR "ERROR: Notify Service returned $NFS_status\n";
    exit 1;
}
$nfs->DeleteFile ($nfsiorfile);

if ($verbose eq '-v') {
    print "TEST SCRIPT: Restarting Notify Service\n",
          "TEST SCRIPT: It should load topology from $nfs_save_xml\n",
          "TEST SCRIPT: and reconnect to registered clients.\n";
}

# sleep to avoid socket-related problems
sleep(10 * $nfs->ProcessStartWaitInterval());

if ($verbose eq '-v') {print "TEST SCRIPT: " . $NFS->CommandLine() . "\n";}
$NFS_status = $NFS->Spawn ();
if ($NFS_status != 0) {
    print STDERR "ERROR: Notify Service returned $NFS_status\n";
    exit 1;
}
if ($nfs->WaitForFileTimed ($nfsiorfile,$nfs->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$nfs_nfsiorfile>\n";
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

# at this point, both the consumer and the supplier should reconnect
# and the remaining events should be delivered
# eventually the consumer will finish

$CON_status = $CON->WaitKill ($con->ProcessStopWaitInterval()+5);
if ($CON_status != 0) {
    print STDERR "ERROR: Consumer returned $CON_status\n";
    $SUP->Kill (); $SUP->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

$SUP_status = $SUP->TerminateWaitKill ($sup->ProcessStopWaitInterval());
if ($SUP_status != 0) {
    print STDERR "ERROR: Supplier returned $SUP_status\n";
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

print "TEST SCRIPT: ****Passed: Reconnection Factory test.\n";

################
# end of test 4
################

################
# begin test 5
################

if ($verbose eq '-v') {print "TEST SCRIPT: Stop the Notification Service\n";}
$NFS_status = $NFS->Kill ($nfs->ProcessStopWaitInterval());
if ($NFS_status != 0) {
    print STDERR "ERROR: Notify Service returned $NFS_status\n";
    exit 1;
}
$nfs->DeleteFile ($nfsiorfile);
$con->DeleteFile ($conidsfile);
$sup->DeleteFile ($supidsfile);
$con->DeleteFile ($channel_id);
$sup->DeleteFile ($channel_id);
$nfs->DeleteFile ($save_xml);
$nfs->DeleteFile ($save_000);
$nfs->DeleteFile ($save_001);
$nfs->DeleteFile ($eventpersist);
$sup->DeleteFile ($supplier_pause);

sleep(10 * $nfs->ProcessStartWaitInterval()); ## avoid lingering sockets
# go back to normal ns args
$NFS->Arguments($ns_args);
if ($verbose eq '-v') {print "TEST SCRIPT: " . $NFS->CommandLine() . "\n";}
$NFS_status = $NFS->Spawn ();
if ($NFS_status != 0) {
    print STDERR "ERROR: Notify Service returned $NFS_status\n";
    exit 1;
}
if ($nfs->WaitForFileTimed ($nfsiorfile,$nfs->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$nfs_nfsiorfile>\n";
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

#configure the consumer to expect 20 events, fail (throw an exception
# after 8 events) then continue to listen for remaining events.
#The Notification service should automatically retry (but may discard)
# the failed events.
$CON->Arguments("-channel $con_channel_id " .
                "-expect 20 -fail 8 $client_args");
if ($verbose eq '-v') {print "TEST SCRIPT: " . $CON->CommandLine() . "\n";}
$CON_status = $CON->Spawn ();
if ($CON_status != 0) {
    print STDERR "ERROR: Consumer returned $CON_status\n";
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($con->WaitForFileTimed ($channel_id,$con->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$con_channel_id>\n";
    $CON->Kill (); $CON->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($con->GetFile ($channel_id) == -1) {
    print STDERR "ERROR: cannot retrieve file <$con_channel_id>\n";
    $CON->Kill (); $CON->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($sup->PutFile ($channel_id) == -1) {
    print STDERR "ERROR: cannot set file <$sup_channel_id>\n";
    $CON->Kill (); $CON->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

$SUP->Arguments("-channel $sup_channel_id -send 20 $client_args");
if ($verbose eq '-v') {print "TEST SCRIPT: " . $SUP->CommandLine() . "\n";}
$SUP_status = $SUP->SpawnWaitKill ($sup->ProcessStartWaitInterval()+5);
if ($SUP_status != 0) {
    print STDERR "ERROR: Supplier returned $SUP_status\n";
    $CON->Kill (); $CON->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

## this is a slow test due to the reconnection
## time, so give it enough time...
$CON_status = $CON->WaitKill ($con->ProcessStopWaitInterval()+225);
if ($CON_status != 0) {
    print STDERR "ERROR: Consumer returned $CON_status\n";
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

print "TEST SCRIPT: ****Passed: Consumer recoverable exception test.\n";

################
# end of test 5
################

################
# begin test 6
################

print "TEST SCRIPT: Stop the Notification Service\n" if ($verbose eq '-v');
$NFS_status = $NFS->Kill ($nfs->ProcessStopWaitInterval());
if ($NFS_status != 0) {
    print STDERR "ERROR: Notify Service returned $NFS_status\n";
    exit 1;
}
$nfs->DeleteFile ($nfsiorfile);
$con->DeleteFile ($conidsfile);
$sup->DeleteFile ($supidsfile);
$con->DeleteFile ($channel_id);
$sup->DeleteFile ($channel_id);
$nfs->DeleteFile ($save_xml);
$nfs->DeleteFile ($save_000);
$nfs->DeleteFile ($save_001);
$nfs->DeleteFile ($eventpersist);
$sup->DeleteFile ($supplier_pause);

## Wait a bit to avoid lingering sockets from the previous tao_cosnotification
sleep(3);

$NFS->Arguments("-ORBSvcConf $nfs_st_both $ns_base_args");
print "TEST SCRIPT: ", $NFS->CommandLine(), "\n" if ($verbose eq '-v');
$NFS_status = $NFS->Spawn ();
if ($NFS_status != 0) {
    print STDERR "ERROR: Notify Service returned $NFS_status\n";
    exit 1;
}
if ($nfs->WaitForFileTimed ($nfsiorfile,$nfs->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$nfs_nfsiorfile>\n";
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

my $expected = 10;
$CON->Arguments("-channel $con_channel_id " .
                "-expect $expected $client_args");
print "TEST SCRIPT: ", $CON->CommandLine(), "\n" if ($verbose eq '-v');
$CON_status = $CON->Spawn ();
if ($CON_status != 0) {
    print STDERR "ERROR: Consumer returned $CON_status\n";
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($con->WaitForFileTimed ($channel_id,$con->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$con_channel_id>\n";
    $CON->Kill (); $CON->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($con->GetFile ($channel_id) == -1) {
    print STDERR "ERROR: cannot retrieve file <$con_channel_id>\n";
    $CON->Kill (); $CON->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($sup->PutFile ($channel_id) == -1) {
    print STDERR "ERROR: cannot set file <$sup_channel_id>\n";
    $CON->Kill (); $CON->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

$SUP->Arguments("-channel $sup_channel_id -send $expected $client_args");
print "TEST SCRIPT: ", $SUP->CommandLine(), "\n" if ($verbose eq '-v');
$SUP_status = $SUP->SpawnWaitKill ($sup->ProcessStartWaitInterval()+45);
if ($SUP_status != 0) {
    print STDERR "ERROR: Supplier returned $SUP_status\n";
    $CON->Kill (); $CON->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

$CON_status = $CON->WaitKill ($con->ProcessStopWaitInterval()+15);
if ($CON_status != 0) {
    print STDERR "ERROR: Consumer returned $CON_status\n";
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

my $next_expected = 3;
$SUP->Arguments("-channel $sup_channel_id -send 3 " .
                "-serial_number $expected $client_args");
print "TEST SCRIPT: ", $SUP->CommandLine(), "\n" if ($verbose eq '-v');
$SUP_status = $SUP->SpawnWaitKill ($sup->ProcessStartWaitInterval()+45);
if ($SUP_status != 0) {
    print STDERR "ERROR: Supplier returned $SUP_status\n";
    $CON->Kill (); $CON->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

$CON->Arguments("-channel $con_channel_id -expect $next_expected " .
                "-serial_number $expected $client_args");
print "TEST SCRIPT: ", $CON->CommandLine(), "\n" if ($verbose eq '-v');
$CON_status = $CON->SpawnWaitKill ($con->ProcessStartWaitInterval()+15);
if ($CON_status != 0) {
    print STDERR "ERROR: Consumer returned $CON_status\n";
    $NFS->Kill (); $NFS->TimedWait (1);
    $status = 1;
}

$NFS_status = $NFS->Kill ($nfs->ProcessStopWaitInterval());
if ($NFS_status != 0) {
    print STDERR "ERROR: Notify Service returned $NFS_status\n";
    $status = 1;
}

################
# end of test 6
################

$nfs->DeleteFile ($nfsiorfile);
$con->DeleteFile ($conidsfile);
$sup->DeleteFile ($supidsfile);
$con->DeleteFile ($channel_id);
$sup->DeleteFile ($channel_id);
$nfs->DeleteFile ($save_xml);
$nfs->DeleteFile ($save_000);
$nfs->DeleteFile ($save_001);
$nfs->DeleteFile ($eventpersist);
$sup->DeleteFile ($supplier_pause);

exit $status;
