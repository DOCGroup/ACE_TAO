eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../../bin';
use PerlACE::Run_Test;

$status = 0;
$iorfile = "ior";

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
    $SV = new PerlACE::Process ("server", @_);

    $SV->Spawn ();

    if (PerlACE::waitforfile_timed ($iorfile, 5) == -1)
    {
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

for $test (@configurations)
{
    print STDERR "\n******************************************************\n";

    unlink $iorfile;

    run_server ($test->{server});

    run_client ($test->{client});
    
    zap_server (0);
}

exit $status;
