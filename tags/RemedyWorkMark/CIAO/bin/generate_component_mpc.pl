eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# Create a MPC file content for a single component implementation.

use Getopt::Std;

##############################################################################
# Grab the options

$flags = join (" ", @ARGV);

if (!getopts ('np:l:c:u:b:ho:') || $opt_h) {
    print "generate_component_mpc.pl [-h] component_name\n";
    print "\n";
    print "    -h         print help message explaining various options\n";
    print "    -p         Dependent component name\n";
    print "    -l         Dependent component path (libpaths)\n";
    print "    -o         Component output path (libout)\n";
    print "    -n         Suppress component make/project\n";
    print "    -c         Create a client makefile\n";
    print "    -u         Unique project name prefix (if not defined, name for -p flag will be used). \n";
    print "    -b         common base project(s) for all generated projects\n";
    print "\n";
    print "generate_component_mpc creates and save a minimum mpc file\n";
    print "called $com_name.mpc that is needed for a single component implementation\n";
    exit (1);
}

if ($#ARGV < 0) {
    print STDERR "No component_name specified, use -h for help\n";
    exit (1);
}

$com_name = shift @ARGV;
$UCOM_NAME = uc $com_name;

##############################################################################
# Prologue

if (defined $opt_b) {
    $base_projs = ", $opt_b ";
} else {
    $base_projs = " ";
}

if (defined $opt_n) {
    $svr_suffix = "_skel";
}
else {
    $svr_suffix = "_svnt";
}

$USVR_SUFFIX = uc $svr_suffix;

if (defined $opt_p) {
    $stub_depend = "$opt_p".'_stub';
    $lib_depend = "$opt_p".'_skel '."$opt_p".'_stub';

    $svr_plibs ='\
                '."$opt_p".'_skel \
                '."$opt_p".'_stub';
}
else {
    $svr_plibs = "";
}

$unique_prefix = "";

if (defined $opt_u) {
    $unique_prefix = "$opt_u" . "_";
}
elsif (defined $opt_p) {
    $unique_prefix = "$opt_p" . "_";
}

if (defined $opt_p) {
    $svr_p_after = "$opt_p".'_skel';
}

if (defined $opt_l) {
    $lib_paths = "libpaths += $opt_l";
}

if (defined $opt_o) {
    $lib_out = "libout = $opt_o";
}

if (defined $opt_c) {
    $client_def =
'project ('."$unique_prefix"."$opt_c".') : ccm_stub, valuetype ' . "$base_projs" . ' {
  exename = '."$opt_c".'
  after += '."$unique_prefix"."$com_name".'_stub
  libs  += '."$com_name".'_stub '."$stub_depend"."
  $lib_paths"."
  $lib_out".'
  IDL_Files {
  }

  Source_Files {
    '."$opt_c".'.cpp
  }

  Header_Files {
  }

  Inline_Files {
  }
}
';
}

if (! defined $opt_n) {
    $lem_gen =
'
project('."$unique_prefix"."$com_name".'_lem_gen) : ciaoidldefaults ' . "$base_projs" . ' {
  after += '."$unique_prefix"."$com_name".'_idl_gen
  custom_only = 1
  idlflags += -Wb,stub_export_macro='."$UCOM_NAME".'_LEM_STUB_Export \
              -Wb,stub_export_include='."$com_name".'_lem_stub_export.h \
              -SS -Gxhst

  IDL_Files {'."
    $com_name".'E.idl
  }
}
'.'
project('."$unique_prefix"."$com_name".'_lem_stub) : ccm_svnt ' . "$base_projs" . ' {
  after += '."$unique_prefix"."$com_name".'_lem_gen '."$unique_prefix"."$com_name".'_stub '."$stub_depend".'
  libs  += '."$stub_depend".' '."$com_name".'_stub'."
  $lib_paths"."
  $lib_out".'
  sharedname = '."$com_name".'_lem_stub
  dynamicflags += '."$UCOM_NAME".'_LEM_STUB_BUILD_DLL

  IDL_Files {
  }

  Source_Files {
    '."$com_name".'EC.cpp
  }

  Header_Files {
    '."$com_name".'EC.h
    '."$com_name".'_lem_stub_export.h
  }

  Inline_Files {
    '."$com_name".'EC.inl
  }
}
';

    $component_def =
'
project('."$unique_prefix"."$com_name".'_exec) : ciao_executor ' . "$base_projs" . ' {
  after   += '."$unique_prefix"."$com_name".'_lem_stub '."$unique_prefix"."$com_name".'_stub
  sharedname = '."$com_name".'_exec
  libs += '."$com_name".'_stub '."$com_name".'_lem_stub '."$stub_depend
  $lib_paths"."
  $lib_out".'
  dynamicflags += '."$UCOM_NAME".'_EXEC_BUILD_DLL

  IDL_Files {
  }

  Source_Files {'."
    $com_name".'_exec.cpp
  }

  Header_Files {'."
    $com_name".'_exec.h'."
    $com_name".'_exec_export.h
  }

  Inline_Files {
  }
}
';
}

