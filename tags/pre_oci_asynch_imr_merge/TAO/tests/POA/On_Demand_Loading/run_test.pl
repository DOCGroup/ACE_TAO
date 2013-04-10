eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$iorbase0 = "ior";

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

$server->AddLibPath ('../Generic_Servant/.');
$client->AddLibPath ('../Generic_Servant/.');

$oneway = "";
$iterations = 100;

$extra_args = "";

$status = 0;

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
      $iorbase0 = $ARGV[$i + 1];
      $i++;
    }
    else {
        $extra_args .= " " . $ARGV[$i];
    }
}

$iorbase1 = $iorbase0."_1";
$iorbase2 = $iorbase0."_2";

$iorfile0 = $server->LocalFile ($iorbase0);
$iorfile1 = $server->LocalFile ($iorbase1);
$iorfile2 = $server->LocalFile ($iorbase2);

$client_iorfile1 = $client->LocalFile ($iorbase1);
$client_iorfile2 = $client->LocalFile ($iorbase2);

$server->DeleteFile($iorbase0);
$server->DeleteFile($iorbase1);
$server->DeleteFile($iorbase2);
$client->DeleteFile ($iorbase1);
$client->DeleteFile ($iorbase2);

$SV = $server->CreateProcess ("server", "-f $iorfile0 $extra_args");
$CL = $client->CreateProcess ("../Generic_Servant/client");

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server->WaitForFileTimed ($iorbase1,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile1>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($server->WaitForFileTimed ($iorbase2,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile2>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$CL->Arguments ("$extra_args $oneway -i $iterations -k file://$client_iorfile1");

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client 1 returned $client_status\n";
    $status = 1;
}

$CL->Arguments ("$extra_args $oneway -i $iterations -k file://$client_iorfile2 -x");

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client 2 returned $client_status\n";
    $status = 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $SV->Kill ();
    $status = 1;
}

$server->DeleteFile($iorbase0);
$server->DeleteFile($iorbase1);
$server->DeleteFile($iorbase2);
$client->DeleteFile ($iorbase1);
$client->DeleteFile ($iorbase2);

exit $status;
