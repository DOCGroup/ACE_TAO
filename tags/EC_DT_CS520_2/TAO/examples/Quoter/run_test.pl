eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a Perl script that runs the client and all the other servers that
# are needed.

use lib '../../../bin';
use PerlACE::Run_Test;

$nsiorfile = PerlACE::LocalFile ("ns.ior");

# number of threads to use for multithreaded clients or servers

$num_threads = 4;

# amount of delay between running the servers

$sleeptime = 8;

# other variables

$status = 0;
$n = 1;
$debug = "";
$cm = "";
$sm = "";

# Parse the arguments

for ($i = 0; $i <= $#ARGV; $i++) {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?") {
        print "run_test [-n num] [-d level] [-h] [-nt] [-cm] [-sm]\n";
        print "\n";
        print "-n num              -- runs the client num times\n";
        print "-d level            -- runs each at debug level <level>\n";
        print "-h                  -- prints this information\n";
        print "-nt num             -- number of threads in the client (twice for server)\n";
        print "                       make sure this is before any -cm or -sm\n";
        print "-cm                 -- use more than one thread in the client\n";
        print "-sm                 -- use more than one thread in the server\n";
        exit;
    }
    elsif ($ARGV[$i] eq "-n") {
      $n = $ARGV[$i + 1];
      $i++;
    }
    elsif ($ARGV[$i] eq "-nt") {
      $num_threads = $ARGV[$i + 1];
      $i++;
    }
    elsif ($ARGV[$i] eq "-d") {
      $debug = $debug." -d $ARGV[$i + 1]";
      $i++;
    }
    elsif ($ARGV[$i] eq "-cm") {
      $cm = "-n ".$num_threads;
    }
    elsif ($ARGV[$i] eq "-sm") {
      $sm = "-n ".($num_threads * 3);
    }
}

# Programs that are run

$NS = new PerlACE::Process ("../../orbsvcs/Naming_Service/Naming_Service",       "-o $nsiorfile");
$LC = new PerlACE::Process ("../../orbsvcs/LifeCycle_Service/LifeCycle_Service", "$debug -ORBInitRef NameService=file://$nsiorfile");

$SV = new PerlACE::Process ("server",          "$debug $sm -ORBInitRef NameService=file://$nsiorfile");
$FF = new PerlACE::Process ("Factory_Finder",  "$debug -ORBInitRef NameService=file://$nsiorfile");
$GF = new PerlACE::Process ("Generic_Factory", "-l $debug -ORBInitRef NameService=file://$nsiorfile");
$CL = new PerlACE::Process ("client",          "-l $debug $cm -ORBInitRef NameService=file://$nsiorfile");

$NS->Spawn ();
sleep $sleeptime;

$LC->Spawn ();
sleep $sleeptime;

$SV->Spawn ();
sleep $sleeptime;

$FF->Spawn ();
sleep $sleeptime;

$GF->Spawn ();
sleep $sleeptime;


for ($j = 1; $j <= $n; ++$j) {
    $client = $CL->SpawnWaitKill (60);

    if ($client != 0) {
        print STDERR "ERROR: client $j returned $client\n";
        $status = 1;
    }
}

$server = $GF->TerminateWaitKill (10);

if ($server != 0) {
    print STDERR "ERROR: Generic Factory returned $server\n";
    $status = 1;
}

$server = $FF->TerminateWaitKill (10);

if ($server != 0) {
    print STDERR "ERROR: Factory Finder returned $server\n";
    $status = 1;
}

$server = $SV->TerminateWaitKill (10);

if ($server != 0) {
    print STDERR "ERROR: Server returned $server\n";
    $status = 1;
}

$server = $LC->TerminateWaitKill (10);

if ($server != 0) {
    print STDERR "ERROR: LifeCycle returned $server\n";
    $status = 1;
}

$server = $NS->TerminateWaitKill (10);

if ($server != 0) {
    print STDERR "ERROR: Naming Service returned $server\n";
    $status = 1;
}

unlink $nsiorfile;

exit $status;
