eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a Perl script that runs all Naming Service tests.  It starts
# all the servers and clients as necessary.

unshift @INC, '../../../../bin';
require Process;
require ACEutils;
require Uniqueid;

# Amount of delay (in seconds) between starting a server and a client
# to allow proper server initialization.
$sleeptime = 8;

# Variables for command-line arguments to client and server
# executables. 
$ns_multicast_port = 10000 + uniqueid ();  
$ns_orb_port = 12000 + uniqueid ();
$iorfile = "ns.ior";
$persistent_ior_file = "pns.ior";
$persistent_log_file = "test_log";
$data_file = "test_run.data";

sub name_server
{
  my $args = "@_ "."-ORBnameserviceport $ns_multicast_port -o $iorfile";
  my $prog = "..$DIR_SEPARATOR..$DIR_SEPARATOR".
      "Naming_Service".$DIR_SEPARATOR.
          "Naming_Service".$EXE_EXT;

  unlink $iorfile;
  $NS = Process::Create ($prog, $args);

  if (ACE::waitforfile_timed ($iorfile, $sleeptime) == -1) {
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

# Options for all simple tests recognized by the 'client' program.
@opts = ("-s -ORBnameserviceior file://$iorfile",
         "-s -ORBnameserviceport $ns_multicast_port",
         "-t -ORBnameserviceior file://$iorfile", 
         "-i -ORBnameserviceior file://$iorfile", 
         "-e -ORBnameserviceior file://$iorfile", 
         "-y -ORBnameserviceior file://$iorfile",
         "-p $persistent_ior_file -ORBnameserviceior file://$iorfile",
         "-c file://$persistent_ior_file -ORBnameserviceior file://$iorfile");

@server_opts = ("", "", "", "", "", "",
                "-ORBEndpoint iiop://localhost:$ns_orb_port -f $persistent_log_file",
                "-ORBEndpoint iiop://localhost:$ns_orb_port -f $persistent_log_file");

@comments = ("Simple Test: \n",
             "Simple Test (using multicast to locate the server): \n",
             "Tree Test: \n",
             "Iterator Test: \n",
             "Exceptions Test: \n",
             "Destroy Test: \n",
             "Persistent Test (Part 1): \n",
             "Persistent Test (Part 2): \n");

$test_number = 0;

unlink $persistent_ior_file, $persistent_log_file;

# Run server and client for each of the tests.  Client uses ior in a
# file to bootstrap to the server.
foreach $o (@opts)
{
  name_server ($server_opts[$test_number]);

  print STDERR "\n          ".$comments[$test_number];

  client ($o);

  $NS->Terminate (); $server = $NS->TimedWait (5);
  if ($server == -1) {
    print STDERR "ERROR: server timedout\n";
    $NS->Kill (); $NS->TimedWait (1);
  }
  $test_number++;
}

unlink $persistent_ior_file, $persistent_log_file;

# Now run the multithreaded test, sending output to the file.
print STDERR "\n          Multithreaded Test:\n";
unlink $data_file;

open (OLDOUT, ">&STDOUT");
open (STDOUT, ">$data_file") or die "can't redirect stdout: $!";
open (OLDERR, ">&STDERR");
open (STDERR, ">&STDOUT") or die "can't redirect stderror: $!";

name_server ();

client ("-ORBnameserviceior file://$iorfile", "-m25");

close (STDERR);
close (STDOUT);
open (STDOUT, ">&OLDOUT");
open (STDERR, ">&OLDERR");

$NS->Terminate (); $server = $NS->TimedWait (5);
if ($server == -1) {
  print STDERR "ERROR: server timedout\n";
  $NS->Kill (); $NS->TimedWait (1);
}

unlink $iorfile;

$FL = Process::Create ($EXEPREFIX."process-m-output.pl",
                       " test_run.data 25");
$filter = $FL->TimedWait (60);
if ($filter == -1) {
  print STDERR "ERROR: filter timedout\n";
  $FL->Kill (); $FL->TimedWait (1);
}
print STDERR "\n";

# @@ Capture any exit status from the processes.
exit 0;

