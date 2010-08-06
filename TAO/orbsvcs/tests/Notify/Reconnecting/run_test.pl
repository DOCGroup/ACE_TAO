eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# ******************************************************************
# Pragma Section
# ******************************************************************

use strict;

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

PerlACE::add_lib_path ('../lib');

# ******************************************************************
# Data Section
# ******************************************************************

my($eventType) = '-any'; # your choice of -any -structured or -sequence

my($status) = 0;
my($notify_port) = PerlACE::random_port();
my($st_topo) = 'ns_st_topo.conf';
my($st_both) = 'ns_st_both.conf';
my(%files) = (# File used to detect notification service startup
              'notify_ior' => PerlACE::LocalFile('notify.ior'),
              # Hard coded file name in Consumer.cpp (for now)
              'consumer_ids' => PerlACE::LocalFile('consumer.ids'),
              # Hard coded file name in Supplier.cpp (for now)
              'supplier_ids' => PerlACE::LocalFile('supplier.ids'),
              # File used to communicate channel # from consumer to supplier
              'channel_id' => PerlACE::LocalFile('channel_id'),
              # File used to save configuration for reconnect test
              'save_xml' => PerlACE::LocalFile('reconnect_test.xml'),
              # File names comes from svc.conf (+.xml & .000)
              'save_000' => PerlACE::LocalFile('reconnect_test.000'),
              # File names comes from svc.conf (+.xml & .001)
              'save_001' => PerlACE::LocalFile('reconnect_test.001'),
              # Event persistent database file (from svc.conf)
              'eventpersist' => PerlACE::LocalFile('event_persist.db'),
              # Hardcode filename written by the Supplier when it pauses.
              'supplier_pause' => PerlACE::LocalFile('Supplier.paused'),
             );

# ******************************************************************
# Main Section
# ******************************************************************

my($verbose) = '';
my($svcconf) = $st_topo;
my($discon)  = '';

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

my($client_args) = "$eventType $verbose -NoNameSvc -ORBInitRef " .
                   "NotifyEventChannelFactory=corbaloc::" .
                   "localhost:$notify_port/NotifyEventChannelFactory ";
my($ns_base_args) = "-ORBObjRefStyle url -NoNameSvc -Boot " .
                    "-IORoutput $files{'notify_ior'} " .
                    "-ORBEndpoint iiop://:$notify_port ";
my($ns_args) = "-ORBSvcConf $svcconf $ns_base_args";

# Delete the all of the files used in the test
unlink(values(%files));

# Define the processes first
my($NS)  = new PerlACE::Process("$PerlACE::TAO_ROOT/orbsvcs/Notify_Service/Notify_Service");
my($CON) = new PerlACE::Process('Consumer');
my($SUP) = new PerlACE::Process('Supplier');

################
# begin test 1
################

if ($verbose eq '-v') {
  print "TEST SCRIPT: Starting Notify Service on port $notify_port\n";
}
$NS->Arguments($ns_args);
if ($verbose eq '-v') {print "TEST SCRIPT: " . $NS->CommandLine() . "\n";}
if ($NS->Spawn() == -1) {
  exit(1);
}

# the ior file is only used to wait for the service to start
if (PerlACE::waitforfile_timed(
                      $files{'notify_ior'},
                      $PerlACE::wait_interval_for_process_creation) == -1) {
  print STDERR "ERROR: Timed out waiting for $files{'notify_ior'}\n";
  $NS->Kill();
  exit(1);
}

# start the consumer,
# write the channel number to channel.id for use by Supplier
# expect to receive 20 events of type any
#
$CON->Arguments("-channel $files{'channel_id'} " .
                "-expect 20 $discon $client_args");
print "TEST SCRIPT: ", $CON->CommandLine(), "\n" if ($verbose eq '-v');
$CON->Spawn();

