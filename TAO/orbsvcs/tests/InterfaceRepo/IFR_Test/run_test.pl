eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../../bin";

require ACEutils;
use Cwd;

$cwd = getcwd();
ACE::checkForTarget($cwd);

$locking = "-m";

$if_repo_server = $EXEPREFIX."..".$DIR_SEPARATOR."..".$DIR_SEPARATOR.
		  "..".$DIR_SEPARATOR."IFR_Service".$DIR_SEPARATOR.
		  "IFR_Service".$EXE_EXT, " $locking";


$init_ref = 
  "-ORBInitRef InterfaceRepository=file://if_repo.ior";

$iorfile = "$cwd$DIR_SEPARATOR" . "if_repo.ior";

$debug = "";
$test = "";
$iterations = "";
$other = "";

for ($i = 0; $i <= $#ARGV; $i++)
{
  SWITCH:
  {
    if ($ARGV[$i] eq "-d")
    {
      $debug = "-d";
      last SWITCH;
    }
    if ($ARGV[$i] eq "-t")
    {
      $test = "-t ".$ARGV[$i + 1];
      $i++;
      last SWITCH;
    }
    if ($ARGV[$i] eq "-i")
    {
      $iterations = "-i ".$ARGV[$i + 1];
      $i++;
      last SWITCH;
    }
    $other .= $ARGV[$i];
  }
}

unlink $iorfile;

$SV = Process::Create ($if_repo_server);

if (ACE::waitforfile_timed ($iorfile, 15) == -1) 
{
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $SV->Kill (); 
  $SV->TimedWait (1);
  exit 1;
}

$CL = Process::Create ($EXEPREFIX."IFR_Test".$EXE_EXT,
                       " $init_ref $debug $test $iterations");

$client = $CL->TimedWait (60);
if ($client == -1) 
{
  print STDERR "ERROR: client timedout\n";
  $CL->Kill (); 
  $CL->TimedWait (1);
}

$SV->Kill (); 
$SV->TimedWait (1);

unlink $iorfile;

exit 0;

