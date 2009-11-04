eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

###############################################################################

use strict;

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use Cwd;
use Sys::Hostname;
use File::Copy;

my $cwd = getcwd();

my $ACE_ROOT = $ENV{ACE_ROOT};

if (!defined $ACE_ROOT) {
    chdir ('../../../../');
    $ACE_ROOT = getcwd ();
    chdir ($cwd);
    print "ACE_ROOT not defined, defaulting to ACE_ROOT=$ACE_ROOT\n";
}

my $airplane_ior = PerlACE::LocalFile ("airplane.ior");
my $nestea_ior = PerlACE::LocalFile ("nestea.ior");
my $imr_activator_ior = PerlACE::LocalFile ("imr_activator.ior");
my $imr_locator_ior = PerlACE::LocalFile ("imr_locator.ior");

my $refstyle = " -ORBObjRefStyle URL";

my $backing_store = "imr_backing_store.xml";
my $P_SVR = new PerlACE::Process (PerlACE::LocalFile("persist server"));
my $nestea_dat = "nestea.dat";

my $protocol = "iiop";
my $host = hostname();
my $port = 12345;
my $endpoint = "-ORBEndpoint " . "$protocol" . "://:" . $port;


my $IMR_LOCATOR = new PerlACE::Process ("../../ImplRepo_Service/ImplRepo_Service");
my $IMR_ACTIVATOR = new PerlACE::Process ("../../ImplRepo_Service/ImR_Activator");
my $TAO_IMR = new PerlACE::Process("$ENV{ACE_ROOT}/bin/tao_imr");

# We want the tao_imr executable to be found exactly in the path
# given, without being modified by the value of -ExeSubDir.
# So, we tell its Process object to ignore the setting of -ExeSubDir.

$TAO_IMR->IgnoreExeSubDir (1);

sub create_acli {
  return new PerlACE::Process (PerlACE::LocalFile ("airplane_client"), " -k file://$airplane_ior");
}

sub create_ncli {
  return new PerlACE::Process (PerlACE::LocalFile ("nestea_client"), " -k file://$nestea_ior");
}

my $A_SVR = new PerlACE::Process (PerlACE::LocalFile ("airplane_server"));
my $A_CLI = create_acli();
my $N_SVR = new PerlACE::Process (PerlACE::LocalFile ("nestea_server"));
my $N_CLI = create_ncli();

# Make sure the files are gone, so we can wait on them.
unlink $airplane_ior;
unlink $nestea_ior;
unlink $imr_locator_ior;
unlink $imr_activator_ior;
unlink $backing_store;
unlink $nestea_dat;
unlink $P_SVR->Executable();



# The Tests

###############################################################################

