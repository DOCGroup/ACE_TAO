#$Id$
# -*- perl -*-
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

unshift @INC, '../../../../../bin';
require Process;
require Uniqueid;

$nsport = 20000 + uniqueid ();
$server_port = 0;
$iorfile = "theior";
$sleeptime = 5;
$exepref = "";
$svnsflags = " -s -o $iorfile";
$clnsflags = " -s -f $iorfile";
$debug = "";
$numflag = "";

# Parse the arguments

for ($i = 0; $i <= $#ARGV; $i++)
{
  SWITCH:
  {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?")
    {
      print "run_test [-h] [-n num] [-mcast] [-sleeptime t] [-debug] [-release]\n";
      print "\n";
      print "-h                  -- prints this information\n";
      print "-n num              -- client uses <num> iterations\n";
      print "-mcast              -- uses the multicast version of the nameservice\n";
      print "                       (default is 5 seconds)\n";
      print "-sleeptime t        -- run_test should sleep for <t> seconds between running\n";
      print "                       the server and client\n";
      print "-debug              -- sets the debug flag for both client and server\n";
      print "-release            -- runs the Release version of the test (for NT use)\n";
      exit;
    }
    if ($ARGV[$i] eq "-mcast")
    {
      $clnsflags = " -ORBnameserviceport $nsport";
      $svnsflags = " -ORBnameserviceport $nsport";
      last SWITCH;
    }
    if ($ARGV[$i] eq "-debug")
    {
      $debug = " -d";
      last SWITCH;
    }
    if ($ARGV[$i] eq "-release")
    {
      $exepref = "Release".$DIR_SEPARATOR;
      last SWITCH;
    }
    if ($ARGV[$i] eq "-n")
    {
      $numflag = " -n ".$ARGV[$i + 1];
      $i++;
      last SWITCH;
    }
    if ($ARGV[$i] eq "-sleeptime")
    {
      $sleeptime = $ARGV[$i + 1];
      $i++;
      last SWITCH;
    }
    print "run_test: Unknown Option: ".$ARGV[$i]."\n";
  }
}

$SV = Process::Create ($exepref."server".$Process::EXE_EXT, 
                       $debug.
                       " -ORBport ".$server_port.
                       " -ORBobjrefstyle url".
                       $svnsflags);

# Put in a wait between the server and client
if ($svnsflags eq "")
{
  sleep $sleeptime;
}
else 
{
  while (!(-e $iorfile))
  { 
    sleep 1;
  }
}

$status = system ($exepref."client".$Process::EXE_EXT.
                  $debug.
                  $numflag.
                  $clnsflags.
                  " -x");

# @@ TODO change to Wait() once the -x option works.
$SV->Kill (); $SV->Wait ();

unlink $iorfile;

# @@ Capture any errors from the server too.
exit $status;
