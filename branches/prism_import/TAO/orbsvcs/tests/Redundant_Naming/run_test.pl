eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a Perl script that runs a Naming Service test.  It starts
# all the servers and clients as necessary.

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use Cwd;

$startdir = getcwd();

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

## Allow the user to determine where the persistent file will be located
## just in case the current directory is not suitable for locking.
## We can't change the name of the persistent file because that is not
## sufficient to work around locking problems for Tru64 when the current
## directory is NFS mounted from a system that does not properly support
## locking.
foreach my $possible ($ENV{TMPDIR}, $ENV{TEMP}, $ENV{TMP}) {
  if (defined $possible && -d $possible) {
    if (chdir($possible)) {
      last;
    }
  }
}

print "INFO: Running the test in ", getcwd(), "\n";

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
my $prog = "$startdir/../../Naming_Service/Naming_Service";
$NS1 = new PerlACE::Process ($prog, $args);

unlink $iorfile1;

$NS1->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile1, $sleeptime) == -1) {
  print STDERR "ERROR: cannot find IOR file <$iorfile1>\n";
  $NS1->Kill ();
  exit 1;
}

my $args = "-ORBEndPoint $ns_endpoint2 -o $iorfile2 -m 0 -r NameService";
my $prog = "$startdir/../../Naming_Service/Naming_Service";
$NS2 = new PerlACE::Process ($prog, $args);

unlink $iorfile2;

$NS2->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile2, $sleeptime) == -1) {
  print STDERR "ERROR: cannot find IOR file <$iorfile>\n";
  $NS2->Kill ();
  exit 1;
}

## Even though the ior file is present, the redundant naming service
## isn't really ready to go (most of the time).  Sleeping 1 second
## allows the redundant naming service to get to a usable state.
sleep(1);

my $args = "-p file://$iorfile1 -q file://$iorfile2";
my $prog = "$startdir/client";

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
