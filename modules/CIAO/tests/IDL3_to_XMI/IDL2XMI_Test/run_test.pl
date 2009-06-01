eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;

$I2X = new PerlACE::Process ("$ENV{'CIAO_ROOT'}/bin/tao_idl3_to_xmi");
#$VAL = new PerlACE::Process ("/usr/bin/xmlstarlet");
$VAL = new PerlACE::Process ("xmlvalidator");

my $idl_tests_dir = "$ENV{'TAO_ROOT'}/tests/IDL_Test";
opendir(DIR, $idl_tests_dir) || die "can't opendir $idl_tests_dir: $!";

my @idls = grep { /\.idl$/ && -f "$idl_tests_dir/$_" } readdir(DIR);
foreach my $idl_file (@idls) {
    my $idl = "$idl_tests_dir/$idl_file";
    my $dtd = "../XMI.dtd";
    (my $xmi = $idl_file) =~ s/\.idl$/.xmi/;
    unlink $xmi;

    $I2X->Arguments ("-I$idl_tests_dir -xd $dtd -of $xmi $idl");

    $target_status = $I2X->SpawnWaitKill (30);

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

    $target_status = $VAL->SpawnWaitKill (30);

    if ($target_status != 0) {
        print STDERR "ERROR: xmlvalidator returned $target_status\n";
        $status = 1;
    }

    unlink $xmi;

    print "\n";
}

closedir DIR;

exit $status;
