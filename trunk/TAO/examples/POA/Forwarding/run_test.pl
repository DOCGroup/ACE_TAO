eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../bin";
use ACEutils;

# Set STDERR to autoflush (weird unbufferd output stuff)
$| = 1;

# Make pretty look thing
print STDERR "\n";

# IOR files must be cleaned up before next run
sub cleanup_ior
{
    unlink "server1", "server2", "server3";
}

# Run the test in it's three forms
sub run_test
{
    # Run the servers
    $args = "-o server1";
    $SRV1 = Process::Create ($EXEPREFIX."server".$EXE_EXT,
                             $args);
    print STDERR ("server $args\n");

    if (ACE::waitforfile_timed ("server1", 15) == -1) 
    {
        print STDERR "ERROR: cannot find file <server1> or <server2>\n";
        $SRV1->Kill (); $SRV1->TimedWait (1);
        exit 1;
    }
    
    $args = "-o server2 -f file://server1";
    $SRV2 = Process::Create  ($EXEPREFIX."server".$EXE_EXT,
                              $args);
    print STDERR ("server $args\n");
    
    if (ACE::waitforfile_timed ("server2", 15) == -1) 
    {
        print STDERR "ERROR: cannot find file <server1> or <server2>\n";
        $SRV1->Kill (); $SRV1->TimedWait (1);
        $SRV2->Kill (); $SRV2->TimedWait (1);
        exit 1;
    }

    $args = "-o server3 -f file://server2";
    $SRV3 = Process::Create ($EXEPREFIX."server".$EXE_EXT,
                             $args);
    print STDERR ("server $args\n");
    
    if (ACE::waitforfile_timed ("server3", 15) == -1) 
    {
	print STDERR "ERROR: cannot find file <server3>\n";
	$SRV1->Kill (); $SRV1->TimedWait (1);
	$SRV2->Kill (); $SRV2->TimedWait (1);
	$SRV3->Kill (); $SRV3->TimedWait (1);
	exit 1;
    }
    
    # Run the client and block until completion
    $args = "-s 3 -k file://server3";
    $CL = Process::Create ($EXEPREFIX."client".$EXE_EXT,
			   $args);
    print STDERR ("client $args\n");
    
    $client = $CL->TimedWait (60);
    if ($client == -1) {
        print STDERR "ERROR: client timedout\n";
        $CL->Kill (); $CL->TimedWait (1);
    }
    
    # Now that the client has finished, kill off the servers
    if ($SRV1->TimedWait (10) == -1 ||
        $SRV2->TimedWait (10) == -1 ||
        $SRV3->TimedWait (10)) 
    {
        print STDERR "ERROR: couldn't terminate the servers nicely\n";
        $SRV1->Kill (); $SRV1->TimedWait (1);
        $SRV2->Kill (); $SRV2->TimedWait (1);
        $SRV3->Kill (); $SRV3->TimedWait (1);
        $status = 1;
    }
    
    if ($client != 0) 
    {
        print STDERR ("\nTest FAILED\n");
        $status = -1;
    } 
    else 
    {
        print STDERR ("\nTest SUCCEEDED\n");
    }
    return $status;
}

cleanup_ior ();
run_test ();
cleanup_ior ();

