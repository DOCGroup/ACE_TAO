eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $client = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

my $number_of_servers = 5;

#These files are used only in ServerRequest_Interceptor2.cpp for internal tasks.
my $last_exp_time = "last_expiration_time";
my $no_more_retries = "no_more_retries";

my @iorfiles = ();
my @servers = ();
my @servers_iorfile = ();
my @client_iorfiles = ();
my @SV = ();

for ($i = 0; $i < $number_of_servers + 1; $i++) {
    $servers[$i] = PerlACE::TestTarget::create_target ($i + 2) || die "Create target $i + 2 failed\n";
    $iorfiles[$i] = "n".$i.".ior";

    $servers_iorfile[$i] = $servers[$i]->LocalFile($iorfiles[$i]);
    $client_iorfiles[$i] = $client->LocalFile($iorfiles[$i]);

    $servers[$i]->DeleteFile ($iorfiles[$i]);
    $client->DeleteFile ($iorfiles[$i]);
    $servers[$i]->DeleteFile ($last_exp_time);
    $servers[$i]->DeleteFile ($no_more_retries);

    $SV [$i] = $servers[$i]->CreateProcess("server2", "-n $i");
}

$CL = $client->CreateProcess ("client2", " -n $number_of_servers");

sub wait_get_send_ior {
    my $number = $_[0];
    if ($servers[$number]->WaitForFileTimed ($iorfiles[$number],
                                             $servers[$number]->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$servers[$number]_iorfile>\n";
        return 1;
    }

    if ($servers[$number]->GetFile ($iorfiles[$number]) == -1) {
        print STDERR "ERROR: cannot retrieve file <$servers[$number]_iorfile>\n";
        return 1;
    }

    if ($client->PutFile ($iorfiles[$number]) == -1) {
        print STDERR "ERROR: cannot set file <$client_iorfiles[$number]>\n";
        return 1;
    }

    return 0;
}

for($i = 0; $i < $number_of_servers; $i++) {
    $process_status = $SV[$i]->Spawn ();
    if ($process_status != 0) {
        print STDERR "ERROR: server $i returned $process_status\n";
        for ($j = 0; $j < $i + 1; $j++) {
            $SV[$j]->Kill (); $SV[$j]->TimedWait (1);
        }
        exit 1;
    }
}

# Loops are cheap.
for ($i = 0; $i < $number_of_servers; $i++) {
    $result = wait_get_send_ior($i);

    if ($result != 0) {
        for ($j = 0; $j < $i + 1; $j++) {
            $SV[$j]->Kill (); $SV[$j]->TimedWait (1);
        }
        exit 1;
    }
}

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 285);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}


for ($i = 0; $i < $number_of_servers; $i++) {
    $server_status = $SV[$i]->WaitKill ($servers[$i]->ProcessStopWaitInterval());

    if ($server_status < 0) {
        print STDERR "ERROR: server $i returned $server_status\n";
        $status = 1;
    }

    $servers[$i]->DeleteFile ($iorfiles[$i]);
    $client->DeleteFile ($iorfiles[$i]);
    $servers[$i]->DeleteFile ($last_exp_time);
    $servers[$i]->DeleteFile ($no_more_retries);
}

exit $status;
