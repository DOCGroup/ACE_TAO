#$Id$
# -*- perl -*-
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use lib "../../../../bin";

require ACEutils;

$airplane_ior = "airplane.ior";
$nestea_ior = "nestea.ior";
$implrepo_ior = "implrepo.ior";

# Make sure the files are gone, so we can wait on them.
unlink $airplane_ior;
unlink $nestea_iro;
unlink $implrepo_ior;

# The Tests

sub airplane_test
{
  $SV = Process::Create ($EXEPREFIX."airplane_server".$Process::EXE_EXT,
                         "-o $airplane_ior -r");

  ACE::waitforfile ($airplane_ior);

  $status = system ($EXEPREFIX."airplane_client".$Process::EXE_EXT.
                    " -k file://$airplane_ior");

  $SV->Kill (); $SV->Wait ();
  unlink $airplane_ior;
}

sub airplane_ir_test
{
  $IR = Process::Create ("..".$DIR_SEPARATOR."..".$DIR_SEPARATOR."ImplRepo_Service".$DIR_SEPARATOR."ImplRepo_Service".$Process::EXE_EXT, 
                         "-ORBsvcconf implrepo.conf -ORBobjrefstyle url -d 0");

  ACE::waitforfile ($implrepo_ior);

  $SV = Process::Create ($EXEPREFIX."airplane_server".$Process::EXE_EXT,
                         "-o $airplane_ior -i -r -ORBobjrefstyle url");

  ACE::waitforfile ($airplane_ior);

  system($EXEPREFIX."airplane_client -k file://$airplane_ior -ORBobjrefstyle url");

  $IR->Kill (); $IR->Wait ();
}

sub nestea_test
{
  $SV = Process::Create ($EXEPREFIX."nestea_server".$Process::EXE_EXT,
                         "-o $nestea_ior");

  ACE::waitforfile ($nestea_ior);

  $status = system ($EXEPREFIX."nestea_client".$Process::EXE_EXT.
                    " -k file://$nestea_ior");

  $SV->Kill (); $SV->Wait ();

  unlink $nestea_ior;
}

sub nestea_ir_test
{
  $IR = Process::Create ("..".$DIR_SEPARATOR."..".$DIR_SEPARATOR."ImplRepo_Service".$DIR_SEPARATOR."ImplRepo_Service".$Process::EXE_EXT, 
                         "-ORBsvcconf implrepo.conf -ORBobjrefstyle url -d 0");

  ACE::waitforfile ($implrepo_ior);

  $SV = Process::Create ($EXEPREFIX."nestea_server".$Process::EXE_EXT,
                         "-o $nestea_ior -i -r -ORBobjrefstyle url");

  ACE::waitforfile ($nestea_ior);

  system($EXEPREFIX."nestea_client -k file://$nestea_ior -ORBobjrefstyle url");

  $IR->Kill (); $IR->Wait ();
}

sub both_ir_test
{
  $IR = Process::Create ("..".$DIR_SEPARATOR."..".$DIR_SEPARATOR."ImplRepo_Service".$DIR_SEPARATOR."ImplRepo_Service".$Process::EXE_EXT, 
                         "-ORBsvcconf implrepo.conf -ORBobjrefstyle url -d 0");

  ACE::waitforfile ($implrepo_ior);

  $ASV = Process::Create ($EXEPREFIX."nestea_server".$Process::EXE_EXT,
                         "-o $nestea_ior -i -r -ORBobjrefstyle url");

  $NSV = Process::Create ($EXEPREFIX."airplane_server".$Process::EXE_EXT,
                         "-o $airplane_ior -i -r -ORBobjrefstyle url");

  ACE::waitforfile ($airplane_ior);

  $ACL = Process::Create ($EXEPREFIX."airplane_client".$Process::EXE_EXT,
                          "-k file://$airplane_ior -ORBobjrefstyle url");

  ACE::waitforfile ($nestea_ior);

  system($EXEPREFIX."nestea_client -k file://$nestea_ior -ORBobjrefstyle url");

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
      print "                      airplane, simple, or implrepo\n";
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
