eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;
$iorfile = PerlACE::LocalFile("ior");

@configurations =
    (
     {
         server => "-b empty_file -l empty_file",
         client => "-b empty_file -p empty_file -x",
     },
     {
         server => "-b bands -l empty_file",
         client => "-b empty_file -p empty_file -x",
     },
     {
         server => "-b empty_file -l lanes",
         client => "-b empty_file -p empty_file -x",
     },
     {
         server => "-b bands -l lanes",
         client => "-b empty_file -p empty_file -x",
     },
     {
         server => "-b empty_file -l empty_file",
         client => "-b bands -p empty_file -x",
     },
     {
         server => "-b empty_file -l lanes",
         client => "-b bands -p empty_file -x",
     },
     {
         server => "-b empty_file -l empty_file",
         client => "-b empty_file -p invocation_priorities -x",
     },
     {
         server => "-b bands -l empty_file",
         client => "-b empty_file -p invocation_priorities -x",
     },
     {
         server => "-b bands -l lanes",
         client => "-b empty_file -p invocation_priorities -x",
     },
     {
         server => "-b empty_file -l empty_file",
         client => "-b bands -p invocation_priorities -x",
     },
     {
         server => "-b empty_file -l lanes",
         client => "-b bands -p invocation_priorities -x",
     },
     );

sub run_client
{
    my $arg = shift;
    $CL = new PerlACE::Process ("client", "-k file://$iorfile " . $arg);

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
    if (PerlACE::is_vxworks_test()) {
        $SV = new PerlACE::ProcessVX ("server", @_);
    }
    else {
        $SV = new PerlACE::Process ("server", @_);
    }
    if ($SV->Spawn () == -1) {
    exit 1;
    }

    if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1)
    {
        check_supported_priorities ($SV);
	print STDERR "ERROR: cannot find ior file: $iorfile\n";
	$status = 1;
	zap_server (1);
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

    unlink $iorfile;

    if ($_[0])
    {
        exit $status;
    }
}

sub check_supported_priorities
{
  $process = shift;
  $returnVal = $process->TimedWait (1);
  if ($returnVal == 2) {
    # Mark as no longer running to avoid errors on exit.
    $process->{RUNNING} = 0;
    exit 0;
  }
}

for $test (@configurations)
{
    print STDERR "\n******************************************************\n";

    unlink $iorfile;

    run_server ($test->{server});

    run_client ($test->{client});

    zap_server (0);
}

exit $status;
