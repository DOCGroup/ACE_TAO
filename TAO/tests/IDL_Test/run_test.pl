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
    my $valuetype_header;
    open my $generated, "<", "valuetypeC.h"
      or die "ERROR: cannot open generated file <valuetypeC.h>: $!\n";
    {
        local $/;
        $valuetype_header = <$generated>;
    }
    close $generated;

    my ($copy_base_decl) = $valuetype_header =~
      /(class TAO_IDL_TEST_STUB_Export OBV_Copy_Base[\s\S]*?\{)/;
    if (!defined $copy_base_decl || $copy_base_decl !~ /DefaultValueRefCountBase/) {
        print STDERR "ERROR: OBV_Copy_Base does not provide the valuetype reference counter\n";
        $status = 1;
    }
    my ($copy_derived_decl) = $valuetype_header =~
      /(class TAO_IDL_TEST_STUB_Export OBV_Copy_Derived[\s\S]*?\{)/;
    if (!defined $copy_derived_decl) {
        print STDERR "ERROR: cannot find generated OBV_Copy_Derived declaration\n";
        $status = 1;
    }
    elsif ($copy_derived_decl =~ /DefaultValueRefCountBase/) {
        print STDERR "ERROR: OBV_Copy_Derived redundantly provides a valuetype reference counter\n";
        $status = 1;
    }

    my @checks = (
        ["valuetype_opt_accessorC.h", qr/ValueBase\s*\*\s*_copy_value\s*\(/,
         "unexpected _copy_value"],
        ["valuetype_opt_accessorC.cpp", qr/Accessor_Optimized_Value::_copy_value\s*\(/,
         "unexpected _copy_value"],
        ["valuetype_opt_accessorC.h", qr/Accessor_Optimized_Value_init/,
         "unexpected _init factory class"],
    );

    foreach my $check (@checks) {
        my ($file, $unexpected_pattern, $diagnostic) = @{$check};
        open my $generated, "<", $file
          or die "ERROR: cannot open generated file <$file>: $!\n";
        local $/;
        my $contents = <$generated>;
        close $generated;

        if ($contents =~ $unexpected_pattern) {
            print STDERR "ERROR: $diagnostic in <$file>\n";
            $status = 1;
        }
    }
}

exit $status;
