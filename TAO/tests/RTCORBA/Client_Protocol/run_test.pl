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

print STDERR "\n********** RTCORBA Client Protocol Policy Unit Test\n\n";

unlink $iorfile1;
unlink $iorfile2;

# Arguments are platform-dependent (UIOP not available on Windows).
$server_args =
    "-s $iorfile1 -c $iorfile2 -p 1413566208 "
    ."-ORBendpoint iiop:// -ORBendpoint shmiop:// -ORBEndpoint uiop:// ";
$client_args =
    "-s file://$iorfile1 -c file://$iorfile2 -p 1413566210 -ORBdebuglevel 1 ";

if ($^O eq "MSWin32")
{
$server_args =
    "-s $iorfile1 -c $iorfile2 -p 1413566210 "
    ."-ORBendpoint iiop:// -ORBendpoint shmiop:// ";
$client_args =
    "-s file://$iorfile1 -c file://$iorfile2 -ORBdebuglevel 1";
}

# Start server.
$SV = Process::Create ($EXEPREFIX."server$EXE_EXT ",
                       $server_args);

if (ACE::waitforfile_timed ($iorfile2, 10) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile2>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

# Start client.
$CL = Process::Create ($EXEPREFIX."client$EXE_EXT ",
                       $client_args);

# Clean up.
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
