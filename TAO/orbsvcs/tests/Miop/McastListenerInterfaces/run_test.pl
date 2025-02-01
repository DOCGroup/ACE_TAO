eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# -*- perl -*-

use lib "$ENV{'ACE_ROOT'}/bin";
use PerlACE::TestTarget;
use Env;

my $IOR_BASE = "server.ior";
my $SVC_CONF_BASE = "miop_svc.conf";
my $ATTACHING_DEBUGGER = $ENV{ATTACHING_DEBUGGER} eq "true" ? 1 : 0;
my $COPY_PREFERRED_INTERFACES = "CopyPreferredInterfaces";

my $LOOPBACK = 2;
my $PUBLIC = 3;
my $IPv4 = 4;
my $IPv6 = 6;

## FF00::/16, FF01::/16, FF02::/16, and FF0F::/16 are prohibited/reserved multicast scopes which TAO cannot use on
## many platforms.
my %MULTICAST_ADDRESSES = (
    $IPv4 => "225.1.1.8",
    $IPv6 => "[FF03::25:334]",
);

# { "version" => $IPv4|$IPv6, "type" => $LOOPBACK|$PUBLIC, "interface" => [if_name], "address" => [ip_addr], "via" => [if_name|ip_addr]}
my @TESTS = ();

my $helper_output = `if_addrs_helper`;
my $helper_rc = $?;
if ($helper_rc == 0) {
    my @lines = split (/\n/, $helper_output);
    for my $line (@lines) {
        $line =~ s/^\s+|\s+$//g;
        my ($type, $result) = split(/:/, $line, 2);
        $result =~ s/^\s+|\s+$//g;
        if ($type eq "LOv4" || $type eq "LOv6" || $type eq "ETHv4" || $type eq "ETHv6") {
            ($if_name, $addr) = split(/ /, $result, 2);
            push @TESTS, {
                "version" => $type eq "LOv6" || $type eq "ETHv6" ? $IPv6 : $IPv4,
                "type" => $type eq "LOv4" || $type eq "LOv6" ? $LOOPBACK : $PUBLIC,
                "interface" => $if_name,
                "address" => $addr,
                "via" => "*=$addr",
                $COPY_PREFERRED_INTERFACES => 0,
            };
            push @TESTS, {
                "version" => $type eq "LOv6" || $type eq "ETHv6" ? $IPv6 : $IPv4,
                "type" => $type eq "LOv4" || $type eq "LOv6" ? $LOOPBACK : $PUBLIC,
                "interface" => $if_name,
                "address" => $addr,
                "via" => "*=$if_name",
                $COPY_PREFERRED_INTERFACES => 0,
            };
            if ($type eq "ETHv4" || $type eq "ETHv6") {
                push @TESTS, {
                    "version" => $type eq "LOv6" || $type eq "ETHv6" ? $IPv6 : $IPv4,
                    "type" => $type eq "LOv4" || $type eq "LOv6" ? $LOOPBACK : $PUBLIC,
                    "interface" => $if_name,
                    "address" => $addr,
                    "via" => "*=$addr",
                    $COPY_PREFERRED_INTERFACES => 1,
                };
                push @TESTS, {
                    "version" => $type eq "LOv6" || $type eq "ETHv6" ? $IPv6 : $IPv4,
                    "type" => $type eq "LOv4" || $type eq "LOv6" ? $LOOPBACK : $PUBLIC,
                    "interface" => $if_name,
                    "address" => $addr,
                    "via" => "*=$if_name",
                    $COPY_PREFERRED_INTERFACES => 1,
                };
            }
        } else {
            print "$line\n";
        }
    }

    if (!@TESTS) {
        print "ERROR: Cannot run this test because if_addrs_helper did not return any useful results.";
        if ($helper_output) {
            print " Output from if_addrs_helper:\n$helper_output";
        } else {
            print "\n";
        }
        exit 1;
    }
} else {
    print "ERROR: Cannot run this test because if_addrs_helper failed.";
    if ($helper_output) {
        print " Output from if_addrs_helper:\n$helper_output";
    } else {
        print "\n";
    }
    exit 1;
}

