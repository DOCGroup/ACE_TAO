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
$implrepo_ior = PerlACE::LocalFile ("implrepo.ior");

$refstyle = " -ORBobjrefstyle URL";

$backing_store = "imr_backing_store";
$nestea_dat = "nestea.dat";

$protocol = "iiop";
$host = hostname();
$port = 12345;
$endpoint = "-ORBEndpoint " . "$protocol" . "://" . "$host" . ":" . $port;


$IMR = new PerlACE::Process ("../../ImplRepo_Service/ImplRepo_Service");

if ($^O eq "MSWin32") {
    $TAO_IMR = new PerlACE::Process ("$ACE_ROOT/bin/tao_imr");
}
else {
    $TAO_IMR = new PerlACE::Process 
                ("$ACE_ROOT/TAO/orbsvcs/ImplRepo_Service/tao_imr");
}

$A_SVR = new PerlACE::Process (PerlACE::LocalFile ("airplane_server"));
$A_CLI = new PerlACE::Process (PerlACE::LocalFile ("airplane_client"),
                               " -k file://$airplane_ior");
$N_SVR = new PerlACE::Process (PerlACE::LocalFile ("nestea_server"));
$N_CLI = new PerlACE::Process (PerlACE::LocalFile ("nestea_client"),
                               " -k file://$nestea_ior");

# Make sure the files are gone, so we can wait on them.
unlink $airplane_ior;
unlink $nestea_ior;
unlink $implrepo_ior;

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

    my $client = $A_CLI->SpawnWaitKill (300);
    
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
    unlink $nestea_dat;
    
    $N_SVR->Arguments ("-o $nestea_ior $refstyle");
    $N_SVR->Spawn ();

    if (PerlACE::waitforfile_timed ($nestea_ior, 10) == -1) {
        print STDERR "ERROR: cannot find $nestea_ior\n";
        $N_SVR->Kill ();
        return 1;
    }
    
    my $client = $N_CLI->SpawnWaitKill (300);

    if ($client != 0) {
        print STDERR "ERROR: client returned $client\n";
        $status = 1;
    }
    
    $server = $N_SVR->TerminateWaitKill (5);
    
    if ($server != 0) {
        print STDERR "ERROR: server returned $server\n";
        $status = 1;
    }
    
    unlink $nestea_dat;
    return $status;
}

###############################################################################
# @todo: This test doesn't clean up too well if something fails

sub nt_service_test
{
    my $result = 0;
    
    my $BIN_IMR = new PerlACE::Process ("$ACE_ROOT/bin/ImplRepo_Service",
                                        "-c install");
    
    print "Copying ImplRepo_Service to bin\n";
    copy ($IMR->Executable (), $BIN_IMR->Executable ());

    print "Installing TAO Implementation Repository Service\n";
    $result = $BIN_IMR->SpawnWaitKill (300);

    if ($result != 0) {
        print STDERR "ERROR: IMR installation returned $result\n";
        return 1;
    }

    print "Starting TAO Implementation Repository Service\n";
    my $NET = new PerlACE::Process ("net", 
                                    "start \"TAO Implementation Repository\"");
    $NET->IgnoreExeSubDir (1);                                    
    
    $result = $NET->SpawnWaitKill (300);
    if ($result != 0) {
        print STDERR "ERROR: net returned $result\n";
        return 1;
    }

    $TAO_IMR->Arguments ("add airplane_server -c \""
                         . $A_SVR->Executable () .
                         " -ORBUseIMR 1\" -w \"$ACE_ROOT/bin\"");
    
    $result = $TAO_IMR->SpawnWaitKill (30);
    
    if ($result != 0) {
        print STDERR "ERROR: tao_imr returned $result\n";
        return 1;
    }

    $A_SVR->Arguments ("-o $airplane_ior -ORBUseIMR 1");
    $A_SVR->Spawn ();

    if (PerlACE::waitforfile_timed ($airplane_ior, 10) == -1) {
        print STDERR "ERROR: cannot find $airplane_ior\n";
        $A_SVR->Kill ();
        return 1;
    }

    $result = $A_CLI->SpawnWaitKill (100);
    
    if ($result != 0) {
        print STDERR "ERROR: airplane client returned $result\n";
        return 1;
    }

    $TAO_IMR->Arguments ("shutdown airplane_server");

    $result = $TAO_IMR->SpawnWaitKill (30);
    
    if ($result != 0) {
        print STDERR "ERROR: tao_imr returned $result\n";
        return 1;
    }

    $result = $A_CLI->SpawnWaitKill (100);
    
    if ($result != 0) {
        print STDERR "ERROR: airplane client returned $result\n";
        return 1;
    }

    $result = $TAO_IMR->SpawnWaitKill (30);
    
    if ($result != 0) {
        print STDERR "ERROR: tao_imr returned $result\n";
        return 1;
    }

    print "Stopping TAO Implementation Repository Service\n";
    $NET->Arguments ("stop \"TAO Implementation Repository\"");
    $NET->SpawnWaitKill (300);

    print "Removing TAO Implementation Repository Service\n";
    $BIN_IMR->Arguments ("-c remove");
    $BIN_IMR->SpawnWaitKill (300);

    print "Removing ImplRepo_Service from bin\n";
    unlink $BIN_IMR->Executable ();
    
    return 0;
}

