eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

###############################################################################
use Cwd;
use Sys::Hostname;
use File::Copy;

BEGIN {
    ### We need to BEGIN this block so we make sure ACE_ROOT is set before
    ### we use it in the use lib line
    $cwd = getcwd();

    $ACE_ROOT = $ENV{ACE_ROOT};

    if (!$ACE_ROOT) {
        chdir ('../../../../');
        $ACE_ROOT = getcwd ();
        chdir ($cwd);
        print "ACE_ROOT not defined, defaulting to ACE_ROOT=$ACE_ROOT\n";
    }
}

use lib "$ACE_ROOT/bin";

use PerlACE::Run_Test;

$airplane_ior = PerlACE::LocalFile ("airplane.ior");
$nestea_ior = PerlACE::LocalFile ("nestea.ior");
$implrepo_ior = PerlACE::LocalFile ("implrepo.ior");

$refstyle = " -ORBobjrefstyle URL";

$backing_store = "imr_backing_store";

$protocol = "iiop";
$host = hostname();
$port = 12345;
$endpoint = "-ORBEndpoint" . "$protocol" . "://" . "$host" . ":" . $port;


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

sub convert_slash($)
{
    $cmd = shift;

    $cmd =~ s/\//\\/g;

    return $cmd;
}

# The Tests

###############################################################################

sub airplane_test
{
    $A_SVR->Arguments ("-o $airplane_ior $refstyle");
    $A_SVR->Spawn ();

    PerlACE::waitforfile ($airplane_ior);

    $A_CLI->SpawnWaitKill (300);

    $A_SVR->Kill (); $A_SVR->Wait ();
}

###############################################################################

sub nestea_test
{
    $N_SVR->Arguments ("-o $nestea_ior $refstyle");
    $N_SVR->Spawn ();

    PerlACE::waitforfile ($nestea_ior);

    $N_CLI->SpawnWaitKill (300);

    $N_SVR->Kill (); $N_SVR->Wait ();
}

###############################################################################

sub nt_service_test
{
    my $BIN_IMR = new PerlACE::Process (convert_slash ("$ACE_ROOT/bin/ImplRepo_Service"),
                                        "-c install");
    
    print "Copying ImplRepo_Service to bin\n";
    copy ($IMR->Executable (), $BIN_IMR->Executable ());

    print "Installing TAO Implementation Repository Service\n";
    $BIN_IMR->SpawnWaitKill (300);

    print "Starting TAO Implementation Repository Service\n";
    my $NET = new PerlACE::Process ($ENV{'SystemRoot'}."/System32/net", 
                                    "start \"TAO Implementation Repository\"");
    $NET->SpawnWaitKill (300);

    $TAO_IMR->Arguments ("add airplane_server -c \""
                         . $A_SVR->Executable () .
                         " -ORBUseIMR 1\" -w \"$ACE_ROOT/bin\"");
    $TAO_IMR->SpawnWaitKill (100);

    $A_SVR->Arguments ("-o $airplane_ior -ORBUseIMR 1");
    $A_SVR->Spawn ();

    PerlACE::waitforfile ($airplane_ior);

    $A_CLI->SpawnWaitKill (100);

    $TAO_IMR->Arguments ("shutdown airplane_server");
    $TAO_IMR->SpawnWaitKill (100);

    $A_CLI->SpawnWaitKill (100);

    $TAO_IMR->SpawnWaitKill (100);

    print "Stopping TAO Implementation Repository Service\n";
    $NET->Arguments ("stop \"TAO Implementation Repository\"");
    $NET->SpawnWaitKill (300);

    print "Removing TAO Implementation Repository Service\n";
    $BIN_IMR->Arguments ("-c remove");
    $BIN_IMR->SpawnWaitKill (300);

    print "Removing ImplRepo_Service from bin\n";
    unlink ($bin_implrepo_server);
}

###############################################################################

