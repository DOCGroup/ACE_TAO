eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../../../bin';
require ACEutils;

$client_conf="client.conf";
$server_conf="server.conf";
$iorfile = "test.ior";

print STDERR "================ ORB Per Priority Test\n";

unlink $iorfile."_1";
unlink $iorfile."_2";
unlink $iorfile."_3";
unlink $iorfile."_4";
unlink $iorfile."_5";
unlink $iorfile."_30";

$SV = Process::Create ($EXEPREFIX."server$EXE_EXT ",
                       " -ORBSvcConf server.conf"
                       . " -t 1 -t 2 -t 3 -t 4 -t 5 -t 30 "
                       . " -o $iorfile");

if (ACE::waitforfile_timed ($iorfile."_1", 5) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

$CL = Process::Create ($EXEPREFIX."client$EXE_EXT ",
                       " -ORBSvcConf client.conf "
                       . " -i file://$iorfile "
                       . " -t 1 -t 2 -t 3 -t 4 -t 5 -t 30 -n 400000");

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

unlink $iorfile."_1";
unlink $iorfile."_2";
unlink $iorfile."_3";
unlink $iorfile."_4";
unlink $iorfile."_5";
unlink $iorfile."_30";


if ($server != 0 || $client != 0) {
  exit 1;
}

exit 0;
