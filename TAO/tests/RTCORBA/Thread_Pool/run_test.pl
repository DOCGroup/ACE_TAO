eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../../bin';
use PerlACE::Run_Test;

$number_of_clients = 4;
$status = 0;

@configurations = 
    (
     {
         file => "ior_1", 
         description => "Invoking methods on servant in default thread pool",
     },
     {
         file => "ior_2", 
         description => "Invoking methods on servant in first RT thread pool (without lanes)",
     },
     {
         file => "ior_3", 
         description => "Invoking methods on servant in second RT thread pool (with lanes)",
     },
     );

for $test (@configurations)
{
    unlink $test->{file};
}

sub run_clients
{
    my @parms = @_;
    $arg = $parms[0];
    $clients = $parms[1];

    for ($i = 0; $i < $clients; $i++)
    {
        $CL[$i] = new PerlACE::Process ("client", $arg);
        $CL[$i]->Spawn ();
    }

    for ($i = 0; $i < $clients; $i++)
    {    
        $client = $CL[$i]->WaitKill (120);
        if ($client != 0) 
        {
            print STDERR "ERROR: client returned $client\n";
            $status = 1;
            goto kill_server;
        }
    }
}

$SV = new PerlACE::Process ("server");

$SV->Spawn ();

for $test (@configurations)
{
    if (PerlACE::waitforfile_timed ($test->{file}, 10) == -1)
    {
        $server = $SV->TimedWait (1);
        if ($server == 2) 
        {
            # Mark as no longer running to avoid errors on exit.
            $SV->{RUNNING} = 0;
            exit $status;
        } 
        else 
        {
            print STDERR "ERROR: cannot find ior file: $test->{file}\n";
            $status = 1;
            goto kill_server;
	}
    }
  }
  
for $test (@configurations)
  {
    print STDERR "\n*************************************************************\n";
    print STDERR "$test->{description}\n";
    print STDERR "*************************************************************\n\n";
      
    run_clients ("-k file://$test->{file}", $number_of_clients);
  }

print STDERR "\n************************\n";
print STDERR "Shutting down the server\n";
print STDERR "************************\n\n";

run_clients ("-k file://$configurations[0]->{file} -i 0 -x", 1);

kill_server:

$server = $SV->WaitKill (5);

if ($server != 0) 
  {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
  }

for $test (@configurations)
  {
    unlink $test->{file};
  }

exit $status

