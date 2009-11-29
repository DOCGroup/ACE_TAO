eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-
# $Id$

use lib  "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;
$file = PerlACE::LocalFile ("server.ior");

unlink $file;

$status = 0;
#S11 uses schedule object, S10 does not
$S11 = new PerlACE::Process ("server",
           "-N 2 -F schedule.cfg -A Server2 -X 1");
$S10 = new PerlACE::Process ("server",
           "-N 2 -F schedule.cfg -A Server2 -X 0");

#S2 uses and invalid config file
$S2 = new PerlACE::Process ("server",
          "-N 2 -F INVALID_FILE.cfg -A Server2 -X 1");

#s3 tries to schedule an invalid object
$S3 = new PerlACE::Process ("server",
          "-N 2 -F schedule.cfg -A Server5 -X 1");

$C1_client_output = PerlACE::LocalFile ("client1_output");
$C2_client_output = PerlACE::LocalFile ("client2_output");
$C3_client_output = PerlACE::LocalFile ("client3_output");

unlink $C1_client_output;
unlink $C2_client_output;
unlink $C3_client_output;



# C11 C21 C31 use RTCosScheduler 1.0 on the client
$C11 = new PerlACE::Process ("client", "-B 0 -R 10 -A 3 -N 1 -C ".  $C1_client_output.  " -F schedule.cfg -T Client1 -X 1");
$C21 = new PerlACE::Process ("client", "-B 0 -R 3 -A 3 -N 1 -C ".  $C2_client_output.  " -F schedule.cfg -T Client2 -X 1");
$C31 = new PerlACE::Process ("client", "-B 2 -R 3 -A 3 -N 1 -C ".  $C3_client_output.  " -F schedule.cfg -T Client3 -X 1");

#C10 C20 C30 Do not use RTCosScheduler 1.0 on the client
$C10 = new PerlACE::Process ("client",
           "-B 0 -R 10 -A 3 -N 1 -C ".
           $C1_client_output.
           " -F schedule.cfg -T Client1 -X 0");
$C20 = new PerlACE::Process ("client",
           "-B 0 -R 3 -A 3 -N 1 -C ".
           $C3_client_output.
           " -F schedule.cfg -T Client2 -X 0");
$C30 = new PerlACE::Process ("client",
           "-B 2 -R 3 -A 3 -N 1 -C ".
           $C3_client_output.
           " -F schedule.cfg -T Client3 -X 0");

#C4 is an invalid activity name
$C4 = new PerlACE::Process ("client",
           "-B 1 -R 3 -A 0 -N 1 -F schedule.cfg ".
           "-T Client4  -X 1");

#C5 is an invalid config file
$C5 = new PerlACE::Process ("client",
           "-B 1 -R 3 -A 0 -N 1 -F INVALID_FILE.cfg ".
           "-T Client2 -X 1");

