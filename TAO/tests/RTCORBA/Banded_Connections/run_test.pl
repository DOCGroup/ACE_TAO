eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../../bin';
require ACEutils;
use Cwd;

$cwd = getcwd();
$iorfile1 = "$cwd$DIR_SEPARATOR" . "test1.ior";
$iorfile2 = "$cwd$DIR_SEPARATOR" . "test2.ior";

ACE::checkForTarget($cwd);

print STDERR "\n********** RTCORBA Priority Banded Connections Unit Test\n";

unlink $iorfile1;
unlink $iorfile2;

# CORBA priorities 66, 75 and 80, etc. are for the SCHED_OTHER class on
# Solaris.  May need to use different values for other platforms
# depending on their native priorities scheme, i.e., based on the
# available range.

$server_args =
    "-n $iorfile1 -o $iorfile2 -b bands.unix -ORBSvcConf server.conf "
    ."-ORBdebuglevel 1 -p 67 -w 78 "
    ."-ORBendpoint iiop://$TARGETHOSTNAME:0/priority=66 "
    ."-ORBendpoint iiop://$TARGETHOSTNAME:0/priority=75 "
    ."-ORBendpoint iiop://$TARGETHOSTNAME:0/priority=80 ";

$client_args =
    "-n file://$iorfile1 -o file://$iorfile2 "
    ."-a 76 -b 80 -c 64";

if ($^O eq "MSWin32")
{
    $server_args =
        "-n $iorfile1 -o $iorfile2 -b bands.nt -ORBSvcConf server.conf "
            ."-ORBdebuglevel 1 -p 1 -w 3 "
            ."-ORBendpoint iiop://$TARGETHOSTNAME:0/priority=2 "
                ."-ORBendpoint iiop://$TARGETHOSTNAME:0/priority=5 "
                    ."-ORBendpoint iiop://$TARGETHOSTNAME:0/priority=6 ";

    $client_args =
        "-n file://$iorfile1 -o file://$iorfile2 "
            ."-a 4 -b 6 -c 3";
}

$SV = Process::Create ($EXEPREFIX."server$EXE_EXT ",
                       $server_args);

if (ACE::waitforfile_timed ($iorfile2, 10) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile2>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

$CL = Process::Create ($EXEPREFIX."client$EXE_EXT ",
                       $client_args);

$client = $CL->TimedWait (60);
if ($client == -1) {
  print STDERR "ERROR: client timedout\n";
  $CL->Kill (); $CL->TimedWait (1);
}

$server = $SV->TimedWait (60);
if ($server == -1) {
  print STDERR "ERROR: server timedout\n";
  $SV->Kill (); $SV->TimedWait (1);
}

unlink $iorfile1;
unlink $iorfile2;

if ($server != 0 || $client != 0) {
  exit 1;
}

exit 0;
