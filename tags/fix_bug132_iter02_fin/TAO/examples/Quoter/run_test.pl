eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a Perl script that runs the client and all the other servers that
# are needed.

unshift @INC, '../../../bin';
require Process;
#require Uniqueid;

$nsiorfile = "ns.ior";

# number of threads to use for multithreaded clients or servers

$num_threads = 4;

# amount of delay between running the servers

$sleeptime = 2;

# other variables

$status = 0;
$n = 1;
$leave = 1;
$debug = "";
$cm = "";
$sm = "";
$other = "";
$c_conf = ""; #-ORBsvcconf client.conf";
$s_conf = ""; #-ORBsvcconf server.conf";

# Programs that are run

$name_server = "..$DIR_SEPARATOR..$DIR_SEPARATOR"."orbsvcs$DIR_SEPARATOR".
               "Naming_Service$DIR_SEPARATOR"."Naming_Service$EXE_EXT";
$lifecycle_server = "..$DIR_SEPARATOR..$DIR_SEPARATOR"."orbsvcs$DIR_SEPARATOR".
                    "LifeCycle_Service$DIR_SEPARATOR"."LifeCycle_Service$EXE_EXT";
$quoter_server = $EXE_PREFIX."server$EXE_EXT";
$factory_finder = $EXE_PREFIX."Factory_Finder$EXE_EXT";
$generic_factory = $EXE_PREFIX."Generic_Factory$EXE_EXT";
$quoter_client = $EXE_PREFIX."client$EXE_EXT";

sub name_server
{
  my $args = "$other -o $nsiorfile";
  $NS = Process::Create ($name_server, $args);
}

sub lifecycle_server
{
  my $args = "$other $debug -ORBInitRef NameService=file://$nsiorfile";
  $LC = Process::Create ($lifecycle_server, $args);
}

sub server
{
  my $args = "$other $debug $sm ".
             "-ORBInitRef NameService=file://$nsiorfile $s_conf";

  $SV = Process::Create ($quoter_server, $args);
}

sub factory_finder
{
  my $args = "$other $debug -ORBInitRef NameService=file://$nsiorfile";

  $FF = Process::Create ($factory_finder, $args);
}

sub generic_factory
{
  my $args = "$other -l $debug -ORBInitRef NameService=file://$nsiorfile";

  $GF = Process::Create ($generic_factory, $args);
}

sub client
{
  my $args = "$other -l $debug $cm ".
            "-ORBInitRef NameService=file://$nsiorfile $c_conf";
  for ($j = 0; $j < $n; $j++)
  {
    $client_ = Process::Create($quoter_client, $args);
    if ( $client_->TimedWait (60) ) {
      print STDERR "ERROR: a client has timedout\n";
      $status = 1;
      $client_->Kill (); $client_->TimedWait (1);
    }
  }
}

# Parse the arguments

for ($i = 0; $i <= $#ARGV; $i++)
{
  SWITCH:
  {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?")
    {
      print "run_test [-n num] [-leave] [-onewin]".
        "[-twowin] [-d level] [-h] [-nt] [-cm] [-sm] [-ns|sv|ff|cl|gf]\n";
      print "\n";
      print "-n num              -- runs the client num times\n";
      print "-leave              -- leaves the servers running and their windows open\n";
      print "-onewin             -- keeps all tests in one window on NT\n";
      print "-twowin             -- put each test in a separate window on NT\n";
      print "-d level            -- runs each at debug level <level>\n";
      print "-h                  -- prints this information\n";
      print "-nt num             -- number of threads in the client (twice for server)\n";
      print "                       make sure this is before any -cm or -sm\n";
      print "-cm                 -- use more than one thread in the client\n";
      print "-sm                 -- use more than one thread in the server\n";
      print "-ns -sv -ff -gf -cl -- runs only one of the executables\n";
      exit;
    }
    if ($ARGV[$i] eq "-n")
    {
      $n = $ARGV[$i + 1];
      $i++;
      last SWITCH;
    }
    if ($ARGV[$i] eq "-nt")
    {
      $num_threads = $ARGV[$i + 1];
      $i++;
      last SWITCH;
    }
    if ($ARGV[$i] eq "-d")
    {
      $debug = $debug." -d $ARGV[$i + 1]";
      $i++;
      last SWITCH;
    }
    if ($ARGV[$i] eq "-cm")
    {
      $cm = "-n ".$num_threads;
      last SWITCH;
    }
    if ($ARGV[$i] eq "-sm")
    {
      $sm = "-n ".($num_threads * 3);
      last SWITCH;
    }
    if ($ARGV[$i] eq "-leave")
    {
      $leave = 0;
      last SWITCH;
    }
    if ($ARGV[$i] eq "-ns")
    {
      name_server ();
      exit;
    }
    if ($ARGV[$i] eq "-sv")
    {
      server ();
      exit;
    }
    if ($ARGV[$i] eq "-ff")
    {
      factory_finder ();
      exit;
    }
    if ($ARGV[$i] eq "-gf")
    {
      generic_factory ();
      exit;
    }
    if ($ARGV[$i] eq "-cl")
    {
      client ();
      exit;
    }
    if ($ARGV[$i] eq "-customconf")  #secret flag from testall.pl
    {
      #use a different set of *.conf files
      $c_conf = "-ORBsvcconf c.conf";
      $s_conf = "-ORBsvcconf s.conf";
      last SWITCH;
    }
    if ($ARGV[$i] eq "-onewin")
    {
      if ($^O eq "MSWin32")
      {
        $Process::newwindow = "no";
      }
      last SWITCH;
    }
    if ($ARGV[$i] eq "-twowin")
    {
      if ($^O eq "MSWin32")
      {
        $Process::newwindow = "yes";
      }
      last SWITCH;
    }

    $other = $other." ".$ARGV[$i];
  }
}

name_server ();
sleep $sleeptime;
lifecycle_server ();
sleep $sleeptime;

server ();
sleep $sleeptime;

factory_finder ();
sleep $sleeptime;

generic_factory ();
sleep $sleeptime;

client ();

if ($leave)
{
  $GF->Terminate ();
  $FF->Terminate ();
  $SV->Terminate ();
  $LC->Terminate ();
  $NS->Terminate ();
  if ($GF->TimedWait (5) == -1 ||
      $FF->TimedWait (5) == -1 ||
      $SV->TimedWait (5) == -1 ||
      $LC->TimedWait (5) == -1 ||
      $NS->TimedWait (5) == -1){
    print STDERR "ERROR: couldn't terminate services nicely\n";
    $status = 1;
    $GF->Kill (); $GF->TimedWait (1);
    $FF->Kill (); $FF->TimedWait (1);
    $SV->Kill (); $SV->TimedWait (1);
    $LC->Kill (); $LC->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
  }
}

unlink $nsiorfile;

if ($status != 0)
{
    exit 1;
}

exit 0;
