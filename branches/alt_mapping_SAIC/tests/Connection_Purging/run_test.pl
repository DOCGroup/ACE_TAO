eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';
$use_uiop = 0;
$use_shmiop = 0;

foreach $arg (@ARGV) {
    if ($arg eq '-debug') {
        $debug_level = '10';
    }
    elsif ($arg eq "-h") {
      print "Usage: $0 [-u | -s]\n" .
            "       -u for UIOP\n" .
            "       -s for SHMIOP\n";
      exit(0);
    }
    elsif ($arg eq "-u") {
        $use_uiop = 1;
    }
    elsif ($arg eq "-s") {
        $use_shmiop = 1;
    }
}

#constants
my $iorbase = "server.ior";
my $socket_name = "socket";
my $server_count = 20;
my @clients_conf = ("client.lru$PerlACE::svcconf_ext", "client.lfu$PerlACE::svcconf_ext",
                    "client.fifo$PerlACE::svcconf_ext", "client.null$PerlACE::svcconf_ext");

#create test targets
my @servers = ();
for($i = 0; $i < $server_count; $i++) {
    $servers[$i] = PerlACE::TestTarget::create_target ($i+1) || die "Create target $i+1 failed\n";
}

my @clients = ();
my $clients_count = 0;

$i = 0;
$j = $server_count + 1;
foreach (@clients_conf) {
    $clients[$i] = PerlACE::TestTarget::create_target ($j) || die "Create target $j failed\n";
    $i++; $j++; $clients_count++;
}

# resources clean
my @servers_iorfile = ();
my @servers_socket = ();
my @servers_endpoint = ();

my @SVS = ();

for($i = 0; $i < $server_count; $i++) {
    $servers_iorfile[$i] = $servers[$i]->LocalFile ("$iorbase.$i");
    $servers[$i]->DeleteFile("$iorbase.$i");
    if ($use_uiop) {
        $servers_socket[$i] = $servers[$i]->LocalFile ("$socket_name.$i");
        $servers[$i]->DeleteFile("$socket_name.$i");
        $servers_endpoint[$i] = "-ORBEndpoint uiop://$servers_socket[$i]";
    }
    elsif ($use_shmiop) {
        $server_conf_base = "server_shmiop$PerlACE::svcconf_ext";
        $server_shmiop_conf = $servers[$i]->LocalFile ($server_shmiop_conf);
        if ($servers[$i]->PutFile ($server_conf_base) == -1) {
            print STDERR "ERROR: cannot set file <$server_shmiop_conf>\n";
            exit 1;
        }
        $servers_endpoint[$i] = "-ORBEndpoint shmiop:// -ORBSvcConf $server_shmiop_conf";
    }
    else {
        $servers_endpoint[$i] = "";
    }

    $SVS[$i] = $servers[$i]->CreateProcess ("server", "-ORBdebuglevel $debug_level ".
                                                 "-o $servers_iorfile[$i] ".
                                                 "$servers_endpoint[$i]");

    my $server_status = $SVS[$i]->Spawn ();

    if ($server_status != 0) {
        print STDERR "ERROR: server Spawn $i returned $server_status\n";
    }
    else {
        if ($servers[$i]->WaitForFileTimed ("$iorbase.$i",
                               $servers[$i]->ProcessStartWaitInterval()) == -1) {
            print STDERR "ERROR: server $i cannot find file <$servers_iorfile[$i]>\n";
            $server_status = 1;
        }
    }

    if ($server_status != 0) {
        for($j = 0; $j < $i; $j++) {
            $SVS[$i]->Kill(); $SVS[$i]->TimedWait(1);
        }
        exit 1;
    }
}

my @clients_iorfile = ();
for($i = 0; $i < $clients_count; $i++) {
    $clients_iorfile[$i] = $clients[$i]->LocalFile ($iorbase);
    $clients[$i]->DeleteFile($iorbase);
}

my @CLS = ();
for($i = 0; $i < $clients_count; $i++) {
    print "========== Client using $clients_conf[$i] configurator file =========\n";
    if ($debug_level < 1) {
        $debug_level = 1; #min value for debug level is one
    }
    $client_conf_file = $clients[$i]->LocalFile ($clients_conf[$i]);
    if ($clients[$i]->PutFile ($clients_conf[$i]) == -1) {
        print STDERR "ERROR: cannot set file <$client_conf_file>\n";
    }
    $CLS[$i] = $clients[$i]->CreateProcess ("client", "-ORBDebugLevel $debug_level ".
                                                      "-k $clients_iorfile[$i] ".
                                                      "-ORBSvcConf $client_conf_file");

    my $client_status = $CLS[$i]->SpawnWaitKill ($clients[$i]->ProcessStartWaitInterval() + 75);

    if ($client_status != 0) {
        print STDERR "ERROR: client $i returned $client_status\n";
        $status = 1;
    }
}

for($i = 0; $i < $server_count; $i++) {
    $SVS[$i]->Kill ();
    $servers[$i]->DeleteFile("$socket_name.$i");
    $servers[$i]->DeleteFile("$iorbase.$i");
}

exit $status;
