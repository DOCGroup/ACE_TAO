eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

my @tests = qw(
               zlibserver
               bzip2server
               lzoserver
               rleserver
              );

foreach my $process (@tests) {

$SV = $server->CreateProcess ("$process");
my $executable = $SV->Executable;
next unless -e $executable;

$test = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval());

if ($test != 0) {
    print STDERR "ERROR: test returned $test\n";
    exit 1;
}
}

exit 0;
