eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../bin";
require Process;

$status = 0;
$POAConfigFile = "POA.cfg";
$ObjectConfigFile = "Object.cfg";

$SV = Process::Create ($EXEPREFIX."server$EXE_EXT", "-POAConfigFile $POAConfigFile -ObjectConfigFile $ObjectConfigFile");


$client = Process::Create($EXEPREFIX."client$EXE_EXT", "-POAConfigFile $POAConfigFile -ObjectConfigFile $ObjectConfigFile");

if ($client->TimedWait (60) == -1) {
  print STDERR "ERROR: client timedout\n";
  $status = 1;
  $client->Kill (); $client->TimedWait (1);
}

$SV->Kill ();

exit $status;
