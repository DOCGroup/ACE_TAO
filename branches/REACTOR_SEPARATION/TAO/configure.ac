dnl $Id$

dnl An autoconf script to automatically configure TAO.
dnl Process this file with autoconf to produce a configure script.

dnl Statically (i.e. at autoconf-time) determine the version of TAO.
dnl This is necessary since the version argument to AC_INIT is
dnl supposed to be a static value, not a dynamic (e.g. a shell
dnl variable).
dnl
dnl Note that this macro removes the newline output by the M4
dnl "esyscmd" built-in.  Unless you understand what you're doing,
dnl particularly with M4, do not modify this macro definition.
define([TAO_VERSION], patsubst(esyscmd(grep TAO_VERSION tao/Version.h | sed 's/.*\"\(.*\)\".*/\1/'), [
]))dnl remove newline ending every `esyscmd' answer

AC_INIT([TAO],
        TAO_VERSION,
        [tao-bugs@cs.wustl.edu],
        [tao])

AC_REVISION([$Id$])

AC_COPYRIGHT([ACE(TM) and TAO(TM) are copyrighted by Douglas C. Schmidt and
his research group at Washington University, University of California,
Irvine, and Vanderbilt University Copyright (c) 1993-2004, all rights
reserved.  Since ACE and TAO are open-source, free software, you are
free to use, modify, and distribute the ACE and TAO source code and
object code produced from the source, as long as you include this
copyright statement along with code built using ACE and TAO.

Please see the file `COPYING' in the top level ACE directory for
additional details.])


dnl Require GNU Autoconf 2.58 or better.  Previous versions did not
dnl correctly support HP-UX.
AC_PREREQ([2.58])

AC_CONFIG_SRCDIR([tao/TAO_Internal.cpp])

AC_CONFIG_AUX_DIR([../aux_config])
AC_CONFIG_MACRO_DIR([../m4])

dnl Check what platform we are running on.
AC_CANONICAL_TARGET([])

dnl Initialize GNU Automake, and require Automake 1.9 or better.
AM_INIT_AUTOMAKE([1.9])


dnl Add maintainer mode option to the option list.
dnl AM_MAINTAINER_MODE

dnl The maintainer of this configure script.
ACE_CONFIGURE_MAINTAINER='tao-users@cs.wustl.edu'


