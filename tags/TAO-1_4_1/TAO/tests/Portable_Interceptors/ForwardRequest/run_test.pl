eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-
#
# $Id$


use lib '../../../../bin';
use PerlACE::Run_Test;

$file1 = PerlACE::LocalFile ("test1.ior");
$file2 = PerlACE::LocalFile ("test2.ior");

unlink $file1;
unlink $file2;

$SV = new PerlACE::Process ("server", "-o $file1 -o $file2");
$CL = new PerlACE::Process ("client", "-k file://$file1 -k file://$file2");

$status = 0;

print STDERR "\n\n==== Running PortableInterceptor::ForwardRequest test\n";

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($file1, 15) == -1) {
    print STDERR "ERROR: cannot find file <$file1>\n";
    $SV->Kill ();
    exit 1;
}

if (PerlACE::waitforfile_timed ($file2, 15) == -1) {
    print STDERR "ERROR: cannot find file <$file2>\n";
    $SV->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $file1;
unlink $file2;

exit $status;
