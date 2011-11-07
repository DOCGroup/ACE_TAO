eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use File::Spec;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

## First test that the -ORBSvcConfDirective works with good options.
my $SV = $server->CreateProcess ("server", "-s");

$server_status = $SV->Spawn ($server->ProcessStartWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit (1);
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit (1);
}

## Next, test that the -ORBSvcConfDirective gives an error with bad
## options.
$SV = $server->CreateProcess ("server", "-f");

## Run the server, but redirect stdout and stderr to /dev/null
## so that the errors messages don't show up on the scoreboard

open(STDOUT, '>' . File::Spec->devnull());
open(STDERR, ">&STDOUT");

$server_status = $SV->Spawn ($server->ProcessStartWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit (1);
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

open(STDOUT, ">&SAVEOUT");
open(STDERR, ">&SAVEERR");

## It should have thrown an exception, so if the server exits
## with a zero status, then something is wrong.
if ($server_status == 0) {
    print STDERR "ERROR: server returned $server\n";
    exit(1);
}


exit(0);
