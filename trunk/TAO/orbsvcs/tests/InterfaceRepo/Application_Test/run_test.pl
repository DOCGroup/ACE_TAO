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

$if_repo_service = $EXEPREFIX."..".$DIR_SEPARATOR."..".$DIR_SEPARATOR.
		   "..".$DIR_SEPARATOR."IFR_Service".$DIR_SEPARATOR.
		   "IFR_Service".$EXE_EXT;


$init_ref = 
  "-ORBInitRef InterfaceRepository=file://if_repo.ior";

if ($^O eq "MSWin32") {
    ### It is in the path
    $tao_ifr = "tao_ifr".$EXE_EXT;
}
else {
    $tao_ifr = "$ACE_ROOT/TAO/orbsvcs/IFR_Service/tao_ifr".$EXE_EXT;
}

$ifr_iorfile = "$cwd$DIR_SEPARATOR" . "if_repo.ior";
$svr_iorfile = "$cwd$DIR_SEPARATOR" . "iorfile";

unlink $ifr_iorfile;
unlink $svr_iorfile;

$IFR = Process::Create ($if_repo_service);

if (ACE::waitforfile_timed ($ifr_iorfile, 15) == -1) 
{
  print STDERR "ERROR: cannot find file <$ifr_iorfile>\n";
  $IFR->Kill (); 
  $IFR->TimedWait (1);
  exit 1;
}

$SV = Process::Create ($EXEPREFIX."server".$EXE_EXT);

if (ACE::waitforfile_timed ($svr_iorfile, 15) == -1) 
{
  print STDERR "ERROR: cannot find file <$svr_iorfile>\n";
  $SV->Kill (); 
  $SV->TimedWait (1);
  exit 1;
}

system ($tao_ifr." test.idl");

$CL = Process::Create ($EXEPREFIX."client".$EXE_EXT,
                       " $init_ref");

$client = $CL->TimedWait (60);
if ($client == -1) 
{
  print STDERR "ERROR: client timedout\n";
  $CL->Kill (); 
  $CL->TimedWait (1);
}

$SV->Kill (); 
$SV->TimedWait (1);

system ($tao_ifr." -r test.idl");

$IFR->Kill (); 
$IFR->TimedWait (1);

unlink $ifr_iorfile;
unlink $svr_iorfile;

exit 0;

