eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a Perl script that runs additional Naming Service tests.  
# It runs all the tests that will *not* run with min CORBA.
# It starts all the servers and clients as necessary.

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
$ns_multicast_port = 10001 + PerlACE::uniqueid (); # Can not be 10000 on Chorus 4.0
$ns_orb_port = 12000 + PerlACE::uniqueid ();
$iorfile = PerlACE::LocalFile ("ns.ior");
$file_persistent_ior_file = PerlACE::LocalFile ("fpns.ior");

$status = 0;

sub name_server
{
    my $args = "-ORBNameServicePort $ns_multicast_port -o $iorfile -m 1 @_";
    my $prog = "../../Naming_Service/Naming_Service";
    $NS = new PerlACE::Process ($prog, $args);

    unlink $iorfile;

    $NS->Spawn ();

    if (PerlACE::waitforfile_timed ($iorfile, $sleeptime) == -1) {
        print STDERR "ERROR: cannot find IOR file <$iorfile>\n";
        $NS->Kill (); 
        exit 1;
    }
}

sub client
{
    my $args = "@_"." ";
    my $prog = "client";

    $CL = new PerlACE::Process ($prog, $args);

    $client = $CL->SpawnWaitKill (60);

    if ($client != 0) {
        print STDERR "ERROR: client returned $client\n";
        $status = 1;
    }
}

## The options below have been reordered due to a
## initialization problem (within the Naming_Service)
## that has only been seen on Windows XP.

# Options for all simple tests recognized by the 'client' program.
@opts = ("-p $file_persistent_ior_file -ORBInitRef NameService=file://$iorfile",
         "-c file://$file_persistent_ior_file -ORBInitRef NameService=file://$iorfile");

@server_opts = ("-ORBEndpoint iiop://$TARGETHOSTNAME:$ns_orb_port -u NameService",
                "-ORBEndpoint iiop://$TARGETHOSTNAME:$ns_orb_port -u NameService");

@comments = ("Flat File Persistent Test (Part 1): \n",
             "Flat File Persistent Test (Part 2): \n");

$test_number = 0;

unlink ($file_persistent_ior_file);

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
  

# Run server and client for each of the tests.  Client uses ior in a
# file to bootstrap to the server.
foreach $o (@opts) {
    name_server ($server_opts[$test_number]);

    print STDERR "\n          ".$comments[$test_number];

    client ($o);

    $NS->Kill ();

    ## For some reason, only on Windows XP, we need to
    ## wait before starting another Naming_Service when
    ## the mmap persistence option is used
    if ($^O eq "MSWin32") {
      sleep(1);
    }

    $test_number++;
}

unlink ($file_persistent_ior_file);

chdir "NameService";
opendir(THISDIR, ".");
@allfiles = grep(!/^\.\.?$/, readdir(THISDIR));
closedir(THISDIR);
unlink @allfiles;
chdir "..";
rmdir "NameService";

unlink $iorfile;

exit $status;
