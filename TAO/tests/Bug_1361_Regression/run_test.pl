eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib '../../../bin';
use PerlACE::Run_Test;
use POSIX "sys_wait_h";

$ENV{"PURIFYOPTIONS"}='-report-pmrs=no -leaks-at-exit=no -fds-inuse-at-exit=no -inuse-at-exit=no -thread-report-at-exit=no -search-mmaps -max_threads=200 -chain-length=20 -free-queue-length=500 -freeze-on-error=no -windows=yes -log-file=./%v-%p.plog -output-limit=20000000000 -show-directory -add-suppression-files=/home/dbinder/work/PURIFY-MT-SUPPRESSIONS';

$iorfile = PerlACE::LocalFile ("server.ior");
unlink $iorfile;

$SV = new PerlACE::Process ("server", " -o $iorfile  -ORBDebugLevel 1 -ORBLogFile server.log");
$CL1 = new PerlACE::Process ("client", "-k file://$iorfile -t 1 -ORBLogFile client.log");
#$CL2 = new PerlACE::Process ("client", "-k file://$iorfile");
#$CL3 = new PerlACE::Process ("client", "-k file://$iorfile");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 250) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

local $start_time = time();
local $max_running_time = 75 * 60;
local $elapsed = time() - $start_time;
my $p = $SV->{'PROCESS'};
print STDERR "server process is $p\n";
while (($elapsed < $max_running_time) ) {

 # Start all clients in parallel
  $client1 = $CL1->Spawn ();
  print STDERR "started client\n";
 # $client2 = $CL2->Spawn ();
 # $client3 = $CL3->Spawn ();

  # ... wait for the clients to die unless they did not startup,
  # ignore errors because they intentionally crash themselves!
  #$CL3->WaitKill(60) unless $client3 < 0;
  #$CL2->WaitKill(60) unless $client2 < 0;
  $CL1->WaitKill(60) unless $client1 < 0;

  print STDERR "checking server alive\n";
  my $pid = waitpid ($SV->{PROCESS}, &WNOHANG);
  if ($pid != 0 && $? != -1) {
    $SV->check_return_value ($?);
    last;
    $server_died = 1;
  }            

  $elapsed = time() - $start_time;
}

if (!$server_died) {
  $SH = new PerlACE::Process ("shutdown", "-k file://$iorfile");
  my $shutdown = $SH->Spawn();
  $SH->WaitKill(10) unless $shutdown < 0;

    print STDERR "Going to call shutdwon\n";
  $server = $SV->WaitKill (5);
}

if ($server != 0) {
    print STDERR "Going to call error value...\n";
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
