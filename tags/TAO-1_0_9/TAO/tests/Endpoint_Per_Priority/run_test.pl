eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../bin';
require ACEutils;

$client_conf="client.conf";
$server_conf="server.conf";
$threads='2';

$iorfile = "test.ior";

print STDERR "================ Multi-threaded test\n";

unlink $iorfile;

$SV = Process::Create ($EXEPREFIX."server$EXE_EXT ",
                       " -ORBSvcConf server.conf"
		       . " -ORBEndPoint iiop://localhost:0/priority=0 "
		       . " -ORBEndPoint iiop://localhost:0/priority=1 "
		       . " -ORBEndPoint iiop://localhost:0/priority=2 "
                       . " -ORBEndPoint iiop://localhost:0/priority=3 "
                       . " -ORBEndPoint iiop://localhost:0/priority=4 "
                       . " -o $iorfile -n $threads");

if (ACE::waitforfile_timed ($iorfile, 5) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

$CL = Process::Create ($EXEPREFIX."client$EXE_EXT ",
                       " -ORBSvcConf client.conf "
                       . " -k file://$iorfile "
                       . " -t 0 -t 1 -i 1000");

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

if ($server != 0 || $client != 0) {
  exit 1;
}

exit 0;
