eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{'ACE_ROOT'}/bin";
use PerlACE::TestTarget;

$CIAO_ROOT = "$ENV{'CIAO_ROOT'}";
$TAO_ROOT = "$ENV{'TAO_ROOT'}";
$DANCE_ROOT = "$ENV{'DANCE_ROOT'}";

#  Processes
$APP = 0;

$status = 0;

$tg = PerlACE::TestTarget::create_target (1) || die "Create target for ns failed\n";

if ($#ARGV == -1) {
    opendir(DIR, ".");
    @files = grep(/\.cdp$/,readdir(DIR));
    closedir(DIR);
}
else {
    @files = @ARGV;
}

foreach $file (@files) {
    print "\n\n*********** Starting test for deployment $file ***********\n";

    # Invoke the test application -.
    print "Invoking test application\n";

open (OLDOUT, ">&STDOUT");
open (STDOUT, ">" . File::Spec->devnull());
open (OLDERR, ">&STDERR");
open (STDERR, ">&STDOUT");

    $APP = $tg->CreateProcess ("./Parsing", "$file");
    $retval = $APP->SpawnWaitKill ($tg->ProcessStartWaitInterval ());
    $status = $status + $retval;

open (STDOUT, ">&OLDOUT");
open (STDERR, ">&OLDERR");

  if ($retval != 0) {
    print STDERR "ERROR: Parsing returned $retval for file\n";
    $status = 1;
  }
 else  {
    print STDERR "Test succeeded\n";
  }

  sleep (1);
}

$APP = $tg->CreateProcess ("./Domain_Parsing", "ArtGallery3Nodes.cdd");
$retval = $APP->SpawnWaitKill ($tg->ProcessStartWaitInterval ());
$status = $status + $retval;

if ($retval != 0) {
    print STDERR "ERROR: Parting returned $retval for domain test.\n";
}

exit $status;
