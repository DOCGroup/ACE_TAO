eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../../bin";
use PerlACE::Run_Test;

$status = 0;
$iorfile = PerlACE::LocalFile ("cubit.ior");
$svnsflags = " -f $iorfile ";
$clnsflags = " -f $iorfile ";
$clflags = "";
$svflags = "";

###############################################################################
# Parse the arguments

for (my $i = 0; $i <= $#ARGV; $i++) {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?") {
        print "run_test [-h] [-n num] [-debug] [-orblite]\n";
        print "\n";
        print "-h                  -- prints this information\n";
        print "-n num              -- client uses <num> iterations\n";
        print "-debug              -- sets the debug flag for both client and server\n";
        print "-orblite            -- Use the lite version of the orb";
        exit;
    }
    elsif ($ARGV[$i] eq "-debug") {
        $clflags .= " -d ";
        $svflags .= " -d ";
    }
    elsif ($ARGV[$i] eq "-n") {
        $clflags .= " -n $ARGV[$i + 1] ";
        $i++;
    }
    elsif ($ARGV[$i] eq "-orblite") {
        $clflags .= " -ORBgioplite ";
        $svflags .= " -ORBgioplite ";
    }
    else {
        print STDERR "ERROR: Unknown Option: ".$ARGV[$i]."\n";    
    }
}

###############################################################################

my $SV = new PerlACE::Process ("../IDL_Cubit/server", $svflags . $svnsflags);
my $CL = new PerlACE::Process ("client", "$clflags $clnsflags -x");

if (! (-x $SV->Executable () && -x $CL->Executable)) {
    print STDERR "ERROR: server and/or client missing or not executable!\n";
    exit 1;
}

# Make sure the file is gone, so we can wait on it.
unlink $iorfile;

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 10) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); 
    exit 1;
}

$client = $CL->SpawnWaitKill (60);
$server = $SV->WaitKill (10);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
