eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use POSIX "sys_wait_h";

my $verbose = 1;
$status = 0;
$srv_threads = '2';
$num_clients = 4;
$baseior = "test.ior";
$iorfile = PerlACE::LocalFile ($baseior);
$srvdownfile = PerlACE::LocalFile ("server_terminated");

unlink $iorfile;
unlink $srvdownfile;

$SV = new PerlACE::Process ("server", "-o $iorfile -n $srv_threads");
for($count = 0; $count < $num_clients; $count++) {
    push(@CL, new PerlACE::Process ("client", "-k file://$iorfile"));
}
$CLS = new PerlACE::Process ("client", "-k file://$iorfile -x");

print STDERR $SV->CommandLine(), "\n" if $verbose;
$SV->Spawn ();
if (PerlACE::waitforfile_timed ($iorfile, 10) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

# Do this in a loop
#print STDERR $CL->CommandLine(), "\n" if $verbose;
foreach $client (@CL) {
  print STDERR $client->CommandLine(), "\n" if $verbose;
  $client->Spawn ();
}
foreach $client (@CL) {
  $client->WaitKill (20);
}

#Server should still be running at this point, now start the client that asks
#the server to shut itself down

$client = $CLS->SpawnWaitKill (15);
if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

#Server should now be shutdown
if (PerlACE::waitforfile_timed ($srvdownfile,
                                10) == -1) {
    print STDERR "ERROR: cannot find file <$srvdownfile>".
        " -- server has not shut down cleanly.\n";
    $SV->Kill ();
    exit 1;
}

$server = $SV->WaitKill (15);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;
unlink $srvdownfile;

exit $status