sub spawn_server
{
  local ( $param_1 ) = @_ ;
  $param_1->Spawn ();
  if (PerlACE::waitforfile_timed ($file, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$file>\n";
    $param_1->Kill ();
    exit 1;
  }
}

sub kill_server
{
  local ($param_1) = @_;
  $server = $param_1->TerminateWaitKill (15);
  if ($server != 0) {
   print STDERR "ERROR: server returned $server\n";
   $status = 1;
  }
  unlink $file;
}

sub run_client
{
  local ($param_1) = @_;
  $client = $param_1->Spawn();
  if ($client != 0) {
     print STDERR "ERROR: client returned $client\n";
     $status = 1;
  }
}


sub test
{
  local($param_0, $param_1, $param_2, $param_3) = @_;
  print STDERR $param_0;

  $client1 = $param_1->Spawn();
  $client2 = $param_2->Spawn();
  $client3 = $param_3->Spawn();

  if ($client1 != 0) {
    print STDERR "ERROR: client returned $client1\n";
    $status = 1;
  }

  if ($client2 != 0) {
    print STDERR "ERROR: client returned $client2\n";
    $status = 1;
  }

  if ($client3 != 0) {
    print STDERR "ERROR: client returned $client3\n";
    $status = 1;
  }


  $param_1->WaitKill (35);
  $param_2->WaitKill (30);
  $param_3->WaitKill (30);
  sleep 2;

  if ($param_0 =~ /Test1/)
    {
      evaluate_output();
    }
  unlink $C1_client_output;
  unlink $C2_client_output;
  unlink $C3_client_output;


}


sub test5
{
  print STDERR "\n==== Testing exceptions\n";
  print STDERR "\n==== Test5 - Testing ClientScheduler ".
               "exception for invalid activity name\n".
               "Should recieve an RTCosScheduling::UnknownName exeption\n";
  run_client($C4);
  $C4->WaitKill(10);
}

sub test6
{
  print STDERR "\n==== Test6 - Testing client exception ".
               "when invalid config file specified\n".
               "Program should abort because no valid file was given\n";
  run_client($C5);
  $C5->WaitKill (10);
}

sub test7
{
  print STDERR "\n==== Test7 - Testing server exception ".
               "when invalid Object Name specified\n";
  print STDERR "==== (Object name not in config file)\n".
               "Should receive an RTCosScheduling::UnknownName exception\n";
  $S3->Spawn();
  sleep 1;
  kill_server($S3);
}


sub test8
{
  print STDERR "\n==== Test8 - Testing server exception ".
               "when invalid config file specified\n".
               "Server Should abort because an invalid config ".
               "filename was given\n";
  $S2->Spawn();
  sleep 1;
  kill_server($S2);
}

sub evaluate_output
{
  # this looks at the three output files to see if the
  # clients ran in the correct order
  open(INFOC1, $C1_client_output);
  open(INFOC2, $C2_client_output);
  open(INFOC3, $C3_client_output);


  @arrayC1=<INFOC1>;
  close (INFOC1);
  @arrayC2=<INFOC2>;
  close (INFOC2);
  @arrayC3=<INFOC3>;
  close (INFOC3);


  $is_valid = 1;
  $counter = 0;

  # check the client execution to make sure
  #it runs in the correct order
  ($BAtimeC1,$client,$activity)=split(/\t/,@arrayC1[0]);
  ($BAtimeC2,$client,$activity)=split(/\t/,@arrayC2[0]);
  ($BAtimeC3,$client,$activity)=split(/\t/,@arrayC3[0]);

  ($CMtimeC1,$client,$activity)=split(/\t/,@arrayC1[1]);
  ($CMtimeC2,$client,$activity)=split(/\t/,@arrayC2[1]);
  ($CMtimeC3,$client,$activity)=split(/\t/,@arrayC3[1]);

  ($BRtimeC1,$client,$activity)=split(/\t/,@arrayC1[2]);
  ($BRtimeC2,$client,$activity)=split(/\t/,@arrayC2[2]);
  ($BRtimeC3,$client,$activity)=split(/\t/,@arrayC3[2]);

  ($FRtimeC1,$client,$activity)=split(/\t/,@arrayC1[3]);
  ($FRtimeC2,$client,$activity)=split(/\t/,@arrayC2[3]);
  ($FRtimeC3,$client,$activity)=split(/\t/,@arrayC3[3]);

  ($DMtimeC1,$client,$activity)=split(/\t/,@arrayC1[4]);
  ($DMtimeC2,$client,$activity)=split(/\t/,@arrayC2[4]);
  ($DMtimeC3,$client,$activity)=split(/\t/,@arrayC3[4]);

  ($DTtimeC1,$client,$activity)=split(/\t/,@arrayC1[5]);
  ($DTtimeC2,$client,$activity)=split(/\t/,@arrayC2[5]);
  ($DTtimeC3,$client,$activity)=split(/\t/,@arrayC3[5]);


  if ($BAtimeC1 gt $BAtimeC2 || $BAtimeC1 gt $BAtimeC3)
    {
      print STDERR "Scheduler Test Failed - activities did not ".
                   "begin in correct order\n";
      $is_valid = 0;
    }
  if ($CMtimeC1 gt $CMtimeC2 || $CMtimeC1 gt $CMtimeC3)
    {
      print STDERR "Scheduler Test Failed - remote method calls not ".
                   "made in correct order\n";
      $is_valid = 0;
    }
  if ($FRtimeC1 gt $FRtimeC3 || $FRtimeC3 gt $FRtimeC2)
    {
      print STDERR "Scheduler Test Failed - Remote Method calls did ".
                   "not finish in correct order\n";
      $is_valid = 0;
    }
  if ($DTtimeC3 gt $DTtimeC2  || $DTtimeC2 gt $DTtimeC1)
    {
      print STDERR "Scheduler Test Failed - Tests did not finish in ".
                   "correct order\n";
      $is_valid = 0;
    }
  if ($BRtimeC1 gt $BAtimeC2)
    {
      print STDERR "Scheduler Test Failed - Client1 remote method call ".
                   "finished before Client2 began local activity\n";
      $is_valid = 0;
    }
  if ($FRtimeC1 gt $BRtimeC3 || $FRtimeC1 gt $BRtimeC2)
    {
      print STDERR "Scheduler Test Failed - Client1 finished before ".
                   "Clients2 or 3 made method calls\n";
      $is_valid = 0;
    }
  if ($BRtimeC3 gt $BRtimeC2 || $FRtimeC3 gt $FRtimeC2)
    {
      print STDERR "Scheduler Test Failed - Client 3 remote method call ".
                   "did not begin before Client2\n";
      $is_valid = 0;
    }

  if ($is_valid == 1)
    {
      print STDERR "The scheduling service worked as expected\n";
    }
  else
    {
      print STDERR "The scheduling service did not work as expected\n";
    }

}


print STDERR "\n==== Running RTCORBA 1.0 Scheduling Service test\n";
print STDERR "==== Note that the first column is the time, ".
             "it will be different for you\n\n";
print STDERR "TIME\t\t\tOBJECT\tLOCATION\tACTIVITY\n";

spawn_server($S11);
test("\n==== Test1 - YES client side scheduling, ".
      "YES server side scheduling\n", $C11, $C21, $C31);
test("\n==== Test2 - NO client side scheduling, ".
     "YES server side scheduling\n", $C10, $C20, $C30);
kill_server($S11);

spawn_server($S10);
test("\n==== Test3 - YES client side scheduling, ".
     "NO server side scheduling\n", $C11, $C21, $C31);
test("\n==== Test4 - NO client side scheduling, ".
     "NO server side scheduling\n",  $C10, $C20, $C30);
kill_server($S10);

spawn_server($S11);
test5();
test6();
kill_server($S11);


test7();
test8();


exit $status;
