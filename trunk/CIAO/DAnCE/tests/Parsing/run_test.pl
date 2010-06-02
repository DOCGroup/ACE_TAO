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

#Redirect the screen output to the null device.
open (OLDOUT, ">&STDOUT");
open (STDOUT, ">" . File::Spec->devnull());
open (OLDERR, ">&STDERR");
open (STDERR, ">&STDOUT");

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
    $APP = $tg->CreateProcess ("./Parsing", "$file");
    $status = $status +
              $APP->SpawnWaitKill ($tg->ProcessStartWaitInterval ());
    sleep (1);
}

exit $status;
