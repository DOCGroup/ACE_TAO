eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../bin";

require ACEutils;

$airplane_ior = "airplane.ior";
$nestea_ior = "nestea.ior";
$implrepo_ior = "implrepo.ior";

$implrepo_server = "..".$DIR_SEPARATOR."..".$DIR_SEPARATOR."ImplRepo_Service".
                   $DIR_SEPARATOR."ImplRepo_Service".$EXE_EXT;
$tao_ir = "tao_ir".$EXE_EXT;
$airplane_server = $EXEPREFIX."airplane_server".$EXE_EXT;
$airplane_client = $EXEPREFIX."airplane_client".$EXE_EXT;
$nestea_server = $EXEPREFIX."nestea_server".$EXE_EXT;
$nestea_client = $EXEPREFIX."nestea_client".$EXE_EXT;

# Make sure the files are gone, so we can wait on them.
unlink $airplane_ior;
unlink $nestea_ior;
unlink $implrepo_ior;

# The Tests

sub airplane_test
{
  $SV = Process::Create ($airplane_server,
                         "-o $airplane_ior");

  ACE::waitforfile ($airplane_ior);

  $status = system ($airplane_client.
                    " -k file://$airplane_ior");

  $SV->Kill (); $SV->Wait ();
}

sub airplane_ir_test
{
  $IR = Process::Create ($implrepo_server, 
                         "-ORBobjrefstyle url -o $implrepo_ior -d 0");

  ACE::waitforfile ($implrepo_ior);

  system ($tao_ir." -ORBImplRepoIOR file://$implrepo_ior add plane -c \"airplane_server -i -ORBImplRepoIOR file://$implrepo_ior\"");

  $SV = Process::Create ($airplane_server,
                         "-o $airplane_ior -i -ORBobjrefstyle url -ORBImplRepoIOR file://$implrepo_ior");

  ACE::waitforfile ($airplane_ior);

  system($airplane_client." -k file://$airplane_ior");

  system($tao_ir." -ORBImplRepoIOR file://$implrepo_ior shutdown plane");

  system($airplane_client." -k file://$airplane_ior");

  $SV->Kill (); $SV->Wait ();
  $IR->Kill (); $IR->Wait ();
}

sub nestea_test
{
  $SV = Process::Create ($nestea_server,
                         "-o $nestea_ior");

  ACE::waitforfile ($nestea_ior);

  $status = system ($nestea_client.
                    " -k file://$nestea_ior");

  $SV->Kill (); $SV->Wait ();
}

sub nestea_ir_test
{
  $IR = Process::Create ($implrepo_server, 
                         "-ORBobjrefstyle url -o $implrepo_ior -d 0");

  ACE::waitforfile ($implrepo_ior);

  system ($tao_ir." -ORBImplRepoIOR file://$implrepo_ior add nestea_server -c \"nestea_server -i -ORBImplRepoIOR file://$implrepo_ior\"");

  $SV = Process::Create ($nestea_server,
                         "-o $nestea_ior -i -ORBobjrefstyle url -ORBImplRepoIOR file://$implrepo_ior");

  ACE::waitforfile ($nestea_ior);

  system ($nestea_client." -k file://$nestea_ior");

  system ($tao_ir." -ORBImplRepoIOR file://$implrepo_ior shutdown nestea_server");

  system ($nestea_client." -k file://$nestea_ior");

  $SV->Kill (); $SV->Wait ();
  $IR->Kill (); $IR->Wait ();
}

sub both_ir_test
{
  $IR = Process::Create ($implrepo_server, 
                         "-ORBobjrefstyle url -o $implrepo_ior -d 0");

  ACE::waitforfile ($implrepo_ior);

  system ($tao_ir." -ORBImplRepoIOR file://$implrepo_ior add plane -c \"airplane_server -i -ORBImplRepoIOR file://$implrepo_ior\"");
  system ($tao_ir." -ORBImplRepoIOR file://$implrepo_ior add nestea -c \"nestea_server -i -ORBImplRepoIOR file://$implrepo_ior\"");

  $ASV = Process::Create ($nestea_server,
                         "-o $nestea_ior -i -ORBobjrefstyle url -ORBImplRepoIOR file://$implrepo_ior");
  $NSV = Process::Create ($airplane_server,
                         "-o $airplane_ior -i -ORBobjrefstyle url -ORBImplRepoIOR file://$implrepo_ior");

  ACE::waitforfile ($nestea_ior);

  $NCL = Process::Create ($nestea_client, "-k file://$nestea_ior");

  ACE::waitforfile ($airplane_ior);

  system($airplane_client." -k file://$airplane_ior");

  $ASV->Kill (); $ASV->Wait ();
  $NSV->Kill (); $NSV->Wait ();
  $NCL->Kill (); $NCL->Wait ();
  $IR->Kill (); $IR->Wait ();
}


# Parse the arguments

for ($i = 0; $i <= $#ARGV; $i++)
{
  SWITCH:
  {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?")
    {
      print "run_test test\n";
      print "\n";
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

    print "run_test: Unknown Option: ".$ARGV[$i]."\n";
  }
}

print "Specify airplane, airplane_ir, nestea, nestea_ir, or both_ir\n"
