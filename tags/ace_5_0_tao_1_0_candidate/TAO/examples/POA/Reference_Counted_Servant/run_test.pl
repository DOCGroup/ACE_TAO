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

unlink $iorfile;

$SV = Process::Create ($EXEPREFIX."server$EXE_EXT", "-f $iorfile $extra_args");

if (ACE::waitforfile_timed ($iorfile, 15) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

$CL  = Process::Create ("../Generic_Servant/client$EXE_EXT ",
			" $extra_args $oneway -i $iterations -f $iorfile -x");

$client = $CL->TimedWait (60);
if ($client == -1) {
  print STDERR "ERROR: client timedout\n";
  $CL->Kill (); $CL->TimedWait (1);
}

$server = $SV->TimedWait (5);
if ($server == -1) {
  print STDERR "ERROR: server timedout\n";
  $SV->Kill (); $SV->TimedWait (1);
}

unlink $iorfile;

if ($server != 0 || $client != 0){
  exit 1;
}

exit 0;
