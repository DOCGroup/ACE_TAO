eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use Cwd;

my @list=("run_failover_test.pl",
          "run_persistance_test.pl",
          "run_equivalance_test.pl");

my $status = 0;

for $cmd (@list) {
    if (-f $cmd) {
        if (system ("perl $cmd") != 0) {
            $status = 1;
        }
    } else {
        print STDERR "ERROR: Can't find $cmd\n";
    }
}
my $test_label = "FT_Naming FaultTolerant";

if ($status != 0) {
    print STDERR "ERROR: One or more $test_label tests Failed\n";
} else {
    print STDERR "INFO: All $test_label tests report OK\n";
}

exit $status;
