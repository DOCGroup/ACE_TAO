eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

###############################################################################
my $ACE_ROOT = $ENV{ACE_ROOT};

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use Cwd;
use Sys::Hostname;
use File::Copy;

use strict;

my $cwd = getcwd();

if (!defined $ACE_ROOT) {
    print "Error: ACE_ROOT not defined.\n";
    return 1;
}

# this matches the orbendpoint used in start_all.conf
my $imr_corbaloc = "corbaloc::localhost:9999";
my $imr_initref = "-orbinitref ImplRepoService=$imr_corbaloc/ImplRepoService";

my $imr_start_file = PerlACE::LocalFile ("imr_status");
my $server_start_file = PerlACE::LocalFile ("server_status");

my $start_conf = PerlACE::LocalFile ("start_all.conf");

my $COMB = new PerlACE::Process (PerlACE::LocalFile ("combined_service"));
my $combined_ior = PerlACE::LocalFile ("combined.ior");
my $cs_initref = "-orbinitref ServiceConfig=file://$combined_ior";

my $CONT = new PerlACE::Process ('controller');
my $controller = $CONT->Executable ();

my $IMRUTIL = new PerlACE::Process("$ACE_ROOT/bin/tao_imr");

my $CLI = new PerlACE::Process ('test');
my $test_initref = "-orbinitref Test=$imr_corbaloc";

my $start_server_cmd = "dynamic DynServer_Loader Service_Object * DynServer:_make_DynServer_Loader() '-orbcollocation per-orb -orbuseimr 1 $imr_initref'";

my $hostname = hostname();
###############################################################################

sub do_test
{
    unlink $combined_ior;

    my $start_time = time();

    # First we start all the servers, including the test server
    $COMB->Arguments("-f $start_conf");
    my $ret = $COMB->Spawn();
    if ($ret != 0) {
        print "ERROR : spawning combined service.\n";
        return $ret;
    }

    ## Wait a little bit for everything to get started
    sleep(2);

    # The client should pass the simple test
    $CLI->Arguments("$test_initref/DynObject1");
    $ret = $CLI->SpawnWaitKill(5);
    if ($ret != 0) {
        print "ERROR : spawning test client 1.\n";
        $COMB->TerminateWaitKill(5);
        return $ret;
    }

    # The client should pass the simple test again, this time with obj #2
    $CLI->Arguments("$test_initref/DynObject2");
    $ret = $CLI->SpawnWaitKill(5);
    if ($ret != 0) {
        print "ERROR : spawning test client 2.\n";
        $COMB->TerminateWaitKill(5);
        return $ret;
    }

    # The server was autoregistered without any start information. We
    # need to update the registration with a command line so that the
    # activator can be used to re-launch it.
    $IMRUTIL->Arguments("$imr_initref  update DynObject1 -l $hostname -c \"$controller $cs_initref"
      . ' -c \\"' . $start_server_cmd . '\\""');
    $ret = $IMRUTIL->SpawnWaitKill(5);
    if ($ret != 0) {
        print "ERROR : Updating DynObject1 cmdline.\n";
        $COMB->TerminateWaitKill(5);
        return $ret;
    }

    # The new command line should now be registered, but only for DynObject1
    $IMRUTIL->Arguments("$imr_initref list -v");
    $ret = $IMRUTIL->SpawnWaitKill(5);
    if ($ret != 0) {
        print "ERROR : Listing ImR Servers.\n";
        $COMB->TerminateWaitKill(5);
        return $ret;
    }

    # Now we can kill the server. We have to do this using the controller, because
    # if we were to use the tao_imr shutdown command, there would be no way for
    # the server to let the service configurator framework know that it stopped.
    # This could be solved by adding a new interface to the combined_service idl.
    $CONT->Arguments($cs_initref . ' -c "remove DynServer_Loader"');
    $ret = $CONT->SpawnWaitKill(5);
    if ($ret != 0) {
        print "ERROR : Shutting down Dynamic Server.\n";
        $COMB->TerminateWaitKill(5);
        return $ret;
    }

    # Both DynObject1 and DynObject2 should now show up as "not running"
    $IMRUTIL->Arguments("$imr_initref list -v");
    $ret = $IMRUTIL->SpawnWaitKill(5);
    if ($ret != 0) {
        print "ERROR : Listing ImR Servers.\n";
        $COMB->TerminateWaitKill(5);
        return $ret;
    }

    # The client should pass the simple test again, because the Activator will
    # launch the controller, which will, in turn, reload DynServer_Loader
    $CLI->Arguments("$test_initref/DynObject1");
    $ret = $CLI->SpawnWaitKill(5);
    if ($ret != 0) {
        print "ERROR : spawning test client 3.\n";
        $COMB->TerminateWaitKill(5);
        return $ret;
    }

    # The client should pass the simple test again with obj #2, because both
    $CLI->Arguments("$test_initref/DynObject2");
    $ret = $CLI->SpawnWaitKill(5);
    if ($ret != 0) {
        print "ERROR : spawning test client 4.\n";
        $COMB->TerminateWaitKill(5);
        return $ret;
    }

    $COMB->TerminateWaitKill(5);
    if ($ret != 0) {
        print "ERROR : Terminating combined service.\n";
        return $ret;
    }

    unlink $combined_ior;

    return $ret;
}

my $ret = do_test();

#unlink $;

exit $ret;
