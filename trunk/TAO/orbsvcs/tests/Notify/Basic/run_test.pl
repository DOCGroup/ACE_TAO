eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../../bin";
use PerlACE::Run_Test;

$experiment_timeout = 60;
$startup_timeout = 60;
$notifyior = PerlACE::LocalFile ("notify.ior");
$namingior = PerlACE::LocalFile ("naming.ior");
$status = 0;

@tests =
  (
   {
    name => "AdminProperties",
    args => "-ORBSvcConf adminproperties.conf",
   },
   {
    name => "ConnectDisconnect",
    args => "",
   },
   {
    name => "Events",
    args => "",
   },
   {
    name => "IdAssignment",
    args => "",
   },
   {
    name => "LifeCycle",
    args => "",
   },
   {
    name => "Simple",
    args => "",
   },
   {
    name => "MultiTypes",
    args => "",
   },
   {
    name => "Filter",
    args => "",
   },
   {
    name => "Updates",
    args => "",
   },
   {
    name => "Sequence",
    args => "",
   },
  );

@default_test_configs =
  (
   "notify.reactive.conf",
   "notify.mt.conf",
   );

if ($#ARGV == -1)
  {
    @test_configs = @default_test_configs;
  }
else
  {
    @test_configs = @ARGV;
  }

$Naming = new PerlACE::Process ("../../../Naming_Service/Naming_Service",
                                "-o $namingior");
unlink $namingior;

$Naming->Spawn ();

if (PerlACE::waitforfile_timed ($namingior, $startup_timeout) == -1) {
  print STDERR "ERROR: waiting for the naming service to start\n";
  $Naming->Kill ();
  exit 1;
}

for $config (@test_configs)
  {
    print STDERR "\nTesting Notification Service with config file = $config ....\n\n";

    $Notification = new PerlACE::Process ("../../../Notify_Service/Notify_Service",
                                          "-ORBInitRef NameService=file://$namingior " .
                                          "-IORoutput $notifyior " .
                                          "-ORBSvcConf $config");
    unlink $notifyior;
    $Notification->Spawn ();
    
    if (PerlACE::waitforfile_timed ($notifyior, $startup_timeout) == -1) {
      print STDERR "ERROR: waiting for the notify service to start\n";
      $Notification->Kill ();
      $Naming->Kill ();
      exit 1;
    }
    
    for $name (@tests)
      {
        ## The MaxQueueLength and MaxEventsPerConsumer are not supported in the Reactive
        ## configuration, so we skip this test for now.
        ## The Notification should actually throw an exception for the property not supported.
        if ($name->{name} eq "AdminProperties" 
            && $config eq "notify.reactive.conf")
          {
            next;
          }

        print STDERR "\nTesting $name->{name}....\n\n";
        $test = new PerlACE::Process ("./$name->{name}",
                                      "-ORBInitRef NameService=file://$namingior " .
                                      "$name->{args} ");
        $test->Spawn ();

        $status = $test->WaitKill ($experiment_timeout);

        if ($status != 0) 
          {
            print STDERR "ERROR: $name returned $status\n";
            break;
          }
      }
    
    $Notification->Kill ();
  }

$Naming->Kill ();

unlink $namingior;
unlink $notifyior;

exit $status;
