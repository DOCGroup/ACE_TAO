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

print STDERR "\n********** RTCORBA SERVER_DECLARED Priority Unit Test\n\n";

unlink $iorfile1;
unlink $iorfile2;

# CORBA priorities 65, 70 and 75 are for the SCHED_OTHER class on
# Solaris.  May need to use different values for other platforms
# depending on their native priorities scheme, i.e., based on the
# available range.

$server_args =
    "-p $iorfile1 -o $iorfile2 -a 65 -b 75 -c 70 -ORBDebugLevel 0 -ORBSvcConf server.conf "
    ."-ORBendpoint iiop://$TARGETHOSTNAME:0/priority=65 "
    ."-ORBendpoint iiop://$TARGETHOSTNAME:0/priority=75 "
    ."-ORBendpoint iiop://$TARGETHOSTNAME:0/priority=73 ";

if ($^O eq "MSWin32")
{
    $server_args =
        "-p $iorfile1 -o $iorfile2 -a 3 -b 5 -c 2 -ORBSvcConf server.conf "
            ."-ORBendpoint iiop://$TARGETHOSTNAME:0/priority=3 "
                ."-ORBendpoint iiop://$TARGETHOSTNAME:0/priority=5 "
                    ."-ORBendpoint iiop://$TARGETHOSTNAME:0/priority=1 ";
}

$client_args = "-p file://$iorfile1 -o file://$iorfile2 -ORBDebugLevel 10";

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
