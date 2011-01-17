eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;

my $target = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

$I2X = $target->CreateProcess ("$ENV{'CIAO_ROOT'}/bin/tao_idl3_to_xmi");
$VAL = $target->CreateProcess ("xmlvalidator");


my $idl_tests_dir = "$ENV{'TAO_ROOT'}/tests/IDL_Test";
opendir(DIR, $idl_tests_dir) || die "can't opendir $idl_tests_dir: $!";

my @idls = grep { /\.idl$/ && -f "$idl_tests_dir/$_" } readdir(DIR);
foreach my $idl_file (@idls) {
    my $idl = "$idl_tests_dir/$idl_file";
    my $dtd = "../XMI.dtd";
    (my $xmi = $idl_file) =~ s/\.idl$/.xmi/;
    $target->LocalFile ($xmi);
    $target->DeleteFile ($xmi);

    $I2X->Arguments ("-as -I$idl_tests_dir -xd $dtd -of $xmi $idl");

    $target_status = $I2X->SpawnWaitKill ($target->ProcessStartWaitInterval ());

    if ($target_status != 0) {
        print STDERR "ERROR: tao_idl3_to_xmi returned $target_status\n";
        $status = 1;
    }

    if (! -f $xmi) {
        print STDERR "ERROR: tao_idl3_to_xmi didn't produce XMI\n";
        print "\n";
        next;
    }

    #$VAL->Arguments ("val -d $dtd $xmi");
    $VAL->Arguments ("-i $xmi");

    $target_status = $VAL->SpawnWaitKill ($target->ProcessStartWaitInterval ());

    if ($target_status != 0) {
        print STDERR "ERROR: xmlvalidator returned $target_status\n";
        $status = 1;
    }

    $target->DeleteFile ($xmi);

    print "\n";
}

closedir DIR;

exit $status;
