eval '(exit $?0)' && eval 'exec perl -w -S $0 ${1+"$@"}'
     & eval 'exec perl -w -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use strict;
use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

my($prog) = 'MonitorManager';

## Avoid code duplication by determining the process type and
## storing it as a string for use later.
my $class = (PerlACE::is_vxworks_test() ? 'PerlACE::ProcessVX' :
                                          'PerlACE::Process');

my $SV = $class->new($prog);
my $server = $SV->Spawn();

if ($server != 0) {
  print STDERR "ERROR: $prog returned $server\n";
  exit(1);
}

sleep($PerlACE::wait_interval_for_process_creation / 3);

$prog = 'MonitorClient';
my $CL = new PerlACE::Process($prog);
my $client = $CL->SpawnWaitKill(20);

my $status = 0;
if ($client != 0) {
  print STDERR "ERROR: $prog returned $client\n";
  $SV->Kill();
  $status = 1;
}

$SV->WaitKill(10);

exit($status);
