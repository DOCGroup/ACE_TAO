eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../../../bin';
require ACEutils;

$iorfile = "test.ior";
$iterations = 4000;
$bufsize = 4000;
$work = 10;
$transport = "";
$type = "";
$level = "object";
$shutdown = "";
$all = 1;
$other = "";

unlink $iorfile;

sub run_test
{
  my $type = shift(@_);

  if ($type == "none" || $type == "transport") {
    $transport = "-ORBNodelay 0";
  }
  else {
    $transport = "";
  }

  if ($all == 0) {
    $shutdown = "-x";
  }

  print STDERR "\n***************** STARTING TEST ******************\n";

  $CL = Process::Create ($EXEPREFIX."client$EXE_EXT ",
                         " $transport -t $type -i $iterations -l $level"
			 . " -m $bufsize -w $work $shutdown ");

  $client = $CL->TimedWait (60);
  if ($client == -1) {
    print STDERR "ERROR: client timedout\n";
    $CL->Kill (); $CL->TimedWait (1);
  }
}

sub run_buffered
{
  my $bufsize = shift (@_);

  print STDERR "\n***************** STARTING TEST ******************\n";

  $CL = Process::Create ($EXEPREFIX."client$EXE_EXT ",
                         " -ORBNodelay 0 -l $level "
			 . " -t none -i $iterations -m $bufsize");

  $client = $CL->TimedWait (60);
  if ($client == -1) {
    print STDERR "ERROR: client timedout\n";
    $CL->Kill (); $CL->TimedWait (1);
  }
}

# Parse the arguments

@types = ("none", "transport", "server", "target", "twoway -x");

@bufsizes = (10, 40, 100, 400, 1000, 2000);

for ($i = 0; $i <= $#ARGV; $i++)
{
  SWITCH:
  {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?") 
    {
      print "Run_Test Perl script for TAO Reliable Oneways Test\n\n";
      print "run_test [-t test type] [-i iterations] [-m buffer size]\n";
      print "\n";
      print "-t test type        -- runs only one type of oneway test\n";
      print "-i iterations       -- number of calls in each test\n";
      print "-l level		 -- level at which policy is set\n";
      print "-m buffer size      -- queue size for buffered oneways\n";
      print "-w servant work	 -- number of loops of 1000 by servant\n";
      exit 0;
    }
    if ($ARGV[$i] eq "-i")
    {
      $iterations = $ARGV[$i + 1];
      $i++;
      last SWITCH;
    }
    if ($ARGV[$i] eq "-m")
    {
      $bufsize = $ARGV[$i + 1];
      $i++;
      last SWITCH;
    }
    if ($ARGV[$i] eq "-w")
    {
      $work = $ARGV[$i + 1];
      $i++;
      last SWITCH;
    }
    if ($ARGV[$i] eq "-t")
    {
      @types = split (',', $ARGV[$i + 1]);
      $all = 0;
      $i++;
      last SWITCH;
    }
    if ($ARGV[$i] eq "-l")
    {
      $level = $ARGV[$i + 1];
      $i++;
      last SWITCH;
    }
    $other .= $ARGV[$i];
  }
}

unlink $iorfile;

$SV = Process::Create ($EXEPREFIX."server$EXE_EXT");

if (ACE::waitforfile_timed ($iorfile, 5) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

if ($all == 1) {
  foreach $bufsize (@bufsizes) {
    run_buffered ($bufsize);
  }
}

print STDERR "\nSyncScope policy set at the ";
print STDERR $level;
print STDERR " level\n";

foreach $type (@types) {
  run_test ($type);
}

$server = $SV->TimedWait (5);
if ($server == -1) {
  print STDERR "ERROR: server timedout\n";
  $SV->Kill (); $SV->TimedWait (1);
}

unlink $iorfile;

if ($server != 0 || $client != 0) {
  exit 1;
}

exit 0;
