eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-
# $Id$


use lib  "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;
$restricted = PerlACE::LocalFile ("restricted.ior");
$permitted = PerlACE::LocalFile ("permitted.ior");


unlink $restricted;
unlink $permitted;

$status = 0;

# Set the SSL environment
# This doesn't work on Windows.  For some reason,
# environment variables aren't propagated to child processes.
#$ENV{'SSL_CERT_FILE'} = 'cacert.pem';

$SV = new PerlACE::Process ("server",
			    "-ORBSvcConf server$PerlACE::svcconf_ext");
$CL = new PerlACE::Process ("client",
			    "-ORBSvcConf client$PerlACE::svcconf_ext");

print STDERR "\n\n==== Running mixed_security_test test\n";

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($permitted, 15) == -1) {
    print STDERR "ERROR: cannot find file <$permitted>\n";
    $SV->Kill ();
    exit 1;
}

if (PerlACE::waitforfile_timed ($restricted, 15) == -1) {
    print STDERR "ERROR: cannot find file <$restricted>\n";
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
