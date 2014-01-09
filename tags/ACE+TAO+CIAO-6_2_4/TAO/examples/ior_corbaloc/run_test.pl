eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

# This is a Perl Script that runs the corbaloc: style URL tests.
# It starts the servers and client.

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my @servers = (
    (PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n"),
    (PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n"),
    (PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n"));
my @iorbases = ("ns1.ior", "ns2.ior", "ns2.ior");
my @SV_NAMES = ("STATUS", "STATUS1", "STATUS");

# We run two tao_cosnaming processes.  One on an arbitrary port,
# and one on the corbaloc: default port of 2809
my @nstargets = (
    (PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n"),
    (PerlACE::TestTarget::create_target (5) || die "Create target 5 failed\n"));
my $def_port = 2809;
my @ns_orb_ports = ($nstargets[0]->RandomPort (),
                    $def_port);

my @clients = (
    (PerlACE::TestTarget::create_target (6) || die "Create target 6 failed\n"),
    (PerlACE::TestTarget::create_target (7) || die "Create target 7 failed\n"));

my @NSes = ();
my @ns_hostnames = ();
for ($i = 0; $i <= $#nstargets; $i++) {
    my $TARGETHOSTNAME = $nstargets[$i]->HostName ();
    my $ns_iorfile = $nstargets[$i]->LocalFile ($iorbases[$i]);
    $nstargets[$i]->DeleteFile($iorbases[$i]);
    foreach $svtarget (@servers) {
        my $sv_iorfile = $svtarget->LocalFile ($iorbases[$i]);
        $svtarget->DeleteFile($iorbases[$i]);
    }
    foreach $cltarget (@clients) {
        my $cl_iorfile = $cltarget->LocalFile ($iorbases[$i]);
        $cltarget->DeleteFile($iorbases[$i]);
    }

    my $NS = $nstargets[$i]->CreateProcess (
        "../../orbsvcs/Naming_Service/tao_cosnaming",
        "-ORBListenEndpoints iiop://$TARGETHOSTNAME:$ns_orb_ports[$i] ".
        "-o $ns_iorfile -m 0");

    print STDERR "Spawning Naming Service on port $ns_orb_ports[$i]......\n";

    $ns_status = $NS->Spawn ();

    if ($ns_status != 0) {
        print STDERR "ERROR: ns returned $ns_status\n";
        foreach $tmp (@NSes) {
            $tmp->Kill (); $tmp->TimedWait (1);
        }
        exit 1;
    }

    if ($nstargets[$i]->WaitForFileTimed (
            $iorbases[$i],
            $nstargets[$i]->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$ns_iorfile>\n";
        foreach $tmp (@NSes) {
            $tmp->Kill (); $tmp->TimedWait (1);
        }
        exit 1;
    }

    if ($nstargets[$i]->GetFile ($iorbases[$i]) == -1) {
        print STDERR "ERROR: cannot retrieve file <$ns_iorfile>\n";
        $NS->Kill (); $NS->TimedWait (1);
        foreach $tmp (@NSes) {
            $tmp->Kill (); $tmp->TimedWait (1);
        }
        exit 1;
    }

    foreach $svtarget (@servers) {
        my $sv_iorfile = $svtarget->LocalFile ($iorbases[$i]);

        if ($svtarget->PutFile ($iorbases[$i]) == -1) {
            print STDERR "ERROR: cannot set file <$sv_iorfile>\n";
            $NS->Kill (); $NS->TimedWait (1);
            foreach $tmp (@NSes) {
                $tmp->Kill (); $tmp->TimedWait (1);
            }
            exit 1;
        }
    }

    foreach $cltarget (@clients) {
        my $cl_iorfile = $cltarget->LocalFile ($iorbases[$i]);

        if ($cltarget->PutFile ($iorbases[$i]) == -1) {
            print STDERR "ERROR: cannot set file <$cl_iorfile>\n";
            $NS->Kill (); $NS->TimedWait (1);
            foreach $tmp (@NSes) {
                $tmp->Kill (); $tmp->TimedWait (1);
            }
            exit 1;
        }
    }

    push @NSes, $NS;
    push @ns_hostnames, $TARGETHOSTNAME;
}

sleep (2);

my @SVs = ();
for ($i = 0; $i <= $#servers; $i++) {
    my $sv_iorfile = $servers[$i]->LocalFile ($iorbases[$i]);

    my $SV = $servers[$i]->CreateProcess (
        "server",
        "-ORBInitRef NameService=file://$sv_iorfile $SV_NAMES[$i]");

    print STDERR "Spawning server with name $SV_NAMES[$i]\n";

    $server_status = $SV->Spawn ();

    if ($server_status != 0) {
        print STDERR "ERROR: server returned $server_status\n";
        foreach $tmp (@NSes) {
            $tmp->Kill (); $tmp->TimedWait (1);
        }
        foreach $tmp (@SVs) {
            $tmp->Kill (); $tmp->TimedWait (1);
        }
        exit 1;
    }

    push @SVs, $SV;
}

sleep (10);

# List of tests to test corbaloc URL syntax.
my @corbaloc_tests = (
    "corbaloc:rir:/NameService",
    "corbaloc:rir:/",
    "corbaloc:iiop:$ns_hostnames[0]:$ns_orb_port[0]/NameService",
    "corbaloc::$ns_hostnames[0]:$ns_orb_port[0]/NameService",
    "corbaloc::1.0\@$ns_hostnames[0]:$ns_orb_port[0]/NameService",
    "corbaloc::$ns_hostnames[0]/NameService",
    "corbaloc::1.1\@$ns_hostnames[0]:$ns_orb_port[0]/NameService",
    "corbaloc::1.2\@$ns_hostnames[0]:$ns_orb_port[0]/NameService");

my @comments = (
    "Using RIR protocol with the object key specified:\n",
    "Using RIR protocol without specifying the object key\n".
    "    (empty object key defaults to \"NameService\"):\n",
    "Using IIOP protocol with the hostname, port specified:\n",
    "Using IIOP protocol, with protocol unspecified, hostname\n".
    "    specified, port specified:\n",
    "Using the default protocol (iiop) with the hostname, port specified:\n",
    "Using IIOP protocol with protocol unspecified, hostname\n".
    "    specified, port unspecified (should default to $def_port):\n",
    "Using version 1.1 of protocol:\n",
    "Using version 1.2 of protocol:\n");

$test_number = 0;

my @CLs = ($clients[0]->CreateProcess ("client"),
           $clients[1]->CreateProcess ("corbaname_client"));

my @client_iorfiles = ($clients[0]->LocalFile ($iorbases[0]),
                       $clients[1]->LocalFile ($iorbases[1]));

print STDERR "\n======= Running corbaloc: tests\n";
foreach $o (@corbaloc_tests) {
    print STDERR "\n".$test_number.".  ".$comments[$test_number];
    print STDERR "             $o\n";

    $CLs[0]->Arguments ("$o -ORBInitRef NameService=file://$client_iorfiles[0]");

    $client_status =
        $CLs[0]->SpawnWaitKill ($clients[0]->ProcessStartWaitInterval() + 45);

    if ($client_status != 0) {
        print STDERR "ERROR: client returned $client_status\n";
        foreach $tmp (@NSes) {
            $tmp->Kill (); $tmp->TimedWait (1);
        }
        foreach $tmp (@SVs) {
            $tmp->Kill (); $tmp->TimedWait (1);
        }
        exit 1;
    }

    $test_number++;
}

@corbaname_tests = (
    "corbaname:iiop:$ns_hostnames[0]:$ns_orb_ports[0]#$SV_NAMES[0]",
    "corbaname::$ns_hostnames[0]:$ns_orb_ports[0]#$SV_NAMES[0]",
    "corbaname::$ns_hostnames[1]#$SV_NAMES[1]",
    "corbaname::$ns_hostnames[1]:$ns_orb_ports[1]#$SV_NAMES[1]",
    "corbaname:rir:#$SV_NAMES[1]".
    " -ORBInitRef NameService=file://$client_iorfiles[1]");

print STDERR "\n======= Running corbaname: tests\n";
foreach $o ( @corbaname_tests) {
    print STDERR "\n$test_number:  $o\n";

    $CLs[1]->Arguments ("$o $corbaname_tests[$i] -ORBdebuglevel $debug_level ");

    $client_status =
        $CLs[1]->SpawnWaitKill ($clients[1]->ProcessStartWaitInterval() + 45);

    if ($client_status != 0) {
        print STDERR "ERROR: client returned $client_status\n";
        foreach $tmp (@NSes) {
            $tmp->Kill (); $tmp->TimedWait (1);
        }
        foreach $tmp (@SVs) {
            $tmp->Kill (); $tmp->TimedWait (1);
        }
        exit 1;
    }

    $test_number++;
}

print STDERR "\nFinalize\n";

$i = 0;
foreach $ns (@NSes) {
    $ns_status =
        $ns->TerminateWaitKill ($nstargets[$i]->ProcessStopWaitInterval());

    if ($ns_status != 0) {
        print STDERR "ERROR: ns returned $ns_status\n";
        foreach $tmp (@NSes) {
            $tmp->Kill (); $tmp->TimedWait (1);
        }
        foreach $tmp (@SVs) {
            $tmp->Kill (); $tmp->TimedWait (1);
        }
        exit 1;
    }

    $nstargets[$i]->DeleteFile($iorbases[$i]);
    $i++;
}

$i = 0;
foreach $sv (@SVs) {
    $server_status =
        $sv->TerminateWaitKill ($servers[$i]->ProcessStopWaitInterval());

    if ($server_status != 0) {
        print STDERR "ERROR: server returned $server_status\n";
        foreach $tmp (@SVs) {
            $tmp->Kill (); $tmp->TimedWait (1);
        }
        exit 1;
    }

    $servers[$i]->DeleteFile($iorbases[$i]);
    $i++;
}

$clients[0]->DeleteFile($iorbases[0]);
$clients[0]->DeleteFile($iorbases[1]);
$clients[1]->DeleteFile($iorbases[0]);
$clients[1]->DeleteFile($iorbases[1]);

exit $status;
