eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# Create a MPC file content for a single component implementation.

use Getopt::Std;

##############################################################################
# Grab the options

$flags = join (" ", @ARGV);

if (!getopts ('dcnf:h') || $opt_h) {
    print "generate_component_mpc.pl [-d] [-h] component_name\n";
    print "\n";
    print "    -d         Turn on debug mode\n";
    print "    -f         Currently not used\n";
    print "    -n         Supress component make/project\n";
    print "    -c         Create a client makefile\n";
    print "\n";
    print "generate_component_mpc creates the minimum mpc file that\n";
    print "is needed for a single component implementation\n";
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

if (defined $opt_c) {
    $client_def =
'
project (client) : ciao_client {
  exename = client
  depends += '."$com_name".'_stub

  IDL_Files {
  }

  Source_Files {
    client.cpp
  }
}
';
}

if (! defined $opt_n) {
    $component_def =
'
project('."$com_name".'_exec) : ciao_server {
  depends   += '."$com_name".'_svnt
  sharedname = '."$com_name".'_exec
  libs      += '."$com_name".'_stub '."$com_name".'_svnt
  dllflags   = '."$UCOM_NAME".'_EXEC_BUILD_DLL

  IDL_Files {
  }

  Source_Files {
    '."$com_name".'_exec.cpp
  }
}
';
}

$mpc_template = '
project('."$com_name".'_stub): ciao_client {
  sharedname = '."$com_name".'_stub
  idlflags += -I $(TAO_ROOT)/CIAO -I $(TAO_ROOT) -I $(TAO_ROOT)/orbsvcs/orbsvcs
  idlflags +=  -Wb,pre_include="ace/pre.h" -Wb,post_include="ace/post.h"
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
  depends += '."$com_name".'_stub
  sharedname  = '."$com_name".'_svnt
  libs    += '."$com_name".'_stub
  idlflags  +=  -I $(TAO_ROOT)/CIAO -I $(TAO_ROOT) -I $(TAO_ROOT)orbsvcs/orbsvcs -Wb,export_macro='."$UCOM_NAME".'_SVNT_Export -Wb,export_include='."$com_name".'_svnt_export.h -Wb,pre_include="ace/pre.h" -Wb,post_include="ace/post.h"
  dllflags = '."$UCOM_NAME".'_SVNT_BUILD_DLL

  IDL_Files {
    '."$com_name".'E.idl
  }

  Source_Files {
    '."$com_name".'_svnt.cpp
    '."$com_name".'S.cpp
    '."$com_name".'EC.cpp
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


print $mpc_template;
