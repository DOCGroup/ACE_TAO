eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../../bin';
use PerlACE::Run_Test;

$iiop_port = 27532;

$extra_server_args = "-d 1 -ORBobjrefstyle url -ORBEndpoint iiop://1.0\@:$iiop_port";

@iorfiles = 
    (
     "persistent_ior",
     "transient_ior",
     "not_used_ior",
     );
     
@configurations = 
    (
     {
         iorfiles => [ "persistent_ior", "transient_ior" ],
         server => "-p persistent_ior -t transient_ior $extra_server_args", 
         clients => [ "-k file://persistent_ior", "-k file://transient_ior -x" ],
     },
     {
         iorfiles => [ "persistent_ior", "transient_ior", "not_used_ior" ],
         server => "-p not_used_ior -t transient_ior $extra_server_args", 
         clients => [ "-k file://not_used_ior", "-k file://transient_ior -x" ],
     },
     );

sub run_client
{
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

    $SV = new PerlACE::Process ("server", $args);

    $SV->Spawn ();

    for $file (@$iorfiles)
    {
        if (PerlACE::waitforfile_timed ($file, 10) == -1)
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