###############################################################################

sub airplane_ir_test
{
    my $status = 0;
    my $result = 0;
    
    $IMR->Arguments ("-o $implrepo_ior");
    $IMR->Spawn ();

    if (PerlACE::waitforfile_timed ($implrepo_ior, 10) == -1) {
        print STDERR "ERROR: cannot find $implrepo_ior\n";
        $IMR->Kill ();
        return 1;
    }

    $TAO_IMR->Arguments ("add airplane_server -c \""
                         . $A_SVR->Executable () 
                         . " -ORBUseIMR 1 -o $airplane_ior\"");
    
    $result = $TAO_IMR->SpawnWaitKill (30);
    
    if ($result != 0) {
        print STDERR "ERROR: tao_imr returned $result\n";
        $IMR->Kill ();
        return 1;
    }


    $A_SVR->Arguments ("-ORBUseIMR 1 -o $airplane_ior");
    $A_SVR->Spawn ();

    if (PerlACE::waitforfile_timed ($airplane_ior, 10) == -1) {
        print STDERR "ERROR: cannot find $airplane_ior\n";
        $IMR->Kill ();
        $A_SVR->Kill ();
        return 1;
    }

    $TAO_IMR->Arguments ("shutdown airplane_server");

    $result = $A_CLI->SpawnWaitKill (100);
    
    if ($result != 0) {
        print STDERR "ERROR: airplane_client 1 returned $result\n";
        $status = 1;
    }
    
    $result = $TAO_IMR->SpawnWaitKill (30);
        
    if ($result != 0) {
        print STDERR "ERROR: tao_imr 1 returned $result\n";
        $status = 1;
    }

    $result = $A_CLI->SpawnWaitKill (100);

    if ($result != 0) {
        print STDERR "ERROR: airplane_client 2 returned $result\n";
        $status = 1;
    }
        
    $result = $TAO_IMR->SpawnWaitKill (30);

    if ($result != 0) {
        print STDERR "ERROR: tao_imr 2 returned $result\n";
        $status = 1;
    }

    my $server = $A_SVR->WaitKill (5);
    
    if ($server != 0) {
        print STDERR "ERROR: airplane server returned $server\n";
        $status = 1;
    }

    my $implrepo = $IMR->TerminateWaitKill (5);
    
    if ($implrepo != 0) {
        print STDERR "ERROR: IMR returned $implrepo\n";
        $status = 1;
    }
    
    return $status;
}

###############################################################################

