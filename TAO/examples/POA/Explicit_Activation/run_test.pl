eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../../bin';
require ACEutils;

$iorfile = "ior";

$oneway = "";
$iterations = 100;

$extra_args = "";

# Parse the arguments
for ($i = 0; $i <= $#ARGV; $i++)
{
  SWITCH:
  {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?")
    {
      print "run_test [-h] [-i iterations] [-o] [-f ior file]\n";
      print "\n";
      print "-h                  -- prints this information\n";
      print "-f                  -- ior file\n";
      print "-i iterations       -- specifies iterations\n";
      print "-o                  -- call issued are oneways\n";
      exit;
    }
    if ($ARGV[$i] eq "-o")
    {
      $oneway = "-o";
      last SWITCH;
    }
    if ($ARGV[$i] eq "-i")
    {
      $iterations = $ARGV[$i + 1];
      $i++;
      last SWITCH;
    }
    if ($ARGV[$i] eq "-f")
    {
      $iorfile = $ARGV[$i + 1];
      $i++;
      last SWITCH;
    }
    $extra_args .= " " . $ARGV[$i];
  }
}

$iorfile_1 = $iorfile."_1";
$iorfile_2 = $iorfile."_2";
$iorfile_3 = $iorfile."_3";

unlink $iorfile_1;
unlink $iorfile_2;
unlink $iorfile_3;


$SV = Process::Create ($EXEPREFIX."server$EXE_EXT", "-f $iorfile $extra_args");

if (ACE::waitforfile_timed ($iorfile_1, 5) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile_1>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

if (ACE::waitforfile_timed ($iorfile_2, 5) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile_2>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

if (ACE::waitforfile_timed ($iorfile_3, 5) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile_3>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

$CL_1  = Process::Create ("../Generic_Servant/client$EXE_EXT ",
			  " $extra_args $oneway -i $iterations -f $iorfile_1");
$CL_2  = Process::Create ("../Generic_Servant/client$EXE_EXT ",
			  " $extra_args $oneway -i $iterations -f $iorfile_2");
$CL_3  = Process::Create ("../Generic_Servant/client$EXE_EXT ",
			  " $extra_args $oneway -i $iterations -f $iorfile_3 -x");


$client_1 = $CL_1->TimedWait (60);
if ($client_1 == -1) {
  print STDERR "ERROR: client 1 timedout\n";
  $CL_1->Kill (); $CL_1->TimedWait (1);
}

$client_2 = $CL_2->TimedWait (60);
if ($client_2 == -1) {
  print STDERR "ERROR: client 2 timedout\n";
  $CL_2->Kill (); $CL_2->TimedWait (1);
}

$client_3 = $CL_3->TimedWait (60);
if ($client_3 == -1) {
  print STDERR "ERROR: client 3 timedout\n";
  $CL_3->Kill (); $CL_3->TimedWait (1);
}


$server = $SV->TimedWait (5);
if ($server == -1) {
  print STDERR "ERROR: server timedout\n";
  $SV->Kill (); $SV->TimedWait (1);
}

unlink $iorfile_1;
unlink $iorfile_2;
unlink $iorfile_3;

if ($server != 0 ||
    $client_1 != 0 ||
    $client_2 != 0 ||
    $client_3 != 0)
{
  exit 1;
}

exit 0;


