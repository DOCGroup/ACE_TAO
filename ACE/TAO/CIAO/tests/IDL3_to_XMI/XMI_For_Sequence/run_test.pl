eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;

my $target = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

my $idl = "test.idl";
my $dtd = "../XMI.dtd";
my $xmi = "generated.xmi";
my $target_idl = $target->LocalFile ($idl);
my $target_dtd = $target->LocalFile ($dtd);
my $target_xmi = $target->LocalFile ($xmi);
$target->DeleteFile($xmi);

$I2X = $target->CreateProcess ("$ENV{'CIAO_ROOT'}/bin/tao_idl3_to_xmi",
                               "-f -xd $target_dtd -of $target_xmi $target_idl");

$target_status = $I2X->SpawnWaitKill ($target->ProcessStartWaitInterval());

if ($target_status != 0) {
    print STDERR "ERROR: tao_idl3_to_xmi returned $target_status\n";
    exit 1;
}

if ($target->GetFile ($xmi) == -1) {
    print STDERR "ERROR: cannot retrieve file <$xmi>\n";
    exit 1;
}

# search for right tag in generated xmi
open (DAT, $xmi) || die ("ERROR: Could not open file <$xmi>!");
@data=<DAT>;
close (DAT);

$num = grep (/<UML:Attribute/, @data);
if ($num == 8) {
    print "TEST OK. Correct tags in there.\n";
} else {
    print STDERR "ERROR: generated XMI does not contain a brace of UML:Attribute tags !\n\nXMI is:\n\n";
    print "@data\n\n";
    $status = 1;
}

$target->DeleteFile($xmi);

exit $status;
