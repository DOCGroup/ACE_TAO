eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../../../../../bin";
use PerlACE::Run_Test;

$experiment_timeout = 600;
$startup_timeout = 60;
$naming_ior = PerlACE::LocalFile ("naming.ior");
$collocated_conf = PerlACE::LocalFile ("colocated.conf");
$status = 0;

$Naming = new PerlACE::Process ("../../../../../../Naming_Service/Naming_Service",
                                "-o $naming_ior");

$Collocated = new PerlACE::Process ("../../../../Driver/Notify_Tests_Driver");

$Collocated_Args = "-ORBInitRef NameService=file://$naming_ior -ORBSvcConf $collocated_conf";

unlink $naming_ior;
$Naming->Spawn ();

if (PerlACE::waitforfile_timed ($naming_ior, $startup_timeout) == -1) {
  print STDERR "ERROR: waiting for the naming service to start\n";
  $Naming->Kill ();
  exit 1;
}

$Collocated->Arguments ($Collocated_Args);
$args = $Collocated->Arguments ();
print STDERR "Running Collocated with arguments: $args\n";
$status = $Colocated->SpawnWaitKill ($experiment_timeout);

if ($status != 0)
  {
    print STDERR "ERROR: Collocated returned $status\n";
    $Notification->Kill ();
    $Naming->Kill ();
    exit 1;
  }

$Naming->Kill ();
unlink $naming_ior;

if ($#ARGV > -1)
  {
    $results_directory = $ARGV[0];
    print STDERR "Saving results to $results_directory\n";

    mkdir $results_directory, 0777;

    @list=glob("*.dat");
    for $file (@list)
      {
        copy ("$file", "$results_directory/$file");
      }

    @list=glob("*.conf");
    for $file (@list)
      {
        copy ("$file", "$results_directory/$file");
      }
  }

exit $status;
