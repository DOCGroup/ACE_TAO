eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../../../../bin";
use PerlACE::Run_Test;
use File::Copy;
use Getopt::Std;

# -n notify.conf -s high_path.conf -c other_paths.conf -o output_dir -h
getopts ("n:s:c:o:h");

if ($opt_h)
{
    print STDERR "-n notify.conf -s high_path.conf -c other_paths.conf -o output_dir -h\n";
    exit 0;
}

$experiment_timeout = 600;
$startup_timeout = 120;

if ($opt_n)
{
    $notify_conf = PerlACE::LocalFile ($opt_n);

}else
{
    $notify_conf = PerlACE::LocalFile ("notify.conf");
}

if ($opt_s)
{
    $high_path_conf = PerlACE::LocalFile ($opt_s);

}else
{
    $high_path_conf = PerlACE::LocalFile ("high_path.conf");
}

if ($opt_c)
{
    $other_paths_conf = PerlACE::LocalFile ($opt_c);

}else
{
    $other_paths_conf = PerlACE::LocalFile ("other_paths.conf");
}

$notify_ior = PerlACE::LocalFile ("notify.ior");

$naming_ior = PerlACE::LocalFile ("naming.ior");

$high_path_ior = PerlACE::LocalFile ("high_path.ior");

@list=glob("*.dat");
for $file (@list)
{
  unlink $file or die "Could not delete $file";
}

$status = 0;

$Naming = new PerlACE::Process ("../../../../../Naming_Service/Naming_Service",
                                "-o $naming_ior");

$Notification = new PerlACE::Process ("../../../../../Notify_Service/Notify_Service");

#$Notify_Args = "-ORBInitRef NameService=file://$naming_ior -IORoutput $notify_ior -ORBSvcConf $notify_conf -ORBDebugLevel 1";

$Notify_Args = "-ORBInitRef NameService=file://$naming_ior -IORoutput $notify_ior -ORBSvcConf $notify_conf";

unlink $naming_ior;
$Naming->Spawn ();

if (PerlACE::waitforfile_timed ($naming_ior, $startup_timeout) == -1) {
  print STDERR "ERROR: waiting for the naming service to start\n";
  $Naming->Kill ();
  exit 1;
}

unlink $notify_ior;
$Notification->Arguments ($Notify_Args);
$args = $Notification->Arguments ();
print STDERR "Running Notification with arguments: $args\n";
$status = $Notification->SpawnWaitKill ($experiment_timeout);

if ($status != 0)
  {
    print STDERR "ERROR: Other_paths returned $status\n";
    $Notification->Kill ();
    $Naming->Kill ();
    exit 1;
  }

exit $status;
