eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../bin';
require Process;
require ACEutils;

print STDERR "\n\n==== Octet sequence passing test\n";

$file="test.ior";

unlink $file;

$SV = Process::Create ($EXEPREFIX."server".$EXE_EXT,
                       "-o $file");
if (ACE::waitforfile_timed ($file, 3) == -1) {
  print STDERR "ERROR: cannot find file <$file>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

$CL = Process::Create ($EXEPREFIX."client".$EXE_EXT,
                       " -i 5000 -k file://$file");

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

if ($client == -1 || $server == -1) {
  exit 1;
}

unlink $file;

print STDERR "\n\n==== Octet sequence performance test\n";

$T = Process::Create ($EXEPREFIX."OctetSeq$EXE_EXT",
                      " -n 32 -l 8192 -h 8192 -s 1 -q");

$client = $T->TimedWait (60);
if ($client == -1) {
  print STDERR "ERROR: test timedout\n";
  $T->Kill (); $T->TimedWait (1);
}

if ($client == -1) {
  exit 1;
}

exit 0;
