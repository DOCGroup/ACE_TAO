eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib '../../../bin';
use PerlACE::Run_Test;
use POSIX "sys_wait_h";

$ENV{"PURIFYOPTIONS"}='-report-pmrs=no -leaks-at-exit=no -fds-inuse-at-exit=no -inuse-at-exit=no -thread-report-at-exit=no -search-mmaps -max_threads=200 -chain-length=20 -free-queue-length=200 -freeze-on-error=no -windows=yes -log-file=./%v-%p.pure.log -output-limit=20000000000 -show-directory -add-suppression-files=/home/dbinder/work/PURIFY-MT-SUPPRESSIONS';

$iorfile = PerlACE::LocalFile ("server$$.ior");
unlink $iorfile;

$debuglevel = int (rand() * 6) + 1;

$SV = new PerlACE::Process ("server", " -o $iorfile -ORBDebugLevel $debuglevel -ORBLogFile server.log");
$threads = int (rand() * 6) + 1;
$CL = new PerlACE::Process ("client", "-k file://$iorfile -t $threads");


$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 250) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

local $start_time = time();
local $max_running_time = 20 * 60;
local $elapsed = time() - $start_time;
my $p = $SV->{'PROCESS'};

while (($elapsed < $max_running_time) ) 
{
 # Start all clients in parallel
  $client = $CL->Spawn ();

  $CL->WaitKill(60) unless $client1 < 0;

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

  $server = $SV->WaitKill (30);
}

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
