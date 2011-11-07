eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use lib ".";
use PerlACE::TestTarget;

my $ifr_service = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $tao_ifr = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $iorbase = "ifr.ior";

my $ifr_service_iorfile = $ifr_service->LocalFile ($iorbase);
$ifr_service->DeleteFile ($iorbase);
my $tao_ifr_iorfile = $tao_ifr->LocalFile ($iorbase);
$tao_ifr->DeleteFile ($iorbase);

# Generate an array containing the path to the IDL files
sub idl_filenames {
    # Need to figure out the platform-independent way to
    # list all the files matching that pattern.
    my @f;
    my $dirpath = "$ENV{TAO_ROOT}/tests/IDL_Test";
    opendir(DIR, $dirpath);
    @idl_files = grep(/.*\.idl$/, readdir(DIR));
    close(DIR);
    return map { "$dirpath/$_" } @idl_files;
}

# Input: array of idl file names
# Output: array of idl file names that had a failure
sub phase1 {
    my $SVC = $ifr_service->CreateProcess ("../../../IFR_Service/tao_ifr_service",
                                           "-o $ifr_service_iorfile");

    my $CMP = $tao_ifr->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_ifr",
                                       "-ORBinitref InterfaceRepository=file://$tao_ifr_iorfile");

    my @failures = ();

    my $sep = "\n" . '-' x 70 . "\n\n";

    foreach $idlfile (@_) {
        print $sep;
        print "phase1: checking $idlfile\n";

        $server_status = $SVC->Spawn ();

        if ($server_status != 0) {
            print STDERR "ERROR: server returned $server_status\n";
            push(@failures, $idlfile);
            next;
        }

        if ($ifr_service->WaitForFileTimed ($iorbase,
                                            $ifr_service->ProcessStartWaitInterval()) == -1) {
            print STDERR "ERROR: cannot find file <$ifr_service_iorfile>\n";
            $SVC->Kill (); $SVC->TimedWait (1);
            push(@failures, $idlfile);
            next;
        }

        if ($ifr_service->GetFile ($iorbase) == -1) {
            print STDERR "ERROR: cannot retrieve file <$tao_ifr_iorfile>\n";
            $SVC->Kill (); $SVC->TimedWait (1);
            push(@failures, $idlfile);
            next;
        }

        if ($tao_ifr->PutFile ($iorbase) == -1) {
            print STDERR "ERROR: cannot set file <$tao_ifr_iorfile>\n";
            $SVC->Kill (); $SVC->TimedWait (1);
            push(@failures, $idlfile);
            next;
        }

        $CMP->Arguments ("-ORBInitRef InterfaceRepository=file://$tao_ifr_iorfile " .
                         "-I$ENV{TAO_ROOT}/tests/IDL_Test " .
                         "$idlfile");
        print STDOUT $CMP->Arguments () . "\n";
        $client_status = $CMP->SpawnWaitKill ($tao_ifr->ProcessStartWaitInterval());

        $server_status = $SVC->Wait (0);

        if ($server_status != -1) {
            push(@failures, $idlfile);
            warn "Service crashed during compilation of $idlfile.\n";
            next;
        }

        print "phase1: $add_r\n";
        if ($client_status != 0) {
            push(@failures, $idlfile);
            warn "Failure during compilation of $idlfile.\n";
            next;
        }

        $CMP->Arguments ("-ORBinitref InterfaceRepository=file://$tao_ifr_iorfile " .
                         "-I$ENV{TAO_ROOT}/tests/IDL_Test " .
                         "$idlfile -r");

        $client_status = $CMP->SpawnWaitKill ($tao_ifr->ProcessStartWaitInterval());

        $server_status = $SVC->Wait (0);

        if ($server_status != -1) {
            push(@failures, $idlfile);
            warn "Service crashed during removal of $idlfile.\n";
            next;
        }

        print "phase1: $add_r\n";
        if ($client_status != 0) {
            push(@failures, $idlfile);
            warn "Failure during removal of $idlfile.\n";
            next;
        }
    }
    continue {
        $SVC->Kill (); $SVC->TimedWait (1);
        $ifr_service->DeleteFile ($iorbase);
        $tao_ifr->DeleteFile ($iorbase);

    }
    return @failures;
}

sub phase2 {

}


@files = ($#ARGV > -1) ? @ARGV : idl_filenames;

@phase1_failures = phase1(@files);

print "END: the following failed phase1:\n", join("\n", @phase1_failures), "\n";
