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

$SV = Process::Create ($EXEPREFIX."server$Process::EXE_EXT", "-f $iorfile $extra_args");

ACE::waitforfile ($iorfile);

$status  = system ("../Generic_Servant/client$Process::EXE_EXT $extra_args $oneway -i $iterations -f $iorfile -x");

unlink $iorfile;

exit $status;
