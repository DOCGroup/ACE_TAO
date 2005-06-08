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
$local = 1;
$assembly = PerlACE::LocalFile ("no-rt-3rates.cad");
$daemon_ior = PerlACE::LocalFile ("daemon.ior");
$am_ior = PerlACE::LocalFile ("am.ior");
$c25_ior = PerlACE::LocalFile ("c25.ior");
$c50_ior = PerlACE::LocalFile ("c50.ior");
$c75_ior = PerlACE::LocalFile ("c75.ior");
$cookie = PerlACE::LocalFile ("ck_demo_deployment");

## The following control how to iterate thru various work amount
$start_work = 80;
$end_work = 401;
$work_step = 10;
$run_time = 60;                 # run for $run_time sec.

## Controlling test behavior
$test_deploy = 0;
$no_daemon = 0;
$reverse_call_order = 0;

# Parse command line argument
while ( $#ARGV >= 0)
{
    if ($ARGV[0] =~ m/^-test_deploy/i) {
        $test_deploy = 1;
    }
    elsif ($ARGV[0] =~ m/^-remote/i) {
        $local = 0;
    }
    elsif ($ARGV[0] =~ m/^-assembly/i) {
        shift;
        if ($#ARGV >= 0) {
            $assembly = PerlACE::LocalFile ("$ARGV[0]");
        }
    }
    elsif ($ARGV[0] =~ m/^-work_step/i) {
        shift;
        if ($#ARGV >= 0) {
            $work_step = $ARGV[0];
        }
    }
    elsif ($ARGV[0] =~ m/^-reverse_call_order/i) {
        $reverse_call_order = 1;
    }
    else {
        die "Invalid flag: $ARGV[0]\n";
    }

    shift;
}

if ($local == 1) {
    $deploy_config = PerlACE::LocalFile ("test.dat");
}
else {
    $deploy_config = PerlACE::LocalFile ("remote.dat");
}

# CIAO Daemon command line arguments
$daemon_args1 = "-ORBEndpoint iiop://localhost:10000 -o $daemon_ior -i CIAO_Installation_Data.ini -n $CIAO_ROOT/tools/ComponentServer/ComponentServer";

$assembly_manager_args = "-o $am_ior -c $deploy_config";

$ad_deploy = " -k file://$am_ior -o $cookie -a $assembly";

$ad_teardown = " -k file://$am_ior -d $cookie -x";

# CIAO daemon process definition
$DS = new PerlACE::Process ("$CIAO_ROOT/tools/Daemon/CIAO_Daemon",
                            "$daemon_args1");

if ($local == 1) {
## Starting up the CIAO daemon
    $DS->Spawn ();
    if (PerlACE::waitforfile_timed ($daemon_ior, 15) == -1) {
        print STDERR "ERROR: Could not find daemon ior file <$daemon_ior>\n";
        $DS->Kill ();
        exit 1;
    }
}

for ($work = $start_work; $work < $end_work; $work += $work_step)
{

    unlink $daemon_ior;
    unlink $am_ior;
    unlink $cookie;

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
    if (PerlACE::waitforfile_timed ($c75_ior, 15) == -1) {
        print STDERR "ERROR: Could not find controller ior file <$controller_ior>\n";
        $AM->Kill ();
        $DS->Kill ();
        exit 1;
    }

    printf "Test work: $work\n";

    if ($reverse_call_order == 0) {
        $all_iors = "-k file://$c25_ior -k file://$c50_ior -k file://$c75_ior";
    }
    else {
        $all_iors = "-k file://$c50_ior -k file://$c75_ior -k file://$c25_ior";
    }

#Start the client to send the trigger message
    $CL = new PerlACE::Process ("../Controllers/client",
                                "$all_iors -w $work");
    $CL->SpawnWaitKill(60);

## Now wait for the test to complete.  Need to figure out a way to
## detect this.
    sleep ($run_time);

#Start the client to send the trigger message
    $CL = new PerlACE::Process ("../Controllers/client",
                                "$all_iors -f");
    $CL->SpawnWaitKill(60);
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


    unlink $cookie;
    unlink $c25_ior;
    unlink $c50_ior;
    unlink $c75_ior;
    unlink $daemon_ior;
    unlink $am_ior;

}

$DS->Kill ();

exit $status;
