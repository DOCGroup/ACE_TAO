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
$shutdown = "";
$all = 1;
$other = "";

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
                         " $transport -t $type -i $iterations "
			 . "-m $bufsize -w $work $shutdown ");

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

  $CL = Process::Create ($EXEPREFIX."client".$EXE_EXT,
                         "-ORBNodelay 0 -t none -i $iterations -m $bufsize");

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
    $other .= $ARGV[$i];
  }
}

if ($all == 1) {
  foreach $bufsize (@bufsizes) {
    run_buffered ($bufsize);
  }
}

foreach $type (@types) {
  run_test ($type);
}

unlink $iorfile;

if ($client != 0) {
  exit 1;
}

exit 0;
