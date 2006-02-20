eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-
# $Id$

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

my $status = 0;
my $file = PerlACE::LocalFile ("test.ior");

unlink $file;

# Set the SSL
# This doesn't work on Windows.  For some reason,
# environment variables aren't propagated to child processes.
# $ENV{'SSL_CERT_FILE'} = 'cacert.pem';

my @test_config =
(
    {
        description => "server supports, client supports protection, Invocations",
        server => "-ORBSvcConf server$PerlACE::svcconf_ext -s 0",
        client => "-ORBSvcConf client$PerlACE::svcconf_ext -d 0",
        fails => 0,
    },
    {
        description => "server supports, client supports protection, Client ID unavailable at the server, but forcing unprotected invocations",
        server => "-ORBSvcConf server$PerlACE::svcconf_ext -s 0",
        client => "-ORBSvcConf client$PerlACE::svcconf_ext -d 1",
        fails => 0,
    },
    {
        description => "server requires, client supports protection, Invocations",
        server => "-ORBSvcConf server$PerlACE::svcconf_ext -s 1",
        client => "-ORBSvcConf client$PerlACE::svcconf_ext -d 0",
        fails => 0,
    },
    {
        description => "server requires, client supports protection, Forcing unprotected invocations",
        server => "-ORBSvcConf server$PerlACE::svcconf_ext -s 1",
        client => "-ORBSvcConf client$PerlACE::svcconf_ext -d 1",
        fails => 1,
    },
    {
        description => "server requires, client does not support protection, Invocations",
        server => "-ORBSvcConf server$PerlACE::svcconf_ext -s 1",
        client => "-d 0",
        fails => 1,
    },
    {
        description => "server requires, client does not support protection, Forcing unprotected invocations",
        server => "-ORBSvcConf server$PerlACE::svcconf_ext -s 1",
        client => "-d 1",
        fails => 1,
    },
    {
        description => "server supports, client does not support protection, Invocations",
        server => "-ORBSvcConf server$PerlACE::svcconf_ext -s 0",
        client => "-d 0",
        fails => 0,
    },
    {
        description => "server supports, client does not support protection, Forcing unprotected invocations",
        server => "-ORBSvcConf server$PerlACE::svcconf_ext -s 0",
        client => "-d 1",
        fails => 1,
    },
);

my $failed_tests = 0;

for my $config (@test_config)
{

    print STDERR "\n\n==== Test: " .
        $config->{description} . " " .
        (($config->{fails} == 0) ? "succeed" : "fail") .
        "\n\n";

    unlink $file;
    my $SV = new PerlACE::Process ("server",
                                "-o $file $config->{server}");

    $SV->Spawn ();

    if (PerlACE::waitforfile_timed ($file, 15) == -1) {
        print STDERR "ERROR: cannot find file <$file>\n";
        $SV->Kill ();
        $status = 1;
    }
    else {

        my $CL = new PerlACE::Process ("client",
                                    "-k file://$file $config->{client} -f $config->{fails}");

        my $client = $CL->SpawnWaitKill (10);

        if ($client != 0) {
            print STDERR "ERROR: client returned $client\n";
            $status = 1;
            $failed_tests ++;
        }

        if ($config->{fails}) {
            $server = $SV->Kill ();
        }
        else {
            $server = $SV->WaitKill (5);
            if ($server != 0) {
                print STDERR "ERROR: server returned $server\n";
                $status = 1;
                $failed_tests ++;
            }
        }

    }

    unlink $file;
}


if ($status == 0) {
    print STDERR "\nSUCCESS: All tests succeeded\n";
}
else {
    print STDERR "\nERROR: ($failed_tests) tests failed. See above.\n";
}
exit $status;
