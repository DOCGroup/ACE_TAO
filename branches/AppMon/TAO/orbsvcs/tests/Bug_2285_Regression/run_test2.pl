eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;
$number_of_servers = 5;

@SV;

unlink PerlACE::LocalFile ("last_expiration_time");
unlink PerlACE::LocalFile ("no_more_retries");

for ($counter = 0; $counter < $number_of_servers; $counter++)
{
    unlink PerlACE::LocalFile ("n".$counter.".ior");
}

for($counter=0; $counter < $number_of_servers; $counter++)
{
    if (PerlACE::is_vxworks_test()) {
        push (@SV, new PerlACE::ProcessVX ("server2", "-n $counter"));
    }
    else {
        push (@SV, new PerlACE::Process ("server2", "-n $counter"));
    }

    $SV[$counter]->Spawn ();
}

# Loops are cheap.
for ($counter2=0; $counter2 < $number_of_servers; $counter2++)
{
    if (PerlACE::waitforfile_timed (PerlACE::LocalFile ("n".$counter2.".ior"),
                        $PerlACE::wait_interval_for_process_creation) == -1)
    {
        print STDERR "ERROR: cannot find file <n$counter2.ior>\n";
        for ($kill_count = 0; $kill_count < $number_of_servers; $kill_count++)
        {
            $SV[$kill_count]->Kill (); $SV[$kill_count]->TimedWait (1);
        }
        exit 1;
    }
}

$CL = new PerlACE::Process ("client2", " -n $number_of_servers");

$client = $CL->SpawnWaitKill (300);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

for ($counter = 0; $counter < $number_of_servers; $counter++)
{
    $SV[$counter]->WaitKill (10);

    unlink PerlACE::LocalFile ("n".$counter.".ior");
}

unlink PerlACE::LocalFile ("last_expiration_time");
unlink PerlACE::LocalFile ("no_more_retries");

exit $status;
