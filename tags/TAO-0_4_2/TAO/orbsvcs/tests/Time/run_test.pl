eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../bin";

require ACEutils;

$server_ior = "server_ior";
$clerk_ior = "clerk_ior";
$implrepo_ior = "implrepo.ior";
$status = 0;

# Make sure the files are gone, so we can wait on them.

unlink $server_ior;
unlink $clerk_ior;
unlink $implrepo_ior;

$implrepo_program = "..$DIR_SEPARATOR..$DIR_SEPARATOR"."ImplRepo_Service".$DIR_SEPARATOR."ImplRepo_Service".$EXE_EXT;
$tao_ir_program = "..$DIR_SEPARATOR..$DIR_SEPARATOR"."ImplRepo_Service".$DIR_SEPARATOR."tao_ir".$EXE_EXT;
$server_program = "..$DIR_SEPARATOR..$DIR_SEPARATOR"."Time_Service".$DIR_SEPARATOR."Time_Service_Server".$EXE_EXT;
$clerk_program = "..$DIR_SEPARATOR..$DIR_SEPARATOR"."Time_Service".$DIR_SEPARATOR."Time_Service_Clerk".$EXE_EXT;
$client_program = $EXEPREFIX."client".$EXE_EXT;

sub time_service_test_using_naming_service
{
    $SV1 = Process::Create ($server_program,"");

    sleep 5;

    $SV2 = Process::Create ($clerk_program,"-t 2");

    sleep 10;

    $CL = Process::Create ($client_program, "");
    
    if ($CL->TimedWait (60) == -1) {
      print STDERR "ERROR: client timedout\n";
      $status = 1;
      $CL->Kill (); $CL->TimedWait (1);
    }

    $SV1->Terminate ();
    $SV2->Terminate ();
    if ($SV1->TimedWait (5) == -1 ||
        $SV2->TimedWait (5) == -1) {
      print STDERR "ERROR: couldn't shutdown the servers nicely\n";
      $status = 1;
      $SV1->Kill (); $SV2->Kill ();
      $SV1->TimedWait (1); $SV2->TimedWait (1);
    }
}

sub time_service_test_using_files
{
    $SV1 = Process::Create ($server_program,
                            "-o $server_ior");

    if (ACE::waitforfile_timed ($server_ior, 5) == -1) {
      print STDERR "ERROR: timedout waiting for file <$server_ior>\n";
      $status = 1;
      $SV1->Kill (); $SV1->TimedWait (1);
    }
    else {
      
      sleep 5;
      
      $SV2 = Process::Create ($clerk_program,
                              "-f $server_ior -o $clerk_ior -t 2");
      
      if (ACE::waitforfile_timed ($clerk_ior, 5) == -1) {
        print STDERR "ERROR: timedout waiting for file <$clerk_ior>\n";
        $status = 1;
        $SV2->Kill (); $SV2->TimedWait (1);
      }
      else {
        
        sleep 10;
        
        $CL = Process::Create ($client_program,
                          " -f $clerk_ior");
        if ($CL->TimedWait (60) == -1) {
          print STDERR "ERROR: client timedout\n";
          $status = 1;
          $CL->Kill (); $CL->TimedWait (1);
        }
      
        $SV1->Terminate ();
        $SV2->Terminate ();
        if ($SV1->TimedWait (5) == -1 ||
            $SV2->TimedWait (5) == -1) {
          print STDERR "ERROR: couldn't shutdown the servers nicely\n";
          $status = 1;
          $SV1->Kill (); $SV2->Kill ();
          $SV1->TimedWait (1); $SV2->TimedWait (1);
        }
      }
    }
    unlink $clerk_ior;
    unlink $server_ior;
}

sub time_service_test_using_ir
{
  $IR = Process::Create ($implrepo_program,
                         "-o $implrepo_ior -d 1");

  if (ACE::waitforfile_timed ($implrepo_ior, 5) == -1) {
    print STDERR "ERROR: timedout waiting for file <$implrepo_ior>\n";
    $IR->Kill (); $IR->TimedWait (1);
    exit 1;
  } 

  $TIR = Process::Create ($tao_ir_program, 
                          "-ORBImplRepoIOR file://$implrepo_ior add ".
                          "time_server -c \"$server_program ".
                          "-ORBImplRepoIOR file://$implrepo_ior -i\"");

  sleep 5;

  $SV1 = Process::Create ($server_program,
                         "-ORBImplRepoIOR file://$implrepo_ior -o $server_ior -i");

  if (ACE::waitforfile_timed ($server_ior, 5) == -1) {
    print STDERR "ERROR: timedout waiting for file <$implerepo_ior>\n";
    $IR->Kill (); $IR->TimedWait (1);
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
  }

  sleep 10;

  $SV2 = Process::Create ($clerk_program,
                          "-f $server_ior -o $clerk_ior");

  sleep 10;

  $CL = Process::Create ($client_program, "-f $clerk_ior");
  
  if ($CL->TimedWait (60) == -1) {
    print STDERR "ERROR: client timedout\n";
    $status = 1;
    $CL->Kill (); $CL->TimedWait (1);
  }
    
  $IR->Terminate ();
  if ($IR->TimedWait (5) == -1) {
    print STDERR "ERROR: couldn't shutdown repository nicely\n";
    $status = 1;
    $IR->Kill (); $IR->TimedWait (1);
  }

  $TIR->Terminate ();
  if ($TIR->TimedWait (5) == -1) {
    print STDERR "ERROR: couldn't shutdown tao_ir nicely\n";
    $status = 1;
    $TIR->Kill (); $TIR->TimedWait (1);
  }

  $SV1->Terminate ();
  $SV2->Terminate ();
  if ($SV1->TimedWait (5) == -1 ||
      $SV2->TimedWait (5) == -1) {
    print STDERR "ERROR: couldn't shutdown the servers nicely\n";
    $status = 1;
    $SV1->Kill (); $SV2->Kill ();
    $SV1->TimedWait (1); $SV2->TimedWait (1);
  }

  unlink $clerk_ior;
  unlink $server_ior;
  unlink $implrepo_ior;
}

# Parse the arguments

for ($i = 0; $i <= $#ARGV; $i++)
{
  SWITCH:
  {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?")
    {
      print "run_test <use_naming> or <use_files>\n";
      print "\n";
      print "use_naming               -- Runs the test using Naming Service\n";
      print "use_files                -- Runs the test using IOR Files\n";
      print "use_ir                   -- Runs the test with the IR\n";
      exit;
    }

    if ($ARGV[$i] eq "use_naming")
    {
      time_service_test_using_naming_service ();
      last SWITCH;
    }

    if ($ARGV[$i] eq "use_files")
    {
      time_service_test_using_files ();
      last SWITCH;
    }

    if ($ARGV[$i] eq "use_ir")
    {
      time_service_test_using_ir ();
      last SWITCH;
    }

    print "run_test: Unknown Option: ".$ARGV[$i]."\n";
  }
}

exit $status;