sub testToString ($) {
    my $test = shift;

    return "" .
           ($test->{"version"} == $IPv4 ? "IPv4" : "IPv6") . ", " .
           ($test->{"type"} == $LOOPBACK ? "loopback interface" : "public interface") . ", interface '" .
           $test->{"interface"} . "', address '" . $test->{"address"} . "', multicast address '" .
           $MULTICAST_ADDRESSES{$test->{"version"}} . "', via '" . $test->{"via"} ."', $COPY_PREFERRED_INTERFACES = " .
           ($test->{$COPY_PREFERRED_INTERFACES} ? "true" : "false");
    ;
}

sub timedWaitWrapper ($$) {
    # For some reason, ->TimedWait does not change the RUNNING flag on processes that have already stopped, making
    # future commands against those processes think they are still running and unable to determine otherwise. This
    # wrapper implementes the behavior that this test needs, but that might break other tests (hence not changing
    # TimedWait itself).
    my $process = shift;
    my $timeout = shift;

    my $rc = $process->TimedWait ($timeout);
    if ($rc != -1) {
        $process->{RUNNING} = 0;
    }
    return $rc;
}

sub startWaitIntervalWrapper($$) {
    my $process = shift;
    my $timeout_override = shift;

    if ($timeout_override > 0) {
        #print "DEBUG: Using timeout override $timeout_override\n";
        return $timeout_override;
    }
    my $timeout = $process->ProcessStartWaitInterval();
    #print "DEBUG: Using default timeout $timeout\n";
    return $timeout;
}

print "NOTE: Will run the following tests: \n";
for my $test (@TESTS) {
    print "  - " . testToString ($test) . "\n";
}
print "\n";

my $status = 0;
my $i = 1;

