eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;
$iorfile = PerlACE::LocalFile ('server.ior');
$TARGETHOSTNAME = 'localhost';
$port = 11000;
$debug = 10;
$endien = (pack('L', 0x41424344) eq 'ABCD' ? '_be' : '');

unlink $iorfile;

if (PerlACE::is_vxworks_test()) {
    $TARGETHOSTNAME = $ENV{'ACE_RUN_VX_TGTHOST'};
    $SV = new PerlACE::ProcessVX ('server',
                             '-ORBEndpoint ' .
                             "iiop://$TARGETHOSTNAME" . ":$port " .
                             "-ORBDebugLevel $debug");
}
else {
    $SV = new PerlACE::Process ('server',
                             '-ORBEndpoint ' .
                             "iiop://$TARGETHOSTNAME" . ":$port " .
                             "-ORBDebugLevel $debug");
}
$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

my($cl) = system("$^X dribble.pl --host=$TARGETHOSTNAME --port=$port " .
                 "--stream=transport_data_dump$endien.dat " .
                 "--layout=transport_data_dump$endien.layout");
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
