eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../../../../bin";
use PerlACE::Run_Test;
use File::Copy;
use Getopt::Std;

@list= ("2_Hops","2_Hops_RTCORBA","3_Hops","3_Hops_RTCORBA","Colocated","Colocated_RTCORBA","Notify","Notify_RTCORBA");

for $file (@list)
{
    if (-d $file)
    {
        print STDERR "Running $file test\n";

        $cmd = "run_test.pl";

        chdir $file or die "Could not chdir to $file";

        $status = system ("perl $cmd");

        if ($status != 0)
        {
            print STDERR "ERROR: $file test returned $status\n";
        }

        chdir "../";
    }
}
