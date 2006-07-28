eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

my($eventType) = "-any"; # your choice of -any -structured or -sequence

my($ACE_ROOT) = $ENV{ACE_ROOT};
my($TAO_ROOT) = "$ACE_ROOT/TAO";

my($notify_port) = "9889";

#file used to detect notification service startup
my($notify_ior) = PerlACE::LocalFile("notify.ior");
#hard coded file name in Consumer.cpp (for now)
my($consumer_ids) = PerlACE::LocalFile("consumer.ids");
#hard coded file name in Supplier.cpp (for now)
my($supplier_ids) = PerlACE::LocalFile("supplier.ids");
#file used to communicate channel # from consumer to supplier
my($channel_id) = PerlACE::LocalFile("channel_id");
#file names comes from svc.conf (+.xml & .000)
my($save_xml) = PerlACE::LocalFile("./reconnect_test.xml");
my($save_000) = PerlACE::LocalFile("./reconnect_test.000");
my($eventpersist) = PerlACE::LocalFile("./event_persist.db");
# hardcode filename written by the Supplier when it pauses.
my($supplier_pause) = PerlACE::LocalFile("Supplier.paused");

my($verbose) = "";

my $svcconf = "ns_st_topo.conf";

# Process command line arguments
foreach $i (@ARGV) {
  if ($i eq "-any") {
    $eventType = "-any";
  }
  elsif ($i eq "-str" or $i eq "-structured") {
    $eventType = "-structured";
  }
  elsif ($i eq "-seq" or $i eq "-sequence") {
    $eventType = "-sequence";
  }
  elsif ($i eq "-mt") {
    $svcconf = "ns_mt_topo.conf";
  }
  elsif ($i eq "-v" or $i eq "-verbose") {
    $verbose = "-v";
  }
  else {
    print "TEST SCRIPT: unknown: $i\n";
    print "TEST SCRIPT: usage: [-any|-str|-seq] -mt -v\n";
    exit -4;
  }
}

my($client_args) = " $eventType $verbose -NoNameSvc -ORBInitRef NotifyEventChannelFactory=corbaloc::localhost:$notify_port/NotifyEventChannelFactory ";
my($ns_args) = " -ORBObjRefStyle url -NoNameSvc -Boot -ORBSvcConf $svcconf -IORoutput $notify_ior -ORBEndpoint iiop://:$notify_port ";
#my($ns_args_hidden) = " -NoNameSvc -Boot -ORBSvcConf $svcconf -IORoutput $notify_ior";
# nuke the topology save files so we start clean
unlink $save_xml;
unlink $save_000;
unlink $eventpersist;
unlink $notify_ior;


##define the processes first
my($NS) = new PerlACE::Process("$TAO_ROOT/orbsvcs/Notify_Service/Notify_Service");
my($CON) = new PerlACE::Process("./Consumer");
my($SUP) = new PerlACE::Process("./Supplier");

if ($verbose eq "-v") {print "TEST SCRIPT: Starting Notify Service on port $notify_port\n";}
$NS->Arguments($ns_args);
if ($verbose eq "-v") {print "TEST SCRIPT: " . $NS->CommandLine . "\n";}
print $NS->CommandLine ();
if ($NS->Spawn() == -1) {
   exit 1;
}

# the ior file is only used to wait for the service to start
if (PerlACE::waitforfile_timed ($notify_ior, 10) == -1) {
   print STDERR "ERROR: Timed out waiting for $notify_ior\n";
   $NS->Kill ();
   exit 1;
}
# be sure consumer doesn't try to reconnect the first time
unlink $consumer_ids;
# don't use previous channel id
unlink $channel_id;

# start the consumer,
# write the channel number to channel.id for use by Supplier
# expect to receive 20 events of type any
#
$CON->Arguments("-channel $channel_id -expect 20 $client_args");
if ($verbose eq "-v") {print "TEST SCRIPT: " . $CON->CommandLine . "\n";}
print $CON->CommandLine ();
$CON->Spawn();

# wait for the channel number file
if (PerlACE::waitforfile_timed ($channel_id, 10) == -1) {
   print STDERR "ERROR: Timed out waiting for Consumer to write $channel_id\n";
   $NS->Kill ();
   $CON->Kill ();
   exit 1;
}

# discard old reconnect information
unlink $supplier_ids;

# send 10 Any's, picking up the channel# from channel.id
$SUP->Arguments("-channel $channel_id -send 10 $client_args");
if ($verbose eq "-v") {print "TEST SCRIPT: " . $SUP->CommandLine . "\n";}
print $SUP->CommandLine ();
$SUP->SpawnWaitKill(60);

