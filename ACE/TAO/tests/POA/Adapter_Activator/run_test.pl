eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

$server->AddLibPath ('../Generic_Servant/.');
$client->AddLibPath ('../Generic_Servant/.');

$iorbase = "ior";

$status = 0;
$oneway = "";
$iterations = 100;

$extra_args = "";

# Parse the arguments
for ($i = 0; $i <= $#ARGV; $i++) {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?") {
        print "run_test [-h] [-i iterations] [-o] [-f ior file]\n";
        print "\n";
        print "-h                  -- prints this information\n";
        print "-f                  -- ior file\n";
        print "-i iterations       -- specifies iterations\n";
        print "-o                  -- call issued are oneways\n";
        exit;
    }
    elsif ($ARGV[$i] eq "-o") {
        $oneway = "-o";
    }
    elsif ($ARGV[$i] eq "-i") {
        $iterations = $ARGV[$i + 1];
        $i++;
    }
    elsif ($ARGV[$i] eq "-f") {
        $iorbase = $ARGV[$i + 1];
        $i++;
    }
    else {
        $extra_args .= " " . $ARGV[$i];
    }
}

$iorfile_1 = $iorbase."_1";
$iorfile_2 = $iorbase."_2";
$iorfile_3 = $iorbase."_3";

$server->DeleteFile($iorfile_1);
$server->DeleteFile($iorfile_2);
$server->DeleteFile($iorfile_3);
$client->DeleteFile($iorfile_1);
$client->DeleteFile($iorfile_2);
$client->DeleteFile($iorfile_3);

$iorfile_server = $server->LocalFile ("$iorbase");

$SV = $server->CreateProcess ("server", "-f $iorfile_server $extra_args");
$CL = $client->CreateProcess ("../Generic_Servant/client");

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server\n";
    exit 1;
}

if ($server->WaitForFileTimed ($iorfile_1,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile_1>\n";
    $SV->Kill ();
    exit 1;
}

if ($server->WaitForFileTimed ($iorfile_2,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile_2>\n";
    $SV->Kill ();
    exit 1;
}

if ($server->WaitForFileTimed ($iorfile_3,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile_3>\n";
    $SV->Kill ();
    exit 1;
}

$iorfile_client1 = $client->LocalFile ("$iorfile_1");
$iorfile_client2 = $client->LocalFile ("$iorfile_2");
$iorfile_client3 = $client->LocalFile ("$iorfile_3");

$CL->Arguments ("$extra_args $oneway -i $iterations -k file://$iorfile_client1");

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 45);

if ($client_status != 0) {
    print STDERR "ERROR: client 1 returned $client_status\n";
    $status = 1;
}

$CL->Arguments ("$extra_args $oneway -i $iterations -k file://$iorfile_client2");

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 45);

if ($client_status != 0) {
    print STDERR "ERROR: client 2 returned $client_status\n";
    $status = 1;
}

$CL->Arguments ("$extra_args $oneway -i $iterations -k file://$iorfile_client3 -x");

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 45);

if ($client_status != 0) {
    print STDERR "ERROR: client 3 returned $client_status\n";
    $status = 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->GetStderrLog();
$client->GetStderrLog();

$server->DeleteFile($iorfile_1);
$server->DeleteFile($iorfile_2);
$server->DeleteFile($iorfile_3);
$client->DeleteFile($iorfile_1);
$client->DeleteFile($iorfile_2);
$client->DeleteFile($iorfile_3);

exit $status;
