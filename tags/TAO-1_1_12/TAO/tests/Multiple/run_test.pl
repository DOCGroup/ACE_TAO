eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../bin";
require Process;


unshift @INC, '../../../bin';
require ACEutils;
use Cwd;


$status = 0;
$EXEPREFIX = "./";
$DIRECT_COLLOC = "-ORBCollocationStrategy direct";
$NO_COLLOC = "-ORBCollocation no";

$cwd = getcwd();

$iorfile = "$cwd$DIR_SEPARATOR" . "s.ior";
unlink $iorfile;

ACE::checkForTarget($cwd);

#
# Test using ThruPOA collocation.
#


$Server = Process::Create ($EXEPREFIX."server$EXE_EXT ",
			   "");

if (ACE::waitforfile_timed ($iorfile, 5) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $Server->Kill (); $Server->TimedWait (1);
  exit 1;
}


$Client = Process::Create($EXEPREFIX."client$EXE_EXT", "");

if ($Client->TimedWait (60) == -1) {
  print STDERR "ERROR: client timedout\n";
  $status = 1;
  $Client->Kill (); 
}

$Server->Kill ();


#
# Test using Direct Collocation
#

unlink $iorfile;

$Server = Process::Create ($EXEPREFIX."server$EXE_EXT", $DIRECT_COLLOC);

if (ACE::waitforfile_timed ($iorfile, 5) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $Server->Kill (); $Server->TimedWait (1);
  exit 1;
}


$Client = Process::Create($EXEPREFIX."client$EXE_EXT", "");

if ($Client->TimedWait (60) == -1) {
  print STDERR "ERROR: client timedout\n";
  $status = $status+1;
  $Client->Kill (); 
}

$Server->Kill ();

#
# Test using No Collocation
#

unlink $iorfile;

$Server = Process::Create ($EXEPREFIX."server$EXE_EXT", $NO_COLLOC);

if (ACE::waitforfile_timed ($iorfile, 5) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $Server->Kill (); $Server->TimedWait (1);
  exit 1;
}


$Client = Process::Create($EXEPREFIX."client$EXE_EXT", "");

if ($Client->TimedWait (60) == -1) {
  print STDERR "ERROR: client timedout\n";
  $status = $status+1;
  $Client3->Kill (); 
}

$Server->Kill ();

exit $status
