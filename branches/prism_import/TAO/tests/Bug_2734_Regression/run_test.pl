eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use strict;

my $status = 0;
my $iorfile = 'server.ior';
my $class = (PerlACE::is_vxworks_test() ? 'PerlACE::ProcessVX' :
                                          'PerlACE::Process');
my $SV = $class->new("server", "-o $iorfile");
my $CL = new PerlACE::Process("client",
                              "-ORBid ClientORB -k file://$iorfile " .
                              "-l 35 -h 40");

unlink($iorfile);
my $server = $SV->Spawn();

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    exit(1);
}

if (PerlACE::waitforfile_timed(
               $iorfile,
               $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill();
    exit(1);
}

my $client = $CL->SpawnWaitKill(60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill(15);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink($iorfile);

exit($status);
