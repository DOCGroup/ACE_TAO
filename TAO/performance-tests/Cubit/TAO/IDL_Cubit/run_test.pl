eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# The first three lines above let this script run without specifying the
# full path to perl, as long as it is in the user's PATH.
# Taken from perlrun man page.

# Do not use environment variables here since not all platforms use ACE_ROOT
use lib "../../../../../bin";

use English;
require Process;
require ACEutils;

$iorfile = "cubit.ior";
$exepref = '.' . $DIR_SEPARATOR;
$svnsflags = " -f $iorfile";
$clnsflags = " -f $iorfile";
$clflags = "";
$svflags = "";

# Parse the arguments

for ($i = 0; $i <= $#ARGV; $i++)
{
  SWITCH:
  {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?")
    {
      print "run_test [-h] [-n num] [-sleeptime t] [-debug] [-release] [-orblite]\n";
      print "\n";
      print "-h                  -- prints this information\n";
      print "-n num              -- client uses <num> iterations\n";
      print "-sleeptime t        -- run_test should sleep for <t> seconds between running\n";
      print "                       the server and client (default is 5 seconds)\n";
      print "-debug              -- sets the debug flag for both client and server\n";
      print "-release            -- runs the Release version of the test (for NT use)\n";
      print "-orblite            -- Use the lite version of the orb";
      exit;
    }
    if ($ARGV[$i] eq "-debug")
    {
      $clflags .= " -d";
      $svflags .= " -d";
      last SWITCH;
    }
    if ($ARGV[$i] eq "-release")
    {
      $exepref = "Release".$DIR_SEPARATOR;
      last SWITCH;
    }
    if ($ARGV[$i] eq "-n")
    {
      $clflags .= " -n $ARGV[$i + 1]";
      $i++;
      last SWITCH;
    }
    if ($ARGV[$i] eq "-sleeptime")
    {
      $ACE::sleeptime = $ARGV[$i + 1];
      $i++;
      last SWITCH;
    }
    if ($ARGV[$i] eq "-orblite")
    {
      $clflags .= " -ORBGIOPlite";
      $svflags .= " -ORBGIOPlite";
      last SWITCH;
    }
    print "run_test: Unknown Option: ".$ARGV[$i]."\n";
  }
}

(-f $exepref."server".$EXE_EXT  &&
 -f $exepref."client".$EXE_EXT)  ||
  die "$0: server and/or client need to be built!\n";

unlink $iorfile;

sleep 2;

print stderr "\nRunning IDL_Cubit with the default ORB protocol.\n\n";

$SV = Process::Create ($exepref."server".$EXE_EXT,
                       $svflags.
                       $svnsflags);

if (ACE::waitforfile_timed ($iorfile, 10) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

$CL = Process::Create ($exepref . "client".$EXE_EXT,
                       " $clflags $clnsflags -x");

$client = $CL->TimedWait (120);
if ($client == -1) {
  print STDERR "ERROR: client timedout\n";
  $CL->Kill (); $CL->TimedWait (1);
}

$server = $SV->TimedWait (10);
if ($server == -1) {
  print STDERR "ERROR: server timedout\n";
  $SV->Kill (); $SV->TimedWait (1);
}

unlink $iorfile;

if ($server != 0 || $client != 0) {
  exit 1;
}

if ($OSNAME ne "MSWin32")
{
  # Only run the UIOP test on Unix platforms.

  Process::Wait();

  # No need to unlink the iorfile again.
  # unlink $iorfile;

  sleep 2;

  print stderr "\nRunning IDL_Cubit with the UIOP protocol.\n\n";

  # Save the original server flags.
  $save_svflags = $svflags;

  $svflags .= " -ORBEndpoint uiop://";

  $SV = Process::Create ($exepref."server".$EXE_EXT,
                         $svflags.
                         $svnsflags);

  if (ACE::waitforfile_timed ($iorfile, 10) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
  }

  $CL = Process::Create ($exepref . "client".$EXE_EXT,
                         " $clflags $clnsflags -x");

  $client = $CL->TimedWait (120);
  if ($client == -1) {
    print STDERR "ERROR: client timedout\n";
    $CL->Kill (); $CL->TimedWait (1);
  }

  $server = $SV->TimedWait (30);
  if ($server == -1) {
    print STDERR "ERROR: server timedout\n";
    $SV->Kill (); $SV->TimedWait (1);
  }

  # Restore the original server flags
  $svflags = $save_svflags;

  unlink $iorfile;

  if ($server != 0 || $client != 0) {
    exit 1;
  }
}


exit 0;