# forget the channel id, depend on the reconnect information
unlink $channel_id;

$SUP->Arguments("-send 10 -serial_number 10 $client_args");
if ($verbose eq "-v") {print "TEST SCRIPT: " . $SUP->CommandLine . "\n";}
print $SUP->CommandLine ();
$SUP->SpawnWaitKill(60);

$status = $CON->WaitKill (60);

if ($status) {
  print STDERR "ERROR: Consumer reported error\n";
  $NS->Kill();
  exit $status;
  }

print "TEST SCRIPT: ****Passed: Supplier reconnect test.\n";

################
#end of test 1
################

# Now start the consumer again.  Depending on reconnect this time
# Let it write the channel.id file so we know when it's up.

if ($verbose eq "-v") {print "TEST SCRIPT: " . $CON->CommandLine . "\n";}
$CON->Spawn();
# wait for the channel number file
if (PerlACE::waitforfile_timed ($channel_id, 10) == -1) {
   print STDERR "ERROR: Timed out waiting for Consumer to write $channel_id\n";
   $NS->Kill ();
   $CON->Kill ();
   exit 1;
}

# forget the channel id, depend on the supplier reconnect information
unlink $channel_id;

$SUP->Arguments("-send 20 -serial_number 0 $client_args");
if ($verbose eq "-v") {print "TEST SCRIPT: " . $SUP->CommandLine . "\n";}
$status= $SUP->SpawnWaitKill(60);
if ($status) {
  print STDERR "ERROR: Supplier reported error\n";
  $CON->Kill();
  $NS->Kill();
  exit $status;
  }

$status = $CON->WaitKill (60);
if ($status) {
  print STDERR "ERROR: Consumer reported error\n";
  $NS->Kill();
  exit $status;
  }

print "TEST SCRIPT: ****Passed: Consumer reconnect test.\n";

################
#end of test 2
################

if ($verbose eq "-v") {print "TEST SCRIPT: Stop the Notification Service\n";}
$NS->Kill();
unlink $notify_ior;

if ($verbose eq "-v") {print "TEST SCRIPT: Restarting Notify Service on port $notify_port\n";}
if ($verbose eq "-v") {print "TEST SCRIPT: It should load topology from $save_xml\n";}

# sleep to avoid socket-related problems
sleep (10 * $PerlACE::Process::WAIT_DELAY_FACTOR);
$NS->Arguments($ns_args);
if ($verbose eq "-v") {print "TEST SCRIPT: " . $NS->CommandLine . "\n";}
$NS->Spawn();

# the ior file is only used to wait for the service to start
if (PerlACE::waitforfile_timed ($notify_ior, 10) == -1) {
   print STDERR "ERROR: Timed out waiting for $notify_ior\n";
   $NS->Kill ();
   exit 1;
}

# Now start the consumer again.  It will use the reconnect info.
# Let it write the channel.id file so we know when it's up.
unlink $channel_id;

if ($verbose eq "-v") {print "TEST SCRIPT: " . $CON->CommandLine . "\n";}
$CON->Spawn();
# wait for the channel number file
if (PerlACE::waitforfile_timed ($channel_id, 10) == -1) {
   print STDERR "ERROR: Timed out waiting for Consumer to write $channel_id\n";
   $NS->Kill ();
   $CON->Kill ();
   exit 1;
}

# forget the channel id, depend on the supplier reconnect information
unlink $channel_id;

$SUP->Arguments("-send 20 -serial_number 0 $client_args");
if ($verbose eq "-v") {print "TEST SCRIPT: " . $SUP->CommandLine . "\n";}
$status = $SUP->SpawnWaitKill(60);
if ($status) {
  print STDERR "ERROR: Supplier reported error\n";
  $NS->Kill();
  $CON->Kill();
  exit $status;
  }

$status = $CON->WaitKill (60);
if ($status) {
  print STDERR "ERROR: Consumer reported error\n";
  $NS->Kill();
  exit $status;
  }

print "TEST SCRIPT: ****Passed: Topology Persistence Test.\n";

################
#end of test 3
################

# Now start the consumer one more time
# Let it write the channel.id file so we know when it's up.
unlink $channel_id;

if ($verbose eq "-v") {print "TEST SCRIPT: " . $CON->CommandLine . "\n";}
$CON->Spawn();
# wait for the channel number file
if (PerlACE::waitforfile_timed ($channel_id, 10) == -1) {
   print STDERR "ERROR: Timed out waiting for Consumer to write $channel_id\n";
   $NS->Kill ();
   $CON->Kill ();
   exit 1;
}

# forget the channel id, depend on the supplier reconnect information
unlink $channel_id;

