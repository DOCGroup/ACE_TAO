#$Id$
# -*- perl -*-
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use lib "../../../../bin";

require ACEutils;

$airplane_ior = "airplane.ior";
$simple_ior = "simple.ior";

# Make sure the files are gone, so we can wait on them.
unlink $airplane_ior;
unlink $simple_ior;
unlink "implrepo.ior";

# The Tests

sub airplane_test
{
  $IR = Process::Create ("..".$DIR_SEPARATOR."..".$DIR_SEPARATOR."ImplRepo_Service".$DIR_SEPARATOR."ImplRepo_Service".$Process::EXE_EXT, 
                         "-ORBsvcconf implrepo.conf");

  ACE::waitforfile ("implrepo.ior");

  $SV = Process::Create ($EXEPREFIX."airplane_server".$Process::EXE_EXT,
                         "-o $airplane_ior -r");

  ACE::waitforfile ($airplane_ior);

  $status = system ($EXEPREFIX."airplane_client".$Process::EXE_EXT.
                    " -f $airplane_ior");

  $SV->Kill (); $SV->Wait ();
  $IR->Kill (); $IR->Wait ();

  unlink $airplane_ior;
}

sub simple_test
{
  $SV = Process::Create ($EXEPREFIX."server".$Process::EXE_EXT,
                         "-o $simple_ior");

  ACE::waitforfile ($simple_ior);

  $status = system ($EXEPREFIX."client".$Process::EXE_EXT.
                    " -f $simple_ior");

  $SV->Kill (); $SV->Wait ();

  unlink $simple_ior;
}

sub implrepo_test
{
  $IR = Process::Create ("..".$DIR_SEPARATOR."..".$DIR_SEPARATOR."ImplRepo_Service".$DIR_SEPARATOR."ImplRepo_Service".$Process::EXE_EXT, 
                         "-ORBsvcconf implrepo.conf");

  ACE::waitforfile ("implrepo.ior");

  system ($EXEPREFIX."server -r -o svr.ior");

  $SV = Process::Create ($EXEPREFIX."server".$Process::EXE_EXT, "");

  sleep $ACE::sleeptime;

  $PSV = Process::Create ($EXEPREFIX."airplane_server".$Process::EXE_EXT,
                          "-o $airplane_ior -r");

  ACE::waitforfile ($airplane_ior);

  $PCL = Process::Create ($EXEPREFIX."airplane_client".$Process::EXE_EXT,
                          " -f $airplane_ior");

  system ("client -f svr.ior");

#  $IR->Kill (); $IR->Wait ();
}


# Parse the arguments

for ($i = 0; $i <= $#ARGV; $i++)
{
  SWITCH:
  {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?")
    {
      print "run_test test1 test2...\n";
      print "\n";
      print "test1               -- Runs a specific test\n";
      print "                       airplane, simple, or implrepo\n";
      exit;
    }
    if ($ARGV[$i] eq "airplane")
    {
      airplane_test ();    
      last SWITCH;
    }
    if ($ARGV[$i] eq "simple")
    {
      simple_test ();    
      last SWITCH;
    }
    if ($ARGV[$i] eq "implrepo")
    {
      implrepo_test ();    
      last SWITCH;
    }

    print "run_test: Unknown Option: ".$ARGV[$i]."\n";
  }
}

