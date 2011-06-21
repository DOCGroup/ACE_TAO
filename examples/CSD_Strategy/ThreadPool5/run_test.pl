eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;

$iorfname_prefix = "server";
$num_servants=1;
$num_clients_per_servant=40;
$num_orb_threads=1;
$num_csd_threads=1;
$collocated_test=0;
$servant_cancellation_option="";

if ($ARGV[0] eq 'collocated') {
    # 1 servant, 1 collocated client, 1 orb thread, 2 strategy working threads
    $num_clients_per_servant=1;
    $collocated_test=1;
    $num_csd_threads=2;
}
elsif ($ARGV[0] eq 'multiple_servants') {
    # multiple servants and single orb thread.
    # 10 servant, 40 client, 1 orb thread, 10 strategy working threads
    $num_servants=10;
    $num_csd_threads=10;
    $num_clients_per_servant=4;
}
elsif ($ARGV[0] eq 'multiple_orb_threads') {
    # multiple servants and multiple orb threads.
    # 10 servant, 40 client, 4 orb thread, 10 strategy working threads
    $num_servants=10;
    $num_csd_threads=10;
    $num_clients_per_servant=4;
    $num_orb_threads=4;
}
elsif ($ARGV[0] eq 'cancel_servant') {
    # Cancel one servant and leave the other alive.
    # 2 servant, 10 client, 5 orb thread, 1 strategy working threads
    $num_clients_per_servant=5;
    $num_servants=2;
    $num_csd_threads=2;
    $num_orb_threads=5;
    $servant_cancellation_option = " -d 1 ";
}
elsif ($ARGV[0] eq '') {
    # default test - 1 servant, 40 clients , 1 orb thread, 1 csd thread
}
else {
    print STDERR "ERROR: invalid parameter $ARGV[0] \n";
    exit 1;
}

$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

$num_clients = $num_servants * $num_clients_per_servant;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

$SV  = $server->CreateProcess("server_main",
                              "-p $iorfname_prefix -s $num_servants ".
                              "-c $num_clients -t $num_orb_threads -n $num_csd_threads ".
                              "-l $collocated_test $servant_cancellation_option");

my @clients = ();
for ($i = 0; $i < $num_clients; $i++) {
    $clients[$i] = PerlACE::TestTarget::create_target ($i+1) || die "Create target $i+1 failed\n";
}

#Delete old ior files.
my @iorfiles = ();
my @server_iorfiles = ();
for ($i = 0; $i < $num_servants; $i++) {
    $servant_id = sprintf("%02d", ($i + 1));
    $iorfiles[$i] = $iorfname_prefix . "_$servant_id.ior";
    $server_iorfiles[$i] = $server->LocalFile($iorfiles[$i]);
    $server->DeleteFile($iorfiles[$i]);
}


my @CLS = ();
my @clients_iorfile = ();
$count = 0;
for ($i = 0; $i < $num_servants; $i++) {
    for ($j = 0; $j < $num_clients_per_servant; $j++) {
        $clients_iorfile[$count] = $clients[$count]->LocalFile($iorfiles[$i]);
        $clients[$count]->DeleteFile($iorfiles[$i]);
        $CLS[$count] = $clients[$count]->CreateProcess ("client_main",
                                                        " -i file://$clients_iorfile[$count]");
        $count ++;
    }
}

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

# Wait for the servant ior files created by server.
for ($i = 0; $i < $num_servants; $i++) {
    if ($server->WaitForFileTimed ($iorfiles[$i],
                                   $server->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$server_iorfiles[$i]>\n";
        $SV->Kill (); $SV->TimedWait (1);
        exit 1;
    }
}

for ($i = 0; $i < $num_servants; $i++) {
    if ($server->GetFile ($iorfiles[$i]) == -1) {
        print STDERR "ERROR: cannot retrieve $i-th file <$server_iorfiles[$i]>\n";
        $SV->Kill (); $SV->TimedWait (1);
        exit 1;
    }
}

$count = 0;
for ($i = 0; $i < $num_servants; $i++) {
    for ($j = 0; $j < $num_clients_per_servant; $j++) {
        if ($clients[$count]->PutFile ($iorfiles[$i]) == -1) {
            print STDERR "ERROR: client $count cannot set file <$clients_iorfile[$count]>\n";
            $SV->Kill (); $SV->TimedWait (1);
            exit 1;
        }
        $count ++;
    }
}

$count = 0;

if ($collocated_test == 0) {
    for ($i = 0; $i < $num_servants; $i++) {
        for ($j = 0; $j < $num_clients_per_servant; $j++) {
            my $client_status = $CLS[$count]->Spawn();
            if ($client_status != 0) {
                print STDERR "ERROR: client $count Spawn returned $client_status\n";
                $status = 1;
            }
            $count ++;
        }
    }

    for ($i = 0; $i < $num_clients; $i++) {
        my $client_status = $CLS[$i]->WaitKill ($clients[$i]->ProcessStartWaitInterval() + 45);

        if ($client_status != 0) {
            print STDERR "ERROR: client $i WaitKill returned $client_status\n";
            $status = 1;
        }
    }
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval() + 45);

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}


#Delete ior files generated by this run.
for ($i = 0; $i < $num_servants; $i++) {
    $server->DeleteFile($iorfiles[$i]);
}

$count = 0;
for ($i = 0; $i < $num_servants; $i++) {
    for ($j = 0; $j < $num_clients_per_servant; $j++) {
        $clients[$count]->DeleteFile($iorfiles[$i]);
        $count++;
    }
}

exit $status;