dnl Until autoconf support in ACE is complete, prevent this script
dnl from running unless the user explictly forces the configure script
dnl to run using the "--enable-maintainer-mode" configure script
dnl option.
dnl if test $USE_MAINTAINER_MODE != yes; then
dnl   AC_MSG_ERROR([
dnl     ACE autoconf support is currently disabled by default since it is
dnl     still under development.  Please use the stock ACE build procedure
dnl     detailed in the file \`ACE-INSTALL.html'.
dnl
dnl     If you wish to experiment with ACE's autoconf support then use the
dnl     \"--enable-maintainer-mode\" configure script option to enable
dnl     autoconf support.  For more details see the file
dnl     \`ACE-configuration.txt'.])
dnl fi  dnl  test $USE_MAINTAINER_MODE != yes

dnl Should we use "egrep" or "grep -E"?  This sets the "$EGREP" shell
dnl variable.
AC_PROG_EGREP

dnl If we are configuring in a CVS controlled directory then don't
dnl continue any further.  The idea is to prevent automatically
dnl generated files from being checked into the repository.  This
dnl will prevent accidental overwrites of ACE's current Makefiles by
dnl the automatically generated ones, for example.
dnl ACE_CHECK_FOR_CVS_DIR

dnl Prevent the configure script from continuing any further if
dnl configuration is being performed in the top-level directory.  The
dnl idea is to prevent files generated during configuration and build
dnl from overwriting the stock files of the same name.
ACE_CHECK_TOP_SRCDIR

dnl Prepare the `tao/config.h.in' header template.
ACE_PREP_CONFIG_HEADER

dnl Allow the standard program name transformations.
dnl We probably don't need AC_ARG_PROGRAM any longer since AM_INIT_AUTOMAKE
dnl handles this functionality.  -- Ossama
dnl AC_ARG_PROGRAM

dnl Generate a header file with all settings.
AC_CONFIG_HEADERS([tao/config.h])

dnl Move before the AC_ARG_ENABLE stuff to prevent autoconf complaints.
dnl This is a bit messy but it makes life easier for me.
dnl                          -Ossama
dnl
dnl SECTION: checks for programs
dnl

dnl Check if system supports "#! /bin/sh" line in scripts
AC_SYS_INTERPRETER

dnl Check the C compiler and preprocessor.
dnl AC_PROG_CC
dnl AC_PROG_CPP
dnl AC_PROG_CC_C_O

dnl Check the C++ compiler and preprocessor.
AC_PROG_CXX
AC_PROG_CXXCPP

dnl Configure libtool
AC_DISABLE_STATIC
AC_PROG_LIBTOOL

ACE_CHECK_THREADS

dnl Set the test language as C++
AC_LANG([C++])

dnl If we are cross compiling disable certain things in the Makefiles.
AM_CONDITIONAL([ACE_CROSS_COMPILED], [test X$cross_compiling = Xyes])

dnl Look for the best awk-style program available.
AC_PROG_AWK

dnl Parse the version information argument.
dnl Note that "TAO_VERSION" is an m4 macro.
tao_version_temp=TAO_VERSION
tao_save_ifs="$IFS"; IFS='.'
set dummy $tao_version_temp 0 0 0
IFS="$tao_save_ifs"

TAO_MAJOR=$2
TAO_MINOR=$3
TAO_BETA=$4

AC_SUBST([TAO_MAJOR])
AC_SUBST([TAO_MINOR])
AC_SUBST([TAO_BETA])

dnl We should have --with/--enable flags for these options.
dnl Until we do, set them to reasonable defaults.
AM_CONDITIONAL(TEMPLATES_REQUIRE_SOURCE, false)
AM_CONDITIONAL(BUILD_RT_CORBA, true)
AM_CONDITIONAL(BUILD_CORBA_MESSAGING, true)
AM_CONDITIONAL(BUILD_MINIMUM_CORBA, false)
AM_CONDITIONAL(BUILD_INTERCEPTORS, true)
AM_CONDITIONAL(BUILD_ACE_UUID, true)
AM_CONDITIONAL(BUILD_AMI, true)
AM_CONDITIONAL(BUILD_SSL, false)

ACE_WITH_ACEXML


dnl
dnl SECTION 16: AC_CONFIG_FILES([FILE...])
dnl
dnl
dnl We can finally create all the files listed here; Makefile is
dnl created from Makefile.in, etc.  Top-level Makefiles should be
dnl created first.

AC_CONFIG_FILES([
  tao-config
  tao-config.1
  Makefile
  TAO_IDL/Makefile
  tao/Makefile
  orbsvcs/Concurrency_Service/Makefile
  orbsvcs/CosEvent_Service/Makefile
  orbsvcs/Dump_Schedule/Makefile
  orbsvcs/Event_Service/Makefile
  orbsvcs/FTRT_Event_Service/Event_Service/Makefile
  orbsvcs/FTRT_Event_Service/Factory_Service/Makefile
  orbsvcs/FTRT_Event_Service/Gateway_Service/Makefile
  orbsvcs/FTRT_Event_Service/Makefile
  orbsvcs/FT_ReplicationManager/Makefile
  orbsvcs/Fault_Detector/Makefile
  orbsvcs/Fault_Notifier/Makefile
  orbsvcs/ImplRepo_Service/Makefile
  orbsvcs/IFR_Service/Makefile
  orbsvcs/LifeCycle_Service/Makefile
  orbsvcs/LoadBalancer/Makefile
  orbsvcs/Logging_Service/Basic_Logging_Service/Makefile
  orbsvcs/Logging_Service/Event_Logging_Service/Makefile
  orbsvcs/Logging_Service/Makefile
  orbsvcs/Logging_Service/Notify_Logging_Service/Makefile
  orbsvcs/Logging_Service/RTEvent_Logging_Service/Makefile
  orbsvcs/Makefile
  orbsvcs/Naming_Service/Makefile
  orbsvcs/Notify_Service/Makefile
  orbsvcs/PSS/Makefile
  orbsvcs/Scheduling_Service/Makefile
  orbsvcs/TAO_Service/Makefile
  orbsvcs/Time_Service/Makefile
  orbsvcs/Trading_Service/Makefile
  orbsvcs/orbsvcs/Makefile
  orbsvcs/performance-tests/Makefile
  orbsvcs/performance-tests/LoadBalancing/Makefile
  orbsvcs/performance-tests/LoadBalancing/LBPerf/Makefile
  orbsvcs/performance-tests/LoadBalancing/LBPerf/RPS/Makefile
  orbsvcs/performance-tests/RTEvent/Makefile
  orbsvcs/performance-tests/RTEvent/Colocated_Roundtrip/Makefile
  orbsvcs/performance-tests/RTEvent/Federated_Roundtrip/Makefile
  orbsvcs/performance-tests/RTEvent/RTCORBA_Baseline/Makefile
  orbsvcs/performance-tests/RTEvent/RTCORBA_Callback/Makefile
  orbsvcs/performance-tests/RTEvent/Roundtrip/Makefile
  orbsvcs/performance-tests/RTEvent/TCP_Baseline/Makefile
  orbsvcs/performance-tests/RTEvent/lib/Makefile
  orbsvcs/tests/Makefile
  orbsvcs/tests/AVStreams/Makefile
  orbsvcs/tests/AVStreams/Asynch_Three_Stage/Makefile
  orbsvcs/tests/AVStreams/Bidirectional_Flows/Makefile
  orbsvcs/tests/AVStreams/Component_Switching/Makefile
  orbsvcs/tests/AVStreams/Full_Profile/Makefile
  orbsvcs/tests/AVStreams/Latency/Makefile
  orbsvcs/tests/AVStreams/Modify_QoS/Makefile
  orbsvcs/tests/AVStreams/Multicast/Makefile
  orbsvcs/tests/AVStreams/Multicast_Full_Profile/Makefile
  orbsvcs/tests/AVStreams/Multiple_Flows/Makefile
  orbsvcs/tests/AVStreams/Pluggable/Makefile
  orbsvcs/tests/AVStreams/Pluggable_Flow_Protocol/Makefile
  orbsvcs/tests/AVStreams/Simple_Three_Stage/Makefile
  orbsvcs/tests/AVStreams/Simple_Two_Stage/Makefile
  orbsvcs/tests/AVStreams/Simple_Two_Stage_With_QoS/Makefile
  orbsvcs/tests/Bug_1334_Regression/Makefile
  orbsvcs/tests/Bug_1393_Regression/Makefile
  orbsvcs/tests/Bug_1395_Regression/Makefile
  orbsvcs/tests/Bug_1630_Regression/Makefile
  orbsvcs/tests/Concurrency/Makefile
  orbsvcs/tests/CosEvent/Makefile
  orbsvcs/tests/CosEvent/Basic/Makefile
  orbsvcs/tests/CosEvent/lib/Makefile
  orbsvcs/tests/EC_Custom_Marshal/Makefile
  orbsvcs/tests/EC_MT_Mcast/Makefile
  orbsvcs/tests/EC_Mcast/Makefile
  orbsvcs/tests/EC_Multiple/Makefile
  orbsvcs/tests/EC_Throughput/Makefile
  orbsvcs/tests/Event/Makefile
  orbsvcs/tests/Event/Basic/Makefile
  orbsvcs/tests/Event/Mcast/Makefile
  orbsvcs/tests/Event/Mcast/Common/Makefile
  orbsvcs/tests/Event/Mcast/Complex/Makefile
  orbsvcs/tests/Event/Mcast/Simple/Makefile
  orbsvcs/tests/Event/Mcast/Two_Way/Makefile
  orbsvcs/tests/Event/Performance/Makefile
  orbsvcs/tests/Event/lib/Makefile
  orbsvcs/tests/FT_App/Makefile
  orbsvcs/tests/FaultTolerance/Makefile
  orbsvcs/tests/FaultTolerance/GroupRef_Manipulation/Makefile
  orbsvcs/tests/FaultTolerance/IOGR/Makefile
  orbsvcs/tests/FaultTolerance/IOGRManipulation/Makefile
  orbsvcs/tests/FtRtEvent/Makefile
  utils/Makefile
  utils/catior/Makefile
  utils/nslist/Makefile
 ])

dnl Note that the "TAO_VERSION" in the message below is an M4 macro
dnl that expands to the version of TAO being configured.
AC_CONFIG_COMMANDS([default],[
  echo ""
  echo "Configuration of TAO TAO_VERSION is now complete."
  echo ""
 ],[])
AC_OUTPUT
