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
define([TAO_VERSION], patsubst(esyscmd(grep TAO_VERSION tao/Version.h | sed 's/.*\" *\(.*\)\".*/\1/'), [
]))dnl remove newline ending every `esyscmd' answer

AC_INIT([TAO],
        TAO_VERSION,
        [tao-bugs@cs.wustl.edu],
        [tao])

AC_REVISION([$Id$])

AC_COPYRIGHT([ACE(TM), TAO(TM), CIAO(TM), and CoSMIC(TM) (henceforth
referred to as "DOC software") are copyrighted by Douglas C.
Schmidt and his research group at Washington University,
University of California, Irvine, and Vanderbilt University,
Copyright (c) 1993-2005, all rights reserved. Since DOC software is
open-source, free software, you are free to use, modify, copy, and
distribute--perpetually and irrevocably--the DOC software source code
and object code produced from the source, as well as copy and
distribute modified versions of this software. You must, however,
include this copyright statement along with code built using DOC
software.

Please see the file `COPYING' in the top level ACE directory for
additional details.])


dnl Require GNU Autoconf 2.58 or better.  Previous versions did not
dnl correctly support HP-UX.
AC_PREREQ([2.58])

AC_CONFIG_SRCDIR([tao/TAO_Internal.cpp])

AC_CONFIG_AUX_DIR([../aux_config])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_MACRO_DIR([../m4])

dnl Check what platform we are running on.
AC_CANONICAL_TARGET([])

dnl Initialize GNU Automake, and require Automake 1.9 or better.
AM_INIT_AUTOMAKE([1.9 foreign no-define])


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
TAO_ENABLE_MINIMUM_CORBA
AM_CONDITIONAL(TEMPLATES_REQUIRE_SOURCE, false)
AM_CONDITIONAL(BUILD_ACE_FOR_TAO, false)
AM_CONDITIONAL(BUILD_RT_CORBA, true)
AM_CONDITIONAL(BUILD_CORBA_MESSAGING, true)
AM_CONDITIONAL(BUILD_INTERCEPTORS, true)
AM_CONDITIONAL(BUILD_ACE_UUID, true)
AM_CONDITIONAL(BUILD_AMI, true)
AM_CONDITIONAL(BUILD_REPO, false)
AM_CONDITIONAL(BUILD_THREADS, true)
AM_CONDITIONAL(BUILD_EC_TYPED_EVENTS, true)
AM_CONDITIONAL(BUILD_WFMO, false)
AM_CONDITIONAL(BUILD_WINCE, false)
AM_CONDITIONAL(BUILD_WINNT, false)
AM_CONDITIONAL(BUILD_WXWINDOWS, false)
AM_CONDITIONAL(BUILD_STL, false)
AM_CONDITIONAL(BUILD_BOOST, false)
AM_CONDITIONAL(BUILD_JAVA, false)
AM_CONDITIONAL(BUILD_TRANSPORT_CURRENT,true)

ACE_ENABLE_ACEXML
ACE_ENABLE_SSL
ACE_ENABLE_FL_REACTOR
ACE_ENABLE_QT_REACTOR
ACE_ENABLE_TK_REACTOR
ACE_ENABLE_XT_REACTOR
ACE_ENABLE_FOX_REACTOR
ACE_PATH_ZZIP
ACE_PATH_ZLIB

TAO_ENABLE_EXAMPLES
TAO_ENABLE_TESTS

dnl The user's/default C++ flags are stored in "CXXFLAGS."  We use
dnl the variable "ACE_CXXFLAGS" to set the C++ flags we want.  At the end
dnl of the configuration process we combine ACE_CXXFLAGS and CXXFLAGS
dnl into CXXFLAGS (e.g., CXXFLAGS="$ACE_CXXFLAGS $CXXFLAGS").  CXXFLAGS
dnl goes after ACE_CXXFLAGS so that the user's C++ flag command line
dnl choices always override the configure script's choices.
ACE_CXXFLAGS=""
ACE_CFLAGS=""

ACE_SET_COMPILER_FLAGS

dnl Disable building of static libraries by default
AC_DISABLE_STATIC

dnl Enable Libtool module support
AC_LIBTOOL_DLOPEN

dnl Enable support for "clean" DLLs.
AC_LIBTOOL_WIN32_DLL

dnl Check for libtool and turn on Automake processing for Libtool
AC_PROG_LIBTOOL

CXXFLAGS="$ACE_CXXFLAGS $X_CFLAGS $CXXFLAGS"
CFLAGS="$ACE_CFLAGS $X_CFLAGS $CFLAGS"
CPPFLAGS="$ACE_CPPFLAGS $CPPFLAGS"
LDFLAGS="$ACE_LDFLAGS $LDFLAGS"

dnl
dnl SECTION 16: AC_CONFIG_FILES([FILE...])
dnl
dnl
dnl We can finally create all the files listed here; Makefile is
dnl created from Makefile.in, etc.  Top-level Makefiles should be
dnl created first.

