eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$

# The first three lines above let this script run without specifying the
# full path to perl, as long as it is in the user's PATH.
# Taken from perlrun man page.

# Do not use environment variables here since not all platforms use ACE_ROOT
use lib "../../../../../bin";

require ACEutils;

$nsport = 20000 + ACE::uniqueid ();
$server_port = 0;
$iorfile = "cubit.ior";
$exepref = '.' . $DIR_SEPARATOR;
$svnsflags = " -o $iorfile";
$clnsflags = " -f $iorfile";
$clflags = " -ORBobjrefstyle url";
$svflags = " -ORBobjrefstyle url";
$mcast = 0;

#make sure the file is gone, so we can wait on it.
unlink $iorfile;

# Parse the arguments

for ($i = 0; $i <= $#ARGV; $i++)
{
  SWITCH:
  {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?")
    {
      print "run_test [-h] [-n num] [-mcast] [-sleeptime t] [-debug] [-release] [-orblite]\n";
      print "\n";
      print "-h                  -- prints this information\n";
      print "-n num              -- client uses <num> iterations\n";
      print "-mcast              -- uses the multicast version of the nameservice\n";
      print "-sleeptime t        -- run_test should sleep for <t> seconds between running\n";
      print "                       the server and client (default is 5 seconds)\n";
      print "-debug              -- sets the debug flag for both client and server\n";
      print "-release            -- runs the Release version of the test (for NT use)\n";
      print "-orblite            -- Use the lite version of the orb";
      exit;
    }
    if ($ARGV[$i] eq "-mcast")
    {
      $mcast = 1;
      $clnsflags = " -ORBnameserviceport $nsport";
      $svnsflags = " -ORBnameserviceport $nsport";
      last SWITCH;
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
      $clargs .= " -ORBgioplite";
      $svargs .= " -ORBgioplite";
      last SWITCH;
    }
    print "run_test: Unknown Option: ".$ARGV[$i]."\n";
  }
}

(-f 'server'  &&  -f 'client')  ||
  die "$0: server and/or client need to be built!\n";

$SV = Process::Create ($exepref."server".$Process::EXE_EXT,
                       $svflags.
                       $svnsflags);

# Put in a wait between the server and client
if ($mcast == 1)
{
  sleep $ACE::sleeptime;
}
else
{
  ACE::waitforfile ($iorfile);
}

unlink

$status = system ($exepref."client".$Process::EXE_EXT.
                  $clflags.
                  $clnsflags.
                  " -x");

# @@ TODO change to Wait() once the -x option works.
$SV->Kill (); $SV->Wait ();

unlink $iorfile;

# @@ Capture any errors from the server too.
exit $status;
