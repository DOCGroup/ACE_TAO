eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../bin';
require ACEutils;
use Cwd;

$cwd = getcwd();
$iorfile = "$cwd$DIR_SEPARATOR" . "test.ior";
$port = ACE::uniqueid () + 10001;  # This can't be 10000 for Chorus 4.0

ACE::checkForTarget($cwd);

unlink $iorfile;
$SV = Process::Create ($EXEPREFIX."server$EXE_EXT ",
		       "-ORBEndpoint iiop://localhost:".$port
		       . " -o $iorfile");

if (ACE::waitforfile_timed ($iorfile, 5) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

print STDERR "==== Running first test, using full IORs\n";

$CL1 = Process::Create ($EXEPREFIX."client$EXE_EXT ",
			"-i 100 -k file://$iorfile");

$client1 = $CL1->TimedWait (60);
if ($client1 == -1) {
  print STDERR "ERROR: client timedout\n";
  $CL1->Kill (); $CL1->TimedWait (1);
}

if ($client2 != 0) {
  print STDERR "ERROR running client (file://)\n";
}

print STDERR "==== Running second test, using corbaloc IORs ($port)\n";

$CL2 = Process::Create ($EXEPREFIX."client$EXE_EXT ",
			"-x -i 100 "
			. "-k corbaloc::localhost:"
			.$port
			."/Simple_Server");

$client2 = $CL2->TimedWait (60);
if ($client2 == -1) {
  print STDERR "ERROR: client timedout\n";
  $CL2->Kill (); $CL2->TimedWait (1);
}

if ($client2 != 0) {
  print STDERR "ERROR running client (iioploc://)\n";
}

$server = $SV->TimedWait (5);
if ($server == -1) {
  print STDERR "ERROR: server timedout\n";
  $SV->Kill (); $SV->TimedWait (1);
}

unlink $iorfile;

if ($server != 0
    || $client1 != 0
    || $client2 != 0) {
  exit 1;
}

exit 0;
