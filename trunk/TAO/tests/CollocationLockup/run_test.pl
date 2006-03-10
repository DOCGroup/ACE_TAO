eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a Perl script that runs some tests.

use lib '../../../bin';
use PerlACE::Run_Test;
use Cwd;
use strict;

# Amount of delay (in seconds) between starting a server and a client
# to allow proper server initialization.
my $sleeptime = 10;

# Variables for command-line arguments to client and server
# executables.
my $iorfile = PerlACE::LocalFile ("SimpleNamingService.ior");

my $status = 0;

my $NS = new PerlACE::Process ("SimpleNamingService");

unlink $iorfile;

$NS->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, $sleeptime) == -1) {
  print STDERR "ERROR: cannot find IOR file <$iorfile>\n";
  $NS->Kill ();
  exit 1;
}

my $CL = new PerlACE::Process ("CollocationLockup",
                               "-ORBInitRef " .
                               "SimpleNamingService=file://$iorfile");

# In testing on various platforms, the builds with the bug failed before 
# 20 seconds and when the bug was fixed it returned before 20 seconds.
my $client = $CL->SpawnWaitKill (20);

if ($client != 0) {
  print STDERR "ERROR: client returned $client\n";
  $status = 1;
}

$NS->Kill ();

unlink $iorfile;

exit $status;