# Start the Supplier, tell it to send 10 messages, pause until it is reconnected
# then send another 10.

unlink ($supplier_pause);

$SUP->Arguments("-send 20 -pause 10 -serial_number 0 $client_args");
if ($verbose eq "-v") {print "TEST SCRIPT: " . $SUP->CommandLine . "\n";}
$SUP->Spawn();

# wait for the supplier's "paused" file
if (PerlACE::waitforfile_timed ($supplier_pause, 30) == -1) {
   print STDERR "ERROR: Timed out waiting for Supplier to write $supplier_pause\n";
   $NS->Kill ();
   $SUP->Kill ();
   $CON->Kill ();
   exit 1;
}
unlink ($supplier_pause);

if ($verbose eq "-v") {print "TEST SCRIPT: Stop the Notification Service\n";}
$NS->Kill();
unlink $notify_ior;

if ($verbose eq "-v") {print "TEST SCRIPT: Restarting Notify Service\n";}
if ($verbose eq "-v") {print "TEST SCRIPT: Use unspecified port to make it \"invisible\" to clients\n";}
if ($verbose eq "-v") {print "TEST SCRIPT: It should load topology from $save_xml\n";}
if ($verbose eq "-v") {print "TEST SCRIPT: and reconnect to registered clients.\n";}

#$NS->Arguments($ns_args_hidden);
# sleep to avoid socket-related problems
sleep (10 * $PerlACE::Process::WAIT_DELAY_FACTOR);

if ($verbose eq "-v") {print "TEST SCRIPT: " . $NS->CommandLine . "\n";}
$NS->Spawn();

# at this point, both the consumer and the supplier should reconnect
# and the remaining events should be delivered
# eventually the consumer will finish

$status = $CON->WaitKill (20);
if ($status) {
  print STDERR "ERROR: Consumer reported error\n";
  $SUP->Kill();
  $NS->Kill();
  exit $status;
  }

$status = $SUP->Kill ();
if ($status) {
  print STDERR "ERROR: Supplier reported error\n";
  $NS->Kill();
  exit $status;
  }

print "TEST SCRIPT: ****Passed: Reconnection Factory test.\n";

################
#end of test 4
################

if ($verbose eq "-v") {print "TEST SCRIPT: Stop the Notification Service\n";}
$NS->Kill();

unlink $notify_ior;
unlink $save_xml;
unlink $save_000;
unlink $eventpersist;
unlink $consumer_ids;
unlink $channel_id;
unlink $supplier_ids;

sleep (10 * $PerlACE::Process::WAIT_DELAY_FACTOR); ## avoid lingering sockets
# go back to normal ns args
$NS->Arguments($ns_args);
if ($verbose eq "-v") {print "TEST SCRIPT: " . $NS->CommandLine . "\n";}
$NS->Spawn();

# the ior file is only used to wait for the service to start
if (PerlACE::waitforfile_timed ($notify_ior, 20) == -1) {
   print STDERR "ERROR: Timed out waiting for $notify_ior\n";
   $NS->Kill ();
   exit 1;
}

#configure the consumer to expect 20 events, fail (throw an exception
# after 8 events) then continue to listen for remaining events.
#The Notification service should automatically retry (but may discard)
# the failed events.
$CON->Arguments("-channel $channel_id -expect 20 -fail 8 $client_args");
if ($verbose eq "-v") {print "TEST SCRIPT: " . $CON->CommandLine . "\n";}
$CON->Spawn();

# wait for the channel number file
if (PerlACE::waitforfile_timed ($channel_id, 10) == -1) {
   print STDERR "ERROR: Timed out waiting for Consumer to write $channel_id\n";
   $NS->Kill ();
   $CON->Kill ();
   exit 1;
}

$SUP->Arguments("-channel $channel_id -send 20 $client_args");
if ($verbose eq "-v") {print "TEST SCRIPT: " . $SUP->CommandLine . "\n";}
$status = $SUP->SpawnWaitKill(20);
if ($status) {
  print STDERR "ERROR: Supplier reported error\n";
  $NS->Kill();
   $CON->Kill ();
  exit $status;
  }

## this is a slow test due to the reconnection
## time, so give it enough time...
$status = $CON->WaitKill (240);

if ($status) {
  print STDERR "ERROR: Consumer reported error\n";
  $NS->Kill();
  exit $status;
  }

print "TEST SCRIPT: ****Passed: Consumer recoverable exception test.\n";

################
#end of test 5
################

if ($verbose eq "-v") {print "TEST SCRIPT: Stop the Notification Service\n";}
$NS->Kill();
unlink $notify_ior;
unlink $save_xml;
unlink $save_000;
unlink $eventpersist;

exit $status;
