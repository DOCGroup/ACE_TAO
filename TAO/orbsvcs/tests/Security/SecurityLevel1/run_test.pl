eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-
# $Id$

use lib  '../../../../../bin';
use PerlACE::Run_Test;

$status = 0;
$file = PerlACE::LocalFile ("test.ior");

unlink $file;

$status = 0;

# Set the SSL environment
$ENV{'SSL_CERT_FILE'} = 'ca.pem';

$SV = new PerlACE::Process ("server",
			    "-o $file -ORBSvcConf server.conf");
$CL = new PerlACE::Process ("client",
			    "-ORBSvcConf client.conf -k file://$file");

print STDERR "\n\n==== Running SecurityLevel1 test\n";

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($file, 15) == -1) {
    print STDERR "ERROR: cannot find file <$file>\n";
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

unlink $file;

exit $status;
