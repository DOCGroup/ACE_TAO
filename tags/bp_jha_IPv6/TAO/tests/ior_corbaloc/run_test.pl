eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a Perl Script that runs the corbaloc: style URL tests.
# It starts the servers and client.

#use lib "../../../bin";

unshift @INC, '../../../bin';
require Process;
require ACEutils;
require Uniqueid;
use Cwd;

$cwd = getcwd();
# Amount of delay (in seconds) between starting a server and a client
# to allow proper server initialization.
$sleeptime = 8;

# Variables for command-line arguments to client and server
# executables.
$ns_multicast_port = 10001 + uniqueid (); # Can not be 10000 on Chorus 4.0
$ns_orb_port = 12000 + uniqueid ();
$iorfile = "$cwd$DIR_SEPARATOR" . "ns.ior";

ACE::checkForTarget($cwd);

sub name_server
{
  my $args = "@_"." -ORBnameserviceport $ns_multicast_port -o $iorfile";
  my $prog = $EXEPREFIX."..$DIR_SEPARATOR..$DIR_SEPARATOR..$DIR_SEPARATOR".
      "TAO".$DIR_SEPARATOR."orbsvcs".$DIR_SEPARATOR.
	"Naming_Service".$DIR_SEPARATOR.
          "Naming_Service".$EXE_EXT;

  # Make sure the files are gone, so we can wait on them.
  unlink $iorfile;

  $NS = Process::Create ($prog, $args);

  if (ACE::waitforfile_timed ($iorfile, $sleeptime) == -1) {
    print STDERR "ERROR: cannot find IOR file <$iorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
  }
}

sub server 
{
  my $args = "@_"." -ORBInitRef NameService=file://$iorfile";
  my $prog = $EXEPREFIX."server".$EXE_EXT;

  $SR = Process::Create ($prog, $args);

  $server = $SR->TimedWait (20);
}

sub client
{
  my $args = "@_"." -ORBInitRef NameService=file://$iorfile";
  my $prog = $EXEPREFIX."client".$EXE_EXT;

  $CL = Process::Create ($prog, $args);

  $client = $CL->TimedWait (60);
  if ($client == -1) {
      print STDERR "ERROR: client timedout\n";
      $CL->Kill (); $CL->TimedWait (1);
    }
}

# Options for all simple tests recognized by the 'client' program.
@opts = ("corbaloc:rir:/NameService",
	 "corbaloc:rir:/",
	 "corbaloc:iiop://$TARGETHOSTNAME:$ns_orb_port/NameService",
	 "corbaloc:iiop://:$ns_orb_port/NameService",
	 "corbaloc:///NameService",
	 "corbaloc://$TARGETHOSTNAME:$ns_orb_port/NameService");


$def_port = 2809;

@name_server_opts = ("-ORBEndPoint iiop://$TARGETHOSTNAME:$ns_orb_port",
		     "-ORBEndPoint iiop://$TARGETHOSTNAME:$ns_orb_port",
		     "-ORBEndPoint iiop://$TARGETHOSTNAME:$ns_orb_port",
		     "-ORBEndPoint iiop://$TARGETHOSTNAME:$ns_orb_port",
		     "-ORBEndPoint iiop://$TARGETHOSTNAME:$def_port",
		     "-ORBEndPoint iiop://$TARGETHOSTNAME:$ns_orb_port");


@comments = ("Using RIR protocol with the Key object specified: \n",
	     "Using RIR protocol without specifying the key object: \n",
	     "Using IIOP protocol with the hostname, port specified: \n",
	     "Using IIOP protocol with the hostname unspecified but port specified: \n",
	     "Using IIOP protocol with both hostname & port unspecified: \n",
	     "Using the default protocol (iiop) with the hostname, port specified: \n");

$test_number = 0;

# Run the naming service, server. Run the client for each of the test. 
# Client uses ior in a file to bootstrap to the server.

foreach $o (@opts)
{  
  name_server ($name_server_opts [$test_number]);

  print STDERR "\n          ".$comments[$test_number];

  server ();

  client ($o);

  $NS->Terminate (); 
  $nser = $NS->TimedWait (60);

  if ($nser == -1) 
    {
      print STDERR "ERROR: name server Timedout\n";
      $NS->Kill (); $NS->TimedWait (1);
    }

  $SR->Terminate ();
  $ser = $NS->TimedWait (60);

  if ($ser == -1) 
    {
      $NS->Kill (); $NS->TimedWait (1);
    }

  $test_number++;

}

exit 0;
