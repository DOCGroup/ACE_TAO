eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../../bin';
use PerlACE::Run_Test;

$status = 0;

@iorfiles = 
    (
     "root",
     "child",
     "default_pool_no_bands_client_propagated",
     "default_pool_no_bands_server_declared",
     "no_lanes_no_bands_client_propagated",
     "no_lanes_no_bands_server_declared",
     "lanes_no_bands_client_propagated",
     "lanes_no_bands_server_declared",
     "default_pool_bands_client_propagated",
     "default_pool_bands_server_declared",
     "no_lanes_bands_client_propagated",
     "no_lanes_bands_server_declared",
     "lanes_bands_client_propagated",
     "lanes_bands_server_declared",
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
        goto kill_server;
    }
}

for $file (@iorfiles)
{
    unlink $file;
}

$SV = new PerlACE::Process ("server");

$SV->Spawn ();

for $file (@iorfiles)
{
    if (PerlACE::waitforfile_timed ($file, 5) == -1)
    {
        print STDERR "ERROR: cannot find ior file: $file\n";
        $status = 1;
        goto kill_server;
    }

    print STDERR "\n******************************************************\n";
    print STDERR "Invoking methods on servant in $file poa\n";
    print STDERR "******************************************************\n\n";

    run_client ("-k file://$file");
}

print STDERR "\n**************************\n";
print STDERR "Shutting down the server\n";
print STDERR "**************************\n\n";

run_client ("-k file://$iorfiles[0] -i 0 -x");

kill_server:

$server = $SV->WaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1
}

for $file (@iorfiles)
{
    unlink $file;
}

exit $status