AC_CONFIG_FILES([
  Makefile
  TAO_IDL/Makefile
  tao/Makefile
  tao/ETCL/Makefile
  orbsvcs/Makefile
  orbsvcs/orbsvcs/Makefile
  orbsvcs/Concurrency_Service/Makefile
  orbsvcs/CosEvent_Service/Makefile
  orbsvcs/Dump_Schedule/Makefile
  orbsvcs/Event_Service/Makefile
  orbsvcs/FTRT_Event_Service/Makefile
  orbsvcs/FTRT_Event_Service/Event_Service/Makefile
  orbsvcs/FTRT_Event_Service/Factory_Service/Makefile
  orbsvcs/FTRT_Event_Service/Gateway_Service/Makefile
  orbsvcs/FT_ReplicationManager/Makefile
  orbsvcs/Fault_Detector/Makefile
  orbsvcs/Fault_Notifier/Makefile
  orbsvcs/IFR_Service/Makefile
  orbsvcs/ImplRepo_Service/Makefile
  orbsvcs/LifeCycle_Service/Makefile
  orbsvcs/LoadBalancer/Makefile
  orbsvcs/Logging_Service/Makefile
  orbsvcs/Logging_Service/Basic_Logging_Service/Makefile
  orbsvcs/Logging_Service/Event_Logging_Service/Makefile
  orbsvcs/Logging_Service/Notify_Logging_Service/Makefile
  orbsvcs/Logging_Service/RTEvent_Logging_Service/Makefile
  orbsvcs/Naming_Service/Makefile
  orbsvcs/Notify_Service/Makefile
  orbsvcs/Scheduling_Service/Makefile
  orbsvcs/TAO_Service/Makefile
  orbsvcs/Time_Service/Makefile
  orbsvcs/Trading_Service/Makefile
  orbsvcs/examples/Makefile
  orbsvcs/examples/CosEC/Makefile
  orbsvcs/examples/CosEC/Factory/Makefile
  orbsvcs/examples/CosEC/RtEC_Based/Makefile
  orbsvcs/examples/CosEC/RtEC_Based/lib/Makefile
  orbsvcs/examples/CosEC/RtEC_Based/bin/Makefile
  orbsvcs/examples/CosEC/RtEC_Based/tests/Makefile
  orbsvcs/examples/CosEC/RtEC_Based/tests/Basic/Makefile
  orbsvcs/examples/CosEC/RtEC_Based/tests/Multiple/Makefile
  orbsvcs/examples/CosEC/Simple/Makefile
  orbsvcs/examples/CosEC/TypedSimple/Makefile
  orbsvcs/examples/FaultTolerance/Makefile
  orbsvcs/examples/FaultTolerance/RolyPoly/Makefile
  orbsvcs/examples/ImR/Makefile
  orbsvcs/examples/ImR/Advanced/Makefile
  orbsvcs/examples/ImR/Combined_Service/Makefile
  orbsvcs/examples/LoadBalancing/Makefile
  orbsvcs/examples/Log/Makefile
  orbsvcs/examples/Log/Basic/Makefile
  orbsvcs/examples/Log/Event/Makefile
  orbsvcs/examples/Log/Notify/Makefile
  orbsvcs/examples/Log/RTEvent/Makefile
  orbsvcs/examples/Notify/Makefile
  orbsvcs/examples/Notify/Federation/Makefile
  orbsvcs/examples/Notify/Federation/Gate/Makefile
  orbsvcs/examples/Notify/Federation/Agent/Makefile
  orbsvcs/examples/Notify/Federation/SpaceCraft/Makefile
  orbsvcs/examples/Notify/Filter/Makefile
  orbsvcs/tests/Makefile
  orbsvcs/tests/Notify/Makefile
  orbsvcs/tests/Notify/lib/Makefile
  orbsvcs/examples/Notify/Lanes/Makefile
  orbsvcs/examples/Notify/MC/Makefile
  orbsvcs/examples/Notify/MC/monitor/Makefile
  orbsvcs/examples/Notify/Subscribe/Makefile
  orbsvcs/examples/Notify/ThreadPool/Makefile
  orbsvcs/examples/ORT/Makefile
  orbsvcs/examples/RtEC/Makefile
  orbsvcs/examples/RtEC/IIOPGateway/Makefile
  orbsvcs/examples/RtEC/Kokyu/Makefile
  orbsvcs/examples/RtEC/MCast/Makefile
  orbsvcs/examples/RtEC/Schedule/Makefile
  orbsvcs/examples/RtEC/Simple/Makefile
  orbsvcs/examples/Security/Makefile
  orbsvcs/examples/Security/Send_File/Makefile
  orbsvcs/performance-tests/Makefile
  orbsvcs/performance-tests/LoadBalancing/Makefile
  orbsvcs/performance-tests/LoadBalancing/LBPerf/Makefile
  orbsvcs/performance-tests/LoadBalancing/LBPerf/RPS/Makefile
  orbsvcs/performance-tests/RTEvent/Makefile
  orbsvcs/performance-tests/RTEvent/lib/Makefile
  orbsvcs/performance-tests/RTEvent/Colocated_Roundtrip/Makefile
  orbsvcs/performance-tests/RTEvent/Federated_Roundtrip/Makefile
  orbsvcs/performance-tests/RTEvent/RTCORBA_Baseline/Makefile
  orbsvcs/performance-tests/RTEvent/RTCORBA_Callback/Makefile
  orbsvcs/performance-tests/RTEvent/Roundtrip/Makefile
  orbsvcs/performance-tests/RTEvent/TCP_Baseline/Makefile
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
  orbsvcs/tests/BiDir_CORBALOC/Makefile
  orbsvcs/tests/Bug_1334_Regression/Makefile
  orbsvcs/tests/Bug_1393_Regression/Makefile
  orbsvcs/tests/Bug_1395_Regression/Makefile
  orbsvcs/tests/Bug_1630_Regression/Makefile
  orbsvcs/tests/Bug_2074_Regression/Makefile
  orbsvcs/tests/Bug_2137_Regression/Makefile
  orbsvcs/tests/Bug_2247_Regression/Makefile
  orbsvcs/tests/Bug_2248_Regression/Makefile
  orbsvcs/tests/Bug_2285_Regression/Makefile
  orbsvcs/tests/Bug_2287_Regression/Makefile
  orbsvcs/tests/Bug_2316_Regression/Makefile
  orbsvcs/tests/Bug_2377_Regression/Makefile
  orbsvcs/tests/Bug_2615_Regression/Makefile
  orbsvcs/tests/Bug_2709_Regression/Makefile
  orbsvcs/tests/Bug_2800_Regression/Makefile
  orbsvcs/tests/Bug_2925_Regression/Makefile
  orbsvcs/tests/Bug_2926_Regression/Makefile
  orbsvcs/tests/Bug_3215_Regression/Makefile
  orbsvcs/tests/Bug_3216_Regression/Makefile
  orbsvcs/tests/COIOP_Naming_Test/Makefile
  orbsvcs/tests/Concurrency/Makefile
  orbsvcs/tests/CosEvent/Makefile
  orbsvcs/tests/CosEvent/lib/Makefile
  orbsvcs/tests/CosEvent/Basic/Makefile
  orbsvcs/tests/CosEvent/Timeout/Makefile
  orbsvcs/tests/EC_Custom_Marshal/Makefile
  orbsvcs/tests/EC_MT_Mcast/Makefile
  orbsvcs/tests/EC_Mcast/Makefile
  orbsvcs/tests/EC_Multiple/Makefile
  orbsvcs/tests/EC_Throughput/Makefile
  orbsvcs/tests/Event/Makefile
  orbsvcs/tests/Event/lib/Makefile
  orbsvcs/tests/Event/Basic/Makefile
  orbsvcs/tests/Event/Mcast/Makefile
  orbsvcs/tests/Event/Mcast/AddrServer/Makefile
  orbsvcs/tests/Event/Mcast/Common/Makefile
  orbsvcs/tests/Event/Mcast/Complex/Makefile
  orbsvcs/tests/Event/Mcast/RTEC_MCast_Federated/Makefile
  orbsvcs/tests/Event/Mcast/Simple/Makefile
  orbsvcs/tests/Event/Mcast/Two_Way/Makefile
  orbsvcs/tests/Event/Performance/Makefile
  orbsvcs/tests/Event/ThreadFlags/Makefile
  orbsvcs/tests/Event/UDP/Makefile
  orbsvcs/tests/FT_App/Makefile
  orbsvcs/tests/FaultTolerance/Makefile
  orbsvcs/tests/FaultTolerance/GroupRef_Manipulation/Makefile
  orbsvcs/tests/FaultTolerance/IOGR/Makefile
  orbsvcs/tests/FaultTolerance/IOGRManipulation/Makefile
  orbsvcs/tests/FtRtEvent/Makefile
  orbsvcs/tests/HTIOP/Makefile
  orbsvcs/tests/HTIOP/AMI/Makefile
  orbsvcs/tests/HTIOP/BiDirectional/Makefile
  orbsvcs/tests/HTIOP/Hello/Makefile
  orbsvcs/tests/IOR_MCast/Makefile
  orbsvcs/tests/ImplRepo/Makefile
  orbsvcs/tests/ImplRepo/Bug_2604_Regression/Makefile
  orbsvcs/tests/ImplRepo/Bug_689_Regression/Makefile
  orbsvcs/tests/ImplRepo/NameService/Makefile
  orbsvcs/tests/ImplRepo/scale/Makefile
  orbsvcs/tests/InterfaceRepo/Makefile
  orbsvcs/tests/InterfaceRepo/Application_Test/Makefile
  orbsvcs/tests/InterfaceRepo/Bug_2962_Regression/Makefile
  orbsvcs/tests/InterfaceRepo/Bug_3155_Regression/Makefile
  orbsvcs/tests/InterfaceRepo/Bug_3174_Regression/Makefile
  orbsvcs/tests/InterfaceRepo/IDL3_Test/Makefile
  orbsvcs/tests/InterfaceRepo/IFR_Inheritance_Test/Makefile
  orbsvcs/tests/InterfaceRepo/IFR_Test/Makefile
  orbsvcs/tests/InterfaceRepo/Latency_Test/Makefile
  orbsvcs/tests/InterfaceRepo/Persistence_Test/Makefile
  orbsvcs/tests/Interoperable_Naming/Makefile
  orbsvcs/tests/LoadBalancing/Makefile
  orbsvcs/tests/LoadBalancing/GenericFactory/Makefile
  orbsvcs/tests/LoadBalancing/GenericFactory/Application_Controlled/Makefile
  orbsvcs/tests/LoadBalancing/GenericFactory/Infrastructure_Controlled/Makefile
  orbsvcs/tests/LoadBalancing/GenericFactory/Manage_Object_Group/Makefile
  orbsvcs/tests/LoadBalancing/LoadMonitor/Makefile
  orbsvcs/tests/LoadBalancing/LoadMonitor/CPU/Makefile
  orbsvcs/tests/Log/Makefile
  orbsvcs/tests/Log/Basic_Log_Test/Makefile
  orbsvcs/tests/Miop/Makefile
  orbsvcs/tests/Miop/McastHello/Makefile
  orbsvcs/tests/Notify/Basic/Makefile
  orbsvcs/tests/Notify/Blocking/Makefile
  orbsvcs/tests/Notify/Bug_1385_Regression/Makefile
  orbsvcs/tests/Notify/Bug_2561_Regression/Makefile
  orbsvcs/tests/Notify/Bug_3252_Regression/Makefile
  orbsvcs/tests/Notify/Destroy/Makefile
  orbsvcs/tests/Notify/Discarding/Makefile
  orbsvcs/tests/Notify/Driver/Makefile
  orbsvcs/tests/Notify/MC/Makefile
  orbsvcs/tests/Notify/MT_Dispatching/Makefile
  orbsvcs/tests/Notify/Ordering/Makefile
  orbsvcs/tests/Notify/Persistent_POA/Makefile
  orbsvcs/tests/Notify/PluggableTopology/Makefile
  orbsvcs/tests/Notify/RT_lib/Makefile
  orbsvcs/tests/Notify/Reconnecting/Makefile
  orbsvcs/tests/Notify/Sequence_Multi_ETCL_Filter/Makefile
  orbsvcs/tests/Notify/Sequence_Multi_Filter/Makefile
  orbsvcs/tests/Notify/Structured_Filter/Makefile
  orbsvcs/tests/Notify/Structured_Multi_Filter/Makefile
  orbsvcs/tests/Notify/Test_Filter/Makefile
  orbsvcs/tests/Notify/XML_Persistence/Makefile
  orbsvcs/tests/Notify/performance-tests/Makefile
  orbsvcs/tests/Notify/performance-tests/Filter/Makefile
  orbsvcs/tests/Notify/performance-tests/RedGreen/Makefile
  orbsvcs/tests/Notify/performance-tests/Throughput/Makefile
  orbsvcs/tests/Notify/Timeout/Makefile
  orbsvcs/tests/Property/Makefile
  orbsvcs/tests/Redundant_Naming/Makefile
  orbsvcs/tests/Sched/Makefile
  orbsvcs/tests/Sched_Conf/Makefile
  orbsvcs/tests/Security/Makefile
  orbsvcs/tests/Security/BiDirectional/Makefile
  orbsvcs/tests/Security/Big_Request/Makefile
  orbsvcs/tests/Security/Bug_1107_Regression/Makefile
  orbsvcs/tests/Security/Bug_2908_Regression/Makefile
  orbsvcs/tests/Security/Callback/Makefile
  orbsvcs/tests/Security/Crash_Test/Makefile
  orbsvcs/tests/Security/EndpointPolicy/Makefile
  orbsvcs/tests/Security/InsecureReferences/Makefile
  orbsvcs/tests/Security/MT_IIOP_SSL/Makefile
  orbsvcs/tests/Security/MT_SSLIOP/Makefile
  orbsvcs/tests/Security/Secure_Invocation/Makefile
  orbsvcs/tests/Security/mixed_security_test/Makefile
  orbsvcs/tests/Security/ssliop_CSD/Makefile
  orbsvcs/tests/Security/ssliop_corbaloc/Makefile
  orbsvcs/tests/Simple_Naming/Makefile
  orbsvcs/tests/Time/Makefile
  orbsvcs/tests/Trading/Makefile
  orbsvcs/tests/ior_corbaname/Makefile
  orbsvcs/tests/tests_svc_loader/Makefile
  orbsvcs/tests/unit/Makefile
  orbsvcs/tests/unit/Notify/Makefile
  orbsvcs/tests/unit/Notify/MC/Makefile
  orbsvcs/tests/unit/Notify/MC/Control/Makefile
  orbsvcs/tests/unit/Notify/MC/MonitorControlExt/Makefile
  orbsvcs/tests/unit/Notify/MC/MonitorManager/Makefile
  orbsvcs/tests/unit/Notify/MC/NotificationServiceMonitor/Makefile
  orbsvcs/tests/unit/Notify/MC/Statistic/Makefile
  orbsvcs/tests/unit/Notify/MC/Statistic_Registry/Makefile
  orbsvcs/tests/unit/Trading/Makefile
  orbsvcs/tests/unit/Trading/Interpreter/Makefile
  docs/Makefile
  docs/tutorials/Makefile
  docs/tutorials/Quoter/Makefile
  docs/tutorials/Quoter/AMI/Makefile
  docs/tutorials/Quoter/Event_Service/Makefile
  docs/tutorials/Quoter/idl/Makefile
  docs/tutorials/Quoter/Naming_Service/Makefile
  docs/tutorials/Quoter/On_Demand_Activation/Makefile
  docs/tutorials/Quoter/RTCORBA/Makefile
  docs/tutorials/Quoter/RT_Event_Service/Makefile
  docs/tutorials/Quoter/Simple/Makefile
  docs/tutorials/Quoter/Simple/Client/Makefile
  docs/tutorials/Quoter/Simple/Impl-Repo/Makefile
  docs/tutorials/Quoter/Simple/ImprovedServer/Makefile
  docs/tutorials/Quoter/Simple/Persistent/Makefile
  docs/tutorials/Quoter/Simple/Server/Makefile
  utils/Makefile
  utils/catior/Makefile
  utils/monitor/Makefile
  utils/nslist/Makefile
  utils/wxNamingViewer/Makefile
 ])

