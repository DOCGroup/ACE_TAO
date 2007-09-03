eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

## Avoid code duplication by determining the process type and
## storing it as a string for use later.
my $class = (PerlACE::is_vxworks_test() ? 'PerlACE::ProcessVX' :
                                          'PerlACE::Process');

## First test that the -ORBSvcConfDirective works with good options.
my $SV = $class->new('server');
$SV->Spawn ();
my $server = $SV->WaitKill(20);
if ($server != 0) {
  print STDERR "ERROR: server returned $server\n";
  exit(1);
}

exit(0);
