eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../bin';
require ACEutils;
use Cwd;

$cwd = getcwd();
$client_conf="$DIR_SEPARATOR" . "muxed.conf";
$client_process="simple_client";
$debug_level='0';
$threads='2';
$iterations='1';

ACE::checkForTarget($cwd);
foreach $i (@ARGV) {
  if ($i eq '-mux') {
    $client_conf = "muxed.conf";
  } elsif ($i eq '-debug') {
    $debug_level = '1';
  } elsif ($i eq '-exclusive') {
    $client_conf = 'exclusive.conf';
  }
}

$iorfile = "server.ior";

unlink $iorfile;
$SV = Process::Create ($EXEPREFIX."server$EXE_EXT",
			  " -ORBdebuglevel $debug_level"
			  . "-d -o $iorfile");

if (ACE::waitforfile_timed ($iorfile, 5) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

$CL = Process::Create ($EXEPREFIX."$client_process$EXE_EXT",
			  " -ORBsvcconf $client_conf "
			  . "-ORBdebuglevel $debug_level"
			  . " -k file://$iorfile "
			  . " -i $iterations -x -d");

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

if ($server != 0 || $client != 0) {
   exit 1;
}
 
exit 0;
 
