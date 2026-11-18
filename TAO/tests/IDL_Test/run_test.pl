eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

sub generated_class_body {
    my ($contents, $class_name) = @_;
    my ($definition) = $contents =~
      /(^class\s+\S+\s+\Q$class_name\E\b[\s\S]*?^\s*\};)/m;
    return $definition;
}

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
    elsif ($copy_base_decl !~ /DefaultValueRefCountBase/) {
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
        print STDERR "ERROR: OBV_Copy_Derived redundantly provides the valuetype reference counter\n";
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
    if (!defined $copy_base_body || $copy_base_body !~ /_copy_value\s*\(/) {
        print STDERR "ERROR: concrete OBV_Copy_Base does not declare _copy_value\n";
        $status = 1;
    }

    my ($copy_operation_derived_body) = $valuetype_header =~
      /(class TAO_IDL_TEST_STUB_Export OBV_Copy_Operation_Derived[\s\S]*?\n\s*};)/;
    if (!defined $copy_operation_derived_body
        || $copy_operation_derived_body !~ /_copy_value\s*\(\)\s*override\s*=\s*0/) {
        print STDERR "ERROR: operation-bearing derived OBV does not require its own copy\n";
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

    my ($abstract_supported_body) = generated_class_body (
      $valuetype_header, "Abstract_Supported_Refcount_Base");
    if (!defined $abstract_supported_body
        || $abstract_supported_body =~ /DefaultValueRefCountBase/)
      {
        print STDERR "ERROR: abstract supported valuetype unexpectedly owns a reference counter\n";
        $status = 1;
      }

    my ($concrete_supported_body) = generated_class_body (
      $valuetype_header, "OBV_Concrete_Supported_Refcount_Derived");
    if (!defined $concrete_supported_body
        || $concrete_supported_body !~ /DefaultValueRefCountBase/
        || $concrete_supported_body !~ /_add_ref\s*\(\)\s*override/)
      {
        print STDERR "ERROR: concrete supported descendant lacks its reference counter overrides\n";
        $status = 1;
      }

    my @checks = (
        ["valuetype_opt_accessorC.h", qr/Accessor_Optimized_Value_init/,
         "missing base _init factory class"],
        ["valuetype_opt_accessorC.h", qr/Accessor_Optimized_Derived_init/,
         "missing derived _init factory class"],
        ["valuetype_opt_accessorC.cpp", qr/Accessor_Optimized_Value_init::create_for_unmarshal\s*\(/,
         "missing base concrete _init factory implementation"],
        ["valuetype_opt_accessorC.cpp", qr/Accessor_Optimized_Derived_init::create_for_unmarshal\s*\(/,
         "missing derived concrete _init factory implementation"],
        ["valuetype_opt_accessorC.cpp", qr/Accessor_Optimized_Value::_copy_value\s*\(/,
         "missing optimized base _copy_value implementation"],
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

    my $optimized_header;
    open my $generated, "<", "valuetype_opt_accessorC.h"
      or die "ERROR: cannot open generated file <valuetype_opt_accessorC.h>: $!\n";
    {
        local $/;
        $optimized_header = <$generated>;
    }
    close $generated;

    foreach my $class_check (
        ["Accessor_Optimized_Value", qr/_copy_value\s*\(\)\s*override/,
         "missing optimized base _copy_value declaration"],
        ["Accessor_Optimized_Derived", qr/_copy_value\s*\(\)\s*override/,
         "missing optimized derived _copy_value declaration"],
        ["Accessor_Optimized_Refcount_Derived",
         qr/void\s+_add_ref\s*\(\)\s*override\s*;[\s\S]*?void\s+_remove_ref\s*\(\)\s*override\s*;/,
         "missing inherited supported-interface refcount overrides"],
        ["Accessor_Optimized_Refcount_Inherited",
         qr/void\s+_add_ref\s*\(\)\s*override\s*;[\s\S]*?void\s+_remove_ref\s*\(\)\s*override\s*;/,
         "missing supported-interface overrides with an inherited refcount"],
        ["Accessor_Optimized_Refcount_Multi_Derived",
         qr/void\s+_add_ref\s*\(\)\s*override\s*;[\s\S]*?void\s+_remove_ref\s*\(\)\s*override\s*;/,
         "missing refcount overrides when a derived valuetype adds a supported interface"],
    ) {
        my ($class_name, $pattern, $diagnostic) = @{$class_check};
        my $class_body = generated_class_body ($optimized_header, $class_name);
        if (!defined $class_body || $class_body !~ $pattern) {
            print STDERR "ERROR: $diagnostic in <$class_name>\n";
            $status = 1;
        }
    }

    my $optimized_parent_body = generated_class_body (
      $optimized_header, "Accessor_Optimized_Refcount_Parent");
    if (!defined $optimized_parent_body
        || $optimized_parent_body !~ /DefaultValueRefCountBase/)
      {
        print STDERR "ERROR: optimized refcount parent does not provide its counter\n";
        $status = 1;
      }

    my $optimized_inherited_body = generated_class_body (
      $optimized_header, "Accessor_Optimized_Refcount_Inherited");
    if (!defined $optimized_inherited_body
        || $optimized_inherited_body =~ /DefaultValueRefCountBase/)
      {
        print STDERR "ERROR: optimized derived valuetype duplicates its inherited counter\n";
        $status = 1;
      }

    my $optimized_multi_derived_body = generated_class_body (
      $optimized_header, "Accessor_Optimized_Refcount_Multi_Derived");
    if (!defined $optimized_multi_derived_body
        || $optimized_multi_derived_body =~ /DefaultValueRefCountBase/)
      {
        print STDERR "ERROR: optimized supported descendant duplicates its inherited counter\n";
        $status = 1;
      }

    my $optimized_source;
    open $generated, "<", "valuetype_opt_accessorC.cpp"
      or die "ERROR: cannot open generated file <valuetype_opt_accessorC.cpp>: $!\n";
    {
        local $/;
        $optimized_source = <$generated>;
    }
    close $generated;

    my ($optimized_derived_copy) = $optimized_source =~
      /Accessor_Optimized_Derived::_copy_value\s*\(\)\s*\{([\s\S]*?)^\}/m;
    if (!defined $optimized_derived_copy
        || $optimized_derived_copy !~ /Accessor_Optimized_Leaf::_downcast/) {
        print STDERR "ERROR: missing deep copy of an optimized direct valuetype member\n";
        $status = 1;
    }

    if ($optimized_source !~ /Accessor_Optimized_Refcount_Multi_Derived::_add_ref\s*\(/s
        || $optimized_source !~ /Accessor_Optimized_Refcount_Multi_Derived::_remove_ref\s*\(/s
        || $optimized_source !~ /Accessor_Optimized_Refcount_Multi_Derived::_copy_value\s*\(/s)
      {
        print STDERR "ERROR: optimized supported descendant lacks refcount or copy definitions\n";
        $status = 1;
      }
}

exit $status;
