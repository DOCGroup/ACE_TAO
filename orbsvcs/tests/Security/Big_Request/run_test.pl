eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use Getopt::Long;

$status = 0;
$opt = "";
$debug_level = '0';
$svc_conf = $PerlACE::svcconf_ext;
$conf_client = "";
$conf_server = "";

sub options () {
    my $help = 0;     # handled locally
    my $man = 0;      # handled locally
    my $ssl = 1;      # handled locally
    my $dotdec = 0;   # handled locally
    my $debug;        # handled locally
    my $shost;        # handled locally
    my $chost;        # handled locally
    my $clog;         # handled locally
    my $slog;         # handled locally

    # Process options.
    if ( @ARGV > 0 ) {
        GetOptions ('help|?' => \$help,
                'manual' => \$man,
                'ssl' => \$ssl,
                'dd=s' => \$dotdec,
                'shost=s' => \$shost,
                'chost=s' => \$chost,
                'slog=s' => \$slog,
                'clog=s' => \$clog,
                'debug=i' => \$debug) or pod2usage(2);
    }

    if ($ssl) {
        $conf_client = " -ORBSvcConf client$svc_conf";
        $conf_server = " -ORBSvcConf server$svc_conf";
    }

    if ($debug) {
        $opt = "$opt -ORBDebugLevel $debug";
    }

    if ($dotdec) {
        if ($dotdec =~ /client/) {
            $conf_client = "$conf_client -ORBDottedDecimalAddresses 1";
        }
        if ($dotdec =~ /server/) {
            $conf_server = "$conf_server -ORBDottedDecimalAddresses 1";
        }
    }

    if ($slog) {
        $conf_server = "$conf_server -ORBLogFile $slog";
    }

    if ($clog) {
        $conf_client = "$conf_client -ORBLogFile $clog";
    }

    if ($shost) {
        $conf_server = "$conf_server -ORBListenEndpoints iiop:///hostname_in_ior=$shost";
    }

    if ($chost) {
       $conf_client = "$conf_client -ORBListenEndpoints iiop:///hostname_in_ior=$chost";
    }

    if ( $man or $help ) {
        # Load Pod::Usage only if needed.
        require "Pod/Usage.pm";
        import Pod::Usage;
        pod2usage(1) if $help;
        pod2usage(VERBOSE => 2) if $man;
        return 0;
    }
    return 1;
}

options () or die "Error: Nothing executed";

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $iorbase = "test.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);


$SV = $server->CreateProcess ("server",
                              "$conf_server " .
                              "$opt " .
                              "-o $server_iorfile ");

$CL = $client->CreateProcess ("client",
                              "$conf_client " .
                              "$opt " .
                              "-k file://$client_iorfile " .
                              "-x");

print STDERR "\n\n==== Running SSLIOP Big_Request test\n";

print STDERR "Executing: server $conf_server $opt -o $iorbase\n";

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server->WaitForFileTimed ($iorbase,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
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

print STDERR "Executing: client $conf_client $opt\n";

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 165);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

exit $status;
