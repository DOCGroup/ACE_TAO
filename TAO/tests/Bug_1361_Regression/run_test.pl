eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use POSIX "sys_wait_h";

$iorfilebase = "server.ior";
$iorfile = PerlACE::LocalFile ("$iorfilebase");
unlink $iorfile;

$debug_opts = '';
foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_opts = '-ORBDebugLevel 10 -ORBVerboseLogging 1 '
                    . '-ORBLogFile client';
    } 
}

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server", "-o $iorfilebase");
}
else {
    $SV = new PerlACE::Process ("server", "-o $iorfile");
}
$threads = int (rand() * 6) + 1;
$CL = new PerlACE::Process ("client", "-k file://$iorfile -t $threads $debug_opts");


$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

local $start_time = time();
local $max_running_time = 720;
local $elapsed = time() - $start_time;
my $p = $SV->{'PROCESS'};

if ($ARGV[0] eq '-quick')  {
    $elapsed = 0;
    $max_running_time = 1;
}

my $client_idx = 0;
while (($elapsed < $max_running_time) )
{
 # ?? Start all clients in parallel
  my $args_saved = $CL->Arguments ();
  $CL->Arguments ("$args_saved$client_idx.log") unless $debug_opts eq '';
  $client = $CL->Spawn ();
  if ($debug_opts ne '') {
    $CL->Arguments ($args_saved);
    print "Spawned client $client_idx\n";
    ++$client_idx;
  }

  $CL->WaitKill(60) unless $client < 0;

  print STDERR "checking server alive\n";

  my $pid = waitpid ($SV->{PROCESS}, &WNOHANG);

  if ($pid != 0 && $? != -1)
  {
    $SV->check_return_value ($?);
    $server_died = 1;
    last;
  }

  $elapsed = time() - $start_time;
  sleep (1);
}

if (!$server_died) {
  $SH = new PerlACE::Process ("shutdown", "-k file://$iorfile");
  my $shutdown = $SH->Spawn();
  $SH->WaitKill(10) unless $shutdown < 0;

  $server = $SV->WaitKill (100);
}

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