for my $test (@TESTS) {
    if ($i > 1) {
        print "--------------------------------------------------------------------------------\n\n";
    }
    print "////// RUNNING TEST #$i //////\nDetails: " . testToString ($test) . "\n";
    $i++;

    my $timeout_override = 0;
    if ($ATTACHING_DEBUGGER) {
        print "Enter a timeout override when the debugger is ready (or just press ENTER to use default timeout): ";
        my $timeout_str = <STDIN>;
        if ($timeout_str) {
            chomp $timeout_str;
            if ($timeout_str) {
                $timeout_override = 0 + $timeout_str;
            }
        }
    }

    my $unicast_address = $test->{"address"};
    my $multicast_address = $MULTICAST_ADDRESSES{$test->{"version"}};
    my $via = $test->{$COPY_PREFERRED_INTERFACES} ? $COPY_PREFERRED_INTERFACES : $test->{"via"};

    my $svc_conf = "
dynamic UIPMC_Factory Service_Object * TAO_PortableGroup:_make_TAO_UIPMC_Protocol_Factory() \"-ORBListenerInterfaces $via\"
static Resource_Factory \"-ORBProtocolFactory IIOP_Factory -ORBProtocolFactory UIPMC_Factory\"
dynamic PortableGroup_Loader Service_Object * TAO_PortableGroup:_make_TAO_PortableGroup_Loader() \"\"
";
    if ($ATTACHING_DEBUGGER) {
        print "DEBUG: $svc_conf";
    }

    my $server_box = PerlACE::TestTarget::create_target (1) || die "Creating server target failed\n";
    my $client_box = PerlACE::TestTarget::create_target (2) || die "Creating client target failed\n";
    my $server_ior_file = $server_box->LocalFile ($IOR_BASE);
    my $client_ior_file = $client_box->LocalFile ($IOR_BASE);
    my $server_svc_conf_file = $server_box->LocalFile ($SVC_CONF_BASE);
    my $client_svc_conf_file = $client_box->LocalFile ($SVC_CONF_BASE);

    $server_box->DeleteFile ($IOR_BASE);
    $client_box->DeleteFile ($IOR_BASE);
    $server_box->DeleteFile ($SVC_CONF_BASE);
    $client_box->DeleteFile ($SVC_CONF_BASE);

    open(FH, '>', $SVC_CONF_BASE) or die "Could not open file $SVC_CONF_BASE for writing: $!";
    print FH $svc_conf;
    close(FH);

    $server_box->PutFile ($SVC_CONF_BASE);
    $client_box->PutFile ($SVC_CONF_BASE);

    my $port = $server_box->RandomPort ();

    my $server = $server_box->CreateProcess (
        "server",
        "-o $server_ior_file -u corbaloc:miop:1.0\@1.0-foo-1/$multicast_address:$port -ORBSvcConf $server_svc_conf_file"
    );
    my $client = undef;
    if ($test->{$COPY_PREFERRED_INTERFACES} ) {
        $via = $test->{"via"};
        print "Testing with '-ORBPreferredInterfaces $via' as well\n";
        $client = $client_box->CreateProcess (
            "client",
            "-ORBPreferredInterfaces $via -ORBEnforcePreferredInterfaces=1 -ORBSvcConf $client_svc_conf_file -ORBIPMulticastLoop 1 -k corbaloc:miop:1.0\@1.0-foo-1/$multicast_address:$port"
        );
    } else {
        $client = $client_box->CreateProcess (
            "client",
            "-ORBSvcConf $client_svc_conf_file -ORBIPMulticastLoop 1 -k corbaloc:miop:1.0\@1.0-foo-1/$multicast_address:$port"
        );
    }

    print "Starting server...\n";
    my $server_status = $server->Spawn ();
    if ($server_status != 0) {
        print STDERR "ERROR: server returned $server_status\n";
        next;
    }
    print "Server started. Waiting for IOR file...\n";
    if ($server_box->WaitForFileTimed ($IOR_BASE, startWaitIntervalWrapper ($server_box, $timeout_override)) == -1) {
        $server_status = timedWaitWrapper ($server, 0.1);
        if ($server_status != -1) {
            print STDERR "ERROR: server died with rc $server_status before IOR file was created.\n";
            $server->Kill ();
            timedWaitWrapper ($server, 1);
            $status = 1;
            next;
        }
        print STDERR "ERROR: cannot find file <$server_ior_file>\n";
        $server->Kill ();
        timedWaitWrapper ($server, 1);
        $status = 1;
        next;
    }
    if ($server_box->GetFile ($IOR_BASE) == -1) {
        print STDERR "ERROR: cannot retrieve file <$server_ior_file>\n";
        $server->Kill ();
        timedWaitWrapper ($server, 1);
        $status = 1;
        next;
    }
    if ($client_box->PutFile ($IOR_BASE) == -1) {
        print STDERR "ERROR: cannot set file <$client_ior_file>\n";
        $server->Kill ();
        timedWaitWrapper ($server, 1);
        $status = 1;
        next;
    }

    print "Copied IOR file from server to client. Starting and waiting on client...\n";
    my $client_status = $client->SpawnWaitKill (startWaitIntervalWrapper ($client_box, $timeout_override));
    print $client_status == -1 ? "Client status: timed out and killed.\n" : "Client exit code: $client_status\n";

    $server_status = $server->WaitKill ($server_box->ProcessStopWaitInterval());
    print $server_status == -1 ? "Server status: timed out and killed.\n" : "Server exit code: $server_status\n";

    if ($client_status != 0) {
        print "TEST RESULT: FAILED (client should never be rc != 0)\n";
        $status = 1;
    } elsif ($server_status != 0) {
        if ($test->{"type"} == $LOOPBACK && $server_status == -1) {
            print "TEST RESULT: SUCCESS (server should time out with loopback as multicast listener interface)\n";
        } else {
            print "TEST RESULT: FAILED\n";
            $status = 1;
        }
    } else {
        if ($test->{"type"} == $LOOPBACK) {
            print "TEST RESULT: FAILED (server should time out with loopback as multicast listener interface)\n";
            $status = 1;
        } else {
            print "TEST RESULT: SUCCESS\n";
        }
    }

    unlink $SVC_CONF_BASE;
}

print "\n";

exit $status;
