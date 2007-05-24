eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use strict;
use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use Sys::Hostname;

my $status = 0;
my $host = hostname();
my $class = (PerlACE::is_vxworks_test() ? 'PerlACE::ProcessVX' :
                                          'PerlACE::Process');
my $SV = $class->new('server',
                     '-ORBUseSharedProfiles 0 ' .
                     '-ORBEndpoint iiop://localhost ' .
                     "-ORBEndpoint iiop://${host}");

my $server = $SV->SpawnWaitKill(20);

if ($server != 0) {
  print STDERR "ERROR: server returned $server \n";
  $status = 1;
}

exit($status);
