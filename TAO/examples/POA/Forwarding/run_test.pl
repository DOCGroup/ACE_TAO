eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../bin";
require ACEutils;

$brace="\#\#\#\#\#";

# There are three ways to run the test with different results, requiring
# different numbers of servers and with variable arguments, hence the
# arrays

# Servers 1 and 2 are used in all tests
@server1arglist = ("-o server1",
                   "-p server1",
                   "-o server1");
@server2arglist = ("-f server1 -o server2",
                   "-g server1 -p server2",
                   "-g server1 -p server2");

# The third server is only needed for the last test, but I'm making it
# a list in case anyone wants to run it in multiple tests, rather than
# just one
@server3arglist = ("",
                   "",
                   "-f server2 -o server3");

# The client is run for all tests
@clientarglist = ("-f server2 -i 5",
                  "-f server2 -i 5",
                  "-f server3 -i 8");

# Descriptions of the tests
@testlist = ("POA approach",
             "servant locator approach",
             "dual POA and servant locator approaches");

$testtype = "";

# The strings to hold the arguments for the client and servers
$server1args = "";
$server2args = "";
$server3args = "";
$clientargs = "";

# Set STDERR to autoflush (weird unbufferd output stuff)
$| = 1;

# Make pretty look thing
print STDERR "\n";

# There's a cleaner way to do this, but I don't know what it is
sub argshift
{
    $server1args = shift (@server1arglist);
    $server2args = shift (@server2arglist);
    $server3args = shift (@server3arglist);
    $clientargs = shift (@clientarglist);
    $testtype = shift (@testlist);
}

# IOR files must be cleaned up before next run
sub cleanup_ior
{
    unlink "server1", "server2", "server3";
}

# Run the test in it's three forms
sub run_test
{
    print STDERR "\n$brace Test of $testtype BEGUN\n";

    # Remove stale IOR files...
    cleanup_ior ();

    # Run the servers
    $SRV1 = Process::Create  (".".$DIR_SEPARATOR."server".$Process::EXE_EXT,
                             "$server1args");
    print STDERR ("server $server1args\n");

    if (ACE::waitforfile_timed ("server1", 5) == -1) {
      print STDERR "ERROR: cannot find file <server1> or <server2>\n";
      $SRV1->Kill (); $SRV1->TimedWait (1);
      exit 1;
    }

    $SRV2 = Process::Create  (".".$DIR_SEPARATOR."server".$Process::EXE_EXT,
                             "$server2args");
    print STDERR ("server $server2args\n");

    if (ACE::waitforfile_timed ("server2", 5) == -1) {
      print STDERR "ERROR: cannot find file <server1> or <server2>\n";
      $SRV1->Kill (); $SRV1->TimedWait (1);
      $SRV2->Kill (); $SRV2->TimedWait (1);
      exit 1;
    }

    if ($server3args ne "") {
      $SRV3 = Process::Create (".".$DIR_SEPARATOR."server".$Process::EXE_EXT,
			       "$server3args");

      if (ACE::waitforfile_timed ("server3", 5) == -1) {
	print STDERR "ERROR: cannot find file <server3>\n";
	$SRV1->Kill (); $SRV1->TimedWait (1);
	$SRV2->Kill (); $SRV2->TimedWait (1);
	$SRV3->Kill (); $SRV3->TimedWait (1);
	exit 1;
      }
    }

    # Run the client and block until completion
    $CL = Process::Create ($EXEPREFIX."client".$Process::EXE_EXT,
			   " $clientargs");
    print STDERR ("client $clientargs\n");

    $client = $CL->TimedWait (60);
    if ($client == -1) {
      print STDERR "ERROR: client timedout\n";
      $CL->Kill (); $CL->TimedWait (1);
    }

    # Now that the client has finished, kill off the servers

    $SRV1->Kill (); $SRV1->TimedWait (1);
    $SRV2->Kill (); $SRV2->TimedWait (1);

    if ($server3args ne "")
    {
        $SRV3->Kill (); $SRV3->TimedWait (1);
    }

    if ($client != 0) {
      print STDERR ("\n$brace Test of $testtype FAILED\n");
      $status = -1;
    } else {
      print STDERR ("\n$brace Test of $testtype SUCCEEDED\n");
    }
    return $status;
}


for ($q = 0; $q < 3; $q++)
{
    argshift ();
    run_test ();
    if ($q < 2)
    {
        print STDERR ("\n\t\t Waiting for things to stabilize...\n");
        sleep ($ACE::sleeptime);
    }
}

cleanup_ior ();
