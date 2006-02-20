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

$Other_paths = new PerlACE::Process ("../../../Driver/Notify_Tests_Driver");

$Other_paths_Args = "-ORBInitRef NameService=file://$naming_ior -IORinput file://$high_path_ior -ORBSvcConf $other_paths_conf";

#$Other_paths_Args = "-ORBInitRef NameService=file://$naming_ior -IORinput file://$high_path_ior -ORBSvcConf $other_paths_conf -ORBDebugLevel 1";

if (PerlACE::waitforfile_timed ($high_path_ior, $startup_timeout) == -1) {
  print STDERR "ERROR: waiting for the high_path to start\n";

  exit 1;
}

unlink $other_paths_ior;
$Other_paths->Arguments ($Other_paths_Args);
$args = $Other_paths->Arguments ();
print STDERR "Running Other_paths with arguments: $args\n";
$status = $Other_paths->SpawnWaitKill ($experiment_timeout);

if ($status != 0)
  {
    print STDERR "ERROR: Other_paths returned $status\n";

    exit 1;
  }

unlink $other_paths_ior;

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
