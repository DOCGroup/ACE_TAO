eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../../../bin";
use PerlACE::Run_Test;
use Getopt::Std;

@list= (
        {
            dir_name => "1_Path_Period_0_Lanes",
            cmd => "perl run_all.pl --all",
        },
        {
            dir_name => "3_Path_Period_10ms_Lanes",
            cmd => "perl run_all.pl --all",
        },
        {
            dir_name => "Paths_vs_Throughput",
            cmd => "perl run_all.pl --all",
        },
        {
            dir_name => "Max_Throughput",
            cmd => "perl run_all.pl",
        },
  );

for $test (@list)
{
    if (-d $test->{dir_name})
    {
        print STDERR "Running $test->{dir_name} test\n";

        $cmd = "run_test.pl";

        chdir $test->{dir_name} or die "Could not chdir to $test->{dir_name}";

        $status = system ("$test->{cmd}");

        if ($status != 0)
        {
            print STDERR "ERROR: $test->{dir_name} test returned $status\n";
        }

        chdir "../";
    }
}
