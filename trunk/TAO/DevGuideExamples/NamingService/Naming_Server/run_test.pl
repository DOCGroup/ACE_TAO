# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use Env (ACE_ROOT);
use lib "$ACE_ROOT/bin";
use PerlACE::Run_Test;

$CS = new PerlACE::Process("NamingMessenger");  

if ($CS->SpawnWaitKill(10) != 0) {
   exit 1;
}

exit 0;



