eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a Perl script that runs a Naming Service test.  It starts
# all the servers and clients as necessary.

use lib '../../../../bin';
use PerlACE::Run_Test;

# Amount of delay (in seconds) between starting a server and a client
# to allow proper server initialization.
$sleeptime = 10;

$quiet = 0;

# check for -q flag
if ($ARGV[0] eq '-q') {
    $quiet = 1;
}

# Variables for command-line arguments to client and server
# executables.
$ns_orb_port1 = 10001 + PerlACE::uniqueid ();
$ns_orb_port2 = 10002 + PerlACE::uniqueid ();
$ns_endpoint1 = "iiop://localhost:$ns_orb_port1";
$ns_endpoint2 = "iiop://localhost:$ns_orb_port2";
$iorfile1 = PerlACE::LocalFile ("ns1.ior");
$iorfile2 = PerlACE::LocalFile ("ns2.ior");

$status = 0;

# Make sure that the directory to use to hold the naming contexts exists
# and is cleaned out
if ( ! -d "NameService" ) {
  mkdir (NameService, 0777);
  }
else {
  chdir "NameService";
  opendir(THISDIR, ".");
  @allfiles = grep(!/^\.\.?$/, readdir(THISDIR));
  closedir(THISDIR);
  unlink @allfiles;
  chdir "..";
  }
  
# Run two Naming Servers in redundant mode and one client.  Client uses iors
# in files to find the individual copies of the Naming Servers.

my $args = "-ORBEndPoint $ns_endpoint1 -o $iorfile1 -m 0 -r NameService";
my $prog = "../../Naming_Service/Naming_Service";
$NS1 = new PerlACE::Process ($prog, $args);

unlink $iorfile1;

$NS1->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile1, $sleeptime) == -1) {
  print STDERR "ERROR: cannot find IOR file <$iorfile>\n";
  $NS1->Kill (); 
  exit 1;
}

my $args = "-ORBEndPoint $ns_endpoint2 -o $iorfile2 -m 0 -r NameService";
my $prog = "../../Naming_Service/Naming_Service";
$NS2 = new PerlACE::Process ($prog, $args);

unlink $iorfile2;

$NS2->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile2, $sleeptime) == -1) {
  print STDERR "ERROR: cannot find IOR file <$iorfile>\n";
  $NS2->Kill (); 
  exit 1;
}

my $args = "-p file://$iorfile1 -q file://$iorfile2";
my $prog = "client";

$CL = new PerlACE::Process ($prog, $args);

$client = $CL->SpawnWaitKill (12);

if ($client != 0) {
  print STDERR "ERROR: client returned $client\n";
  $status = 1;
}

$NS1->Kill ();
$NS2->Kill ();

unlink $iorfile1;
unlink $iorfile2;

exit $status;
