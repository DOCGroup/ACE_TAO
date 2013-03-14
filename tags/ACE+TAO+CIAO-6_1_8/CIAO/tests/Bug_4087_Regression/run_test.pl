eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;

my $target = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

my $idl3 = "test.idl";
my $idl2 = "test_IDL2.idl";
my $target_idl3 = $target->LocalFile ($idl3);
my $target_idl2 = $target->LocalFile ($idl2);
$target->DeleteFile($idl2);

$I3toI2 = $target->CreateProcess ("$ENV{'ACE_ROOT'}/bin/tao_idl3_to_idl2",
                               "$target_idl3");

$target_status = $I3toI2->SpawnWaitKill ($target->ProcessStartWaitInterval());

if ($target_status != 0) {
    print STDERR "ERROR: tao_idl3_to_idl2 returned $target_status\n";
    exit 1;
}

if ($target->GetFile ($idl2) == -1) {
    print STDERR "ERROR: cannot retrieve file <$idl2>\n";
    exit 1;
}

# search for right tag in generated idl2
open (DAT, $idl2) || die ("ERROR: Could not open file <$idl2>!");
@data=<DAT>;
close (DAT);

$num = grep (/provide_voltage_/, @data);
if ($num == 2) {
    print "TEST OK. Correct operations in there.\n";
} else {
    print STDERR "ERROR: generated IDL2 does not contain a brace of provide_voltage_xxx operations !\n\nIDL2 is:\n\n";
    print "@data\n\n";
    $status = 1;
}

$target->DeleteFile($idl2);

exit $status;
