eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../../../../bin";
use PerlACE::Run_Test;
use File::Copy;
use Getopt::Long;

GetOptions ("rt" => \$rt,
            "cos" => \$cos,
            "all" => \$all);

@list=("Paths_1","Paths_5","Paths_10","Paths_20");

for $file (@list)
{
    # default to rt
    if (!$all && !$rt && !$cos)
    {
        $rt = 1;
    }

    if (-d $file)
    {
        if ($all || $rt)
        {
            print STDERR "Running $file test with RT Notification\n";

            $cmd = "run_test.pl -n rt_notify.conf -s high_path.conf -c $file/other_paths.conf -o $file/rt_output";

            $status = system ("perl $cmd");

            if ($status != 0)
            {
                print STDERR "ERROR: $file test returned $status\n";
            }
        }


        if ($all || $cos)
        {
            print STDERR "Running $file test with COS Notification\n";

            $cmd = "run_test.pl -n cos_notify.conf -s high_path.conf -c $file/other_paths.conf -o $file/cos_output";

            $status = system ("perl $cmd");

            if ($status != 0)
            {
                print STDERR "ERROR: $file test returned $status\n";
            }
        }
    }
}
