eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../../bin';
require ACEutils;
use Cwd;

$cwd = getcwd();

# Test parameters.
$iorfile = "$cwd$DIR_SEPARATOR" . "test.ior";
$data_file = "$cwd$DIR_SEPARATOR" . "test_run.data";
$iterations = 50;
$priority1 = 65;
$priority2 = 70;
$priority3 = 75;
if ($^O eq "MSWin32")
{
    $priority1 = 6;
    $priority2 = 1;
    $priority3 = 5;
}

ACE::checkForTarget($cwd);

# Clean up leftovers from previous runs.
unlink $iorfile;
unlink $data_file;


$server_args =
    "-o $iorfile -ORBdebuglevel 1 -ORBsvcconf server.conf "
    ."-ORBendpoint iiop://$TARGETHOSTNAME:0/priority=$priority1 "
    ."-ORBendpoint iiop://$TARGETHOSTNAME:0/priority=$priority2 "
    ."-ORBendpoint iiop://$TARGETHOSTNAME:0/priority=$priority3 "
    ."-ORBendpoint shmiop://$TARGETHOSTNAME:0/priority=$priority1 "
    ."-ORBendpoint shmiop://$TARGETHOSTNAME:0/priority=$priority2 "
    ."-ORBendpoint shmiop://$TARGETHOSTNAME:0/priority=$priority3 ";

$client_args =
    "-o file://$iorfile  "
    ."-a $priority1 -b $priority2 -e 1413566210 -f 0 -n $iterations";


print STDERR "\n********** MT Client Protocol & CLIENT_PROPAGATED combo Test\n\n";

# Redirect the output of the test run to a file, so that we can process it later.
open (OLDOUT, ">&STDOUT");
open (STDOUT, ">$data_file") or die "can't redirect stdout: $!";
open (OLDERR, ">&STDERR");
open (STDERR, ">&STDOUT") or die "can't redirect stderror: $!";

# Run server and client.
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

close (STDERR);
close (STDOUT);
open (STDOUT, ">&OLDOUT");
open (STDERR, ">&OLDERR");

unlink $iorfile;

if ($server != 0 || $client != 0) {
  exit 1;
}

# Run a processing script on the test output.
$FL = Process::Create ($EXEPREFIX."process-output.pl",
                       " $data_file $iterations $priority1 $priority2");
$filter = $FL->TimedWait (60);
if ($filter == -1) {
  print STDERR "ERROR: filter timedout\n";
  $FL->Kill (); $FL->TimedWait (1);
}
  print STDERR "\n";

exit 0;