sub nestea_ir_test
{
    unlink $nestea_dat;
    my $status = 0;
    my $result = 0;
    
    $IMR->Arguments ("-o $implrepo_ior");
    $IMR->Spawn ();

    if (PerlACE::waitforfile_timed ($implrepo_ior, 10) == -1) {
        print STDERR "ERROR: cannot find $implrepo_ior\n";
        $IMR->Kill ();
        return 1;
    }

    $TAO_IMR->Arguments ("add nestea_server -c \""
                         . $N_SVR->Executable () 
                         . " -ORBUseIMR 1 -o $nestea_ior\"");
    
    $result = $TAO_IMR->SpawnWaitKill (30);
    
    if ($result != 0) {
        print STDERR "ERROR: tao_imr returned $result\n";
        $IMR->Kill ();
        return 1;
    }


    $N_SVR->Arguments ("-ORBUseIMR 1 -o $nestea_ior");
    $N_SVR->Spawn ();

    if (PerlACE::waitforfile_timed ($nestea_ior, 10) == -1) {
        print STDERR "ERROR: cannot find $nestea_ior\n";
        $IMR->Kill ();
        $N_SVR->Kill ();
        return 1;
    }

    $TAO_IMR->Arguments ("shutdown nestea_server");

    $result = $N_CLI->SpawnWaitKill (100);
    
    if ($result != 0) {
        print STDERR "ERROR: nestea client 1 returned $result\n";
        $status = 1;
    }
    
    $result = $TAO_IMR->SpawnWaitKill (30);
        
    if ($result != 0) {
        print STDERR "ERROR: tao_imr 1 returned $result\n";
        $status = 1;
    }

    $result = $N_CLI->SpawnWaitKill (100);

    if ($result != 0) {
        print STDERR "ERROR: nestea client 2 returned $result\n";
        $status = 1;
    }
        
    $result = $TAO_IMR->SpawnWaitKill (30);

    if ($result != 0) {
        print STDERR "ERROR: tao_imr 2 returned $result\n";
        $status = 1;
    }

    my $server = $N_SVR->WaitKill (5);
    
    if ($server != 0) {
        print STDERR "ERROR: nestea server returned $server\n";
        $status = 1;
    }

    my $implrepo = $IMR->TerminateWaitKill (5);
    
    if ($implrepo != 0) {
        print STDERR "ERROR: IMR returned $implrepo\n";
        $status = 1;
    }

    unlink $nestea_dat;    
    return $status;
}

###############################################################################

sub persistent_ir_test
{
    my $status = 0;
    my $result = 0;
    unlink $backing_store;
 
    $IMR->Arguments ("$endpoint -o $implrepo_ior -p $backing_store -d 0");
    $IMR->Spawn ();

    if (PerlACE::waitforfile_timed ($implrepo_ior, 10) == -1) {
        print STDERR "ERROR: cannot find $implrepo_ior\n";
        $IMR->Kill ();
        return 1;
    }

    $TAO_IMR->Arguments ("-ORBInitRef ImplRepoService=file://$implrepo_ior add airplane_server -c \"".$A_SVR->Executable ()." -ORBUseIMR 1 $refstyle -ORBInitRef ImplRepoService=file://$implrepo_ior\"");
    $result = $TAO_IMR->SpawnWaitKill (30);
    
    if ($result != 0) {
        print STDERR "ERROR: tao_imr returned $result\n";
        $IMR->Kill ();
        return 1;
    }

    $A_SVR->Arguments ("-o $airplane_ior -ORBUseIMR 1 $refstyle -ORBInitRef ImplRepoService=file://$implrepo_ior");
    $A_SVR->Spawn ();

    if (PerlACE::waitforfile_timed ($airplane_ior, 10) == -1) {
        print STDERR "ERROR: cannot find $airplane_ior\n";
        $IMR->Kill ();
        $A_SVR->Kill ();
        return 1;
    }

    $TAO_IMR->Arguments ("-ORBInitRef ImplRepoService=file://$implrepo_ior shutdown airplane_server");
    
    $result = $A_CLI->SpawnWaitKill (100);

    if ($result != 0) {
        print STDERR "ERROR: airplane client returned $result\n";
        $status = 1;
    }

    $result = $TAO_IMR->SpawnWaitKill (30);
    
    if ($result != 0) {
        print STDERR "ERROR: tao_imr returned $result\n";
        $status = 1;
    }

    $result = $A_CLI->SpawnWaitKill (100);

    if ($result != 0) {
        print STDERR "ERROR: airplane client returned $result\n";
        $status = 1;
    }

    $result = $TAO_IMR->SpawnWaitKill (30);
    
    if ($result != 0) {
        print STDERR "ERROR: tao_imr returned $result\n";
        $status = 1;
    }

    $result = $A_SVR->WaitKill (10);

    if ($result != 0) {
        print STDERR "ERROR: airplane server returned $result\n";
        $status = 1;
    }

    print "\nShutting down Implementation Repository\n\n";
    $IMR->Kill (); 

    print "Restarting Implementation Repository.\n";
    $IMR->Arguments ("$endpoint -p $backing_store -d 0");
    $IMR->Spawn ();

    PerlACE::waitforfile ($implrepo_ior);
    
    $A_CLI->SpawnWaitKill (100);
    $TAO_IMR->SpawnWaitKill (30);

    $IMR->Kill (); 

    unlink $backing_store;
}

