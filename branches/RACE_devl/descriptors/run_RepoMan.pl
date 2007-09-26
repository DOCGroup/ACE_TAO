eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# run_test.pl,v 1.5 2006/01/31 21:54:08 wotte Exp
# -*- perl -*-

use lib "$ENV{'ACE_ROOT'}/bin";
use PerlACE::Run_Test;

$ACE_ROOT = "$ENV{'ACE_ROOT'}";
$CIAO_ROOT = "$ENV{'CIAO_ROOT'}";

# Set up NamingService environment
$ENV{"NameServiceIOR"} = "corbaloc:iiop:localhost:40000/NameService";

# Invoke JAWS HTTP Server.
#print "Invoking JAWS HTTP Server\n";
#$JAWS = new PerlACE::Process ("$ACE_ROOT/apps/JAWS/server/main");
#$JAWS->Spawn ();


# Invoke RepoMan Daemon.
print "Invoking RepoMan Daemon\n";
 $RM = new PerlACE::Process ("$CIAO_ROOT/bin/RepositoryManagerDaemon", " -v RM -s localhost:5432");
 $RM->SpawnWaitKill (5000);

print "Shutting down rest of the processes.\n";

kill_open_processes ();

exit 0;
