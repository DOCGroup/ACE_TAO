eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../bin";

require ACEutils;
use Cwd;
use Sys::Hostname;

$cwd = getcwd();
$airplane_ior = "$cwd$DIR_SEPARATOR" . "airplane.ior";
$nestea_ior = "$cwd$DIR_SEPARATOR" . "nestea.ior";
$implrepo_ior = "$cwd$DIR_SEPARATOR" . "implrepo.ior";

$refstyle = " -ORBobjrefstyle URL";

$backing_store = "imr_backing_store";

$protocol = "iiop";
$host = hostname();
$port = 12345;
$endpoint = "-ORBEndpoint" . "$protocol" . "://" . "$host" . ":" . $port;

ACE::checkForTarget($cwd);

$implrepo_server = $EXEPREFIX."..".$DIR_SEPARATOR."..".$DIR_SEPARATOR."ImplRepo_Service".
                   $DIR_SEPARATOR."ImplRepo_Service".$EXE_EXT;

$nt_implrepo_server = $EXEPREFIX."..".$DIR_SEPARATOR."..".$DIR_SEPARATOR."ImplRepo_Service".
                      $DIR_SEPARATOR."NT_ImplRepo_Service".$EXE_EXT;

if ($^O eq "MSWin32")
{
  $tao_imr = "tao_imr".$EXE_EXT;
}
else
{
  $tao_imr = $EXEPREFIX."..".$DIR_SEPARATOR."..".$DIR_SEPARATOR.
             "ImplRepo_Service".$DIR_SEPARATOR."tao_imr".$EXE_EXT;
}

$airplane_server = $EXEPREFIX."airplane_server".$EXE_EXT." ";
$airplane_client = $EXEPREFIX."airplane_client".$EXE_EXT." ";
$nestea_server = $EXEPREFIX."nestea_server".$EXE_EXT." ";
$nestea_client = $EXEPREFIX."nestea_client".$EXE_EXT." ";

$airplane_path = $ENV{"ACE_ROOT"}.$DIR_SEPARATOR."TAO".$DIR_SEPARATOR.
                 "orbsvcs".$DIR_SEPARATOR."tests".$DIR_SEPARATOR."ImplRepo".
		 $DIR_SEPARATOR."airplane_server";
$working_directory = $ENV{"ACE_ROOT"}.$DIR_SEPARATOR."TAO".$DIR_SEPARATOR.
                     "orbsvcs".$DIR_SEPARATOR."tests".$DIR_SEPARATOR."ImplRepo";

# Make sure the files are gone, so we can wait on them.
unlink $airplane_ior;
unlink $nestea_ior;
unlink $implrepo_ior;

# The Tests

sub airplane_test
{
  $SV = Process::Create ($airplane_server,
                         "-o $airplane_ior $refstyle");

  ACE::waitforfile ($airplane_ior);

  $status = system ($airplane_client." -k file://$airplane_ior");

  $SV->Kill (); $SV->Wait ();
}

sub nt_service_ir_test
{
  system ($nt_implrepo_server." -i");

  system ($nt_implrepo_server." -s");

  system ($tao_imr." add airplane_server -c \"$airplane_path -ORBUseIMR 1\" -w $working_directory");

  $SV = Process::Create ($airplane_server,
                        "-o $airplane_ior -ORBUseIMR 1");

  ACE::waitforfile ($airplane_ior);

  system($airplane_client." -k file://$airplane_ior");

  system($tao_imr." shutdown airplane_server");

  system($airplane_client." -k file://$airplane_ior");

  system($tao_imr." shutdown airplane_server");

  system ($nt_implrepo_server." -k");

  system ($nt_implrepo_server." -r");
}

sub airplane_ir_test
{
  $IR = Process::Create ($implrepo_server, 
                         "-o $implrepo_ior");

  ACE::waitforfile ($implrepo_ior);

  system ($tao_imr." add airplane_server -c \"$airplane_server -ORBUseIMR 1 -o $airplane_ior\"");

  $SV = Process::Create ($airplane_server,
                         "-ORBUseIMR 1 -o $airplane_ior");

  ACE::waitforfile ($airplane_ior);

  system($airplane_client." -k file://$airplane_ior");

  system($tao_imr." shutdown airplane_server");

  system($airplane_client." -k file://$airplane_ior");

  system($tao_imr." shutdown airplane_server");

  $IR->Kill (); $IR->Wait ();
}

sub nestea_test
{
  $SV = Process::Create ($nestea_server, "-o $nestea_ior $refstyle");

  ACE::waitforfile ($nestea_ior);

  $status = system ($nestea_client." -k file://$nestea_ior");

  $SV->Kill (); $SV->Wait ();
}