if test $tao_build_examples = yes; then
  AC_CONFIG_FILES([
    examples/Makefile
    examples/AMH/Makefile
    examples/AMH/Sink_Server/Makefile
    examples/AMI/Makefile
    examples/AMI/FL_Callback/Makefile
    examples/Advanced/Makefile
    examples/Advanced/ch_3/Makefile
    examples/Buffered_AMI/Makefile
    examples/Buffered_Oneways/Makefile
    examples/CSD_Strategy/Makefile
    examples/CSD_Strategy/ThreadPool/Makefile
    examples/CSD_Strategy/ThreadPool2/Makefile
    examples/CSD_Strategy/ThreadPool3/Makefile
    examples/CSD_Strategy/ThreadPool4/Makefile
    examples/CSD_Strategy/ThreadPool5/Makefile
    examples/CSD_Strategy/ThreadPool6/Makefile
    examples/Callback_Quoter/Makefile
    examples/Content_Server/Makefile
    examples/Content_Server/AMI_Iterator/Makefile
    examples/Content_Server/AMI_Observer/Makefile
    examples/Content_Server/SMI_Iterator/Makefile
    examples/Event_Comm/Makefile
    examples/Kokyu_dsrt_schedulers/Makefile
    examples/Kokyu_dsrt_schedulers/fp_example/Makefile
    examples/Kokyu_dsrt_schedulers/mif_example/Makefile
    examples/Kokyu_dsrt_schedulers/muf_example/Makefile
    examples/Load_Balancing/Makefile
    examples/Load_Balancing_persistent/Makefile
    examples/Logging/Makefile
    examples/OBV/Makefile
    examples/OBV/Typed_Events/Makefile
    examples/Persistent_Grid/Makefile
    examples/PluggableUDP/Makefile
    examples/PluggableUDP/tests/Makefile
    examples/PluggableUDP/tests/Performance/Makefile
    examples/PluggableUDP/tests/SimplePerformance/Makefile
    examples/Quoter/Makefile
    examples/RTCORBA/Makefile
    examples/RTCORBA/Activity/Makefile
    examples/RTScheduling/Makefile
    examples/RTScheduling/MIF_Scheduler/Makefile
    examples/RTScheduling/Fixed_Priority_Scheduler/Makefile
    examples/Simple/Makefile
    examples/Simple/bank/Makefile
    examples/Simple/chat/Makefile
    examples/Simple/echo/Makefile
    examples/Simple/grid/Makefile
    examples/Simple/time-date/Makefile
    examples/Simple/time/Makefile
    examples/Simulator/Makefile
    examples/Simulator/Event_Supplier/Makefile
    examples/TypeCode_Creation/Makefile
    examples/ior_corbaloc/Makefile
    examples/mfc/Makefile
  ])
