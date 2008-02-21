eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use lib ".";
use PerlACE::Run_Test;
use IFR;

# Generate an array containing the path to the IDL files
sub idl_filenames {
    # Need to figure out the platform-independent way to
    # list all the files matching that pattern.
    my @f;
    my $dirpath = "$ENV{TAO_ROOT}/tests/IDL_Test"; 
    opendir(DIR, $dirpath);
    @f = grep(/.*\.idl$/, readdir(DIR));
    close(DIR);
    return map { "$dirpath/$_" } @f;
}


# Input: array of idl file names
# Output: array of idl file names that had a failure
sub phase1 {
    my $svc = new IFR::Service;
    my $compiler = new IFR::Compiler(options => "-I$ENV{TAO_ROOT}/tests/IDL_Test");
    my @failures = ();

    my $sep = "\n" . '-' x 70 . "\n\n";

    foreach $idlfile (@_) {
	print $sep;
	print "phase1: checking $idlfile\n";
	$svc->start();
	if (! $svc->is_running()) {
	    warn "Unable to start IFR Service: $!\n";
	    next;
	}

	my $add_r = $compiler->compile(files => ($idlfile));
	print "phase1: $add_r\n";
	if (! $svc->is_running()) {
	    push(@failures, $idlfile);
	    warn "Service crashed during compilation of $idlfile.\n";
	    next;
	}
        if ($add_r != 0) {
	    push(@failures, $idlfile);
	    warn "Failure during compilation of $idlfile.\n";
	    next;
	}

	my $rem_r = $compiler->compile(files => ($idlfile),
				       options => "-r" );
	if (! $svc->is_running() | $rem_r != 0) {
	    push(@failures, $idlfile);
	    warn "Failure during removal of $idlfile.\n";
	    next;
	}
    }
    continue {
	$svc->stop();
    }
    return @failures;
}

sub phase2 {

}


@files = ($#ARGV > -1) ? @ARGV : idl_filenames;

@phase1_failures = phase1(@files);

print "END: the following failed phase1:\n", join("\n", @phase1_failures), "\n";
