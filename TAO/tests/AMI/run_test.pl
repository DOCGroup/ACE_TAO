eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../bin';
use PerlACE::Run_Test;

$client_conf = PerlACE::LocalFile ("muxed.conf");

$debug_level = '0';
$iterations = '1';

foreach $i (@ARGV) {
    if ($i eq '-mux') {
        $client_conf = PerlACE::LocalFile ("muxed.conf");
    } 
    elsif ($i eq '-debug') {
        $debug_level = '1';
    } 
    elsif ($i eq '-exclusive') {
        $client_conf = PerlACE::LocalFile ('exclusive.conf');
    }
}

$iorfile = PerlACE::LocalFile ("server.ior");

unlink $iorfile;

$SV = new PerlACE::Process ("server", 
                            "-ORBdebuglevel $debug_level -d -o $iorfile");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 15) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
} 

$CL = new PerlACE::Process ("simple_client",
                            "-ORBsvcconf $client_conf "
                            . "-ORBdebuglevel $debug_level"
                            . " -k file://$iorfile "
                            . " -i $iterations -x -d");

$client = $CL->SpawnWaitKill (60);
$server = $SV->WaitKill (5);

unlink $iorfile;

if ($server != 0 || $client != 0) {
    exit 1;
}
 
exit 0;
 