fi

if test $tao_build_tests = yes; then
  AC_CONFIG_FILES([

    performance-tests/Makefile
    performance-tests/Anyop/Makefile
    performance-tests/CSD_Strategy/Makefile
    performance-tests/CSD_Strategy/TestInf/Makefile
    performance-tests/CSD_Strategy/TestServant/Makefile
    performance-tests/CSD_Strategy/TestApps/Makefile
    performance-tests/Callback/Makefile
    performance-tests/Cubit/Makefile
    performance-tests/Cubit/TAO/Makefile
    performance-tests/Cubit/TAO/IDL_Cubit/Makefile
    performance-tests/Cubit/TAO/DII_Cubit/Makefile
    performance-tests/Cubit/TAO/MT_Cubit/Makefile
    performance-tests/Latency/Makefile
    performance-tests/Latency/AMH_Single_Threaded/Makefile
    performance-tests/Latency/AMI/Makefile
    performance-tests/Latency/Collocation/Makefile
    performance-tests/Latency/DII/Makefile
    performance-tests/Latency/DSI/Makefile
    performance-tests/Latency/Deferred/Makefile
    performance-tests/Latency/Single_Threaded/Makefile
    performance-tests/Latency/Thread_Per_Connection/Makefile
    performance-tests/Latency/Thread_Pool/Makefile
    performance-tests/Memory/Makefile
    performance-tests/Memory/IORsize/Makefile
    performance-tests/Memory/Single_Threaded/Makefile
    performance-tests/POA/Makefile
    performance-tests/POA/Create_Reference/Makefile
    performance-tests/POA/Demux/Makefile
    performance-tests/POA/Implicit_Activation/Makefile
    performance-tests/POA/Object_Creation_And_Registration/Makefile
    performance-tests/Pluggable/Makefile
    performance-tests/Protocols/Makefile
    performance-tests/RTCorba/Makefile
    performance-tests/RTCorba/Multiple_Endpoints/Makefile
    performance-tests/RTCorba/Multiple_Endpoints/Common/Makefile
    performance-tests/RTCorba/Multiple_Endpoints/Orb_Per_Priority/Makefile
    performance-tests/RTCorba/Multiple_Endpoints/Single_Endpoint/Makefile
    performance-tests/RTCorba/Oneways/Makefile
    performance-tests/RTCorba/Oneways/Reliable/Makefile
    performance-tests/RTCorba/Thread_Pool/Makefile
    performance-tests/Sequence_Latency/Makefile
    performance-tests/Sequence_Latency/AMH_Single_Threaded/Makefile
    performance-tests/Sequence_Latency/AMI/Makefile
    performance-tests/Sequence_Latency/DII/Makefile
    performance-tests/Sequence_Latency/DSI/Makefile
    performance-tests/Sequence_Latency/Deferred/Makefile
    performance-tests/Sequence_Latency/Single_Threaded/Makefile
    performance-tests/Sequence_Latency/Thread_Per_Connection/Makefile
    performance-tests/Sequence_Latency/Thread_Pool/Makefile
    performance-tests/Throughput/Makefile
    interop-tests/Makefile
    interop-tests/wchar/Makefile
    tests/Makefile
    tests/AMH_Exceptions/Makefile
    tests/AMH_Oneway/Makefile
    tests/AMI/Makefile
    tests/AMI_Buffering/Makefile
    tests/AMI_Timeouts/Makefile
    tests/Abstract_Interface/Makefile
    tests/Any/Makefile
    tests/Any/Recursive/Makefile
    tests/BiDirectional/Makefile
    tests/BiDirectional_DelayedUpcall/Makefile
    tests/BiDirectional_MultipleORB/Makefile
    tests/BiDirectional_NestedUpcall/Makefile
    tests/Big_AMI/Makefile
    tests/Big_Oneways/Makefile
    tests/Big_Reply/Makefile
    tests/Big_Request_Muxing/Makefile
    tests/Big_Twoways/Makefile
    tests/Blocking_Sync_None/Makefile
    tests/Bug_1020_Basic_Regression/Makefile
    tests/Bug_1020_Regression/Makefile
    tests/Bug_1254_Regression/Makefile
    tests/Bug_1269_Regression/Makefile
    tests/Bug_1270_Regression/Makefile
    tests/Bug_1330_Regression/Makefile
    tests/Bug_1361_Regression/Makefile
    tests/Bug_1383_Regression/Makefile
    tests/Bug_1476_Regression/Makefile
    tests/Bug_1476_Test/Makefile
    tests/Bug_1482_Regression/Makefile
    tests/Bug_1495_Regression/Makefile
    tests/Bug_1535_Regression/Makefile
    tests/Bug_1551_Regression/Makefile
    tests/Bug_1568_Regression/Makefile
    tests/Bug_1627_Regression/Makefile
    tests/Bug_1635_Regression/Makefile
    tests/Bug_1636_Regression/Makefile
    tests/Bug_1639_Regression/Makefile
    tests/Bug_1670_Regression/Makefile
    tests/Bug_1676_Regression/Makefile
    tests/Bug_1693_Test/Makefile
    tests/Bug_1812_Regression/Makefile
    tests/Bug_1813_Regression/Makefile
    tests/Bug_1869_Regression/Makefile
    tests/Bug_1904_Regression/Makefile
    tests/Bug_2084_Regression/Makefile
    tests/Bug_2085_Regression/Makefile
    tests/Bug_2119_Regression/Makefile
    tests/Bug_2122_Regression/Makefile
    tests/Bug_2124_Regression/Makefile
    tests/Bug_2126_Regression/Makefile
    tests/Bug_2134_Regression/Makefile
    tests/Bug_2144_Regression/Makefile
    tests/Bug_2174_Regression/Makefile
    tests/Bug_2183_Regression/Makefile
    tests/Bug_2186_Regression/Makefile
    tests/Bug_2188_Regression/Makefile
    tests/Bug_2201_Regression/Makefile
    tests/Bug_2222_Regression/Makefile
    tests/Bug_2234_Regression/Makefile
    tests/Bug_2243_Regression/Makefile
    tests/Bug_2289_Regression/Makefile
    tests/Bug_2319_Regression/Makefile
    tests/Bug_2328_Regression/Makefile
    tests/Bug_2345_Regression/Makefile
    tests/Bug_2349_Regression/Makefile
    tests/Bug_2356_Regression/Makefile
    tests/Bug_2375_Regression/Makefile
    tests/Bug_2399_Regression/Makefile
    tests/Bug_2417_Regression/Makefile
    tests/Bug_2419_Regression/Makefile
    tests/Bug_2424_Regression/Makefile
    tests/Bug_2429_Regression/Makefile
    tests/Bug_2494_Regression/Makefile
    tests/Bug_2503_Regression/Makefile
    tests/Bug_2542_Regression/Makefile
    tests/Bug_2543_Regression/Makefile
    tests/Bug_2549_Regression/Makefile
    tests/Bug_2560_Regression/Makefile
    tests/Bug_2593_Regression/Makefile
    tests/Bug_2595_Regression/Makefile
    tests/Bug_2654_Regression/Makefile
    tests/Bug_2669_Regression/Makefile
    tests/Bug_2677_Regression/Makefile
    tests/Bug_2678_Regression/Makefile
    tests/Bug_2683_Regression/Makefile
    tests/Bug_2702_Regression/Makefile
    tests/Bug_2734_Regression/Makefile
    tests/Bug_2735_Regression/Makefile
    tests/Bug_2768_Regression/Makefile
    tests/Bug_2791_Regression/Makefile
    tests/Bug_2792_Regression/Makefile
    tests/Bug_2795_Regression/Makefile
    tests/Bug_2804_Regression/Makefile
    tests/Bug_2805_Regression/Makefile
    tests/Bug_2809_Regression/Makefile
    tests/Bug_2826_Regression/Makefile
    tests/Bug_2844_Regression/Makefile
    tests/Bug_2869_Regression/Makefile
    tests/Bug_2909_Regression/Makefile
    tests/Bug_2918_Regression/Makefile
    tests/Bug_2935_Regression/Makefile
    tests/Bug_2936_Regression/Makefile
    tests/Bug_2953_Regression/Makefile
    tests/Bug_2966_Regression/Makefile
    tests/Bug_3000_Regression/Makefile
    tests/Bug_3042_Regression/Makefile
    tests/Bug_3108_Regression/Makefile
    tests/Bug_3163_Regression/Makefile
    tests/Bug_3171_Regression/Makefile
    tests/Bug_3198_Regression/Makefile
    tests/Bug_3251_Regression/Makefile
    tests/Bug_3276_Regression/Makefile
    tests/Bug_3299_Regression/Makefile
    tests/Bug_3311_Regression/Makefile
    tests/Bug_3315_Regression/Makefile
    tests/Bug_3321_Regression/Makefile
    tests/Bug_933_Regression/Makefile
    tests/CDR/Makefile
    tests/COIOP/Makefile
    tests/CORBA_e_Implicit_Activation/Makefile
    tests/CSD_Collocation/Makefile
    tests/CSD_Strategy_Tests/Makefile
    tests/CSD_Strategy_Tests/TP_Common/Makefile
    tests/CSD_Strategy_Tests/TP_Foo_B/Makefile
    tests/CSD_Strategy_Tests/Broken/Makefile
    tests/CSD_Strategy_Tests/TP_Foo_A/Makefile
    tests/CSD_Strategy_Tests/TP_Foo_C/Makefile
    tests/CSD_Strategy_Tests/TP_Test_1/Makefile
    tests/CSD_Strategy_Tests/TP_Test_2/Makefile
    tests/CSD_Strategy_Tests/TP_Test_3/Makefile
    tests/CSD_Strategy_Tests/TP_Test_4/Makefile
    tests/CSD_Strategy_Tests/TP_Test_Dynamic/Makefile
    tests/CSD_Strategy_Tests/TP_Test_Static/Makefile
    tests/Cache_Growth_Test/Makefile
    tests/CallbackTest/Makefile
    tests/Client_Leaks/Makefile
    tests/CodeSets/Makefile
    tests/CodeSets/libs/Makefile
    tests/CodeSets/libs/IBM1047_ISO8859/Makefile
    tests/CodeSets/libs/UCS4_UTF16/Makefile
    tests/CodeSets/libs/UTF16_UCS2/Makefile
    tests/CodeSets/simple/Makefile
    tests/Codec/Makefile
    tests/Collocated_Forwarding/Makefile
    tests/Collocation/Makefile
    tests/CollocationLockup/Makefile
    tests/Collocation_Exception_Test/Makefile
    tests/Collocation_Oneway_Tests/Makefile
    tests/Collocation_Opportunities/Makefile
    tests/Collocation_Tests/Makefile
    tests/Compression/Makefile
    tests/Connect_Strategy_Test/Makefile
    tests/Connection_Failure/Makefile
    tests/Connection_Purging/Makefile
    tests/Connection_Timeout/Makefile
    tests/Crash_On_Write/Makefile
    tests/Crashed_Callback/Makefile
    tests/DII_AMI_Forward/Makefile
    tests/DII_Collocation_Tests/Makefile
    tests/DII_Collocation_Tests/oneway/Makefile
    tests/DII_Collocation_Tests/twoway/Makefile
    tests/DIOP/Makefile
    tests/DLL_ORB/Makefile
    tests/DSI_AMH/Makefile
    tests/DSI_AMI_Gateway/Makefile
    tests/DSI_Gateway/Makefile
    tests/DiffServ/Makefile
    tests/DynAny_Test/Makefile
    tests/DynUnion_Test/Makefile
    tests/Explicit_Event_Loop/Makefile
    tests/Exposed_Policies/Makefile
    tests/FL_Cube/Makefile
    tests/Faults/Makefile
    tests/File_IO/Makefile
    tests/Forwarding/Makefile
    tests/GIOP_Fragments/Makefile
    tests/GIOP_Fragments/Java_Big_Reply/Makefile
    tests/GIOP_Fragments/Java_Big_Request/Makefile
    tests/GIOP_Fragments/PMB_With_Fragments/Makefile
    tests/HandleExhaustion/Makefile
    tests/Hang_Shutdown/Makefile
    tests/Hello/Makefile
    tests/ICMG_Any_Bug/Makefile
    tests/IDL_Test/Makefile
    tests/IORManipulation/Makefile
    tests/IORManipulation/filter/Makefile
    tests/IOR_Endpoint_Hostnames/Makefile
    tests/IPV6/Makefile
    tests/InterOp-Naming/Makefile
    tests/Leader_Followers/Makefile
    tests/LongDouble/Makefile
    tests/LongUpcalls/Makefile
    tests/MProfile/Makefile
    tests/MProfile_Connection_Timeout/Makefile
    tests/MProfile_Forwarding/Makefile
    tests/MT_BiDir/Makefile
    tests/MT_Client/Makefile
    tests/MT_Server/Makefile
    tests/MT_Timeout/Makefile
    tests/Monitor/Makefile
    tests/Monitor/Marshal_Buffer/Makefile
    tests/Multiple/Makefile
    tests/Multiple_Inheritance/Makefile
    tests/Muxed_GIOP_Versions/Makefile
    tests/Muxing/Makefile
    tests/Native_Exceptions/Makefile
    tests/NestedUpcall/Makefile
    tests/NestedUpcall/MT_Client_Test/Makefile
    tests/NestedUpcall/Simple/Makefile
    tests/NestedUpcall/Triangle_Test/Makefile
    tests/Nested_Event_Loop/Makefile
    tests/Nested_Upcall_Crash/Makefile
    tests/No_Server_MT_Connect_Test/Makefile
    tests/OBV/Makefile
    tests/OBV/Any/Makefile
    tests/OBV/Collocated/Makefile
    tests/OBV/Collocated/Forward/Makefile
    tests/OBV/Factory/Makefile
    tests/OBV/Forward/Makefile
    tests/OBV/Simple/Makefile
    tests/OBV/TC_Alignment/Makefile
    tests/OBV/Truncatable/Makefile
    tests/OBV/ValueBox/Makefile
    tests/ORB_Local_Config/Makefile
    tests/ORB_Local_Config/Bug_1459/Makefile
    tests/ORB_Local_Config/Bug_2612/Makefile
    tests/ORB_Local_Config/Bunch/Makefile
    tests/ORB_Local_Config/Separation/Makefile
    tests/ORB_Local_Config/Service_Dependency/Makefile
    tests/ORB_Local_Config/Shared/Makefile
    tests/ORB_Local_Config/Simple/Makefile
    tests/ORB_Local_Config/Two_DLL_ORB/Makefile
    tests/ORB_destroy/Makefile
    tests/ORB_init/Makefile
    tests/ORB_init/Portspan/Makefile
    tests/ORB_shutdown/Makefile
    tests/ORT/Makefile
    tests/Object_Loader/Makefile
    tests/Objref_Sequence_Test/Makefile
    tests/OctetSeq/Makefile
    tests/Ondemand_Write/Makefile
    tests/Oneway_Buffering/Makefile
    tests/Oneway_Send_Timeouts/Makefile
    tests/Oneway_Timeouts/Makefile
    tests/Oneways_Invoking_Twoways/Makefile
    tests/Optimized_Connection/Makefile
    tests/POA/Makefile
    tests/POA/Generic_Servant/Makefile
    tests/POA/Adapter_Activator/Makefile
    tests/POA/Bug_1592_Regression/Makefile
    tests/POA/Bug_2511_Regression/Makefile
    tests/POA/Current/Makefile
    tests/POA/DSI/Makefile
    tests/POA/Deactivate_Object/Makefile
    tests/POA/Default_Servant/Makefile
    tests/POA/Default_Servant2/Makefile
    tests/POA/EndpointPolicy/Makefile
    tests/POA/Etherealization/Makefile
    tests/POA/Excessive_Object_Deactivations/Makefile
    tests/POA/Explicit_Activation/Makefile
    tests/POA/Explicit_Activation/Alt_Resources/Makefile
    tests/POA/FindPOA/Makefile
    tests/POA/Forwarding/Makefile
    tests/POA/Identity/Makefile
    tests/POA/Loader/Makefile
    tests/POA/MT_Servant_Locator/Makefile
    tests/POA/Nested_Non_Servant_Upcalls/Makefile
    tests/POA/NewPOA/Makefile
    tests/POA/Non_Servant_Upcalls/Makefile
    tests/POA/Object_Reactivation/Makefile
    tests/POA/On_Demand_Activation/Makefile
    tests/POA/On_Demand_Loading/Makefile
    tests/POA/POAManagerFactory/Makefile
    tests/POA/POA_BiDir/Makefile
    tests/POA/POA_Destruction/Makefile
    tests/POA/Persistent_ID/Makefile
    tests/POA/Policies/Makefile
    tests/POA/Reference_Counted_Servant/Makefile
    tests/POA/Reference_Counting/Makefile
    tests/POA/RootPOA/Makefile
    tests/POA/Single_Threaded_POA/Makefile
    tests/POA/TIE/Makefile
    tests/POA/wait_for_completion/Makefile
    tests/Parallel_Connect_Strategy/Makefile
    tests/Param_Test/Makefile
    tests/Permanent_Forward/Makefile
    tests/Policies/Makefile
    tests/Portable_Interceptors/Makefile
    tests/Portable_Interceptors/AMI/Makefile
    tests/Portable_Interceptors/AdvSlot/Makefile
    tests/Portable_Interceptors/AdvSlotDblCpy/Makefile
    tests/Portable_Interceptors/AdvSlotExt/Makefile
    tests/Portable_Interceptors/Benchmark/Makefile
    tests/Portable_Interceptors/Bug_1559/Makefile
    tests/Portable_Interceptors/Bug_2088/Makefile
    tests/Portable_Interceptors/Bug_2133/Makefile
    tests/Portable_Interceptors/Bug_2510_Regression/Makefile
    tests/Portable_Interceptors/Bug_3079/Makefile
    tests/Portable_Interceptors/Bug_3080/Makefile
    tests/Portable_Interceptors/Collocated/Makefile
    tests/Portable_Interceptors/Collocated/Dynamic/Makefile
    tests/Portable_Interceptors/Collocated/Service_Context_Manipulation/Makefile
    tests/Portable_Interceptors/Dynamic/Makefile
    tests/Portable_Interceptors/ForwardRequest/Makefile
    tests/Portable_Interceptors/IORInterceptor/Makefile
    tests/Portable_Interceptors/ORB_Shutdown/Makefile
    tests/Portable_Interceptors/PICurrent/Makefile
    tests/Portable_Interceptors/PolicyFactory/Makefile
    tests/Portable_Interceptors/Processing_Mode_Policy/Makefile
    tests/Portable_Interceptors/Processing_Mode_Policy/Collocated/Makefile
    tests/Portable_Interceptors/Processing_Mode_Policy/Remote/Makefile
    tests/Portable_Interceptors/Recursive_ORBInitializer/Makefile
    tests/Portable_Interceptors/Redirection/Makefile
    tests/Portable_Interceptors/Register_Initial_References/Makefile
    tests/Portable_Interceptors/Request_Interceptor_Flow/Makefile
    tests/Portable_Interceptors/Service_Context_Manipulation/Makefile
    tests/Portable_Interceptors/Slot/Makefile
    tests/QtTests/Makefile
    tests/Queued_Message_Test/Makefile
    tests/RTCORBA/Makefile
    tests/RTCORBA/Activate_Object_Multiple_ORBs/Makefile
    tests/RTCORBA/Banded_Connections/Makefile
    tests/RTCORBA/Client_Propagated/Makefile
    tests/RTCORBA/Client_Protocol/Makefile
    tests/RTCORBA/Collocation/Makefile
    tests/RTCORBA/Destroy_Thread_Pool/Makefile
    tests/RTCORBA/Diffserv/Makefile
    tests/RTCORBA/Dynamic_Thread_Pool/Makefile
    tests/RTCORBA/Explicit_Binding/Makefile
    tests/RTCORBA/Linear_Priority/Makefile
    tests/RTCORBA/MT_Client_Protocol_Priority/Makefile
    tests/RTCORBA/ORB_init/Makefile
    tests/RTCORBA/Persistent_IOR/Makefile
    tests/RTCORBA/Policies/Makefile
    tests/RTCORBA/Policy_Combinations/Makefile
    tests/RTCORBA/Priority_Inversion_With_Bands/Makefile
    tests/RTCORBA/Private_Connection/Makefile
    tests/RTCORBA/Profile_And_Endpoint_Selection/Makefile
    tests/RTCORBA/RTMutex/Makefile
    tests/RTCORBA/Server_Declared/Makefile
    tests/RTCORBA/Server_Protocol/Makefile
    tests/RTCORBA/Thread_Pool/Makefile
    tests/RTScheduling/Makefile
    tests/RTScheduling/Current/Makefile
    tests/RTScheduling/DT_Spawn/Makefile
    tests/RTScheduling/Scheduling_Interceptor/Makefile
    tests/RTScheduling/Thread_Cancel/Makefile
    tests/RTScheduling/VoidData/Makefile
    tests/Reliable_Oneways/Makefile
    tests/Sequence_Unit_Tests/Makefile
    tests/Servant_To_Reference_Test/Makefile
    tests/Server_Connection_Purging/Makefile
    tests/Server_Leaks/Makefile
    tests/Server_Port_Zero/Makefile
    tests/Single_Read/Makefile
    tests/Smart_Proxies/Makefile
    tests/Smart_Proxies/Benchmark/Makefile
    tests/Smart_Proxies/Collocation/Makefile
    tests/Smart_Proxies/On_Demand/Makefile
    tests/Smart_Proxies/Policy/Makefile
    tests/Smart_Proxies/dtor/Makefile
    tests/Stack_Recursion/Makefile
    tests/Strategies/Makefile
    tests/Timed_Buffered_Oneways/Makefile
    tests/Timeout/Makefile
    tests/TransportCurrent/Makefile
    tests/TransportCurrent/lib/Makefile
    tests/TransportCurrent/Framework/Makefile
    tests/TransportCurrent/IIOP/Makefile
    tests/Two_Objects/Makefile
    tests/Typedef_String_Array/Makefile
    tests/UNKNOWN_Exception/Makefile
    tests/Xt_Stopwatch/Makefile
    tests/objref_comparison_test/Makefile
  ])
  AC_CONFIG_COMMANDS([bug_2399], [
      mkdir -p tests/Bug_2399_Regression/mytmp
   ],[])
fi

dnl Note that the "TAO_VERSION" in the message below is an M4 macro
dnl that expands to the version of TAO being configured.
AC_CONFIG_COMMANDS([default],[
  echo ""
  echo "Configuration of TAO TAO_VERSION is now complete."
  echo ""
 ],[])
AC_OUTPUT
