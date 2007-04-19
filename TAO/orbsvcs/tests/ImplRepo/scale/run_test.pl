eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

###############################################################################
use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use Cwd;
use Sys::Hostname;
use File::Copy;

use strict;

my $cwd = getcwd();

my $imr_activator_ior = PerlACE::LocalFile ("imr_activator.ior");
my $imr_locator_ior = PerlACE::LocalFile ("imr_locator.ior");

my $refstyle = " -ORBobjrefstyle URL";

my $persistxml = PerlACE::LocalFile ("persist.xml");
my $persist = PerlACE::LocalFile ("persist.dat");

my $IMR_LOCATOR = new PerlACE::Process ("$PerlACE::TAO_ROOT/orbsvcs/ImplRepo_Service/ImplRepo_Service");
my $IMR_ACTIVATOR = new PerlACE::Process ("$PerlACE::TAO_ROOT/orbsvcs/ImplRepo_Service/ImR_Activator");
my $TAO_IMR = new PerlACE::Process("$PerlACE::ACE_ROOT/bin/tao_imr");

my $SVR = new PerlACE::Process (PerlACE::LocalFile ("server"));
my $CLI = new PerlACE::Process (PerlACE::LocalFile ("client"));

my $servers_count = 1;
my $obj_count = 1;
my $use_activator = 0;

my $objprefix = "TstObj";
my $client_wait_time = 5;

# Make sure the files are gone, so we can wait on them.
unlink $imr_locator_ior;
unlink $imr_activator_ior;
unlink $persistxml;
unlink $persist;

sub scale_test
{
    print "Running scale test with $servers_count servers and $obj_count objects.\n";
    
    my $status = 0;
    my $result = 0;
    my $start_time = time();

    my $imr_initref = "-ORBInitRef ImplRepoService=file://$imr_locator_ior";

    $IMR_LOCATOR->Arguments ("-d 1 -o $imr_locator_ior -orbendpoint iiop://:9876");
    $IMR_LOCATOR->Spawn ();
    if (PerlACE::waitforfile_timed ($imr_locator_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
        print STDERR "ERROR: cannot find $imr_locator_ior\n";
        $IMR_LOCATOR->Kill ();
        return 1;
    }

    if ($use_activator) {
    
        $IMR_ACTIVATOR->Arguments ("-d 1 -o $imr_activator_ior $imr_initref");
        $IMR_ACTIVATOR->Spawn ();
        if (PerlACE::waitforfile_timed ($imr_activator_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
            print STDERR "ERROR: cannot find $imr_activator_ior\n";
            $IMR_ACTIVATOR->Kill ();
            return 1;
        }

        for(my $i = 0; $i < $servers_count; $i++) {
            for (my $j = 0; $j < $obj_count; $j++) {
                $TAO_IMR->Arguments ("$imr_initref add $objprefix" . '_' . $i . "_" . $j . " -c \""
                        . $SVR->Executable ()
                        . " -ORBUseIMR 1 -p $objprefix" . '_' . "$i -c $obj_count $imr_initref\"");

                $result = $TAO_IMR->SpawnWaitKill (5);
                if ($result != 0) {
                    print STDERR "ERROR: tao_imr returned $result\n";
                    $IMR_ACTIVATOR->Kill ();
                    $IMR_LOCATOR->Kill ();
                    return 1;
                }
            }
        }
        
        $TAO_IMR->Arguments ("$imr_initref list");
        $result = $TAO_IMR->SpawnWaitKill (25);
        if ($result != 0) {
            print STDERR "ERROR: tao_imr list returned $result\n";
            $IMR_ACTIVATOR->Kill ();
            $IMR_LOCATOR->Kill ();
            return 1;
        }
    }
    
    for(my $i = 0; $i < $servers_count; $i++ ) {
    
        my $server_start_file = PerlACE::LocalFile ($objprefix . "_$i.status");
        unlink $server_start_file;
  
        if (! $use_activator) {
            $SVR->Arguments ("-ORBUseIMR 1 -p $objprefix" . '_' . "$i -c $obj_count $imr_initref");
            $SVR->Spawn ();
            if (PerlACE::waitforfile_timed ($server_start_file, $PerlACE::wait_interval_for_process_creation) == -1) {
                print STDERR "ERROR: cannot find $server_start_file\n";
                $IMR_ACTIVATOR->Kill ();
                $IMR_LOCATOR->Kill ();
                $SVR->Kill ();
                return 1;
            }
        } else {
            # For some reason the servers take forever to spawn when using the activator
            $client_wait_time *= $obj_count;
        }
        unlink $server_start_file;

        for (my $j = 0; $j < $obj_count; $j++) {
            $CLI->Arguments ("-orbinitref Test=corbaloc::localhost:9876/$objprefix" . '_' . $i . '_' . $j);
            $result = $CLI->SpawnWaitKill ($client_wait_time);
            if ($result != 0) {
                print STDERR "ERROR: client returned $result\n";
                $status = 1;
                last;
            }
        }

        # Shutting down any server object within the server will shutdown the whole server
        $TAO_IMR->Arguments ("$imr_initref shutdown $objprefix" . '_' . $i . "_0");
        $result = $TAO_IMR->SpawnWaitKill (10);
        if ($result != 0) {
            print STDERR "ERROR: tao_imr shutdown returned $result\n";
            $status = 1;
            last;
        }
        if (! $use_activator) {
            $result = $SVR->WaitKill(5);
            if ($result != 0) {
                print STDERR "ERROR: server not shutdown correctly.\n";
                $status = 1;
                last;
            }
        }
        
        if ($status == 1) {
            last;
        }
    }

    if ($use_activator) {
        my $imr_activator = $IMR_ACTIVATOR->TerminateWaitKill (5);
        if ($imr_activator != 0) {
            print STDERR "ERROR: IMR returned $imr_activator\n";
            $status = 1;
        }
    }
    
    my $imr_locator = $IMR_LOCATOR->TerminateWaitKill (5);
    if ($imr_locator != 0) {
        print STDERR "ERROR: IMR returned $imr_locator\n";
        $status = 1;
    }

    my $test_time = time() - $start_time;
    my $total_objs = $obj_count * $servers_count;
    
    print "\nFinished. The test took $test_time seconds for $total_objs imr-ified objects.\n";

    return $status;
}

sub usage() {
    print "Usage: run_test.pl [-servers <num=1>] [-objects <num=1>] [-use_activator]\n";
}

###############################################################################
###############################################################################

if ($#ARGV >= 0) {
  for (my $i = 0; $i <= $#ARGV; $i++) {
    if ($ARGV[$i] eq "-servers") {
        $i++;
        $servers_count = $ARGV[$i];
    }
    elsif ($ARGV[$i] eq "-objects") {
        $i++;
        $obj_count = $ARGV[$i];
    }
    elsif ($ARGV[$i] eq "-use_activator") {
        $use_activator = 1;
    }
    else {
        usage();
        exit 1;
    }
  }
}

my $ret = scale_test();

unlink $imr_locator_ior;
unlink $imr_activator_ior;
unlink $persistxml;
unlink $persist;

exit $ret;
