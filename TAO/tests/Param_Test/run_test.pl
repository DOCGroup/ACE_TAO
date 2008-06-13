eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use PerlACE::TestTarget;

$server = PerlACE::TestTarget::create_target("server") || die "Create server target failed\n";
$client = PerlACE::TestTarget::create_target("client") || die "Create client target failed\n";

$iorbase = "server.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
$server->DeleteFile($server_iorfile);
$client->DeleteFile($client_iorfile);

$invocation = "sii";
$num = 5;
$debug = "";
$status = 0;

# Parse the arguments

@types = ("short", "ulonglong", "ubstring", "bdstring",
          "ubwstring", "bdwstring", "fixed_struct",
          "ub_strseq", "bd_strseq", "ub_wstrseq", "bd_wstrseq",
          "var_struct", "nested_struct", "recursive_struct",
          "ub_struct_seq", "bd_struct_seq",
          "ub_array_seq", "bd_array_seq", "multdim_array",
          "any", "objref", "objref_sequence", "objref_struct",
          "any_sequence",
          "ub_short_sequence", "ub_long_sequence",
          "bd_short_sequence", "bd_long_sequence",
          "fixed_array", "var_array", "typecode", "exception",
          "big_union", "small_union", "recursive_union", "complex_any");


for ($i = 0; $i <= $#ARGV; $i++) {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?") {
      print "Run_Test Perl script for TAO Param Test\n\n";
      print "run_test [-n num] [-d] [-onewin] [-h] [-t type] [-i (dii|sii)] [-chorus <target>]\n";
      print "\n";
      print "-n num              -- runs the client num times\n";
      print "-d                  -- runs each in debug mode\n";
      print "-h                  -- prints this information\n";
      print "-t type             -- runs only one type of param test\n";
      print "-i (dii|sii)        -- Changes the type of invocation\n";
      print "-chorus <target>    -- Run tests on chorus target\n";
      exit 0;
    }
    elsif ($ARGV[$i] eq "-n") {
      $num = $ARGV[$i + 1];
      $i++;
    }
    elsif ($ARGV[$i] eq "-d") {
      $debug = $debug." -d";
    }
    elsif ($ARGV[$i] eq "-t") {
      @types = split (',', $ARGV[$i + 1]);
      $i++;
    }
    elsif ($ARGV[$i] eq "-i") {
      $invocation = $ARGV[$i + 1];
      $i++;
    }
}

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server", "$debug -o server.ior");
}
else {
    $SV = $server->CreateProcess ("server", "$debug -o $server_iorfile");
}
$CL = $client->CreateProcess ("client");

foreach $type (@types) {
    $server->DeleteFile($server_iorfile); # Ignore errors
    $client->DeleteFile($client_iorfile);

    print STDERR "==== Testing $type === wait....\n";

    $server_status = $SV->Spawn ();

    if ($server_status != 0) {
        print STDERR "ERROR: server returned $server_status\n";
        $status = 1;
    }
    else
    {
        if ($server->WaitForFileTimed ($server_iorfile,
                                       $server->ProcessStartWaitInterval()) == -1) {
            print STDERR "ERROR: cannot find file <$server_iorfile>\n";
            $SV->Kill ();
            exit 1;
        }

        if ($server->GetFile ($server_iorfile, $iorbase) == -1) {
            print STDERR "ERROR: cannot retrieve file <$server_iorfile>\n";
            $SV->Kill (); $SV->TimedWait (1);
            exit 1;
        }
        if ($client->PutFile ($iorbase, $client_iorfile) == -1) {
            print STDERR "ERROR: cannot set file <$client_iorfile>\n";
            $SV->Kill (); $SV->TimedWait (1);
            exit 1;
        }

        $CL->Arguments ("$debug -f $client_iorfile  -i $invocation -t $type -n $num -x");
        $client_status = $CL->SpawnWaitKill (60);

        if ($client_status != 0) {
            print STDERR "ERROR: client returned $client_status\n";
            $status = 1;
        }

        $server_status = $SV->WaitKill (10);

        if ($server_status != 0) {
            print STDERR "ERROR: server returned $server_status\n";
            $status = 1;
        }

        $server->GetStderrLog();
    }

    $server->DeleteFile($server_iorfile);
    $client->DeleteFile($client_iorfile);

    print STDERR "==== Test for $type finished ===\n";
}

$server->DeleteFile($server_iorfile);

exit $status;
