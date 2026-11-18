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
    if (!defined $copy_base_decl) {
        print STDERR "ERROR: cannot find generated OBV_Copy_Base declaration\n";
        $status = 1;
    }
    elsif ($copy_base_decl =~ /DefaultValueRefCountBase/) {
        print STDERR "ERROR: OBV_Copy_Base unexpectedly provides the valuetype reference counter\n";
        $status = 1;
    }
    my ($copy_derived_decl) = $valuetype_header =~
      /(class TAO_IDL_TEST_STUB_Export OBV_Copy_Derived[\s\S]*?\{)/;
    if (!defined $copy_derived_decl) {
        print STDERR "ERROR: cannot find generated OBV_Copy_Derived declaration\n";
        $status = 1;
    }
    elsif ($copy_derived_decl !~ /DefaultValueRefCountBase/) {
        print STDERR "ERROR: OBV_Copy_Derived does not provide the valuetype reference counter\n";
        $status = 1;
    }

    my ($copy_derived_body) = $valuetype_header =~
      /(class TAO_IDL_TEST_STUB_Export OBV_Copy_Derived[\s\S]*?\n\s*};)/;
    if (!defined $copy_derived_body || $copy_derived_body !~ /_copy_value\s*\(/) {
        print STDERR "ERROR: concrete OBV_Copy_Derived does not declare _copy_value\n";
        $status = 1;
    }

    my ($copy_base_body) = $valuetype_header =~
      /(class TAO_IDL_TEST_STUB_Export OBV_Copy_Base[\s\S]*?\n\s*};)/;
    if (!defined $copy_base_body || $copy_base_body =~ /_copy_value\s*\(/) {
        print STDERR "ERROR: application-factory OBV_Copy_Base unexpectedly declares _copy_value\n";
        $status = 1;
    }

    my ($application_factory_decl) = $valuetype_header =~
      /(class TAO_IDL_TEST_STUB_Export OBV_Application_Factory_Value[\s\S]*?\n\s*};)/;
    if (!defined $application_factory_decl) {
        print STDERR "ERROR: cannot find generated OBV_Application_Factory_Value declaration\n";
        $status = 1;
    }
    elsif ($application_factory_decl =~ /DefaultValueRefCountBase|_copy_value\s*\(/) {
        print STDERR "ERROR: application factory valuetype unexpectedly owns generated refcount or copy support\n";
        $status = 1;
    }

    my ($supported_value_decl) = $valuetype_header =~
      /(class TAO_IDL_TEST_STUB_Export OBV_Supported_Interface_Value[\s\S]*?\n\s*};)/;
    if (!defined $supported_value_decl
        || $supported_value_decl !~ /DefaultValueRefCountBase/
        || $supported_value_decl !~ /_add_ref\s*\(\)\s*override/)
      {
        print STDERR "ERROR: supported-interface valuetype does not resolve reference-count methods\n";
        $status = 1;
      }

    my @checks = (
        ["valuetype_opt_accessorC.h", qr/Accessor_Optimized_Value_init/,
         "missing _init factory class"],
        ["valuetype_opt_accessorC.cpp", qr/Accessor_Optimized_Value_init::create_for_unmarshal\s*\(/,
         "missing concrete _init factory implementation"],
    );

    foreach my $check (@checks) {
        my ($file, $unexpected_pattern, $diagnostic) = @{$check};
        open my $generated, "<", $file
          or die "ERROR: cannot open generated file <$file>: $!\n";
        local $/;
        my $contents = <$generated>;
        close $generated;

        if ($contents !~ $unexpected_pattern) {
            print STDERR "ERROR: $diagnostic in <$file>\n";
            $status = 1;
        }
    }
}

exit $status;