sub nestea_ir_test
{
  $IR = Process::Create ($implrepo_server," -o $implrepo_ior -d 0 $refstyle");

  ACE::waitforfile ($implrepo_ior);

  system ($tao_imr." -ORBInitRef ImplRepoService=file://$implrepo_ior add nestea_server -c \"$nestea_server -ORBUseIMR 1 $refstyle -ORBInitRef ImplRepoService=file://$implrepo_ior\"");

  system ($tao_imr." -ORBInitRef ImplRepoService=file://$implrepo_ior ior nestea_server -f $nestea_ior");

  system ($nestea_client." -k file://$nestea_ior");

  system ($tao_imr." -ORBInitRef ImplRepoService=file://$implrepo_ior shutdown nestea_server");

  system ($nestea_client." -k file://$nestea_ior");

  system ($tao_imr." -ORBInitRef ImplRepoService=file://$implrepo_ior shutdown nestea_server");

  $IR->Kill (); $IR->Wait ();
}

sub persistent_ir_test
{
  unlink $backing_store;
 
  $IR = Process::Create ($implrepo_server," $endpoint -o $implrepo_ior -p $backing_store -d 0");

  ACE::waitforfile ($implrepo_ior);

  system ($tao_imr." -ORBInitRef ImplRepoService=file://$implrepo_ior add airplane_server -c \"$airplane_server -ORBUseIMR 1 $refstyle -ORBInitRef ImplRepoService=file://$implrepo_ior\"");

  $SV = Process::Create ($airplane_server,
                         "-o $airplane_ior -ORBUseIMR 1 $refstyle -ORBInitRef ImplRepoService=file://$implrepo_ior");

  ACE::waitforfile ($airplane_ior);

  system($airplane_client." -k file://$airplane_ior");

  system($tao_imr." -ORBInitRef ImplRepoService=file://$implrepo_ior shutdown airplane_server");

  system($airplane_client." -k file://$airplane_ior");

  system($tao_imr." -ORBInitRef ImplRepoService=file://$implrepo_ior shutdown airplane_server");

  print "\nShutting down Implementation Repository\n\n";

  $IR->Kill (); $IR->Wait ();

  print "Restarting Implementation Repository.\n";

  $IR_NEW = Process::Create ($implrepo_server," $endpoint -p $backing_store -d 0");

  ACE::waitforfile ($implrepo_ior);

  system($airplane_client." -k file://$airplane_ior");

  system($tao_imr." -ORBInitRef ImplRepoService=file://$implrepo_ior shutdown airplane_server");

  $IR_NEW->Kill (); $IR_NEW->Wait ();

  unlink $backing_store;
}

sub both_ir_test
{
  $IR = Process::Create ($implrepo_server, "-o $implrepo_ior -d 0 $refstyle");
  ACE::waitforfile ($implrepo_ior);

  system ($tao_imr." -ORBInitRef ImplRepoService=file://$implrepo_ior add airplane_server -c \"$airplane_server -ORBUseIMR 1 $refstyle -ORBInitRef ImplRepoService=file://$implrepo_ior\"");
  system ($tao_imr." -ORBInitRef ImplRepoService=file://$implrepo_ior add nestea_server -c \"$nestea_server -ORBUseIMR 1 $refstyle -ORBInitRef ImplRepoService=file://$implrepo_ior\"");

  $ASV = Process::Create ($nestea_server,
                         "-o $nestea_ior -ORBUseIMR 1 $refstyle -ORBInitRef ImplRepoService=file://$implrepo_ior");
  $NSV = Process::Create ($airplane_server,
                         "-o $airplane_ior -ORBUseIMR 1 $refstyle -ORBInitRef ImplRepoService=file://$implrepo_ior");

  ACE::waitforfile ($nestea_ior);

  $NCL = Process::Create ($nestea_client, "-k file://$nestea_ior");
  $ACL = Process::Create ($airplane_client, "-k file://$airplane_ior");

  $ncl_status = $NCL->TimedWait (300);
  system ($tao_imr." -ORBInitRef ImplRepoService=file://$implrepo_ior shutdown nestea_server");

  $NCL = Process::Create ($nestea_client, "-k file://$nestea_ior");

  $acl_status = $ACL->TimedWait (300);

  system ($tao_imr." -ORBInitRef ImplRepoService=file://$implrepo_ior shutdown airplane_server");

  $ACL = Process::Create ($airplane_client, "-k file://$airplane_ior");

  $ncl_status = $NCL->TimedWait (300);
  $acl_status = $ACL->TimedWait (300);

  system ($tao_imr." -ORBInitRef ImplRepoService=file://$implrepo_ior shutdown nestea_server");
  system ($tao_imr." -ORBInitRef ImplRepoService=file://$implrepo_ior shutdown airplane_server");

  $ASV->Kill (); $ASV->Wait ();
  $NSV->Kill (); $NSV->Wait ();
  $NCL->Kill (); $NCL->Wait ();
  $ACL->Kill (); $ACL->Wait ();
  $IR->Kill (); $IR->Wait ();
}


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
      print "test               -- Runs a specific test\n";
      print "                      airplane, airplane_ir, nestea, nestea_ir, both_ir\n";
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
    if ($ARGV[$i] eq "nt_service_ir")
    {
      nt_service_ir_test ();
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

print "Specify airplane, airplane_ir, nestea, nestea_ir, or both_ir\n"
