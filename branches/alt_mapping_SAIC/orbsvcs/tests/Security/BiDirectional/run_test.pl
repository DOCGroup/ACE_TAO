eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use Getopt::Long;

$status = 0;

$status = 0;
$opt = "";
$conf_client = "";
$conf_server = "";
$iter = 10;

sub options () {
    my $help = 0;       # handled locally
    my $man = 0;        # handled locally
    my $ssl = 1;        # handled locally
    my $dotdec = 0;     # handled locally
    my $debug;        # handled locally
    my $shost;        # handled locally
    my $chost;        # handled locally

    # Process options.
    if ( @ARGV > 0 ) {
        GetOptions ('help|?' => \$help,
                'manual' => \$man,
                'iter=i' => \$iter,
                'ssl' => \$ssl,
                'dd=s' => \$dotdec,
                'shost=s' => \$shost,
                'chost=s' => \$chost,
                'debug=i' => \$debug) or pod2usage(2);
    }

    if ($ssl) {
        $conf_client = " -ORBSvcConf client.conf";
        $conf_server = " -ORBSvcConf server.conf";
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


# Make sure OpenSSL knows where to find the trust store
$ENV{'SSL_CERT_DIR'} = './ssl';

options () or die "Error: Nothing executed";

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $iorbase = "test.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

print STDERR "Executing: server $conf_server $opt -o $iorbase -i $iter\n";

$SV = $server->CreateProcess ("server",
                              "$conf_server " .
                              "$opt " .
                              "-o $server_iorfile " .
                              "-i $iter");

$CL = $client->CreateProcess ("client",
                              "$conf_client " .
                              "$opt " .
                              "-k file://$client_iorfile");
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

print STDERR "Executing: server find file\n";

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

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());

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

__END__

=head1 NAME

run_test.pl - A driver to run the test

=head1 SYNOPSIS

B<run_test.pl> [B<-help|?>] [B<-iter iterations>] [B<-chost host>]
               [B<-dd who>] [B<-shost host>] [B<-man>] [B<-ssl>]
               [B<-debug level>]


=head1 DESCRIPTION

This is a test that exercises the birectional GIOP connection
implementation in TAO over SSLIOP connection. Start the server like this

    $ server -ORBSvcConf server.conf -o <file.ior> -i <no_iterations>
    $ client -ORBSvcConf client.conf -k file://<file.ior> -x

Or, simply

    $ ./run_test.pl

The server starts up writing the IOR to the file. The client then starts
up, creates its own object and passes the reference to the server. Then
it invokes a method on the server to indicate that it is ready for
callback. The server then callsback the client on the same connection
<no_iterations> times. If the server creates a new connection the server
would crash itself.

Basicaly, the test is a copy of the $TAO_ROOT/tests/BiDirectional with
added support for SSLIOP.

=head1 OPTIONS

[B<-help|?>]
    Print a short help message

[B<-iter iterations>]
    The number of iterations to make. Default is 10.

[B<-chost hostname>]
    The hostname_in_ior for the client. Used only if specified.

[B<-shost hostname>]
    The hostname_in_ior for the server. Used only if specified.

[B<-man>]
    Prints this nice manual page

[B<-dd who>]
    Request the usage of -ORBDottedDecimalAddresses 1" for the client, the
    server or both. Values of who can be "client", "server" or
    "client,server".

[B<-ssl>]
    Enables the use of SSLIOP instead of IIOP. Default.

[B<-debug level>]
    Enables debugging. Values for level are 1-10. Default is 0.

=cut
