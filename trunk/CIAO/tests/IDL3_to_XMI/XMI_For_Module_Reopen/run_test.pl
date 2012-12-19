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

$I2X = $target->CreateProcess ("$ENV{'ACE_ROOT'}/bin/tao_idl3_to_xmi",
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

$A_occurs = 0;
$A_indent = "";
$A_line = "";
$A_xmiid = "one";
$B_occurs = 0;
$B_indent = "";
$B_line = "";
$B_ns = "two";
@data = ();

# search for right tag in generated xmi
open (DAT, $xmi) || die ("ERROR: Could not open file <$xmi>!");
while ($line = <DAT>) {
    push @data, $line;
    if ($line =~ /(\s*)<UML:Package name=\"A\".*xmi\.id=\"([^\"]*)\"/) {
        ++$A_occurs;
        $A_indent = $1;
        $A_xmiid = $2 unless ($2 eq "");
        $A_line = $line;
    } elsif ($line =~ /(\s*)<UML:Package name=\"B\".*namespace=\"([^\"]*)\"/) {
        ++$B_occurs;
        $B_indent = $1;
        $B_ns = $2 unless ($2 eq "");
        $B_line = $line;
    }
}
close (DAT);

if ($A_occurs == 1 && $B_occurs == 1 && $A_xmiid eq $B_ns &&
        length ($A_indent) < length ($B_indent)) {
    print "TEST OK. Correct tags are there.\n$A_line$B_line";
} else {
    print STDERR "ERROR: generated XMI does is not correct!\n\nXMI is:\n\n";
    print "@data\n\n";
    $status = 1;
}

$target->DeleteFile($xmi);

exit $status;
