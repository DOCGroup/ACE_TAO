eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use strict;

if ($^O eq 'hpux') {
  print "This test will not run properly on HP-UX.\n",
        "When one process uses up all of the file descriptors, no other\n",
        "processes run by the same user can start.\n";
  exit(0);
}

my $status = 0;
my $iorfile = 'server.ior';
my $logfile = 'server.log';
my $class = (PerlACE::is_vxworks_test() ? 'PerlACE::ProcessVX' :
                                          'PerlACE::Process');
my $SV = $class->new('server', '-ORBAcceptErrorDelay 5 -ORBDebugLevel 1 ' .
                               "-ORBLogFile $logfile");
my $CL = new PerlACE::Process('client');

unlink($iorfile, $logfile);
my $server = $SV->Spawn();

if ($server != 0) {
  print STDERR "ERROR: server returned $server\n";
  exit(1);
}

if (PerlACE::waitforfile_timed(
               $iorfile,
               $PerlACE::wait_interval_for_process_creation) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $SV->Kill();
  exit(1);
}

my $client = $CL->SpawnWaitKill(30);

if ($client != 0) {
  print STDERR "ERROR: client returned $client\n";
  $status = 1;
}

$server = $SV->WaitKill(5);

if ($server != 0) {
  print STDERR "ERROR: server returned $server\n";
  $status = 1;
}

if ($status == 0) {
  if (open(FH, $logfile)) {
    my $error_achieved = 0;
    my $reregister = 0;
    while(<FH>) {
      if (/TAO_Acceptor::handle_accept_error.+Too many files open/) {
        ++$error_achieved;
      }
      elsif (/TAO_Acceptor::handle_expiration.+registering\s+the\s+acceptor/) {
        ++$reregister;
      }
    }
    close(FH);

    if (!$error_achieved) {
      print "ERROR: The error situation was not achieved\n";
      ++$status;
    }
    if (!$reregister) {
      print "ERROR: The acceptor was not reregistered\n";
      ++$status;
    }
  }
}

unlink($iorfile, $logfile);

exit($status);
