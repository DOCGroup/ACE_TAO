eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../bin';
require ACEutils;

$client_conf="client.global.conf";
$client_process="client";
$debug_level='0';
$threads='10';
$status = 0;

foreach $i (@ARGV) {
  if ($i eq '-tss') {
    $client_conf = "client.tss.conf";
  } elsif ($i eq '-debug') {
    $debug_level = '1';
  } elsif ($i eq '-creation') {
    $client_process = 'orb_creation';
    $threads='2';
  }
}

$iorfile = "server.ior";
$SV = Process::Create ($EXEPREFIX."server$EXE_EXT ",
                       " -ORBsvcconf server.conf  -ORBdebuglevel $debug_level"
                       . " -o $iorfile");

if (ACE::waitforfile_timed ($iorfile, 30) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

$CL = Process::Create ($EXEPREFIX."$client_process$EXE_EXT ",
                       " -ORBsvcconf $client_conf "
                       . "-ORBdebuglevel $debug_level"
                       . " -k file://$iorfile "
                       . " -n $threads -i 1000 "
                       . " -x");

$client = $CL->TimedWait (240);
if ($client == -1) {
  print STDERR "ERROR: client timedout\n";
  $CL->Kill (); $CL->TimedWait (1);
  $status = 1;
}

$server = $SV->TimedWait (30);
if ($server == -1) {
  print STDERR "ERROR: server timedout\n";
  $SV->Kill (); $SV->TimedWait (1);
  $status = 1;
}

unlink $iorfile;

exit $status;