sub airplane_ir_test
{
    $IMR->Arguments ("-o $implrepo_ior");
    $IMR->Spawn ();

    PerlACE::waitforfile ($implrepo_ior);

    $TAO_IMR->Arguments ("add airplane_server -c \""
                         . $A_SVR->Executable () 
                         . " -ORBUseIMR 1 -o $airplane_ior\"");
    $TAO_IMR->SpawnWaitKill (300);

    $A_SVR->Arguments ("-ORBUseIMR 1 -o $airplane_ior");
    $A_SVR->Spawn ();

    PerlACE::waitforfile ($airplane_ior);

    $TAO_IMR->Arguments ("shutdown airplane_server");

    $A_CLI->SpawnWaitKill (100);
    $TAO_IMR->SpawnWaitKill (100);
    $A_CLI->SpawnWaitKill (100);
    $TAO_IMR->SpawnWaitKill (100);

    $IMR->Kill (); $IMR->Wait ();
}

###############################################################################

sub nestea_ir_test
{
    $IMR->Arguments ("-o $implrepo_ior -d 0 $refstyle");
    $IMR->Spawn ();

    PerlACE::waitforfile ($implrepo_ior);

    $TAO_IMR->Arguments ("add nestea_server -c \""
                         . $N_SVR->Executable () 
                         . " -ORBUseIMR 1 -o $nestea_ior\"");
    $TAO_IMR->SpawnWaitKill (300);

    $N_SVR->Arguments ("-ORBUseIMR 1 -o $nestea_ior");
    $N_SVR->Spawn ();

    PerlACE::waitforfile ($nestea_ior);

    $TAO_IMR->Arguments ("shutdown nestea_server");

    $N_CLI->SpawnWaitKill (100);
    $TAO_IMR->SpawnWaitKill (100);
    $N_CLI->SpawnWaitKill (100);
    $TAO_IMR->SpawnWaitKill (100);

    $IMR->Kill (); $IMR->Wait ();
}

###############################################################################

sub persistent_ir_test
{
    unlink $backing_store;
 
    $IMR->Arguments ("$endpoint -o $implrepo_ior -p $backing_store -d 0");
    $IMR->Spawn ();

    PerlACE::waitforfile ($implrepo_ior);

    $TAO_IMR->Arguments ("-ORBInitRef ImplRepoService=file://$implrepo_ior add airplane_server -c \"".$A_SVR->Executable ()." -ORBUseIMR 1 $refstyle -ORBInitRef ImplRepoService=file://$implrepo_ior\"");
    $TAO_IMR->SpawnWaitKill (100);

    $A_SVR->Arguments ("-o $airplane_ior -ORBUseIMR 1 $refstyle -ORBInitRef ImplRepoService=file://$implrepo_ior");
    $A_SVR->Spawn ();

    PerlACE::waitforfile ($airplane_ior);
    $TAO_IMR->Arguments ("-ORBInitRef ImplRepoService=file://$implrepo_ior shutdown airplane_server");

    $A_CLI->SpawnWaitKill (100);
    $TAO_IMR->SpawnWaitKill (100);
    $A_CLI->SpawnWaitKill (100);
    $TAO_IMR->SpawnWaitKill (100);

    print "\nShutting down Implementation Repository\n\n";
    $IMR->Kill (); $IMR->Wait ();

    print "Restarting Implementation Repository.\n";
    $IMR->Arguments ("$endpoint -p $backing_store -d 0");
    $IMR->Spawn ();

    PerlACE::waitforfile ($implrepo_ior);
    
    $A_CLI->SpawnWaitKill (100);
    $TAO_IMR->SpawnWaitKill (100);

    $IMR->Kill (); $IMR->Wait ();

    unlink $backing_store;
}

###############################################################################

