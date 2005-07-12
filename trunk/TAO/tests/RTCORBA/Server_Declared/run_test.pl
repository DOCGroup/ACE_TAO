eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../../bin';
use PerlACE::Run_Test;

$iorfile1 = PerlACE::LocalFile ("test1.ior");
$iorfile2 = PerlACE::LocalFile ("test2.ior");

unlink $iorfile1;
unlink $iorfile2;
$status = 0;

$server_args =
    "-a 45 -b 55";

print STDERR "Value is " . $^O;

if ($^O eq "MSWin32") {
    $server_args =
        "-a 1 -b 2";
}
elsif ($^O eq "dec_osf") {
    $server_args =
        "-a 20 -b 30";
}
elsif ($^O eq "hpux") {
    $continuous = 1;
    $server_args =
        "-a 17 -b 29";
}

if ($continuous) {
  $server_args .= " -ORBSvcConf continuous$PerlACE::svcconf_ext";
}

$client_args = "-p file://$iorfile1 -o file://$iorfile2";
if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server", "-p test1.ior -o test2.ior $server_args");
}
else {
    $SV = new PerlACE::Process ("server", "-p $iorfile1 -o $iorfile2 $server_args");
}
$CL = new PerlACE::Process ("client", $client_args);

print STDERR "\n********** RTCORBA SERVER_DECLARED Priority Unit Test\n\n";


$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile2, 10) == -1) 
{
    $server = $SV->TimedWait (1);
    if ($server == 2) 
    {
        # Mark as no longer running to avoid errors on exit.
        $SV->{RUNNING} = 0;
        exit $status;
    } 
    else 
    {
        print STDERR "ERROR: cannot find file <$iorfile2>\n";
        $SV->Kill ();
        exit 1;
    }
}

$client = $CL->SpawnWaitKill (60);

if ($client != 0) 
{
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (60);

if ($server != 0) 
{
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile1;
unlink $iorfile2;

exit $status;
