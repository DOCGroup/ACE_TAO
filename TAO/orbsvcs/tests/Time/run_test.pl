eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../bin";

require ACEutils;
use Cwd;

$cwd = getcwd();
$server_ior = "$cwd$DIR_SEPARATOR" . "server_ior";
$clerk_ior = "$cwd$DIR_SEPARATOR" . "clerk_ior";
$implrepo_ior = "$cwd$DIR_SEPARATOR" . "implrepo.ior";
$status = 0;

# Make sure the files are gone, so we can wait on them.

unlink $server_ior;
unlink $clerk_ior;

ACE::checkForTarget($cwd);

$server_program = $EXEPREFIX."..$DIR_SEPARATOR..$DIR_SEPARATOR"."Time_Service".$DIR_SEPARATOR."Time_Service_Server".$EXE_EXT;
$clerk_program = $EXEPREFIX."..$DIR_SEPARATOR..$DIR_SEPARATOR"."Time_Service".$DIR_SEPARATOR."Time_Service_Clerk".$EXE_EXT;
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

    if (ACE::waitforfile_timed ($server_ior, 15) == -1) {
      print STDERR "ERROR: timedout waiting for file <$server_ior>\n";
      $status = 1;
      $SV1->Kill (); $SV1->TimedWait (1);
    }
    else {
      
      sleep 5;
      
      $SV2 = Process::Create ($clerk_program,
                              "-f $server_ior -o $clerk_ior -t 2");
      
      if (ACE::waitforfile_timed ($clerk_ior, 15) == -1) {
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

# Parse the arguments

for ($i = 0; $i <= $#ARGV; $i++)
{
  SWITCH:
  {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?")
    {
      print "run_test [-chorus <target>] <use_naming> or <use_files>\n";
      print "\n";
      print "-chorus <target>         -- Run the test on the Chorus target\n";
      print "use_naming               -- Runs the test using Naming Service\n";
      print "use_files                -- Runs the test using IOR Files\n";
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
    print "run_test: Unknown Option: ".$ARGV[$i]."\n";
  }
}

exit $status;