sub both_ir_test
{
    $IMR->Arguments ("-o $implrepo_ior -d 0 $refstyle");
    $IMR->Spawn ();

    PerlACE::waitforfile ($implrepo_ior);

    $TAO_IMR->Arguments ("-ORBInitRef ImplRepoService=file://$implrepo_ior add airplane_server -c \""
                          . $A_SVR->Executable ()
                          . " -ORBUseIMR 1 $refstyle -ORBInitRef ImplRepoService=file://$implrepo_ior\"");
    $TAO_IMR->SpawnWaitKill (100);

    $TAO_IMR->Arguments ("-ORBInitRef ImplRepoService=file://$implrepo_ior"
                         . " add nestea_server"
                         . " -c \"" . $N_SVR->Executable ()
                           . " -ORBUseIMR 1"
                           . " $refstyle -ORBInitRef"
                           . " ImplRepoService=file://$implrepo_ior\"");
    $TAO_IMR->SpawnWaitKill (100);

    $N_SVR->Arguments ("-o $nestea_ior -ORBUseIMR 1 $refstyle -ORBInitRef ImplRepoService=file://$implrepo_ior");
    $N_SVR->Spawn ();

    $A_SVR->Arguments ("-o $airplane_ior -ORBUseIMR 1 $refstyle -ORBInitRef ImplRepoService=file://$implrepo_ior");
    $A_SVR->Spawn ();

    PerlACE::waitforfile ($nestea_ior);
    PerlACE::waitforfile ($airplane_ior);

    $N_CLI->Spawn ();
    $A_CLI->Spawn ();

    $N_CLI->WaitKill (100);
    $A_CLI->WaitKill (100);

    $TAO_IMR->Arguments ("-ORBInitRef ImplRepoService=file://$implrepo_ior shutdown nestea_server");
    $TAO_IMR->SpawnWaitKill (100);

    $N_CLI->Spawn (100);
    
    $TAO_IMR->Arguments ("-ORBInitRef ImplRepoService=file://$implrepo_ior shutdown airplane_server");
    $TAO_IMR->SpawnWaitKill (100);

    $A_CLI->SpawnWaitKill (100);
    $N_CLI->WaitKill (100);

    $TAO_IMR->Arguments ("-ORBInitRef ImplRepoService=file://$implrepo_ior shutdown nestea_server");
    $TAO_IMR->SpawnWaitKill (100);
    $TAO_IMR->Arguments ("-ORBInitRef ImplRepoService=file://$implrepo_ior shutdown airplane_server");
    $TAO_IMR->SpawnWaitKill (100);

    $A_SVR->Kill (); $A_SVR->Wait ();
    $N_SVR->Kill (); $N_SVR->Wait ();
    $IMR->Kill (); $IMR->Wait ();
}

###############################################################################
###############################################################################

# Parse the arguments

for ($i = 0; $i <= $#ARGV; $i++)
{
  SWITCH:
  {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?")
    {
      print "run_test [-chorus <target>] test\n";
      print "\n";
      print "-chorus <target>   -- Runs the test on Chorus target\n";
      print "test               -- Runs a specific test:\n";
      print "                         airplane, airplane_ir, nt_service, ",
                                      "nestea, nestea_ir,\n";
      print "                         both_ir, persistent_ir\n";
      exit;
    }
    if ($ARGV[$i] eq "airplane")
    {
      airplane_test ();
      exit;
    }
    if ($ARGV[$i] eq "airplane_ir")
    {
      airplane_ir_test ();
      exit;
    }
    if ($ARGV[$i] eq "nt_service")
    {
      nt_service_test ();
      exit;
    }
    if ($ARGV[$i] eq "nestea")
    {
      nestea_test ();
      exit;
    }
    if ($ARGV[$i] eq "nestea_ir")
    {
      nestea_ir_test ();
      exit;
    }
    if ($ARGV[$i] eq "both_ir")
    {
      both_ir_test ();
      exit;
    }
    if ($ARGV[$i] eq "persistent_ir")
    {
      persistent_ir_test ();
      exit;
    }
    print "run_test: Unknown Option: ".$ARGV[$i]."\n";
  }
}

both_ir_test ();
