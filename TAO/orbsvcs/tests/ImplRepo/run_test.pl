eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

###############################################################################
use lib "../../../../bin";
use PerlACE::Run_Test;
use Cwd;
use Sys::Hostname;
use File::Copy;

$cwd = getcwd();

$ACE_ROOT = $ENV{ACE_ROOT};

if (!defined $ACE_ROOT) {
    chdir ('../../../../');
    $ACE_ROOT = getcwd ();
    chdir ($cwd);
    print "ACE_ROOT not defined, defaulting to ACE_ROOT=$ACE_ROOT\n";
}

$airplane_ior = PerlACE::LocalFile ("airplane.ior");
$nestea_ior = PerlACE::LocalFile ("nestea.ior");
$imr_activator_ior = PerlACE::LocalFile ("imr_activator.ior");
$imr_locator_ior = PerlACE::LocalFile ("imr_locator.ior");

$refstyle = " -ORBobjrefstyle URL";

$backing_store = "imr_backing_store";
$nestea_dat = "nestea.dat";

$protocol = "iiop";
$host = hostname();
$port = 12345;
$endpoint = "-ORBEndpoint " . "$protocol" . "://" . "$host" . ":" . $port;


$IMR_LOCATOR = new PerlACE::Process ("../../ImplRepo_Service/ImplRepo_Service");
$IMR_ACTIVATOR = new PerlACE::Process ("../../ImplRepo_Service/ImR_Activator");
$TAO_IMR = new PerlACE::Process("$ACE_ROOT/bin/tao_imr");

# We want the tao_imr executable to be found exactly in the path
# given, without being modified by the value of -ExeSubDir.
# So, we tell its Process object to ignore the setting of -ExeSubDir.

$TAO_IMR->IgnoreExeSubDir (1);

$A_SVR = new PerlACE::Process (PerlACE::LocalFile ("airplane_server"));
$A_CLI = new PerlACE::Process (PerlACE::LocalFile ("airplane_client"),
                               " -k file://$airplane_ior");
$N_SVR = new PerlACE::Process (PerlACE::LocalFile ("nestea_server"));
$N_CLI = new PerlACE::Process (PerlACE::LocalFile ("nestea_client"),
                               " -k file://$nestea_ior");

# Make sure the files are gone, so we can wait on them.
unlink $airplane_ior;
unlink $nestea_ior;
unlink $imr_locator_ior;
unlink $imr_activator_ior;
unlink $backing_store;
unlink $nestea.dat;


# The Tests

###############################################################################

