eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# Create a MPC file content for a single component implementation.

use Getopt::Std;

##############################################################################
# Grab the options

$flags = join (" ", @ARGV);

if (!getopts ('dcnip:l:h') || $opt_h) {
    print "generate_component_mpc.pl [-d] [-h] component_name\n";
    print "\n";
    print "    -d         Turn on debug mode\n";
    print "    -p         Dependent component name\n";
    print "    -l         Dependent component path\n";
    print "    -i         Use an executor definition IDL file\n";
    print "    -n         Supress component make/project\n";
    print "    -c         Create a client makefile\n";
    print "\n";
    print "generate_component_mpc creates and save a minimum mpc file\n";
    print "called $com_name.mpc that is needed for a single component implementation\n";
    exit (1);
}

if (defined $opt_d) {
    print "Debugging Turned on\n";

    if (defined $opt_f) {
        print "Dependency to $opt_f\n";
    }
}


if ($#ARGV < 0) {
    print STDERR "No component_name specified, use -h for help\n";
    exit (1);
}

$com_name = shift @ARGV;
$UCOM_NAME = uc $com_name;

##############################################################################
# Prologue

if (defined $opt_p) {
    $stub_depend = "depends += $opt_p".'_stub';
    $svnt_depend = "$opt_p".'_svnt';
    $lib_depend = "$opt_p".'_stub '."$opt_p".'_svnt';
    $client_depend = "$com_name".'_stub '."$opt_p"."_stub";
}
else {
    $client_depend = "$com_name".'_stub';
}

if (defined $opt_l) {
    $lib_paths = "libpaths += $opt_l";
}

if (defined $opt_c) {
    $client_def =
'
project ('."$com_name".'_client) : ciao_client {
  exename = client
  depends += '."$client_depend
  $lib_paths".'

  IDL_Files {
  }

  Source_Files {
    client.cpp
  }
}
';
}

if (defined $opt_i) {
    $exec_impl_idl = "$com_name".'EI.idl';
    $exec_impl_cpp = "$com_name".'EIC.cpp';
}


if (! defined $opt_n) {
    $component_def =
'
project('."$com_name".'_exec) : ciao_server {
  depends   += '."$com_name".'_svnt
  sharedname = '."$com_name".'_exec
  libs      += '."$com_name".'_stub '."$com_name".'_svnt'." $lib_depend
  $lib_paths".'
  idlflags  +=  -Wb,export_macro='."$UCOM_NAME".'_EXEC_Export -Wb,export_include='."$com_name".'_exec_export.h
  dllflags   = '."$UCOM_NAME".'_EXEC_BUILD_DLL

  IDL_Files {'."
    $exec_impl_idl".'
  }

  Source_Files {'."
    $exec_impl_cpp
    $com_name".'_exec.cpp
  }
}
';
}

$mpc_template = '// $Id$
// This file is generated with "'."generate_component_mpc.pl $flags".'"

project('."$com_name".'_stub): ciao_client {'."
  $stub_depend".'
  sharedname = '."$com_name".'_stub
  idlflags += -Wb,stub_export_macro='."$UCOM_NAME".'_STUB_Export -Wb,stub_export_include='."$com_name".'_stub_export.h -Wb,skel_export_macro='."$UCOM_NAME".'_SVNT_Export -Wb,skel_export_include='."$com_name".'_svnt_export.h
  dllflags   = '."$UCOM_NAME".'_STUB_BUILD_DLL

  IDL_Files {
    '."$com_name".'.idl
  }

  Source_Files {
    '."$com_name".'C.cpp
  }
}

project('."$com_name".'_svnt) : ciao_server {
  depends += '."$svnt_depend $com_name".'_stub
  sharedname  = '."$com_name".'_svnt
  libs    += '."$com_name".'_stub'." $lib_depend
  $lib_paths".'
  idlflags  +=  -Wb,export_macro='."$UCOM_NAME".'_SVNT_Export -Wb,export_include='."$com_name".'_svnt_export.h
  dllflags = '."$UCOM_NAME".'_SVNT_BUILD_DLL

  IDL_Files {
    '."$com_name".'E.idl
  }

  Source_Files {
    '."$com_name".'EC.cpp
    '."$com_name".'S.cpp
    '."$com_name".'_svnt.cpp
  }
}
'."
$component_def

$client_def
";

##############################################################################
# Static Stuff

##############################################################################
# Print the stuff out


# MPC files
open (MPCFILE, ">", "$com_name".".mpc");
print MPCFILE $mpc_template;

print "Run the following command also:\n\n";
print "\tgenerate_export_file.pl $UCOM_NAME".'_STUB > '."$com_name".'_stub_export.h'."\n";
print "\tgenerate_export_file.pl $UCOM_NAME".'_SVNT > '."$com_name".'_svnt_export.h'."\n";
if (! defined $opt_n) {
    print "\tgenerate_export_file.pl $UCOM_NAME".'_EXEC > '."$com_name".'_exec_export.h'."\n";
}
