eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use File::Spec;

## Avoid code duplication by determining the process type and
## storing it as a string for use later.
my $class = (PerlACE::is_vxworks_test() ? 'PerlACE::ProcessVX' :
                                          'PerlACE::Process');

## First test that the -ORBSvcConfDirective works with good options.
my $SV = $class->new('server', '-s');
$SV->Spawn ();
my $server = $SV->WaitKill($PerlACE::wait_interval_for_process_creation);
if ($server != 0) {
  print STDERR "ERROR: server returned $server\n";
  exit(1);
}

## Next, test that the -ORBSvcConfDirective gives an error with bad
## options.
$SV = $class->new('server', '-f');

## Run the server, but redirect stdout and stderr to /dev/null
## so that the errors messages don't show up on the scoreboard
open(SAVEOUT, ">&STDOUT");
open(SAVEERR, ">&STDERR");
open(STDOUT, '>' . File::Spec->devnull());
open(STDERR, ">&STDOUT");
$SV->Spawn ();
$server = $SV->WaitKill(5);
open(STDOUT, ">&SAVEOUT");
open(STDERR, ">&SAVEERR");

## It should have thrown an exception, so if the server exits
## with a zero status, then something is wrong.
if ($server == 0) {
  print STDERR "ERROR: server returned $server\n";
  exit(1);
}


exit(0);
