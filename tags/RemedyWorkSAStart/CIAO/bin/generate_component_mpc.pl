eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
      if 0;

# $Id$
# Create a MPC file content for a single component implementation.

use Getopt::Std;

##############################################################################
# Grab the options

$flags = join (" ", @ARGV);

sub VERSION_MESSAGE {
    my ($output) = shift;

    print $output  "generate_component_mpc.pl [-h] component_name\n";
    print $output  "\n";
    print $output  "    -h         print $output  help message explaining various options\n";
    print $output  "    -p         Dependent component name\n";
    print $output  "    -l         Dependent component path (libpaths)\n";
    print $output  "    -o         Component output path (libout)\n";
    print $output  "    -I         Include path for all projects (IDL/cpp)\n";
    print $output  "    -n         Suppress component make/project\n";
    print $output  "    -c         Create a client makefile\n";
    print $output  "    -u         Unique project name prefix (if not defined, name for -p flag will be used). \n";
    print $output  "    -b         common base project(s) for all generated projects\n";
    print $output  "    -e         Use events\n";
    print $output  "\n";
    print $output  "generate_component_mpc creates and save a minimum mpc file\n";
    print $output  "called $com_name.mpc that is needed for a single component implementation\n";
}

%options=();
$Getopt::Std::STANDARD_HELP_VERSION = true;

if (!getopts ('henp:o:c:u:b:l:I:', \%options) || defined $options{h}) {
    VERSION_MESSAGE (STDERR);
    exit (1);
}

if (defined $options{help}) {
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

if (defined $options{l}) {
    $lib_paths = 'libpaths += ' . "$options{l}";
}

if (defined $options{o}) {
    $lib_out = 'libout = ' . "$options{o}";
    $exe_out = 'exeout = ' . "$options{o}";
}

if (defined $options{I}) {
    $include_paths_cpp = 'includes += ' . "$options{I}";
    $include_paths_idl = '-I ' . "$options{I}";
}

if (defined $options{b}) {
    if (defined $options{e}){
        $base_projs = ", avoids_ccm_noevent, $options{b} ";
    } else {
        $base_projs = ", $options{b} ";
    }
} else {
    if (defined $options{e}) {
        $base_projs = ", avoids_ccm_noevent ";
    } else {
        $base_projs = " ";
    }
}

if (defined $options{n}) {
    $svr_suffix = "_skel";
}
else {
    $svr_suffix = "_svnt";
}

$USVR_SUFFIX = uc $svr_suffix;

if (defined $options{p}) {
    $stub_depend = "$options{p}".'_stub ' . "$options{p}".'_lem_stub';

    $svr_plibs ='\
                '."$options{p}".'_skel \
                '."$options{p}".'_stub \
                '."$options{p}".'_lem_stub';
}
else {
    $svr_plibs = "";
}

$unique_prefix = "";

if (defined $options{u}) {
    $unique_prefix = "$options{u}" . "_";
}
elsif (defined $options{p}) {
    $unique_prefix = "$options{p}" . "_";
}

$svr_p_after = "";
if (defined $options{p}) {
  $svr_p_after = "$options{p}".'_skel ' . "$options{p}" . '_lem_stub';
}

if (defined $options{c}) {
    $client_def =
'project ('."$unique_prefix"."$options{c}".') : ccm_stub, valuetype' . "$base_projs" . ' {
  exename = '."$options{c}".'
  after += '."$unique_prefix"."$com_name".'_stub
  libs  += '."$com_name".'_stub '."$stub_depend"."
  $include_paths_cpp"."
  $lib_paths"."
  $exe_out".'
  IDL_Files {
  }

  Source_Files {
    '."$options{c}".'.cpp
  }

  Header_Files {
  }

  Inline_Files {
  }
}
';
}

    $lem_gen =
'
project('."$unique_prefix"."$com_name".'_lem_gen) : ciaoidldefaults' . "$base_projs" . '{
  after += '."$unique_prefix"."$com_name".'_idl_gen
  custom_only = 1
  idlflags += -Wb,stub_export_macro='."$UCOM_NAME".'_LEM_STUB_Export \
              -Wb,stub_export_include='."$com_name".'_lem_stub_export.h \
              -SS -Gxhst '."$include_paths_idl".'

  IDL_Files {'."
    $com_name".'E.idl
  }
}
'.'
project('."$unique_prefix"."$com_name".'_lem_stub) : ccm_svnt' . "$base_projs" . '{
  after += '."$unique_prefix"."$com_name".'_lem_gen '."$unique_prefix"."$com_name".'_stub '."$stub_depend".'
  libs  += '."$stub_depend".' '."$com_name".'_stub'."
  $include_paths_cpp"."
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

if (!defined $options{n}) {
    $component_def =
'
project('."$unique_prefix"."$com_name".'_exec) : ciao_executor' . "$base_projs" . '{
  after   += '. "$options{p}" . '_lem_stub ' . "$unique_prefix"."$com_name".'_lem_stub '."$unique_prefix"."$com_name".'_stub
  sharedname = '."$com_name".'_exec
  libs += '."$options{p}" . '_lem_stub ' . "$com_name".'_stub '."$com_name".'_lem_stub '."$stub_depend
  $include_paths_cpp"."
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
    $cli_idlflags =
'
  idlflags += -Wb,stub_export_macro='."$UCOM_NAME".'_STUB_Export \
              -Wb,stub_export_include='."$com_name".'_stub_export.h \
              -Wb,skel_export_macro='."$UCOM_NAME"."$USVR_SUFFIX".'_Export \
              -Wb,skel_export_include='."$com_name"."$svr_suffix".'_export.h \
              -Wb,exec_export_macro='."$UCOM_NAME".'_EXEC_Export \
              -Wb,exec_export_include='."$com_name".'_exec_export.h \
              -Gxhex -Gxhsk -Gxhst'
;
}
else {
  $cli_idlflags =
'
  idlflags += -Wb,stub_export_macro='."$UCOM_NAME".'_STUB_Export \
              -Wb,stub_export_include='."$com_name".'_stub_export.h \
              -Wb,skel_export_macro='."$UCOM_NAME"."$USVR_SUFFIX".'_Export \
              -Wb,skel_export_include='."$com_name"."$svr_suffix".'_export.h \
              -Gxhex -Gxhsk -Gxhst'
  ;
}

$cli_base = "ccm_stub";
$svr_base = "ciao_servant";
$svr_after = "";

$svr_libs = "$com_name".'_stub '. "$com_name".'_lem_stub ';

if (defined $options{n}) {
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

project('."$unique_prefix"."$com_name".'_idl_gen) : componentidldefaults' . "$base_projs" . '{
  custom_only = 1'."
  $cli_idlflags $include_paths_idl".'

  IDL_Files {
    '."$com_name".'.idl
  }
}
'."$lem_gen".'
project('."$unique_prefix"."$com_name".'_stub) : '."$cli_base". "$base_projs" . '{
  after += '."$unique_prefix"."$com_name".'_idl_gen '."$stub_depend".'
  libs  += '."$stub_depend"."
  $include_paths_cpp"."
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

project('."$unique_prefix"."$com_name"."$svr_suffix".') : '."$svr_base". "$base_projs" . ' {
  after      += '."$svr_p_after "."$svr_after".' '."$unique_prefix"."$com_name".'_lem_stub'.'
  sharedname  = '."$com_name"."$svr_suffix".'
  libs       += '."$svr_libs $svr_plibs
  $include_paths_cpp"."
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
