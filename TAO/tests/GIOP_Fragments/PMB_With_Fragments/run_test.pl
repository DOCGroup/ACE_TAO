eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib '../../../../bin';
use PerlACE::Run_Test;

$status = 0;
$iorfile = PerlACE::LocalFile ('server.ior');
$TARGETHOSTNAME = 'localhost';
$port = PerlACE::uniqueid () + 12000;
$debug = 0;

unlink $iorfile;

$SV  = new PerlACE::Process ('server',
                             '-ORBEndpoint ' .
                             "iiop://$TARGETHOSTNAME" . ":$port " .
                             "-ORBDebugLevel $debug");
$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 15) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

my($cl) = system("$^X dribble.pl --port=$port " .
                 "--stream=giop1.2_fragments.dat " .
                 "--layout=giop1.2_fragments.layout");
if ($cl != 0) {
    print STDERR "ERROR: client returned $cl\n";
    ++$status;
}

$server = $SV->WaitKill (10);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    ++$status;
}

unlink $iorfile;
exit $status;
