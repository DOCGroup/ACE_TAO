eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../../bin';
require ACEutils;
use Cwd;

$cwd = getcwd();
$iorfile = "$cwd$DIR_SEPARATOR" . "test.ior";

ACE::checkForTarget($cwd);

print STDERR "\n********** MT Client Protocol & CLIENT_PROPAGATED combo Test\n\n";

unlink $iorfile;

# CORBA priorities 65, 70 and 75 are for the SCHED_OTHER class on
# Solaris.  May need to use different values for other platforms
# depending on their native priorities scheme, i.e., based on the
# available range.

$server_args =
    "-o $iorfile "
    ."-ORBendpoint iiop://$TARGETHOSTNAME:0/priority=65 "
    ."-ORBendpoint iiop://$TARGETHOSTNAME:0/priority=75 "
    ."-ORBendpoint iiop://$TARGETHOSTNAME:0/priority=70 "
    ."-ORBendpoint shmiop://$TARGETHOSTNAME:0/priority=65 "
    ."-ORBendpoint shmiop://$TARGETHOSTNAME:0/priority=75 "
    ."-ORBendpoint shmiop://$TARGETHOSTNAME:0/priority=70 ";

$client_args =
    "-o file://$iorfile -ORBdebuglevel 1 "
    ."-a 65 -b 70 -e 1413566210 -f 0";

if ($^O eq "MSWin32")
{
    $server_args =
        "-o $iorfile1 -o $iorfile2 -a 3 -b 5 -c 2 -ORBSvcConf server.conf "
            ."-ORBendpoint iiop://$TARGETHOSTNAME:0/priority=3 "
                ."-ORBendpoint iiop://$TARGETHOSTNAME:0/priority=5 "
                    ."-ORBendpoint iiop://$TARGETHOSTNAME:0/priority=1 ";
}

$SV = Process::Create ($EXEPREFIX."server$EXE_EXT ",
                       $server_args);

if (ACE::waitforfile_timed ($iorfile, 10) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile>\n";
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

unlink $iorfile;

if ($server != 0 || $client != 0) {
  exit 1;
}

exit 0;
