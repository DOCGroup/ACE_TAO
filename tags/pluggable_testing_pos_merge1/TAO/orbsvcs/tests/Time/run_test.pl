eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../bin";

require ACEutils;

$server_ior = "server_ior";
$clerk_ior = "clerk_ior";
$implrepo_ior = "implrepo.ior";

# Make sure the files are gone, so we can wait on them.

unlink $server_ior;
unlink $clerk_ior;
unlink $implrepo_ior;

$time_dir = "..$DIR_SEPARATOR..".$DIR_SEPARATOR."Time_Service".$DIR_SEPARATOR;

sub time_service_test_using_naming_service
{
    $SV1 = Process::Create ($time_dir."server".$Process::EXE_EXT,"");

    sleep 5;

    $SV2 = Process::Create ($time_dir."clerk".$Process::EXE_EXT,"-t 2");

    sleep 10;

    $status = system ($EXEPREFIX."client".$Process::EXE_EXT.
                      "");

    $SV1->Kill ();
    $SV2->Kill ();
    $SV1->Wait ();
    $SV2->Wait ();
}

sub time_service_test_using_files
{
    $SV1 = Process::Create ($time_dir."server".$Process::EXE_EXT,
                            "-o $server_ior");

    ACE::waitforfile ($server_ior);
    sleep 5;

    $SV2 = Process::Create ($time_dir."clerk".$Process::EXE_EXT,
                            "-f $server_ior -o clerk_ior -t 2");

    ACE::waitforfile ($clerk_ior);

    sleep 10;

    $status = system ($EXEPREFIX."client".$Process::EXE_EXT.
                      " -f clerk_ior");

    $SV1->Kill ();
    $SV2->Kill ();
    $SV1->Wait ();
    $SV2->Wait ();

    unlink $clerk_ior;
    unlink $server_ior;
}

sub time_service_test_using_ir
{
  $ir_dir = "..".$DIR_SEPARATOR."..".$DIR_SEPARATOR."ImplRepo_Service".$DIR_SEPARATOR;
  $IR = Process::Create ($ir_dir."ImplRepo_Service".$Process::EXE_EXT,
                         "-ORBsvcconf implrepo.conf -ORBobjrefstyle url -d 1");

  ACE::waitforfile ($implrepo_ior);

  $SV1 = Process::Create ($time_dir."server".$Process::EXE_EXT,
                         "-o $server_ior -i -r -ORBobjrefstyle url");

  ACE::waitforfile ($server_ior);

  sleep 10;

  $SV2 = Process::Create ($time_dir."clerk".$Process::EXE_EXT,
                          "-f $server_ior -o clerk_ior -ORBobjrefstyle url");

  sleep 10;

  system($EXEPREFIX."client -f $clerk_ior");

  $IR->Kill ();
  $IR->Wait ();
  $SV1->Kill ();
  $SV1->Wait ();
  $SV2->Kill ();
  $SV2->Wait ();

  unlink $clerk_ior;
  unlink $server_ior;
  unlink $implrepo_ior;
}

# Parse the arguments

for ($i = 0; $i <= $#ARGV; $i++)
{
  SWITCH:
  {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?")
    {
      print "run_test <use_naming> or <use_files>\n";
      print "\n";
      print "use_naming               -- Runs the test using Naming Service\n";
      print "use_files                -- Runs the test using IOR Files\n";
      exit;
    }

    if ($ARGV[$i] eq "use_naming")
    {
      time_service_test_using_naming_service ();
      last SWITCH;
    }

    if ($ARGV[$i] eq "use_files")
    {
      time_service_test_using_files ();
      last SWITCH;
    }

    if ($ARGV[$i] eq "use_ir")
    {
      time_service_test_using_ir ();
      last SWITCH;
    }

    print "run_test: Unknown Option: ".$ARGV[$i]."\n";
  }
}