# wait for the channel number file
if (PerlACE::waitforfile_timed(
                 $files{'channel_id'},
                 $PerlACE::wait_interval_for_process_creation) == -1) {
  print STDERR "ERROR: Timed out waiting for Consumer ",
               "to write $files{'channel_id'}\n";
  $NS->Kill();
  $CON->Kill();
  exit(1);
}

# send 10 Any's, picking up the channel# from channel.id
$SUP->Arguments("-channel $files{'channel_id'} -send 10 $client_args");
print "TEST SCRIPT: ", $SUP->CommandLine(), "\n" if ($verbose eq '-v');
$SUP->SpawnWaitKill(60);

# forget the channel id, depend on the reconnect information
unlink($files{'channel_id'});

$SUP->Arguments("-send 10 -serial_number 10 $discon $client_args");
if ($verbose eq '-v') {print "TEST SCRIPT: " . $SUP->CommandLine() . "\n";}
$SUP->SpawnWaitKill(60);

$status = $CON->WaitKill(60);

if ($status) {
  print STDERR "ERROR: Consumer reported error\n";
  $NS->Kill();
  exit($status);
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
$CON->Spawn();
# wait for the channel number file
if (PerlACE::waitforfile_timed(
                 $files{'channel_id'},
                 $PerlACE::wait_interval_for_process_creation) == -1) {
  print STDERR "ERROR: Timed out waiting for Consumer ",
               "to write $files{'channel_id'}\n";
  $NS->Kill();
  $CON->Kill();
  exit(1);
}

# forget the channel id, depend on the supplier reconnect information
unlink $files{'channel_id'};

$SUP->Arguments("-send 20 -serial_number 0 $client_args");
if ($verbose eq '-v') {print "TEST SCRIPT: " . $SUP->CommandLine() . "\n";}
$status = $SUP->SpawnWaitKill(60);
if ($status) {
  print STDERR "ERROR: Supplier reported error\n";
  $CON->Kill();
  $NS->Kill();
  exit($status);
}

$status = $CON->WaitKill(60);
if ($status) {
  print STDERR "ERROR: Consumer reported error\n";
  $NS->Kill();
  exit($status);
}

print "TEST SCRIPT: ****Passed: Consumer reconnect test.\n";

################
# end of test 2
################

################
# begin test 3
################

if ($verbose eq '-v') {print "TEST SCRIPT: Stop the Notification Service\n";}
$NS->Kill();
unlink $files{'notify_ior'};

if ($verbose eq '-v') {
  print "TEST SCRIPT: Restarting Notify Service on port $notify_port\n",
        "TEST SCRIPT: It should load topology from $files{'save_xml'}\n";
}

# sleep to avoid socket-related problems
sleep(10 * $PerlACE::Process::WAIT_DELAY_FACTOR);
$NS->Arguments($ns_args);
if ($verbose eq '-v') {print "TEST SCRIPT: " . $NS->CommandLine() . "\n";}
$NS->Spawn();

# the ior file is only used to wait for the service to start
if (PerlACE::waitforfile_timed(
                 $files{'notify_ior'},
                 $PerlACE::wait_interval_for_process_creation) == -1) {
  print STDERR "ERROR: Timed out waiting for $files{'notify_ior'}\n";
  $NS->Kill();
  exit(1);
}

# Now start the consumer again.  It will use the reconnect info.
# Let it write the channel.id file so we know when it's up.
unlink $files{'channel_id'};

if ($verbose eq '-v') {print "TEST SCRIPT: " . $CON->CommandLine() . "\n";}
$CON->Spawn();
# wait for the channel number file
if (PerlACE::waitforfile_timed(
                 $files{'channel_id'},
                 $PerlACE::wait_interval_for_process_creation) == -1) {
  print STDERR "ERROR: Timed out waiting for Consumer ",
               "to write $files{'channel_id'}\n";
  $NS->Kill();
  $CON->Kill();
  exit(1);
}

# forget the channel id, depend on the supplier reconnect information
unlink $files{'channel_id'};

$SUP->Arguments("-send 20 -serial_number 0 $client_args");
if ($verbose eq '-v') {print "TEST SCRIPT: " . $SUP->CommandLine() . "\n";}
$status = $SUP->SpawnWaitKill(60);
if ($status) {
  print STDERR "ERROR: Supplier reported error\n";
  $NS->Kill();
  $CON->Kill();
  exit($status);
}

$status = $CON->WaitKill(60);
if ($status) {
  print STDERR "ERROR: Consumer reported error\n";
  $NS->Kill();
  exit($status);
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
unlink $files{'channel_id'};

if ($verbose eq '-v') {print "TEST SCRIPT: " . $CON->CommandLine() . "\n";}
$CON->Spawn();
# wait for the channel number file
if (PerlACE::waitforfile_timed(
                 $files{'channel_id'},
                 $PerlACE::wait_interval_for_process_creation) == -1) {
  print STDERR "ERROR: Timed out waiting for Consumer ",
               "to write $files{'channel_id'}\n";
  $NS->Kill();
  $CON->Kill();
  exit(1);
}

# forget the channel id, depend on the supplier reconnect information
unlink $files{'channel_id'};

# Start the Supplier, tell it to send 10 messages,
# pause until it is reconnected then send another 10.
$SUP->Arguments("-send 20 -pause 10 -serial_number 0 $client_args");
if ($verbose eq '-v') {print "TEST SCRIPT: " . $SUP->CommandLine() . "\n";}
$SUP->Spawn();

# wait for the supplier's "paused" file
if (PerlACE::waitforfile_timed(
                 $files{'supplier_pause'},
                 $PerlACE::wait_interval_for_process_creation) == -1) {
  print STDERR "ERROR: Timed out waiting for Supplier ",
               "to write $files{'supplier_pause'}\n";
  $NS->Kill();
  $SUP->Kill();
  $CON->Kill();
  exit(1);
}

if ($verbose eq '-v') {print "TEST SCRIPT: Stop the Notification Service\n";}
$NS->Kill();
unlink($files{'notify_ior'});

if ($verbose eq '-v') {
  print "TEST SCRIPT: Restarting Notify Service\n",
        "TEST SCRIPT: It should load topology from $files{'save_xml'}\n",
        "TEST SCRIPT: and reconnect to registered clients.\n";
}

# sleep to avoid socket-related problems
sleep(10 * $PerlACE::Process::WAIT_DELAY_FACTOR);

if ($verbose eq '-v') {print "TEST SCRIPT: " . $NS->CommandLine() . "\n";}
$NS->Spawn();

# at this point, both the consumer and the supplier should reconnect
# and the remaining events should be delivered
# eventually the consumer will finish

$status = $CON->WaitKill(20);
if ($status) {
  print STDERR "ERROR: Consumer reported error\n";
  $SUP->Kill();
  $NS->Kill();
  exit($status);
}

$status = $SUP->Kill();
if ($status) {
  print STDERR "ERROR: Supplier reported error\n";
  $NS->Kill();
  exit($status);
}

print "TEST SCRIPT: ****Passed: Reconnection Factory test.\n";

################
# end of test 4
################

################
# begin test 5
################

if ($verbose eq '-v') {print "TEST SCRIPT: Stop the Notification Service\n";}
$NS->Kill();

unlink(values(%files));

sleep(10 * $PerlACE::Process::WAIT_DELAY_FACTOR); ## avoid lingering sockets
# go back to normal ns args
$NS->Arguments($ns_args);
if ($verbose eq '-v') {print "TEST SCRIPT: " . $NS->CommandLine() . "\n";}
$NS->Spawn();

# the ior file is only used to wait for the service to start
if (PerlACE::waitforfile_timed(
                 $files{'notify_ior'},
                 $PerlACE::wait_interval_for_process_creation) == -1) {
  print STDERR "ERROR: Timed out waiting for $files{'notify_ior'}\n";
  $NS->Kill();
  exit(1);
}

#configure the consumer to expect 20 events, fail (throw an exception
# after 8 events) then continue to listen for remaining events.
#The Notification service should automatically retry (but may discard)
# the failed events.
$CON->Arguments("-channel $files{'channel_id'} " .
                "-expect 20 -fail 8 $client_args");
if ($verbose eq '-v') {print "TEST SCRIPT: " . $CON->CommandLine() . "\n";}
$CON->Spawn();

# wait for the channel number file
if (PerlACE::waitforfile_timed(
                 $files{'channel_id'},
                 $PerlACE::wait_interval_for_process_creation) == -1) {
  print STDERR "ERROR: Timed out waiting for Consumer ",
               "to write $files{'channel_id'}\n";
  $NS->Kill();
  $CON->Kill();
  exit(1);
}

$SUP->Arguments("-channel $files{'channel_id'} -send 20 $client_args");
if ($verbose eq '-v') {print "TEST SCRIPT: " . $SUP->CommandLine() . "\n";}
$status = $SUP->SpawnWaitKill(20);
if ($status) {
  print STDERR "ERROR: Supplier reported error\n";
  $NS->Kill();
  $CON->Kill();
  exit($status);
}

## this is a slow test due to the reconnection
## time, so give it enough time...
$status = $CON->WaitKill(240);

if ($status) {
  print STDERR "ERROR: Consumer reported error\n";
  $NS->Kill();
  exit($status);
}

print "TEST SCRIPT: ****Passed: Consumer recoverable exception test.\n";

################
# end of test 5
################

################
# begin test 6
################

print "TEST SCRIPT: Stop the Notification Service\n" if ($verbose eq '-v');
$NS->Kill();
unlink(values(%files));

## Wait a bit to avoid lingering sockets from the previous Notify_Service
sleep(3);

$NS->Arguments("-ORBSvcConf $st_both $ns_base_args");
print "TEST SCRIPT: ", $NS->CommandLine(), "\n" if ($verbose eq '-v');
if ($NS->Spawn() == -1) {
  exit(1);
}

# the ior file is only used to wait for the service to start
if (PerlACE::waitforfile_timed(
                 $files{'notify_ior'},
                 $PerlACE::wait_interval_for_process_creation) == -1) {
  print STDERR "ERROR: Timed out waiting for $files{'notify_ior'}\n";
  $NS->Kill();
  exit(1);
}

my($expected) = 10;
$CON->Arguments("-channel $files{'channel_id'} " .
                "-expect $expected $client_args");
print "TEST SCRIPT: ", $CON->CommandLine(), "\n" if ($verbose eq '-v');
$CON->Spawn();

# wait for the channel number file
if (PerlACE::waitforfile_timed(
                 $files{'channel_id'},
                 $PerlACE::wait_interval_for_process_creation) == -1) {
  print STDERR "ERROR: Timed out waiting for Consumer ",
               "to write $files{'channel_id'}\n";
  $NS->Kill();
  $CON->Kill();
  exit(1);
}

$SUP->Arguments("-channel $files{'channel_id'} -send $expected $client_args");
print "TEST SCRIPT: ", $SUP->CommandLine(), "\n" if ($verbose eq '-v');
$SUP->SpawnWaitKill(60);

my($con) = $CON->WaitKill(30);
if ($con != 0) {
  $NS->Kill();
  exit(1);
}

my($next_expected) = 3;
$SUP->Arguments("-channel $files{'channel_id'} -send 3 " .
                "-serial_number $expected $client_args");
print "TEST SCRIPT: ", $SUP->CommandLine(), "\n" if ($verbose eq '-v');
$SUP->SpawnWaitKill(60);

$CON->Arguments("-channel $files{'channel_id'} -expect $next_expected " .
                "-serial_number $expected $client_args");
print "TEST SCRIPT: ", $CON->CommandLine(), "\n" if ($verbose eq '-v');
$status = $CON->SpawnWaitKill(30);

$NS->Kill();

################
# end of test 6
################

unlink(values(%files));
exit($status);
