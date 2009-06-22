eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use strict;
use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

my($prog) = 'MonitorControlExt';

## Avoid code duplication by determining the process type and
## storing it as a string for use later.
my $class = (PerlACE::is_vxworks_test() ? 'PerlACE::ProcessVX' :
                                          'PerlACE::Process');

my $SV = $class->new($prog);
my $server = $SV->SpawnWaitKill(10);

my $status = 0;
if ($server != 0) {
  print STDERR "ERROR: $prog returned $server\n";
  $status = 1;
}

exit($status);
