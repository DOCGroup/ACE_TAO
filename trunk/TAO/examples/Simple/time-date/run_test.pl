eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../bin";
require ACEutils;
require Process;

$status = 0;
$iorfile = "./ior";

$SV = Process::Create ($EXEPREFIX."server$EXE_EXT", "");

if (ACE::waitforfile_timed ($iorfile, 5) == -1) {
  print STDERR "ERROR: timedout waiting for file <$iorfile>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

$CL  = Process::Create ($EXEPREFIX."client$EXE_EXT",
                            "-f $iorfile -ORBSkipServiceConfigOpen");

if ($CL->TimedWait (60) == -1) {
  print STDERR "ERROR: client timedout\n";
  $status = 1;
  $CL->Kill (); $CL->TimedWait (1);
}

$SV->Kill (); $SV->TimedWait (1);

unlink $iorfile;

exit $status;
