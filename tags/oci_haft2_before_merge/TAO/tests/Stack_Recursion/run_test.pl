eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib '../../../bin';
use PerlACE::Run_Test;

$iorfile = PerlACE::LocalFile ("server.ior");
@files= <core.*>;
unlink $iorfile;
unlink (@files);
$status = 0;

$SV  = new PerlACE::Process ("server", "-o $iorfile");
$CL = new PerlACE::Process ("client", " -k file://$iorfile");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 15) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
} 

$CL->Spawn (60);

$client = $CL->WaitKill (480);

if ($client != 0) {
    print STDERR "ERROR: client returned $client1\n";
    $status = 1;
}

$server = $SV->TerminateWaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

@files= <core.*>;
#unlink (@files);
unlink $iorfile;

exit $status;
