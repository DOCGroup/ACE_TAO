eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use File::Basename;

my $status = 0;

my $SV = new PerlACE::Process ("Interpreter");

my $server = $SV->SpawnWaitKill (10);

if ($server != 0) {
    print STDERR "ERROR: ", basename($SV->Executable()),
                 " returned $server \n";
    $status = 1;
}

exit $status;
