eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../bin';
require ACEutils;

$iorfile = "ior";

unlink $iorfile;

$SV = Process::Create ($EXEPREFIX."server$EXE_EXT");

if (ACE::waitforfile_timed ($iorfile, 5) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

print STDOUT "\nFlushing based on implicit message counts\n\n";

$CL = Process::Create ($EXEPREFIX."client$EXE_EXT ",
                       "-c 5 -b -1 -f -1 -t -1");

$client = $CL->TimedWait (60);
if ($client == -1) {
  print STDERR "ERROR: client timedout\n";
  $CL->Kill (); $CL->TimedWait (1);
}

print STDOUT "\nFlushing based on implicit message bytes\n\n";

$CL = Process::Create ($EXEPREFIX."client$EXE_EXT ",
                       "-b 250 -c -1 -f -1 -t -1");

$client = $CL->TimedWait (60);
if ($client == -1) {
  print STDERR "ERROR: client timedout\n";
  $CL->Kill (); $CL->TimedWait (1);
}

print STDOUT "\nFlushing based on implicit timeout\n\n";

$CL = Process::Create ($EXEPREFIX."client$EXE_EXT ",
                       "-t 5000 -b -1 -c -1 -f -1");

$client = $CL->TimedWait (60);
if ($client == -1) {
  print STDERR "ERROR: client timedout\n";
  $CL->Kill (); $CL->TimedWait (1);
}

print STDOUT "\nExplicit queue flushing (and server shutdown)\n\n";

$CL = Process::Create ($EXEPREFIX."client$EXE_EXT ",
                       "-f 5 -b -1 -c 100 -t -1 -x");

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
