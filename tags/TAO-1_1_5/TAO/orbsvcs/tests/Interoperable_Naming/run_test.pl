eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a perl script that runs the NamingContextExt test. It starts 
# the Naming service, server and the client as necessary

unshift @INC, '../../../../bin';
require Process;
require ACEutils;
use Cwd;

$cwd = getcwd ();

# Amount of delay (in seconds) between starting a server and a client 
# to allow proper server initialization.
$sleeptime = 8;

# Variables for command-line arguments to client and server
# executables
$iorfile = "$cwd$DIR_SEPARATOR" . "ns.ior";

ACE::checkForTarget($cwd);

sub name_server
  {
    my $args = "@_"." -o $iorfile";
    my $prog = $EXEPREFIX."..$DIR_SEPARATOR..$DIR_SEPARATOR..$DIR_SEPARATOR".
      "orbsvcs".$DIR_SEPARATOR.
	"Naming_Service".$DIR_SEPARATOR.
	  "Naming_Service".$EXE_EXT;

    # Make sure the files are gone, so we can wait on them.
    unlink $iorfile;

    $NS = Process::Create ($prog, $args);

    if (ACE::waitforfile_timed ($iorfile, $sleeptime) == -1) 
      {
	print STDERR "ERROR: cannot find IOR file <$iorfile>\n";
	$NS->Kill (); $NS->TimedWait (1);
	exit 1;
      }
  }

sub client
  {
    my $args = "@_"." ";
    my $prog = $EXEPREFIX."client".$EXE_EXT;
    
    $CL = Process::Create ($prog, $args);
    
    $client = $CL->TimedWait (60);
    if ($client == -1) {
      print STDERR "ERROR: client timedout\n";
      $CL->Kill (); $CL->TimedWait (1);
    }
  }

# Options for all the tests recognized by the 'client' program
$opts = "-s -ORBInitRef NameService=file://$iorfile";  

# Run the server and client for the test. 
#
name_server ();
client ($opts);

$NS->Terminate (); $server = $NS->TimedWait (5);
if ($server == -1) 
  {
    print STDERR "ERROR: server timedout\n";
    $NS->Kill (); $NS->TimedWait (1);
  }

exit 0;