sub airplane_test
{
    my $status = 0;

    $A_SVR->Arguments ("-o $airplane_ior $refstyle");
    $A_SVR->Spawn ();

    if (PerlACE::waitforfile_timed ($airplane_ior, 10) == -1) {
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

    if (PerlACE::waitforfile_timed ($nestea_ior, 10) == -1) {
        print STDERR "ERROR: cannot find $nestea_ior\n";
        $N_SVR->Kill ();
        return 1;
    }

    my $client = $N_CLI->SpawnWaitKill (10);

    if ($client != 0) {
        print STDERR "ERROR: client returned $client\n";
        $status = 1;
    }

    $server = $N_SVR->TerminateWaitKill (5);

    if ($server != 0) {
        print STDERR "ERROR: server returned $server\n";
        $status = 1;
    }

    return $status;
}

###############################################################################
# @todo: This test doesn't clean up too well if something fails

sub nt_service_test
{
    my $result = 0;

    # Just to show that it's possible, this test uses corbaloc instead of ior file.
    my $imr_initref = "-orbinitref ImplRepoService=corbaloc::localhost:8888/ImplRepoService";

    # To avoid having to ensure that they LocalSystem account has the correct path
    # we simply copy the imr executables to the same directory as the DLL's.
    my $BIN_IMR_LOCATOR = new PerlACE::Process ("$ACE_ROOT/lib/ImplRepo_Service","");
    my $BIN_IMR_ACTIVATOR = new PerlACE::Process ("$ACE_ROOT/lib/ImR_Activator","");

    print "Copying ImplRepo services to the same location as the dlls.\n";
    copy ($IMR_LOCATOR->Executable (), $BIN_IMR_LOCATOR->Executable ());
    copy ($IMR_ACTIVATOR->Executable (), $BIN_IMR_ACTIVATOR->Executable ());

    print "Stopping any existing TAO ImR Services\n";
    system("net stop taoimractivator > nul 2>&1");
    system("net stop taoimrlocator > nul 2>&1");

    print "Removing any existing TAO ImR Services\n";
    $BIN_IMR_ACTIVATOR->Arguments ("-c remove");
    $BIN_IMR_LOCATOR->Arguments ("-c remove");
    $BIN_IMR_ACTIVATOR->SpawnWaitKill (5);
    $BIN_IMR_LOCATOR->SpawnWaitKill (5);

    print "Installing TAO ImR Services\n";
    $BIN_IMR_ACTIVATOR->Arguments ("-c install $imr_initref");
    $BIN_IMR_LOCATOR->Arguments ("-c install -orbendpoint iiop://:8888");

    $result = $BIN_IMR_LOCATOR->SpawnWaitKill (5);
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
    system("net start taoimrlocator > nul 2>&1");
    system("net start taoimractivator > nul 2>&1");

    $TAO_IMR->Arguments ("add airplane_server -c \""
                         . $A_SVR->Executable () .
                         " -ORBUseIMR 1\" -w \"$ACE_ROOT/lib\" $imr_initref");

    $result = $TAO_IMR->SpawnWaitKill (10);
    if ($result != 0) {
        print STDERR "ERROR: tao_imr add airplane_server returned $result\n";
        return 1;
    }

    $A_SVR->Arguments ("-o $airplane_ior -ORBUseIMR 1 $imr_initref");
    $A_SVR->Spawn ();

    if (PerlACE::waitforfile_timed ($airplane_ior, 10) == -1) {
        print STDERR "ERROR: cannot find $airplane_ior\n";
        $A_SVR->Kill ();
        return 1;
    }

    $result = $A_CLI->SpawnWaitKill (10);
    if ($result != 0) {
        print STDERR "ERROR: airplane client returned $result\n";
        return 1;
    }

    $TAO_IMR->Arguments ("shutdown airplane_server $imr_initref");

    $result = $TAO_IMR->SpawnWaitKill (10);
    if ($result != 0) {
        print STDERR "ERROR: tao_imr shutdown airplane_server returned $result\n";
        return 1;
    }

    $result = $A_SVR->WaitKill(5);
    if ($result != 0) {
        print STDERR "ERROR: airplane_server returned $result\n";
    }

    print "Stopping TAO Implementation Repository Service\n";
    system("net stop taoimractivator > nul 2>&1");
    system("net stop taoimrlocator > nul 2>&1");

    print "Removing TAO ImR Services\n";
    $BIN_IMR_ACTIVATOR->Arguments ("-c remove");
    $BIN_IMR_ACTIVATOR->SpawnWaitKill (5);
    $BIN_IMR_LOCATOR->Arguments ("-c remove");
    $BIN_IMR_LOCATOR->SpawnWaitKill (5);

    print "Removing ImplRepo_Service copy.\n";
    unlink $BIN_IMR_ACTIVATOR->Executable ();
    unlink $BIN_IMR_LOCATOR->Executable ();

    return 0;
}

###############################################################################

sub airplane_ir_test
{
    my $status = 0;
    my $result = 0;

    my $imr_initref = "-ORBInitRef ImplRepoService=file://$imr_locator_ior";

    $IMR_LOCATOR->Arguments ("-d 1 -o $imr_locator_ior");
    $IMR_LOCATOR->Spawn ();

    if (PerlACE::waitforfile_timed ($imr_locator_ior, 5) == -1) {
        print STDERR "ERROR: cannot find $imr_locator_ior\n";
        $IMR_LOCATOR->Kill ();
        return 1;
    }

    $IMR_ACTIVATOR->Arguments ("-d 1 -o $imr_activator_ior $imr_initref");
    $IMR_ACTIVATOR->Spawn ();

    if (PerlACE::waitforfile_timed ($imr_activator_ior, 5) == -1) {
        print STDERR "ERROR: cannot find $imr_activator_ior\n";
        $IMR_ACTIVATOR->Kill ();
        return 1;
    }

    $TAO_IMR->Arguments ("$imr_initref add airplane_server -c \""
                        . $A_SVR->Executable ()
                       . " -ORBUseIMR 1 -o $airplane_ior $imr_initref\"");

    $result = $TAO_IMR->SpawnWaitKill (5);
    if ($result != 0) {
        print STDERR "ERROR: tao_imr returned $result\n";
        $IMR_ACTIVATOR->Kill ();
        $IMR_LOCATOR->Kill ();
        return 1;
    }

    $A_SVR->Arguments ("-ORBUseIMR 1 -o $airplane_ior $imr_initref");
    $A_SVR->Spawn ();

    if (PerlACE::waitforfile_timed ($airplane_ior, 10) == -1) {
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
        print STDERR "ERROR: IMR returned $implrepo\n";
        $status = 1;
    }

    my $imr_locator = $IMR_LOCATOR->TerminateWaitKill (5);
    if ($imr_locator != 0) {
        print STDERR "ERROR: IMR returned $implrepo\n";
        $status = 1;
    }

    return $status;
}

###############################################################################

sub nestea_ir_test
{
    my $status = 0;
    my $result = 0;

    my $imr_initref = "-ORBInitRef ImplRepoService=file://$imr_locator_ior";

    $IMR_LOCATOR->Arguments ("-d 1 -o $imr_locator_ior");
    $IMR_LOCATOR->Spawn ();

    if (PerlACE::waitforfile_timed ($imr_locator_ior, 10) == -1) {
      print STDERR "ERROR: cannot find $implrepo_ior\n";
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

    $TAO_IMR->Arguments ("$imr_initref add nestea_server -c \""
                         . $N_SVR->Executable ()
                         . " -ORBUseIMR 1 $imr_initref -o $nestea_ior\"");

    $result = $TAO_IMR->SpawnWaitKill (10);
    if ($result != 0) {
        print STDERR "ERROR: tao_imr returned $result\n";
        $IMR_ACTIVATOR->Kill ();
        $IMR_LOCATOR->Kill ();
        return 1;
    }

    $N_SVR->Arguments ("-ORBUseIMR 1 -o $nestea_ior $imr_initref");
    $N_SVR->Spawn ();

    if (PerlACE::waitforfile_timed ($nestea_ior, 10) == -1) {
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

    # This should cause the activator to spawn another server.
    $result = $N_CLI->SpawnWaitKill (20);
    if ($result != 0) {
        print STDERR "ERROR: nestea client 2 returned $result\n";
        $status = 1;
    }

    $result = $TAO_IMR->SpawnWaitKill (10);
    if ($result != 0) {
        print STDERR "ERROR: tao_imr 1 returned $result\n";
        $status = 1;
    }

    # Since the second server was started by the activator, it is not
    # managed by perl, and we can't wait for it to die. So sleep a few secs.
    sleep(5);

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

sub persistent_ir_test
{
    my $status = 0;
    my $result = 0;

    my $imr_initref = "-ORBInitRef ImplRepoService=file://$imr_locator_ior";

    $IMR_LOCATOR->Arguments ("-d 1 -o $imr_locator_ior");
    $IMR_LOCATOR->Spawn ();
    if (PerlACE::waitforfile_timed ($imr_locator_ior, 10) == -1) {
        print STDERR "ERROR: cannot find $implrepo_ior\n";
      $IMR_LOCATOR->Kill ();
        return 1;
    }

    $IMR_ACTIVATOR->Arguments ("-d 1 -o $imr_activator_ior -p $backing_store $imr_initref");
    $IMR_ACTIVATOR->Spawn ();

    if (PerlACE::waitforfile_timed ($imr_activator_ior, 30) == -1) {
        print STDERR "ERROR: cannot find $imr_activator_ior\n";
        $IMR_ACTIVATOR->Kill ();
        $IMR_LOCATOR->Kill ();
        return 1;
    }

    $TAO_IMR->Arguments ("$imr_initref add airplane_server -c \"" . $A_SVR->Executable () . " -ORBUseIMR 1 $refstyle $imr_initref\"");
    $result = $TAO_IMR->SpawnWaitKill (10);

    if ($result != 0) {
        print STDERR "ERROR: tao_imr returned $result\n";
        return 1;
    }

    $A_SVR->Arguments ("-o $airplane_ior -ORBUseIMR 1 $refstyle $imr_initref");
    $A_SVR->Spawn ();

    if (PerlACE::waitforfile_timed ($airplane_ior, 10) == -1) {
        print STDERR "ERROR: cannot find $airplane_ior\n";
        $IMR->Kill ();
        $A_SVR->Kill ();
        return 1;
    }

    $TAO_IMR->Arguments ("$imr_initref shutdown airplane_server");

    $result = $A_CLI->SpawnWaitKill (10);
    if ($result != 0) {
        print STDERR "ERROR: airplane client returned $result\n";
        $status = 1;
    }

    $result = $TAO_IMR->SpawnWaitKill (10);
    if ($result != 0) {
        print STDERR "ERROR: tao_imr shutdown returned $result\n";
        $status = 1;
    }

    $result = $A_SVR->WaitKill (10);
    if ($result != 0) {
        print STDERR "ERROR: airplane server returned $result\n";
        $status = 1;
    }

    # Should cause the activator to spawn another server.
    $result = $A_CLI->SpawnWaitKill (20);
    if ($result != 0) {
        print STDERR "ERROR: airplane client 2 returned $result\n";
        $status = 1;
    }

    $result = $TAO_IMR->SpawnWaitKill (10);
    if ($result != 0) {
        print STDERR "ERROR: tao_imr shutdown 2 returned $result\n";
        $status = 1;
    }

    # Since the second server was started by the activator, it is not
    # managed by perl, and we can't wait for it to die. So sleep a few secs.
    sleep(5);

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

    # Unlink so that we can wait on them again to know the servers started.
    unlink $imr_locator_ior;
    unlink $imr_activator_ior;

    print "Restarting Implementation Repository.\n";
    $IMR_LOCATOR->Spawn ();
    if (PerlACE::waitforfile_timed ($imr_locator_ior, 10) == -1) {
      print STDERR "ERROR: cannot find $implrepo_ior\n";
      $IMR_LOCATOR->Kill ();
      return 1;
    }

    # Should reload the server list without having to run tao_imr again.
    $IMR_ACTIVATOR->Spawn ();
    if (PerlACE::waitforfile_timed ($imr_activator_ior, 30) == -1) {
        print STDERR "ERROR: cannot find $imr_activator_ior\n";
        $IMR_ACTIVATOR->Kill ();
        $IMR_LOCATOR->Kill ();
        return 1;
    }

    # Should cause the activator to spawn another server.
    $result = $A_CLI->SpawnWaitKill (20);
    if ($result != 0) {
        print STDERR "ERROR: airplane client 3 returned $result\n";
        $status = 1;
    }

    $result = $TAO_IMR->SpawnWaitKill (10);
    if ($result != 0) {
        print STDERR "ERROR: tao_imr shutdown 3 returned $result\n";
        $status = 1;
    }

    # Since the second server was started by the activator, it is not
    # managed by perl, and we can't wait for it to die. So sleep a few secs.
    sleep(5);

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

sub both_ir_test
{
    my $status = 0;

    my $imr_initref = "-ORBInitRef ImplRepoService=file://$imr_locator_ior";

    $IMR_LOCATOR->Arguments ("-d 1 -o $imr_locator_ior $refstyle");
    $IMR_LOCATOR->Spawn ();

    if (PerlACE::waitforfile_timed ($imr_locator_ior, 10) == -1) {
        print STDERR "ERROR: cannot find $imr_locator_ior\n";
        $IMR_LOCATOR->Kill ();
        return 1;
    }

    $IMR_ACTIVATOR->Arguments ("-o $imr_activator_ior $imr_initref $refstyle -d 1");
    $IMR_ACTIVATOR->Spawn ();

    if (PerlACE::waitforfile_timed ($imr_activator_ior, 10) == -1) {
        print STDERR "ERROR: cannot find $imr_activator_ior\n";
        $IMR_ACTIVATOR->Kill ();
        $IMR_LOCATOR->Kill ();
        return 1;
    }

    $TAO_IMR->Arguments ("$imr_initref add nestea_server -c \""
                         . $N_SVR->Executable ()
                       . " -ORBUseIMR 1 $refstyle -o $nestea_ior $imr_initref\"");
    $TAO_IMR->SpawnWaitKill (10);

    $TAO_IMR->Arguments ("$imr_initref add airplane_server -c \""
                        . $A_SVR->Executable ()
                       . " -ORBUseIMR 1 $refstyle -o $airplane_ior $imr_initref\"");
    $TAO_IMR->SpawnWaitKill (10);

    $N_SVR->Arguments (" -o $nestea_ior -ORBUseIMR 1 $refstyle $imr_initref");
    $N_SVR->Spawn ();

    $A_SVR->Arguments (" -o $airplane_ior -ORBUseIMR 1 $refstyle $imr_initref");
    $A_SVR->Spawn ();

    if (PerlACE::waitforfile_timed ($nestea_ior, 10) == -1) {
        print STDERR "ERROR: cannot find $nestea_ior\n";
        $IMR_ACTIVATOR->Kill ();
        $IMR_LOCATOR->Kill ();
        $A_SVR->Kill ();
        $N_SVR->Kill ();
        return 1;
    }

    if (PerlACE::waitforfile_timed ($airplane_ior, 10) == -1) {
        print STDERR "ERROR: cannot find $airplane_ior\n";
        $IMR_ACTIVATOR->Kill ();
        $IMR_LOCATOR->Kill ();
        $A_SVR->Kill ();
        $N_SVR->Kill ();
        return 1;
    }

    # todo : Add error checking.

    $N_CLI->Spawn ();
    $A_CLI->Spawn ();

    $N_CLI->WaitKill (10);
    $A_CLI->WaitKill (10);

    $TAO_IMR->Arguments ("$imr_initref shutdown nestea_server");
    $TAO_IMR->SpawnWaitKill (10);
    $N_SVR->WaitKill(10);

    $N_CLI->Spawn (20);

    $TAO_IMR->Arguments ("$imr_initref shutdown airplane_server");
    $TAO_IMR->SpawnWaitKill (10);
    $A_SVR->WaitKill(10);

    $A_CLI->SpawnWaitKill (20);
    $N_CLI->WaitKill (10);

    $TAO_IMR->Arguments ("$imr_initref shutdown nestea_server");
    $TAO_IMR->SpawnWaitKill (10);

    $TAO_IMR->Arguments ("$imr_initref shutdown airplane_server");
    $TAO_IMR->SpawnWaitKill (10);

    sleep(5);

    $IMR_ACTIVATOR->Kill ();
    $IMR_LOCATOR->Kill ();
}

###############################################################################
###############################################################################

# Parse the arguments

for ($i = 0; $i <= $#ARGV; $i++) {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?") {
        print "run_test test\n";
        print "\n";
        print "test               -- Runs a specific test:\n";
        print "                         airplane, airplane_ir, nt_service, ",
                                       "nestea, nestea_ir,\n";
        print "                         both_ir, persistent_ir\n";
        exit;
    }
    elsif ($ARGV[$i] eq "airplane") {
        exit airplane_test ();
    }
    elsif ($ARGV[$i] eq "airplane_ir") {
        exit airplane_ir_test ();
    }
    elsif ($ARGV[$i] eq "nt_service_ir") {
        exit nt_service_test ();
    }
    elsif ($ARGV[$i] eq "nestea") {
        exit nestea_test ();
    }
    elsif ($ARGV[$i] eq "nestea_ir") {
        exit nestea_ir_test ();
    }
    elsif ($ARGV[$i] eq "both_ir") {
        exit both_ir_test ();
    }
    elsif ($ARGV[$i] eq "persistent_ir") {
        exit persistent_ir_test ();
    }
    else {
        print "run_test: Unknown Option: ".$ARGV[$i]."\n";
    }
}

# if nothing else, run both_ir

exit both_ir_test ();