$cli_idlflags =
  'idlflags += -Wb,stub_export_macro='."$UCOM_NAME".'_STUB_Export \
              -Wb,stub_export_include='."$com_name".'_stub_export.h \
              -Wb,skel_export_macro='."$UCOM_NAME"."$USVR_SUFFIX".'_Export \
              -Wb,skel_export_include='."$com_name"."$svr_suffix".'_export.h \
              -Wb,exec_export_macro='."$UCOM_NAME".'_EXEC_Export \
              -Wb,exec_export_include='."$com_name".'_exec_export.h \
              -Gxhex -Gxhsk -Gxhst'
;

$cli_base = "ccm_stub";
$svr_base = "ciao_servant";
$svr_after = "";

$svr_libs = "$com_name".'_stub '. "$com_name".'_lem_stub ';

if (defined $opt_n) {
    $svr_after = "$unique_prefix"."$com_name".'_stub';

    $svr_libs = "$com_name".'_stub
';

    $svr_src =
'
    '."$com_name".'S.cpp
';

    $svr_hdr =
'
    '."$com_name".'S.h
    '."$com_name".'_skel_export.h
';

    $svr_base = "ciao_executor";
}
else {

    $svr_idl = "$com_name".'E.idl';

    $svr_src =
'
    '."$com_name".'S.cpp
    '."$com_name".'_svnt.cpp
';

    $svr_hdr =
'
    '."$com_name".'S.h
    '."$com_name".'_svnt.h
    '."$com_name".'_svnt_export.h
';
}

$mpc_template = '// $Id$
// This file is generated with "'."generate_component_mpc.pl $flags".'"

project('."$unique_prefix"."$com_name".'_idl_gen) : componentidldefaults ' . "$base_projs" . ' {
  custom_only = 1
  '."$cli_idlflags".'

  IDL_Files {
    '."$com_name".'.idl
  }
}
'."$lem_gen".'
project('."$unique_prefix"."$com_name".'_stub) : '."$cli_base ". "$base_projs" . ' {
  after += '."$unique_prefix"."$com_name".'_idl_gen '."$stub_depend".'
  libs  += '."$stub_depend"."
  $lib_paths"."
  $lib_out".'
  sharedname = '."$com_name".'_stub
  dynamicflags += '."$UCOM_NAME".'_STUB_BUILD_DLL

  IDL_Files {
  }

  Source_Files {
    '."$com_name".'C.cpp
  }

  Header_Files {
    '."$com_name".'C.h
    '."$com_name".'_stub_export.h
  }

  Inline_Files {
    '."$com_name".'C.inl
  }
}
'."$component_def".'

project('."$unique_prefix"."$com_name"."$svr_suffix".') : '."$svr_base ". "$base_projs" . ' {
  after      += '."$svr_p_after "."$svr_after".' '."$unique_prefix"."$com_name".'_lem_stub'.'
  sharedname  = '."$com_name"."$svr_suffix".'
  libs       += '."$svr_libs $svr_plibs
  $lib_paths"."
  $lib_out".'
  dynamicflags += '."$UCOM_NAME"."$USVR_SUFFIX".'_BUILD_DLL
  '.'
  IDL_Files {
  }

  Source_Files {'."$svr_src".'  }

  Header_Files {'."$svr_hdr".'  }

  Inline_Files {
    '."$com_name".'S.inl
  }
}

'."$client_def
";

##############################################################################
# Static Stuff

##############################################################################
# Print the stuff out


# MPC files
open (MPCFILE, ">", "$com_name".".mpc");
print MPCFILE $mpc_template;
