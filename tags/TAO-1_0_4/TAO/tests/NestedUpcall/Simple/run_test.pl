eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../../bin';
require ACEutils;

$iorfile = "test.ior";

unlink $iorfile;

print STDERR "\nrunning ST version of the client and the server\n\n";

$SV = Process::Create ($EXEPREFIX."server".$EXE_EXT,
		       " -f $iorfile");

if (ACE::waitforfile_timed ($iorfile, 5) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

$CL  = Process::Create ($EXEPREFIX."client$EXE_EXT ",
			" -k file://$iorfile -x");

$client = $CL->TimedWait (60);
if ($client == -1) {
  print STDERR "ERROR: client timedout\n";
  $CL->Kill (); $CL->TimedWait (1);
}

$server = $SV->TimedWait (10);
if ($server == -1) {
  print STDERR "ERROR: server timedout\n";
  $SV->Kill (); $SV->TimedWait (1);
}

unlink $iorfile;

if ($client != 0 || server != 0) {
  exit 1;
}

print STDERR "\nrunning MT version of the client and the server\n\n";

$SV = Process::Create ($EXEPREFIX."server".$EXE_EXT,
		       " -f $iorfile -t 2");

if (ACE::waitforfile_timed ($iorfile, 5) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

$CL  = Process::Create ($EXEPREFIX."client$EXE_EXT ",
			" -k file://$iorfile -x -n 2");

$client = $CL->TimedWait (60);
if ($client == -1) {
  print STDERR "ERROR: client timedout\n";
  $CL->Kill (); $CL->TimedWait (1);
}

$server = $SV->TimedWait (10);
if ($server == -1) {
  print STDERR "ERROR: server timedout\n";
  $SV->Kill (); $SV->TimedWait (1);
}

exit 0;
