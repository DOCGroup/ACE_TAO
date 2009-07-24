eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$iorfile = PerlACE::LocalFile ("test.ior");
     
# Current supported forward once exceptions:
#      EF_OBJECT_NOT_EXIST = 0x1,  
#      EF_COMM_FAILURE     = 0x2,   
#      EF_TRANSIENT        = 0x4,   
#      EF_INV_OBJREF       = 0x8,   

@configurations = 
    (
     {   description => "case 1: not forward upon OBJECT_NOT_EXIST exception", 
         orb_exception_forward_opt => "-ORBForwardOnceOnObjectNotExist 0",
         num_requests => "1",
         client_expect_num_ex => "1",
         client_expect_ex_kind => "1", 
         server_raise_ex_kind => "1", 
         servant_calls => "1",
     },
     {   description => "case 2: default Fault Tolerant always forward upon COMM_FAILURE exception", 
         orb_exception_forward_opt => "-ORBForwardOnceOnCommFailure 0",
         num_requests => "1",
         client_expect_num_ex => "0",
         client_expect_ex_kind => "0", 
         server_raise_ex_kind => "2", 
         servant_calls => "0",  # use 0 to indicate request looping case
     },
     {   description => "case 3: default Fault Tolerant always forward upon TRANSIENT exception", 
         orb_exception_forward_opt => "-ORBForwardOnceOnTransient 0",
         num_requests => "1",
         client_expect_num_ex => "0",
         client_expect_ex_kind => "0", 
         server_raise_ex_kind => "4", 
         servant_calls => "0",  # use 0 to indicate request looping case
     },
     {   description => "case 4: not forward upon INV_OBJREF exception", 
         orb_exception_forward_opt => "-ORBForwardOnceOnInvObjref 0",
         num_requests => "1",
         client_expect_num_ex => "1", 
         client_expect_ex_kind => "8", 
         server_raise_ex_kind => "8", 
         servant_calls => "1",
     },
     {   description => "case 5: forward once upon OBJECT_NOT_EXIST exception", 
         orb_exception_forward_opt => "-ORBForwardOnceOnObjectNotExist 1",
         num_requests => "4",
         client_expect_num_ex => "4", 
         client_expect_ex_kind => "1", 
         server_raise_ex_kind => "1", 
         servant_calls => "5", # 4 requests + 1 forward 
     },
     {   description => "case 6: forward once upon COMM_FAILURE exception", 
         orb_exception_forward_opt => "-ORBForwardOnceOnCommFailure 1",
         num_requests => "4", 
         client_expect_num_ex => "4", 
         client_expect_ex_kind => "2", 
         server_raise_ex_kind => "2", 
         servant_calls => "5", # 4 requests + 1 forward 
     },
     {   description => "case 7: forward once upon TRANSIENT exception", 
         orb_exception_forward_opt => "-ORBForwardOnceOnTransient 1",
         num_requests => "4",
         client_expect_num_ex => "4", 
         client_expect_ex_kind => "4", 
         server_raise_ex_kind => "4", 
         servant_calls => "5", # 4 requests + 1 forward 
     },
     {   description => "case 8: forward once upon INV_OBJREF exception", 
         orb_exception_forward_opt => "-ORBForwardOnceOnInvObjref 1",
         num_requests => "4",
         client_expect_num_ex => "4", 
         client_expect_ex_kind => "8", 
         server_raise_ex_kind => "8", 
         servant_calls => "5", # 4 requests + 1 forward 
     },
    );
         
unlink $iorfile;

$port = PerlACE::uniqueid () + 10001;  # This can't be 10000 for Chorus 4.0
$status = 0;

if (PerlACE::is_vxworks_test()) {
  $TARGETHOSTNAME = $ENV{'ACE_RUN_VX_TGTHOST'};
  $SV  = new PerlACE::ProcessVX ("server", "-ORBEndpoint iiop://$TARGETHOSTNAME:$port -o $iorfile");
}
else {
  $TARGETHOSTNAME = "localhost";
  $SV  = new PerlACE::Process ("server", "-ORBEndpoint iiop://$TARGETHOSTNAME:$port -o $iorfile");
}


for $test (@configurations)
{
  print STDERR "\n==== Running $test->{description} \n\n";

  unlink $iorfile;

  if (PerlACE::is_vxworks_test()) {
    $TARGETHOSTNAME = $ENV{'ACE_RUN_VX_TGTHOST'};
    $SV  = new PerlACE::ProcessVX ("server", "-ORBEndpoint iiop://$TARGETHOSTNAME:$port -o $iorfile "
                   . "-e $test->{server_raise_ex_kind} -r $test->{servant_calls}");
  }
  else {
    $TARGETHOSTNAME = "localhost";
    $SV  = new PerlACE::Process ("server", "-ORBEndpoint iiop://$TARGETHOSTNAME:$port -o $iorfile "
         . "-e $test->{server_raise_ex_kind}  -r $test->{servant_calls}");
  }


  $CL = new PerlACE::Process ("client", 
                              "-k corbaloc::$TARGETHOSTNAME:$port/Simple_Server -x -e $test->{client_expect_ex_kind} " 
                              . "-n $test->{client_expect_num_ex} -i $test->{num_requests} "
                              . "$test->{orb_exception_forward_opt}");
                               
  print STDERR $SV->CommandLine () . "\n";
  print STDERR $CL->CommandLine () . "\n";
  $SV->Spawn ();

  if (PerlACE::waitforfile_timed ($iorfile,
                          $PerlACE::wait_interval_for_process_creation) == -1) {
      print STDERR "ERROR: cannot find file <$iorfile>\n";
      $SV->Kill ();
      exit 1;
  }

  $client = $CL->SpawnWaitKill (60);

  if ($client != 0) {
      print STDERR "ERROR: client returned $client\n";
      $status = 1;
  }

  $server = $SV->WaitKill (15);

  if ($server != 0) {
      print STDERR "ERROR: server returned $server\n";
      $status = 1;
  }
}

unlink $iorfile;

exit $status;
