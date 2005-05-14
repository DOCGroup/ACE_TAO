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

$experiment_timeout = 120;
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


$High_path = new PerlACE::Process ("../../../Driver/Notify_Tests_Driver");

$High_path_Args = "-ORBInitRef NameService=file://$naming_ior -IORoutput $high_path_ior -ORBSvcConf $high_path_conf";


if (PerlACE::waitforfile_timed ($notify_ior, $startup_timeout) == -1) {
  print STDERR "ERROR: waiting for the notify service to start\n";
  exit 1;
}

unlink $high_path_ior;
$High_path->Arguments ($High_path_Args);
$args = $High_path->Arguments ();
print STDERR "Running High_path with arguments: $args\n";
$status = $High_path->SpawnWaitKill ($experiment_timeout);

if ($status != 0)
  {
    print STDERR "ERROR: Other_paths returned $status\n";
    $High_path->Kill ();

    exit 1;
  }

if ($opt_o)
  {
      $results_directory = PerlACE::LocalFile ($opt_o);

      if (! -e $results_directory)
      {
          mkdir $results_directory, 0777;
      }

      print STDERR "Saving results to $results_directory\n";

      @list=glob("*.dat");
      for $file (@list)
      {
          move ("$file", "$results_directory/$file");
      }
  }

exit $status;
