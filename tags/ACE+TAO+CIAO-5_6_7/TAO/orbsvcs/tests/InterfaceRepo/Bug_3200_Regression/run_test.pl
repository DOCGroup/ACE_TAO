eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$locking = "";

$iorfile = "if_repo.ior";

$status = 0;

$nice = "";

print <<EOF;
              ********** IMPORTANT NOTE ***********
While it is easy to confirm this failure via inspection of
output from tao_ifr, at the time of committing this test tao_ifr
did not consistently exit with a non-zero return code when hitting
an error, making automatic detection of success/failure difficult.

If/when this behavior of tao_ifr changes, this test driver can be
updated so that it properly detects success/failure of tao_ifr.
******************************************************************
EOF

unlink $iorfile;

$SV = new PerlACE::Process ("../../../IFR_Service/IFR_Service", " $nice " . " -o $iorfile" . " $locking");
$CL1 = new PerlACE::Process ("$ENV{ACE_ROOT}/bin/tao_ifr",
                             "-ORBInitRef InterfaceRepository=file://$iorfile b1.idl");
$CL2 = new PerlACE::Process ("$ENV{ACE_ROOT}/bin/tao_ifr",
                             "-ORBInitRef InterfaceRepository=file://$iorfile b2.idl");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

$client1 = $CL1->SpawnWaitKill (60);

if ($client1 != 0) {
    print STDERR "ERROR: client returned $client1\n";
    $status = 1;
}

$client2 = $CL2->SpawnWaitKill (60);

if ($client2 != 0) {
    print STDERR "ERROR: client returned $client2\n";
    $status = 1;
}

$server = $SV->TerminateWaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status;