###############################################################################

sub both_ir_test
{
    my $status = 0;
    $IMR->Arguments ("-o $implrepo_ior -d 0 $refstyle");
    $IMR->Spawn ();

    if (PerlACE::waitforfile_timed ($implrepo_ior, 10) == -1) {
        print STDERR "ERROR: cannot find $implrepo_ior\n";
        $IMR->Kill ();
        return 1;
    }

    $TAO_IMR->Arguments ("-ORBInitRef ImplRepoService=file://$implrepo_ior add airplane_server -c \""
                          . $A_SVR->Executable ()
                          . " -ORBUseIMR 1 $refstyle -ORBInitRef ImplRepoService=file://$implrepo_ior\"");
    $TAO_IMR->SpawnWaitKill (30);

    $TAO_IMR->Arguments ("-ORBInitRef ImplRepoService=file://$implrepo_ior"
                         . " add nestea_server"
                         . " -c \"" . $N_SVR->Executable ()
                           . " -ORBUseIMR 1"
                           . " $refstyle -ORBInitRef"
                           . " ImplRepoService=file://$implrepo_ior\"");
    $TAO_IMR->SpawnWaitKill (30);

    $N_SVR->Arguments ("-o $nestea_ior -ORBUseIMR 1 $refstyle -ORBInitRef ImplRepoService=file://$implrepo_ior");
    $N_SVR->Spawn ();

    $A_SVR->Arguments ("-o $airplane_ior -ORBUseIMR 1 $refstyle -ORBInitRef ImplRepoService=file://$implrepo_ior");
    $A_SVR->Spawn ();

    if (PerlACE::waitforfile_timed ($airplane_ior, 10) == -1) {
        print STDERR "ERROR: cannot find $airplane_ior\n";
        $IMR->Kill ();
        $A_SVR->Kill ();
        $N_SVR->Kill ();
        return 1;
    }

    if (PerlACE::waitforfile_timed ($nestea_ior, 10) == -1) {
        print STDERR "ERROR: cannot find $nestea_ior\n";
        $IMR->Kill ();
        $A_SVR->Kill ();
        $N_SVR->Kill ();
        return 1;
    }

    $N_CLI->Spawn ();
    $A_CLI->Spawn ();

    $N_CLI->WaitKill (100);
    $A_CLI->WaitKill (100);

    $TAO_IMR->Arguments ("-ORBInitRef ImplRepoService=file://$implrepo_ior shutdown nestea_server");
    $TAO_IMR->SpawnWaitKill (30);

    $N_CLI->Spawn (100);
    
    $TAO_IMR->Arguments ("-ORBInitRef ImplRepoService=file://$implrepo_ior shutdown airplane_server");
    $TAO_IMR->SpawnWaitKill (30);

    $A_CLI->SpawnWaitKill (100);
    $N_CLI->WaitKill (100);

    $TAO_IMR->Arguments ("-ORBInitRef ImplRepoService=file://$implrepo_ior shutdown nestea_server");
    $TAO_IMR->SpawnWaitKill (30);
    $TAO_IMR->Arguments ("-ORBInitRef ImplRepoService=file://$implrepo_ior shutdown airplane_server");
    $TAO_IMR->SpawnWaitKill (30);

    $A_SVR->Kill (); 
    $N_SVR->Kill (); 
    $IMR->Kill (); 
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
    elsif ($ARGV[$i] eq "nt_service") {
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