sub airplane_test
{
    my $status = 0;

    $A_SVR->Arguments ("-o $airplane_ior $refstyle");
    $A_SVR->Spawn ();

    if (PerlACE::waitforfile_timed ($airplane_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
        print STDERR "ERROR: cannot find $airplane_ior\n";
        $A_SVR->Kill ();
        return 1;
    }

    my $client = $A_CLI->SpawnWaitKill (10);

    if ($client != 0) {
        print STDERR "ERROR: client returned $client\n";
        $status = 1;
    }

    my $server = $A_SVR->TerminateWaitKill (5);

    if ($server != 0) {
        print STDERR "ERROR: server returned $server\n";
        $status = 1;
    }

    return $status;
}

###############################################################################

sub nestea_test
{
    my $status = 0;

    $N_SVR->Arguments ("-o $nestea_ior $refstyle");
    $N_SVR->Spawn ();

    if (PerlACE::waitforfile_timed ($nestea_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
        print STDERR "ERROR: cannot find $nestea_ior\n";
        $N_SVR->Kill ();
        return 1;
    }

    my $client = $N_CLI->SpawnWaitKill (10);

    if ($client != 0) {
        print STDERR "ERROR: client returned $client\n";
        $status = 1;
    }

    my $server = $N_SVR->TerminateWaitKill (5);

    if ($server != 0) {
        print STDERR "ERROR: server returned $server\n";
        $status = 1;
    }

    return $status;
}

sub nt_service_test_i
{
    my ($imr_initref, $BIN_IMR_ACTIVATOR, $BIN_IMR_LOCATOR) = @_;

    print "Installing TAO ImR Services\n";
    $BIN_IMR_ACTIVATOR->Arguments ("-c install $imr_initref -d 0");
    $BIN_IMR_LOCATOR->Arguments ("-c install -d 0 -orbendpoint iiop://:8888");

    my $result = $BIN_IMR_LOCATOR->SpawnWaitKill (5);
    if ($result != 0) {
        print STDERR "ERROR: IMR Locator installation returned $result\n";
        return 1;
    }

    $result = $BIN_IMR_ACTIVATOR->SpawnWaitKill (5);
    if ($result != 0) {
        print STDERR "ERROR: IMR Activator installation returned $result\n";
        return 1;
    }

    # Starting the activator will also start the locator
    print "Starting TAO Implementation Repository Services\n";
    # Starting the activator should start the ImR automatically
    #system("net start taoimr 2>&1");
    system("net start taoimractivator 2>&1");

    # No need to specify imr_initref or -orbuseimr 1 for servers spawned by activator
    $TAO_IMR->Arguments ("$imr_initref add airplane_server -c \""
                         . $A_SVR->Executable() .
                         "\" -w \"$ACE_ROOT/lib\"");
    $result = $TAO_IMR->SpawnWaitKill (10);
    if ($result != 0) {
        print STDERR "ERROR: tao_imr add airplane_server returned $result\n";
        return 1;
    }

    $TAO_IMR->Arguments ("$imr_initref list -v");
    $result = $TAO_IMR->SpawnWaitKill (10);
    if ($result != 0) {
        print STDERR "ERROR: tao_imr list -v returned $result\n";
        return 1;
    }

    $TAO_IMR->Arguments ("$imr_initref ior airplane_server -f $airplane_ior");
    $result = $TAO_IMR->SpawnWaitKill (10);
    if ($result != 0) {
        print STDERR "ERROR: tao_imr ior airplane_server returned $result\n";
        return 1;
    }
    if (PerlACE::waitforfile_timed ($airplane_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
        print STDERR "ERROR: cannot find $airplane_ior\n";
        $A_SVR->Kill ();
        return 1;
    }

    $result = $A_CLI->SpawnWaitKill (20);
    if ($result != 0) {
        print STDERR "ERROR: airplane client returned $result\n";
        return 1;
    }

    $TAO_IMR->Arguments ("$imr_initref shutdown airplane_server");
    $result = $TAO_IMR->SpawnWaitKill (20);
    if ($result != 0) {
        print STDERR "ERROR: tao_imr shutdown airplane_server returned $result\n";
        return 1;
    }

    return 0;
}

sub nt_service_test
{
    my $result = 0;

    # Just to show that it's possible, this test uses corbaloc instead of ior file.
    my $imr_initref = "-orbinitref ImplRepoService=corbaloc::localhost:8888/ImplRepoService";

    # To avoid having to ensure that they LocalSystem account has the correct path
    # we simply copy the imr executables to the same directory as the DLL's.
    my $BIN_IMR_LOCATOR = new PerlACE::Process ("$ACE_ROOT/lib/ImplRepo_Service","");
    my $BIN_IMR_ACTIVATOR = new PerlACE::Process ("$ACE_ROOT/lib/ImR_Activator","");
    $BIN_IMR_LOCATOR->IgnoreExeSubDir(1);
    $BIN_IMR_ACTIVATOR->IgnoreExeSubDir(1);

    print "Copying ImplRepo services to the same location as the dlls.\n";
    unlink $BIN_IMR_LOCATOR->Executable ();
    copy ($IMR_LOCATOR->Executable (), $BIN_IMR_LOCATOR->Executable ());
    unlink $BIN_IMR_ACTIVATOR->Executable ();
    copy ($IMR_ACTIVATOR->Executable (), $BIN_IMR_ACTIVATOR->Executable ());

    print "Stopping any existing TAO ImR Services\n";
    system("net stop taoimractivator > nul 2>&1");
    system("net stop taoimr > nul 2>&1");

    print "Removing any existing TAO ImR Services\n";
    $BIN_IMR_ACTIVATOR->Arguments ("-c remove");
    $BIN_IMR_LOCATOR->Arguments ("-c remove");
    $BIN_IMR_ACTIVATOR->SpawnWaitKill (5);
    $BIN_IMR_LOCATOR->SpawnWaitKill (5);

    $result = nt_service_test_i ($imr_initref, $BIN_IMR_ACTIVATOR, $BIN_IMR_LOCATOR);

    print "Stopping TAO Implementation Repository Service\n";
    system("net stop taoimractivator 2>&1");
    system("net stop taoimr 2>&1");

    print "Removing TAO ImR Services\n";
    $BIN_IMR_ACTIVATOR->Arguments ("-c remove");
    $BIN_IMR_ACTIVATOR->SpawnWaitKill (5);
    $BIN_IMR_LOCATOR->Arguments ("-c remove");
    $BIN_IMR_LOCATOR->SpawnWaitKill (5);

    print "Removing ImplRepo_Service copy.\n";
    unlink $BIN_IMR_ACTIVATOR->Executable ();
    unlink $BIN_IMR_LOCATOR->Executable ();

    return $result;
}

###############################################################################

sub airplane_ir_test
{
    my $status = 0;
    my $result = 0;

    my $imr_initref = "-ORBInitRef ImplRepoService=file://$imr_locator_ior";

    $IMR_LOCATOR->Arguments ("-d 2 -o $imr_locator_ior $refstyle");
    $IMR_LOCATOR->Spawn ();

    if (PerlACE::waitforfile_timed ($imr_locator_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
        print STDERR "ERROR: cannot find $imr_locator_ior\n";
        $IMR_LOCATOR->Kill ();
        return 1;
    }

    $IMR_ACTIVATOR->Arguments ("-d 2 -o $imr_activator_ior $imr_initref");
    $IMR_ACTIVATOR->Spawn ();

    if (PerlACE::waitforfile_timed ($imr_activator_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
        print STDERR "ERROR: cannot find $imr_activator_ior\n";
        $IMR_ACTIVATOR->Kill ();
        $IMR_LOCATOR->Kill ();
        return 1;
    }

    # No need to specify imr_initref or -orbuseimr 1 for servers spawned by activator
    # Can use update to add servers.
    $TAO_IMR->Arguments ("$imr_initref update airplane_server -c \""
                         . $A_SVR->Executable ()
      . " -o $airplane_ior \"");

    $result = $TAO_IMR->SpawnWaitKill (5);
    if ($result != 0) {
        print STDERR "ERROR: tao_imr returned $result\n";
        $IMR_ACTIVATOR->Kill ();
        $IMR_LOCATOR->Kill ();
        return 1;
    }

    $A_SVR->Arguments ("-ORBUseIMR 1 -o $airplane_ior $imr_initref");
    $A_SVR->Spawn ();

    if (PerlACE::waitforfile_timed ($airplane_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
        print STDERR "ERROR: cannot find $airplane_ior\n";
        $IMR_ACTIVATOR->Kill ();
        $IMR_LOCATOR->Kill ();
        $A_SVR->Kill ();
        return 1;
    }

    $result = $A_CLI->SpawnWaitKill (10);
    if ($result != 0) {
        print STDERR "ERROR: airplane_client 1 returned $result\n";
        $status = 1;
    }

    $TAO_IMR->Arguments ("$imr_initref shutdown airplane_server");

    $result = $TAO_IMR->SpawnWaitKill (10);
    if ($result != 0) {
        print STDERR "ERROR: tao_imr 1 returned $result\n";
        $status = 1;
    }

    # This client should force a new airplane_server to be started
    $result = $A_CLI->SpawnWaitKill (10);
    if ($result != 0) {
        print STDERR "ERROR: airplane_client 2 returned $result\n";
        $status = 1;
    }

    $result = $TAO_IMR->SpawnWaitKill (10);
    if ($result != 0) {
        print STDERR "ERROR: tao_imr 2 returned $result\n";
        $status = 1;
    }

    my $server = $A_SVR->WaitKill (5);
    if ($server != 0) {
        print STDERR "ERROR: airplane server returned $server\n";
        $status = 1;
    }

    my $imr_activator = $IMR_ACTIVATOR->TerminateWaitKill (5);
    if ($imr_activator != 0) {
        print STDERR "ERROR: Activator returned $imr_activator\n";
        $status = 1;
    }

    my $imr_locator = $IMR_LOCATOR->TerminateWaitKill (5);
    if ($imr_locator != 0) {
        print STDERR "ERROR: ImR returned $imr_locator\n";
        $status = 1;
    }

    return $status;
}

###############################################################################

sub nestea_ir_test
{
    my $status = 0;
    my $result = 0;

    my $imr_initref = "-orbobjrefstyle URL -ORBInitRef ImplRepoService=file://$imr_locator_ior";

    $IMR_LOCATOR->Arguments ("-d 2 -o $imr_locator_ior $refstyle");
    $IMR_LOCATOR->Spawn ();

    if (PerlACE::waitforfile_timed ($imr_locator_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
      print STDERR "ERROR: cannot find $imr_locator_ior\n";
      $IMR_LOCATOR->Kill ();
      return 1;
    }

    $IMR_ACTIVATOR->Arguments ("-d 2 -o $imr_activator_ior $imr_initref");
    $IMR_ACTIVATOR->Spawn ();

    if (PerlACE::waitforfile_timed ($imr_activator_ior, 30) == -1) {
        print STDERR "ERROR: cannot find $imr_activator_ior\n";
        $IMR_ACTIVATOR->Kill ();
        $IMR_LOCATOR->Kill ();
        return 1;
    }

    $N_SVR->Arguments ("-ORBUseIMR 1 -o $nestea_ior $imr_initref");
    $N_SVR->Spawn ();

    if (PerlACE::waitforfile_timed ($nestea_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
        print STDERR "ERROR: cannot find $nestea_ior\n";
        $N_SVR->Kill ();
        $IMR_ACTIVATOR->Kill ();
        $IMR_LOCATOR->Kill ();
        return 1;
    }

    $result = $N_CLI->SpawnWaitKill (10);
    if ($result != 0) {
        print STDERR "ERROR: nestea client 1 returned $result\n";
        $status = 1;
    }

    $TAO_IMR->Arguments ("$imr_initref shutdown nestea_server");

    $result = $TAO_IMR->SpawnWaitKill (10);
    if ($result != 0) {
        print STDERR "ERROR: tao_imr 1 returned $result\n";
        $status = 1;
    }

    my $server = $N_SVR->WaitKill (5);
    if ($server != 0) {
        print STDERR "ERROR: nestea server returned $server\n";
        $status = 1;
    }

    # No need to specify imr_initref or -orbuseimr 1 for servers spawned by activator
    $TAO_IMR->Arguments ("$imr_initref update nestea_server -l $host -c \""
                         . $N_SVR->Executable ()
                         . " -o $nestea_ior\"");
    $result = $TAO_IMR->SpawnWaitKill (10);
    if ($result != 0) {
        print STDERR "ERROR: tao_imr returned $result\n";
        $IMR_ACTIVATOR->Kill ();
        $IMR_LOCATOR->Kill ();
        return 1;
    }

    # This should cause the activator to spawn another server.
    $result = $N_CLI->SpawnWaitKill (20);
    if ($result != 0) {
        print STDERR "ERROR: nestea client 2 returned $result\n";
        $status = 1;
    }

    $TAO_IMR->Arguments ("$imr_initref shutdown nestea_server");
    $result = $TAO_IMR->SpawnWaitKill (10);
    if ($result != 0) {
        print STDERR "ERROR: tao_imr 1 returned $result\n";
        $status = 1;
    }

    # This should destroy the POA, causing another to be created the next time
    # the server is spawned.
    $TAO_IMR->Arguments ("$imr_initref remove nestea_server");
    $result = $TAO_IMR->SpawnWaitKill (10);
    if ($result != 0) {
        print STDERR "ERROR: tao_imr 1 returned $result\n";
        $status = 1;
    }

    # No need to specify imr_initref or -orbuseimr 1 for servers spawned by activator
    $TAO_IMR->Arguments ("$imr_initref add nestea_server -c \""
                         . $N_SVR->Executable ()
                         . " -o $nestea_ior\"");

    $result = $TAO_IMR->SpawnWaitKill (10);
    if ($result != 0) {
        print STDERR "ERROR: tao_imr returned $result\n";
        $IMR_ACTIVATOR->Kill ();
        $IMR_LOCATOR->Kill ();
        return 1;
    }

    # This should cause the activator to spawn another server.
    $result = $N_CLI->SpawnWaitKill (20);
    if ($result != 0) {
        print STDERR "ERROR: nestea client 2 returned $result\n";
        $status = 1;
    }

    # This call should block until the server shuts down
    $TAO_IMR->Arguments ("$imr_initref shutdown nestea_server");
    $result = $TAO_IMR->SpawnWaitKill (10);
    if ($result != 0) {
        print STDERR "ERROR: tao_imr 1 returned $result\n";
        $status = 1;
    }

    my $implrepo = $IMR_ACTIVATOR->TerminateWaitKill (5);
    if ($implrepo != 0) {
        print STDERR "ERROR: IMR_Activator returned $implrepo\n";
        $status = 1;
    }

    $implrepo = $IMR_LOCATOR->TerminateWaitKill (5);
    if ($implrepo != 0) {
        print STDERR "ERROR: IMR_Locator returned $implrepo\n";
        $status = 1;
    }

    return $status;
}

###############################################################################

sub perclient
{
    my $status = 0;
    my $result = 0;

    my $imr_initref = "-orbobjrefstyle URL -ORBInitRef ImplRepoService=file://$imr_locator_ior";

    # specify an endpoint so that we can use corbaloc url for the client.
    $IMR_LOCATOR->Arguments ("-d 2 -orbendpoint iiop://:8888 -o $imr_locator_ior $refstyle");
    $IMR_LOCATOR->Spawn ();

    if (PerlACE::waitforfile_timed ($imr_locator_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
      print STDERR "ERROR: cannot find $imr_locator_ior\n";
      $IMR_LOCATOR->Kill ();
      return 1;
    }

    $IMR_ACTIVATOR->Arguments ("-d 2 -o $imr_activator_ior $imr_initref");
    $IMR_ACTIVATOR->Spawn ();

    if (PerlACE::waitforfile_timed ($imr_activator_ior, 30) == -1) {
        print STDERR "ERROR: cannot find $imr_activator_ior\n";
        $IMR_ACTIVATOR->Kill ();
        $IMR_LOCATOR->Kill ();
        return 1;
    }

    # No need to specify imr_initref or -orbuseimr 1 for servers spawned by activator
    $TAO_IMR->Arguments ("$imr_initref add nestea_server -a PER_CLIENT -c \""
                         . $N_SVR->Executable ()
                         . " -o $nestea_ior\"");
    $result = $TAO_IMR->SpawnWaitKill (10);
    if ($result != 0) {
        print STDERR "ERROR: tao_imr returned $result\n";
        $IMR_ACTIVATOR->Kill ();
        $IMR_LOCATOR->Kill ();
        return 1;
    }

    $N_CLI->Arguments("-k corbaloc::localhost:8888/nestea_server");

    # Running the client should start a server instance
    $result = $N_CLI->SpawnWaitKill (10);
    if ($result != 0) {
        print STDERR "ERROR: nestea client 1 returned $result\n";
        $status = 1;
    }
    if (PerlACE::waitforfile_timed ($nestea_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
        print STDERR "ERROR: cannot find $nestea_ior\n";
        $IMR_ACTIVATOR->Kill ();
        $IMR_LOCATOR->Kill ();
        return 1;
    }

    unlink $nestea_ior;

    $N_CLI->Arguments("-s -k corbaloc::localhost:8888/nestea_server");

    # Running the client again should start another server instance
    $result = $N_CLI->SpawnWaitKill (10);
    if ($result != 0) {
        print STDERR "ERROR: nestea client 2 returned $result\n";
        $status = 1;
    }
    if (PerlACE::waitforfile_timed ($nestea_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
        print STDERR "ERROR: cannot find $nestea_ior\n";
        $IMR_ACTIVATOR->Kill ();
        $IMR_LOCATOR->Kill ();
        return 1;
    }

    # Note : We have to wait long enough for the first server to self-destruct
    # or it will print out an exception when it can't notify the imr of its shutdown.
    sleep 10;

    my $implrepo = $IMR_ACTIVATOR->TerminateWaitKill (5);
    if ($implrepo != 0) {
        print STDERR "ERROR: IMR_Activator returned $implrepo\n";
        $status = 1;
    }

    $implrepo = $IMR_LOCATOR->TerminateWaitKill (5);
    if ($implrepo != 0) {
        print STDERR "ERROR: IMR_Locator returned $implrepo\n";
        $status = 1;
    }

    return $status;
}

###############################################################################

sub shutdown_repo
{
    my $status = 0;
    my $result = 0;

    my $imr_initref = "-orbobjrefstyle URL -ORBInitRef ImplRepoService=file://$imr_locator_ior";

    unlink "test.repo";

    # Specify an endpoint so that we can restart on the same port.
    # Specify persistence so that we can test that shutdown-repo -a works after reconnect
    $IMR_LOCATOR->Arguments ("-p test.repo -d 1 -orbendpoint iiop://:8888 -o $imr_locator_ior $refstyle");

    unlink $imr_locator_ior;
    $IMR_LOCATOR->Spawn ();
    if (PerlACE::waitforfile_timed ($imr_locator_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
      print STDERR "ERROR: cannot find $imr_locator_ior\n";
      $IMR_LOCATOR->Kill ();
      return 1;
    }

    $IMR_ACTIVATOR->Arguments ("-d 1 -o $imr_activator_ior $imr_initref");
    $IMR_ACTIVATOR->Spawn ();
    if (PerlACE::waitforfile_timed ($imr_activator_ior, 30) == -1) {
        print STDERR "ERROR: cannot find $imr_activator_ior\n";
        $IMR_ACTIVATOR->Kill ();
        $IMR_LOCATOR->Kill ();
        return 1;
    }

    # Kill the ImR, but leave the activator running
    $TAO_IMR->Arguments ("$imr_initref shutdown-repo");
    $result = $TAO_IMR->SpawnWaitKill (5);
    if ($result != 0) {
        print STDERR "ERROR: tao_imr returned $result\n";
        $IMR_ACTIVATOR->Kill ();
        $IMR_LOCATOR->Kill ();
        return 1;
    }

    my $imr_result = $IMR_LOCATOR->WaitKill (5);
    if ($imr_result != 0) {
        print STDERR "ERROR: ImR returned $imr_result\n";
        return 1;
    }

    unlink $imr_locator_ior;
    $IMR_LOCATOR->Spawn ();
    if (PerlACE::waitforfile_timed ($imr_locator_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
      print STDERR "ERROR: cannot find $imr_locator_ior\n";
      $IMR_LOCATOR->Kill ();
      return 1;
    }

    $TAO_IMR->Arguments ("$imr_initref shutdown-repo -a");
    $result = $TAO_IMR->SpawnWaitKill (5);
    if ($result != 0) {
        print STDERR "ERROR: tao_imr returned $result\n";
        $IMR_ACTIVATOR->Kill ();
        $IMR_LOCATOR->Kill ();
        return 1;
    }

    $imr_result = $IMR_ACTIVATOR->WaitKill (5);
    if ($imr_result != 0) {
        print STDERR "ERROR: IMR_Activator returned $imr_result\n";
        return 1;
    }

    $imr_result = $IMR_LOCATOR->WaitKill (5);
    if ($imr_result != 0) {
        print STDERR "ERROR: IMR_Locator returned $imr_result\n";
        return 1;
    }

    unlink "test.repo";

    return $status;
}

###############################################################################

sub persistent_ir_test
{
    my $result = 0;

    my $imr_initref = "-ORBInitRef ImplRepoService=file://$imr_locator_ior";

    unlink $imr_locator_ior;
    ## Be sure to start the ImR on a consistent endpoint, so that any created IORs
    ## remain valid even if the ImR restarts.
    $IMR_LOCATOR->Arguments ("-orbendpoint iiop://:8888 -x $backing_store -d 2 -o $imr_locator_ior $refstyle");
    $IMR_LOCATOR->Spawn ();
    if (PerlACE::waitforfile_timed ($imr_locator_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
      print STDERR "ERROR: cannot find $imr_locator_ior\n";
      $IMR_LOCATOR->Kill ();
      return 1;
    }

    unlink $imr_activator_ior;
    $IMR_ACTIVATOR->Arguments ("-d 2 -o $imr_activator_ior $imr_initref");
    $IMR_ACTIVATOR->Spawn ();
    if (PerlACE::waitforfile_timed ($imr_activator_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
        print STDERR "ERROR: cannot find $imr_activator_ior\n";
        $IMR_ACTIVATOR->Kill ();
        $IMR_LOCATOR->Kill ();
        return 1;
    }

    unlink $P_SVR->Executable();
    # Copy the server to a path with spaces to ensure that these
    # work corrrectly.
    copy ($A_SVR->Executable(), $P_SVR->Executable());
    chmod(0755, $P_SVR->Executable());

    # No need to specify imr_initref or -orbuseimr 1 for servers spawned by activator
    $TAO_IMR->Arguments ("$imr_initref add airplane_server -c \""
        . '\"' . $P_SVR->Executable() . '\"' . "\" " . $refstyle);
    $result = $TAO_IMR->SpawnWaitKill (10);

    if ($result != 0) {
        print STDERR "ERROR: tao_imr returned $result\n";
        unlink $P_SVR->Executable();
        return 1;
    }

    unlink $airplane_ior;
    ## This will write out the imr-ified IOR. Note : If you don't use -orbendpoint
    ## when starting the ImR, then this IOR will no longer be valid when the ImR
    ## restarts below. You can fix this by creating a new valid IOR, or starting
    ## the ImR on a consistent endpoint.
    $A_SVR->Arguments ("-o $airplane_ior -ORBUseIMR 1 $refstyle $imr_initref");
    $A_SVR->Spawn ();
    if (PerlACE::waitforfile_timed ($airplane_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
        print STDERR "ERROR: cannot find $airplane_ior\n";
        $IMR_LOCATOR->Kill ();
        $IMR_ACTIVATOR->Kill ();
        $A_SVR->Kill ();
        return 1;
    }

    $result = $A_CLI->SpawnWaitKill (10);
    if ($result != 0) {
        print STDERR "ERROR: airplane client returned $result\n";
        $IMR_LOCATOR->Kill ();
        $IMR_ACTIVATOR->Kill ();
        $A_SVR->Kill ();
        return 1;
    }

    $TAO_IMR->Arguments ("$imr_initref shutdown airplane_server");
    $result = $TAO_IMR->SpawnWaitKill (10);
    if ($result != 0) {
        print STDERR "ERROR: tao_imr shutdown returned $result\n";
        $IMR_LOCATOR->Kill ();
        $IMR_ACTIVATOR->Kill ();
        $A_SVR->Kill ();
        return 1;
    }

    $result = $A_SVR->WaitKill (1);
    if ($result != 0) {
        print STDERR "ERROR: airplane server returned $result\n";
        $IMR_LOCATOR->Kill ();
        $IMR_ACTIVATOR->Kill ();
        return 1;
    }

    # Should cause the activator to spawn another server.
    $result = $A_CLI->SpawnWaitKill (20);
    if ($result != 0) {
        print STDERR "ERROR: airplane client 2 returned $result\n";
        $IMR_LOCATOR->Kill ();
        $IMR_ACTIVATOR->Kill ();
        return 1;
    }

    # Shutdown airplane_server
    $result = $TAO_IMR->SpawnWaitKill (10);
    if ($result != 0) {
        print STDERR "ERROR: tao_imr shutdown 2 returned $result\n";
        $IMR_LOCATOR->Kill ();
        $IMR_ACTIVATOR->Kill ();
        return 1;
    }

    my $implrepo = $IMR_LOCATOR->TerminateWaitKill (5);
    if ($implrepo != 0) {
        print STDERR "ERROR: IMR_Locator returned $implrepo\n";
        $IMR_ACTIVATOR->Kill ();
        return 1;
    }

    # Unlink so that we can wait on them again to know the server started.
    unlink $imr_locator_ior;
    print "Restarting Implementation Repository.\n";
    $IMR_LOCATOR->Spawn ();
    if (PerlACE::waitforfile_timed ($imr_locator_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
      print STDERR "ERROR: cannot find $imr_locator_ior\n";
      $IMR_LOCATOR->Kill ();
      return 1;
    }

    # Should cause the activator to spawn another server.
    $result = $A_CLI->SpawnWaitKill (20);
    if ($result != 0) {
        print STDERR "ERROR: airplane client 3 returned $result\n";
        $IMR_LOCATOR->Kill ();
        $IMR_ACTIVATOR->Kill ();
        return 1;
    }

    # Shutdown airplane_server
    $result = $TAO_IMR->SpawnWaitKill (10);
    if ($result != 0) {
        print STDERR "ERROR: tao_imr shutdown 3 returned $result\n";
        $IMR_LOCATOR->Kill ();
        $IMR_ACTIVATOR->Kill ();
        return 1;
    }

    $result = $IMR_ACTIVATOR->TerminateWaitKill (5);
    if ($result != 0) {
        print STDERR "ERROR: IMR_Activator returned $result\n";
        $IMR_LOCATOR->Kill ();
        return 1;
    }

    $result = $IMR_LOCATOR->TerminateWaitKill (5);
    if ($result != 0) {
        print STDERR "ERROR: IMR_Locator returned $result\n";
        return 1;
    }

    unlink $P_SVR->Executable();
    unlink $imr_locator_ior;
    unlink $imr_activator_ior;
    unlink $airplane_ior;

    return 0;
}

###############################################################################

sub both_ir_test
{
    my $status = 0;

    my $imr_initref = "-ORBInitRef ImplRepoService=file://$imr_locator_ior";

    $IMR_LOCATOR->Arguments ("-d 2 -t 5 -o $imr_locator_ior $refstyle");
    $IMR_LOCATOR->Spawn ();

    if (PerlACE::waitforfile_timed ($imr_locator_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
        print STDERR "ERROR: cannot find $imr_locator_ior\n";
        $IMR_LOCATOR->Kill ();
        return 1;
    }

    $IMR_ACTIVATOR->Arguments ("-e 16384 -o $imr_activator_ior $imr_initref $refstyle -d 2");
    $IMR_ACTIVATOR->Spawn ();

    if (PerlACE::waitforfile_timed ($imr_activator_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
        print STDERR "ERROR: cannot find $imr_activator_ior\n";
        $IMR_ACTIVATOR->Kill ();
        $IMR_LOCATOR->Kill ();
        return 1;
    }

    ## Note : It's crucial NOT to write out an IOR file when the activator
    ## starts the server, or at least to write out a different file name
    ## than the IOR files we're using for the clients. Otherwise a client
    ## may attempt to use a partially written file.
    # No need to specify imr_initref or -orbuseimr 1 for servers spawned by activator
    $TAO_IMR->Arguments ("$imr_initref add nestea_server -c \""
                         . $N_SVR->Executable ()
      . " $refstyle\"");
    $TAO_IMR->SpawnWaitKill (10);

    # No need to specify imr_initref or -orbuseimr 1 for servers spawned by activator
    $TAO_IMR->Arguments ("$imr_initref add airplane_server -c \""
                         . $A_SVR->Executable ()
       . " $refstyle\"");
    $TAO_IMR->SpawnWaitKill (10);

    $N_SVR->Arguments (" -o $nestea_ior -ORBUseIMR 1 $imr_initref $refstyle");
    $N_SVR->Spawn ();
    if (PerlACE::waitforfile_timed ($nestea_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
        print STDERR "ERROR: cannot find $nestea_ior\n";
        $IMR_ACTIVATOR->Kill ();
        $IMR_LOCATOR->Kill ();
        $A_SVR->Kill ();
        $N_SVR->Kill ();
        return 1;
    }

    $A_SVR->Arguments (" -o $airplane_ior -ORBUseIMR 1 $imr_initref $refstyle");
    $A_SVR->Spawn ();
    if (PerlACE::waitforfile_timed ($airplane_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
        print STDERR "ERROR: cannot find $airplane_ior\n";
        $IMR_ACTIVATOR->Kill ();
        $IMR_LOCATOR->Kill ();
        $A_SVR->Kill ();
        $N_SVR->Kill ();
        return 1;
    }

    my @clients;

    for (1 .. 5) {
       push @clients, &create_acli();
       push @clients, &create_ncli();
    }

    print "\n## Spawning multiple simultaneous clients with both servers running.\n";
    map $_->Spawn(), @clients;
    map $_->WaitKill(30), @clients;

    $TAO_IMR->Arguments ("$imr_initref shutdown nestea_server");
    $TAO_IMR->SpawnWaitKill (15);

    $TAO_IMR->Arguments ("$imr_initref shutdown airplane_server");
    $TAO_IMR->SpawnWaitKill (15);

    $A_SVR->WaitKill(1);
    $N_SVR->WaitKill(1);

    print "\n\n\n\n## Spawning multiple simultaneous clients with no servers running.\n";

    map $_->Spawn(), @clients;
    map $_->WaitKill(30), @clients;

    $TAO_IMR->Arguments ("$imr_initref shutdown nestea_server");
    $TAO_IMR->SpawnWaitKill (15);
    $TAO_IMR->Arguments ("$imr_initref shutdown airplane_server");
    $TAO_IMR->SpawnWaitKill (15);

    $IMR_ACTIVATOR->Kill ();
    $IMR_LOCATOR->Kill ();
}

###############################################################################
###############################################################################

# Parse the arguments

my $ret = 0;

if ($#ARGV >= 0) {
for (my $i = 0; $i <= $#ARGV; $i++) {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?") {
        print "run_test test\n";
        print "\n";
        print "test               -- Runs a specific test:\n";
        print "                         airplane, airplane_ir, nt_service_ir, ",
                                       "nestea, nestea_ir,\n";
        print "                         both_ir, persistent_ir\n";
        exit 1;
    }
    elsif ($ARGV[$i] eq "airplane") {
        $ret = airplane_test ();
    }
    elsif ($ARGV[$i] eq "airplane_ir") {
        $ret = airplane_ir_test ();
    }
    elsif ($ARGV[$i] eq "nt_service_ir") {
        $ret = nt_service_test ();
    }
    elsif ($ARGV[$i] eq "nestea") {
        $ret = nestea_test ();
    }
    elsif ($ARGV[$i] eq "nestea_ir") {
        $ret = nestea_ir_test ();
    }
    elsif ($ARGV[$i] eq "both_ir") {
        $ret = both_ir_test ();
    }
    elsif ($ARGV[$i] eq "persistent_ir") {
        $ret = persistent_ir_test ();
    }
    elsif ($ARGV[$i] eq "perclient") {
        $ret = perclient();
    }
    elsif ($ARGV[$i] eq "shutdown") {
        $ret = shutdown_repo();
    }
    else {
        print "run_test: Unknown Option: ".$ARGV[$i]."\n";
    }
}
} else {
  $ret = both_ir_test();
}


# Make sure the files are gone, so we can wait on them.
unlink $airplane_ior;
unlink $nestea_ior;
unlink $imr_locator_ior;
unlink $imr_activator_ior;
unlink $backing_store;
unlink $nestea_dat;
unlink $P_SVR->Executable();

exit $ret;
