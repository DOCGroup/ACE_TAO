eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $process = $server->CreateProcess ("main");
my $status = $process->SpawnWaitKill ($server->ProcessStartWaitInterval());
if ($status != 0) {
    print STDERR "ERROR: test returned $status\n";
}

if ($status == 0) {
    my @checks = (
        ["valuetype_opt_accessorC.h", qr/ValueBase\s*\*\s*_copy_value\s*\(/],
        ["valuetype_opt_accessorC.cpp", qr/Accessor_Optimized_Value::_copy_value\s*\(/],
    );

    foreach my $check (@checks) {
        my ($file, $unexpected_pattern) = @{$check};
        open my $generated, "<", $file
          or die "ERROR: cannot open generated file <$file>: $!\n";
        local $/;
        my $contents = <$generated>;
        close $generated;

        if ($contents =~ $unexpected_pattern) {
            print STDERR "ERROR: unexpected _copy_value in <$file>\n";
            $status = 1;
        }
    }
}

exit $status;
