eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# Create a MPC file content for a single component implementation.

use Getopt::Std;

##############################################################################
# Grab the options

$flags = join (" ", @ARGV);

if (!getopts ('np:l:c:u:h') || $opt_h) {
    print "generate_component_mpc.pl [-h] component_name\n";
    print "\n";
    print "    -h         print help message explaining various options\n";
    print "    -p         Dependent component name\n";
    print "    -l         Dependent component path\n";
    print "    -n         Suppress component make/project\n";
    print "    -c         Create a client makefile\n";
    print "    -u         Unique project name prefix (if not defined, name for -p flag will be used). \n";
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

if (defined $opt_c) {
    $client_def =
'project ('."$unique_prefix"."$opt_c".') : ccm_stub, valuetype {
  exename = '."$opt_c".'
  after += '."$unique_prefix"."$com_name".'_stub
  libs  += '."$com_name".'_stub '."$stub_depend"."
  $lib_paths".'
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
    $cidl_gen =
'
project('."$unique_prefix"."$com_name".'_cidl_gen) : ciaocidldefaults, taoidldefaults {
  avoids += ace_for_tao
  custom_only = 1
  cidlflags += --svnt-export-macro '."$UCOM_NAME".'_SVNT_Export \
               --svnt-export-include '."$com_name".'_svnt_export.h
  idlflags += -Wb,export_macro='."$UCOM_NAME".'_EXEC_Export \
              -Wb,export_include='."$com_name".'_exec_export.h \
              -SS
              
  CIDL_Files {'."
    $com_name".'.cidl
  }
  
  IDL_Files {'."
    $com_name".'E.idl
  }
}
';

    $component_def =
'
project('."$unique_prefix"."$com_name".'_exec) : ciao_executor {
  avoids += ace_for_tao
  after   += '."$unique_prefix"."$com_name".'_cidl_gen '."$unique_prefix"."$com_name".'_stub
  sharedname = '."$com_name".'_exec
  libs += '."$com_name".'_stub '."$stub_depend
  $lib_paths".'
  dynamicflags   = '."$UCOM_NAME".'_EXEC_BUILD_DLL

  IDL_Files {
  }

  Source_Files {'."
    $com_name".'EC.cpp'."
    $com_name".'_exec.cpp
  }
  
  Header_Files {'."
    $com_name".'EC.h'."
    $com_name".'_exec.h'."
    $com_name".'_exec_export.h
  }
  
  Inline_Files {'."
    $com_name".'EC.inl
  }
}
';
}

$cli_idlflags =
  'idlflags += -Wb,stub_export_macro='."$UCOM_NAME".'_STUB_Export \
              -Wb,stub_export_include='."$com_name".'_stub_export.h \
              -Wb,skel_export_macro='."$UCOM_NAME"."$USVR_SUFFIX".'_Export \
              -Wb,skel_export_include='."$com_name"."$svr_suffix".'_export.h'
;

$cli_base = "ccm_stub";
$svr_base = "ciao_servant";
$svr_after = "$unique_prefix"."$com_name".'_exec';

$svr_libs = "$com_name".'_exec \
                '."$com_name".'_stub';

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
    $cidl_block = "";
}
else {
    $cidl_block =
'
  CIDL_Files {
  }
';

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

project('."$unique_prefix"."$com_name".'_idl_gen) : taoidldefaults, anytypecode {
  avoids += ace_for_tao
  custom_only = 1
  '."$cli_idlflags".'

  IDL_Files {
    '."$com_name".'.idl
  }
}
'."$cidl_gen".'
project('."$unique_prefix"."$com_name".'_stub) : '."$cli_base".' {
  avoids += ace_for_tao
  after += '."$unique_prefix"."$com_name".'_idl_gen '."$stub_depend".'
  libs  += '."$stub_depend"."
  $lib_paths".'
  sharedname = '."$com_name".'_stub
  dynamicflags   = '."$UCOM_NAME".'_STUB_BUILD_DLL

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

project('."$unique_prefix"."$com_name"."$svr_suffix".') : '."$svr_base".' {
  avoids += ace_for_tao
  after      += '."$svr_p_after "."$svr_after".'
  sharedname  = '."$com_name"."$svr_suffix".'
  libs       += '."$svr_libs $svr_plibs
  $lib_paths".'
  dynamicflags = '."$UCOM_NAME"."$USVR_SUFFIX".'_BUILD_DLL
  '."$cidl_block".'
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

$ACE_ROOT= "$ENV{'ACE_ROOT'}";

print "The following commands have been executed:\n\n";

$command = "generate_export_file.pl $UCOM_NAME".'_STUB > '."$com_name".'_stub_export.h';
print "\t$command"."\n";
system ("$ACE_ROOT".'/bin/'."$command");

$command = "generate_export_file.pl $UCOM_NAME"."$USVR_SUFFIX".' > '."$com_name"."$svr_suffix".'_export.h';
print "\t$command"."\n";
system ("$ACE_ROOT".'/bin/'."$command");

if (! defined $opt_n) {
    $command = "generate_export_file.pl $UCOM_NAME".'_EXEC > '."$com_name".'_exec_export.h';
    print "\t$command"."\n";
    system ("$ACE_ROOT".'/bin/'."$command");
}
