# $Id$
# -*- perl -*-
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use lib "../../../../bin";
require ACEutils;

$brace="\#\#\#\#\#";

# There are three ways to run the test with different results, requiring
# different numbers of servers and with variable arguments, hence the
# arrays

# Servers 1 and 2 are used in all tests
@server1arglist = ("-o server1 -ORBport 10040 -ORBobjrefstyle url",
		   "-p server1  -ORBport 10040 -ORBobjrefstyle url",
		   "-o server1 -ORBport 10040 -ORBobjrefstyle url");
@server2arglist = ("-f server1 -o server2 -ORBport 10041 -ORBobjrefstyle url",
		   "-g server1 -p server2 -ORBport 10041 -ORBobjrefstyle url",
		   "-g server1 -p server2 -ORBport 10041 -ORBobjrefstyle url");

# The third server is only needed for the last test, but I'm making it
# a list in case anyone wants to run it in multiple tests, rather than
# just one
@server3arglist = ("",
		   "",
		   "-f server2 -o server3 -ORBport 10043 -ORBobjrefstyle url");

# The client is run for all tests
@clientarglist = ("-f server2 -i 5",
		  "-g server2 -i 5",
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

    # Run the servers
    $SRV1 = Process::Create  (".".$DIR_SEPARATOR."server".$Process::EXE_EXT,
			     "$server1args");
    print STDERR ("server $server1args\n");
    ACE::waitforfile ("server1");

    $SRV2 = Process::Create  (".".$DIR_SEPARATOR."server".$Process::EXE_EXT,
			     "$server2args");
    print STDERR ("server $server2args\n");
    ACE::waitforfile ("server2");

    if ($server3args ne "")
    {
	$SRV3 = Process::Create (".".$DIR_SEPARATOR."server".$Process::EXE_EXT,
				 "$server3args");
        ACE::waitforfile ("server3");
    }

    # Run the client and block until completion
    $status  = system ("client$Process::EXE_EXT $clientargs");
    print STDERR ("client $clientargs");

    # Now that the client has finished, kill off the servers

    $SRV1->Kill (); $SRV1->Wait ();
    $SRV2->Kill (); $SRV2->Wait ();

    if ($server3args ne "")
    {
	$SRV3->Kill (); $SRV3->Wait ();
    }

    if ($status != 0)
    {
	print STDERR ("\n$brace Test of $testtype FAILED\n");
	$retval = -1;
    }
    else
    {
	print STDERR ("\n$brace Test of $testtype SUCCEEDED\n");
    }
    cleanup_ior ();
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
