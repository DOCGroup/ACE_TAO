eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;
$iiop_port = 27532;
$tp_iiop_port = 27533;

$continuous = ($^O eq 'hpux');

$extra_server_args = ($continuous ? "-ORBSvcConf continuous$PerlACE::svcconf_ext " : '') .
                     "-d 1 -ORBEndpoint iiop://1.2\@:$iiop_port -ORBLaneEndpoint 2:0 iiop://1.2\@:$tp_iiop_port";

@iorfiles =
    (
     "not_used_ior_1",
     "not_used_ior_2",
     "persistent_ior",
     "transient_ior",
     "tp_persistent_ior",
     );

@configurations =
    (
     {
         iorfiles => [ "persistent_ior", "tp_persistent_ior", "transient_ior" ],
         server => "-a tp_persistent_ior -p persistent_ior -t transient_ior $extra_server_args",
         clients => [ "-k file://".PerlACE::LocalFile("tp_persistent_ior"),
                      "-k file://".PerlACE::LocalFile("persistent_ior"),
                      "-k file://".PerlACE::LocalFile("transient_ior")." -x" ],
     },
     {
         iorfiles => [ "not_used_ior_1", "not_used_ior_2", "transient_ior" ],
         server => "-a not_used_ior_1 -p not_used_ior_2 -t transient_ior $extra_server_args",
         clients => [ "-k file://".PerlACE::LocalFile("tp_persistent_ior"),
                      "-k file://".PerlACE::LocalFile("persistent_ior"),
                      "-k file://".PerlACE::LocalFile("transient_ior")." -x" ],
     },
     );

sub run_client
{
    print "\nRunning client with the following args: @_\n\n";

    $CL = new PerlACE::Process ("client", @_);

    $CL->Spawn ();

    $client = $CL->WaitKill (120);

    if ($client != 0)
    {
        print STDERR "ERROR: client returned $client\n";
        $status = 1;
        zap_server (1);
    }
}

sub run_server
{
    my @parms = @_;
    my $args = $parms[0];
    my $iorfiles = $parms[1];

    print "\nRunning server with the following args: $args\n\n";

    if (PerlACE::is_vxworks_test()) {
        $SV = new PerlACE::ProcessVX ("server", $args);
    }
    else {
        $SV = new PerlACE::Process ("server", $args);
    }

    $SV->Spawn ();

    # $server = $SV->Wait (10);
    # if ($server == 0)
    # {
    #    return 0;
    # }

    for $file (@$iorfiles)
    {
        if (PerlACE::waitforfile_timed ($file, 5) == -1)
        {
            print STDERR "ERROR: cannot find ior file: $file\n";
            $status = 1;
            zap_server (1);
        }
    }
}

sub zap_server
{
    $server = $SV->WaitKill (5);

    if ($server != 0)
    {
        print STDERR "ERROR: server returned $server\n";
        $status = 1;
    }

    if ($_[0])
    {
        for $file (@iorfiles)
        {
            unlink $file;
        }

        exit $status;
    }
}

for $file (@iorfiles)
{
    unlink $file;
}

for $test (@configurations)
{
    print STDERR "\n******************************************************\n";

    run_server ($test->{server}, $test->{iorfiles});

    my $clients = $test->{clients};
    for $args (@$clients)
    {
        run_client ($args);
    }

    zap_server (0);
}

for $file (@iorfiles)
{
    unlink $file;
}

exit $status;
