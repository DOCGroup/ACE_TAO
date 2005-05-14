eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

#
# @@ Run this script from ../descriptors/ instead.
#

use lib "$ENV{'ACE_ROOT'}/bin";
use PerlACE::Run_Test;

## Source in the environment setting
$ACE_ROOT=$ENV{'ACE_ROOT'};
$CIAO_ROOT=$ENV{'CIAO_ROOT'};

if ($CIAO_ROOT eq "") {
    $CIAO_ROOT="$ACE_ROOT/TAO/CIAO";
}


$status = 0;
$assembly = PerlACE::LocalFile ("demo.cad");
$deploy_config = PerlACE::LocalFile ("test.dat");
#$deploy_config = PerlACE::LocalFile ("remote.dat");
$daemon_ior = PerlACE::LocalFile ("daemon.ior");
$am_ior = PerlACE::LocalFile ("am.ior");
$controller_ior = PerlACE::LocalFile ("controller.ior");
$cookie = PerlACE::LocalFile ("ck_demo_deployment");

$compserver="$CIAO_ROOT/tools/ComponentServer/ComponentServer";

## The following control how to iterate thru various work amount
$start_work = 10;
$end_work = 400;
$work_step = 10;
$run_time = 30;                 # run for $run_time sec.

unlink $daemon_ior;
unlink $am_ior;
unlink $cookie;

$test_deploy = 0;               # Do not run client/controller at all
$no_daemon = 0;                 # Do not start daemon process
$no_stop = 0;                   # just invoke the start() operation in
                                # client, no stop() invocation.

# Parse command line argument
while ( $#ARGV >= 0)
{
    if ($ARGV[0] =~ m/^-test_deploy/i) {
        $test_deploy = 1;
    }
    elsif ($ARGV[0] =~ m/^-no_daemon/i) {
        $no_daemon = 1;
    }
    elsif ($ARGV[0] =~ m/^-no_stop/i) {
        $no_stop = 1;
    }
    else {
        die "Invalid flag: $ARGV[0]\n";
    }

    shift;
}

# CIAO Daemon command line arguments
$daemon_args1 = "-ORBEndpoint iiop://localhost:10000 -o $daemon_ior -i CIAO_Installation_Data.ini -n $compserver";

$assembly_manager_args = "-o $am_ior -c $deploy_config";

$ad_deploy = " -k file://$am_ior -o $cookie -a $assembly";

$ad_teardown = " -k file://$am_ior -d $cookie -x";

# CIAO daemon process definition
$DS = new PerlACE::Process ("$CIAO_ROOT/tools/Daemon/CIAO_Daemon",
                            "$daemon_args1");

if ($no_daemon == 0) {
## Starting up the CIAO daemon
    $DS->Spawn ();
    if (PerlACE::waitforfile_timed ($daemon_ior, 15) == -1) {
        print STDERR "ERROR: Could not find daemon ior file <$daemon_ior>\n";
        $DS->Kill ();
        exit 1;
    }
}

## Starting up the CIAO Assembly Manager
$AM = new PerlACE::Process("$CIAO_ROOT/tools/Assembly_Deployer/Assembly_Manager",
                           $assembly_manager_args);
$AM->Spawn ();
if (PerlACE::waitforfile_timed ($am_ior, 15) == -1) {
    print STDERR "ERROR: Could not find assembly ior file <$am_ior>\n";
    $AM->Kill ();
    $DS->Kill ();
    exit 1;
}

## Now actually deploy the application
$AD = new PerlACE::Process("$CIAO_ROOT/tools/Assembly_Deployer/Assembly_Deployer",
                           $ad_deploy);
if ($AD->SpawnWaitKill (60) == -1) {
    print STDERR "ERROR: Failed to deploy assembly file <$assembly>\n";
    $AD->Kill ();
    $AM->Kill ();
    $DS->Kill ();
}

## Make sure the application is up and running
if (PerlACE::waitforfile_timed ($controller_ior, 15) == -1) {
    print STDERR "ERROR: Could not find controller ior file <$controller_ior>\n";
    $AM->Kill ();
    $DS->Kill ();
    exit 1;
}

# Don't start the test
if ($test_deploy == 0) {
    for ($work = $start_work; $work < $end_work; $work += $work_step)
    {
        printf "Test work: $work\n";

#Start the client to send the trigger message
        $CL = new PerlACE::Process ("../Controllers/client",
                                    "-k file://$controller_ior -w $work");
        $CL->SpawnWaitKill(60);

        if ($no_stop == 0) {
## Now wait for the test to complete.  Need to figure out a way to
## detect this.
            sleep ($run_time);

#Start the client to send the trigger message
            $CL = new PerlACE::Process ("../Controllers/client",
                                        "-k file://$controller_ior -f");
            $CL->SpawnWaitKill(60);
        }
    }
}
else {
    print "Test deploying the application -=-=- Not invoking client\n";
    sleep (1);
}

## Now teardown the application
$AD = new PerlACE::Process("$CIAO_ROOT/tools/Assembly_Deployer/Assembly_Deployer",
                           $ad_teardown);
if ($AD->SpawnWaitKill (60) == -1) {
    print STDERR "ERROR: Failed to teardown assembly file <$assembly>\n";
    $AD->Kill ();
    $AM->Kill ();
    $DS->Kill ();
    exit 1;
}

#$ctrl = $DS->WaitKill (5);
#$AM->WaitKill(5);
#$AD->WaitKill(5);

$AM->WaitKill (2);
$DS->Kill ();


unlink $cookie;
unlink $controller_ior;
unlink $daemon_ior;
unlink $am_ior;

exit $status;
