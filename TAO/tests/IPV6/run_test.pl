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

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $iorbase = "server.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

# -*- perl -*-
$server_arg = "-ORBdebuglevel $debug_level -o $server_iorfile -ORBUseSharedProfile 0 ";
$client_arg = "-k file://$client_iorfile ";

@configurations =
    ({
         description => "Testing server with '-ORBConnectIPV6Only 1'.\n",
         server => "$server_arg -ORBConnectIPV6Only 1",
         client => "$client_arg",
         error => 0,
         url => 0,
         url_address => "",
     },{
         description => "Testing client with '-ORBPreferIPV6Interfaces 1'.\n",
         server => "$server_arg ",
         client => "$client_arg -ORBPreferIPV6Interfaces 1",
         error => 0,
         url => 0,
         url_address => "",
     },{
         description => "Testing IPV4 server with client with '-ORBConnectIPV6Only 1'.\n",
         server => "$server_arg -ORBListenEndpoints iiop://127.0.0.1",
         client => "$client_arg -x -ORBConnectIPV6Only 1",
         error => 1,
         url => 0,
         url_address => "",
     },{
         description => "Testing IPV4 server with client with '-ORBPreferIPV6Interfaces 1'.\n",
         server => "$server_arg -ORBListenEndpoints iiop://127.0.0.1",
         client => "$client_arg -ORBPreferIPV6Interfaces 1",
         error => 0,
         url => 0,
         url_address => "",
     },{
         description => "Testing IPV4 server with client and URL-style IOR",
         server => "$server_arg -ORBDebuglevel 10 -ORBListenEndpoints iiop://0.0.0.0 -ORBObjRefStyle url",
         client => "$client_arg -ORBDebuglevel 10",
         error => 0,
         url => 0,
         url_address => "",
     },{
         description => "Testing IPV4 server with client and IPV6-forced URL",
         server => "$server_arg -ORBListenEndpoints iiop://0.0.0.0 -ORBObjRefStyle url",
         client => "-x",
         error => 1,
         url => 1,
         url_address => "corbaloc:iiop:1.2\@[::1]:",
     },{
         description => "Testing IPV6 server with client and IPV4-forced URL",
         server => "$server_arg -ORBListenEndpoints iiop://[::1] -ORBObjRefStyle url",
         client => "-x",
         error => 1,
         url => 1,
         url_address => "corbaloc:iiop:1.2\@127.0.0.1:",
     },{
         description => "Testing IPV6Only server with client and IPV4-forced URL",
         server => "$server_arg -ORBConnectIPV6Only 1 -ORBObjRefStyle url",
         client => "-x",
         error => 1,
         url => 1,
         url_address => "corbaloc:iiop:1.2\@127.0.0.1:",
     },{
         description => "Testing server with client and IPV4-forced URL",
         server => "$server_arg -ORBObjRefStyle url",
         client => "",
         error => 0,
         url => 1,
         url_address => "corbaloc:iiop:1.2\@127.0.0.1:",
     },
    );


sub run_test_
{
    my $server_args = $_[0]->{server};
    my $client_args = $_[0]->{client};
    my $error = $_[0]->{error};
    my $test_status  = 0;

    print STDERR "\n******************************************************\n";
    print $_[0]->{description};

    print "\nRunning server with the following args:\n$server_args\n\n";

    $SV = $server->CreateProcess ("server", $server_args);
    $server_status = $SV->Spawn ();

    if ($server_status != 0) {
        print STDERR "ERROR: server returned $server_status\n";
        return 1;
    }

    if ($server->WaitForFileTimed ($iorbase,
                               $server->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$server_iorfile>\n";
        $SV->Kill (); $SV->TimedWait (1);
        return 1;
    }

    if ($server->GetFile ($iorbase) == -1) {
        print STDERR "ERROR: cannot retrieve file <$server_iorfile>\n";
        $SV->Kill (); $SV->TimedWait (1);
        return 1;
    }

    if ($client->PutFile ($iorbase) == -1) {
        print STDERR "ERROR: cannot set file <$client_iorfile>\n";
        $SV->Kill (); $SV->TimedWait (1);
        return 1;
    }

    if ($_[0]->{url}) {
        my $fh;
        open $fh, $client_iorfile;
        my $url = <$fh>;
        close $fh;

        print "Changing corbaloc url from:\n$url\nto\n";

        my $url_addr = $_[0]->{url_address};
        if ($url =~ /corbaloc:iiop:1.[01234]\@[\[].*[\]]\:.*/) {
          $url =~ s/(corbaloc:iiop:1.[01234]\@[\[].*[\]]\:)(.*)/$url_addr$2/;
        }
        else {
          $url =~ s/(corbaloc:iiop:1.[01234]\@[^:]*\:)(.*)/$url_addr$2/;
        }
        print $url . "\n";

        $client_args .= " -k \"$url\"";
    }

    print "\nRunning client with the following args:\n$client_args\n\n";

    $CL = $client->CreateProcess ("client", $client_args);

    $client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 285);
    if ($client_status != 0) {
        print STDERR "ERROR: client returned $client_status\n";
        $test_status = 1;
    }

    if ($error) {
        $server_status = $SV->TerminateWaitKill ($server->ProcessStopWaitInterval());

        if ($server_status != 0) {
            print STDERR "ERROR: server returned $server_status\n";
            $test_status = 1;
        }
    }
    else {
        $server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

        if ($server_status != 0) {
            print STDERR "ERROR: server returned $server_status\n";
            $test_status = 1;
        }
    }

    return $test_status;
}

for $test (@configurations) {
    if (run_test_($test) != 0) {
        $status = 1;
    }

    $server->DeleteFile($iorbase);
    $client->DeleteFile($iorbase);
}

exit $status;
