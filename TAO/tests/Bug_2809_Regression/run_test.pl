eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $class = (PerlACE::is_vxworks_test() ? 'PerlACE::ProcessVX' :
                                          'PerlACE::Process');
$T = new $class ("server");

$server = $T->SpawnWaitKill ($PerlACE::wait_interval_for_process_creation);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    exit 1;
}

exit $status;
