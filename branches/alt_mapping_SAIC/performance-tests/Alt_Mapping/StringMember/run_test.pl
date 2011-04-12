eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $server = PerlACE::TestTarget::create_target(1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target(2) || die "Create target 2 failed\n";

$iorbase = "StringMember.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

$num = 10000;
$len = 40;
$status = 0;

# Parse the arguments

for ($i = 0; $i <= $#ARGV; $i++) {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?") {
      print "Run_Test Perl script for TAO StringMember Test\n\n";
      print "run_test [-n num] [-l length]\n";
      print "\n";
      print "-n num              -- num iterations for stats\n";
      print "-l length           -- string member length\n";
      exit 0;
    }
    elsif ($ARGV[$i] eq "-n") {
      $num = $ARGV[$i + 1];
      $i++;
    }
    elsif ($ARGV[$i] eq "-l") {
      $len = $ARGV[$i + 1];
      $i++;
    }
}

$SV = $server->CreateProcess ("server");
$CL = $client->CreateProcess ("client");

$server->DeleteFile($iorbase); # Ignore errors
$client->DeleteFile($iorbase);

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
} else {
    if ($server->WaitForFileTimed ($iorbase,
                                    $server->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$server_iorfile>\n";
        $SV->Kill ();
        exit 1;
    }

    if ($server->GetFile ($iorbase) == -1) {
        print STDERR "ERROR: cannot retrieve file <$server_iorfile>\n";
        $SV->Kill (); $SV->TimedWait (1);
        exit 1;
    }
    if ($client->PutFile ($iorbase) == -1) {
        print STDERR "ERROR: cannot set file <$client_iorfile>\n";
        $SV->Kill (); $SV->TimedWait (1);
        exit 1;
    }

    $CL->Arguments ("-n $num -l $len");
    $client_status = $CL->SpawnWaitKill ($server->ProcessStartWaitInterval() + 45);

    if ($client_status != 0) {
        print STDERR "ERROR: client returned $client_status\n";
        $status = 1;
    }

    $server_status = $SV->Kill ();

    if ($server_status != 0) {
        print STDERR "ERROR: server returned $server_status\n";
        $status = 1;
    }

    $server->GetStderrLog();

    $server->DeleteFile($iorbase);
    $client->DeleteFile($iorbase);
}

$server->DeleteFile($iorbase);

exit $status;
