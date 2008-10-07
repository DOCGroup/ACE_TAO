# Set the version number here.
%define ACEVER  5.6.6
%define TAOVER  1.6.6
%define CIAOVER 0.6.6
# Set is_major_ver if the version is X.Y instead X.Y.Z

# Conditional build
# Default values are --with guilibs    (fltk, tk ,xt and qt support)
#                    --with rnq        (ACE_HAS_REACTOR_NOTIFICATION_QUEUE)
#                    --with ipv6       (IPv6 support)
#                    --with opt        (Optimized build)

#
# Read: If neither macro exists, then add the default definition.
%{!?_with_guilibs: %{!?_without_guilibs: %define _with_guilibs --with-guilibs}}
%{!?_with_rnq: %{!?_without_rnq: %define _with_rnq --with-rnq}}
%{!?_with_ipv6: %{!?_without_ipv6: %define _with_ipv6 --with-ipv6}}
%{!?_with_opt: %{!?_without_opt: %define _with_opt --with-opt}}
#
# Read: It's an error if both or neither required options exist.
%{?_with_guilibs: %{?_without_guilibs: %{error: both _with_guilibs and _without_guilibs}}}
%{?_with_rnq: %{?_without_rnq: %{error: both _with_rnq and _without_rnq}}}
%{?_with_ipv6: %{?_without_ipv6: %{error: both _with_ipv6 and _without_ipv6}}}
%{?_with_opt: %{?_without_opt: %{error: both _with_opt and _without_opt}}}

%{!?skip_make:%define skip_make 0}
%{!?make_nosrc:%define make_nosrc 0}
%{!?is_major_ver:%define is_major_ver 0}

# I think Mandrake sets this to .bz2, but Fedora doesn't seem to set it
%{!?_extension:%define _extension .gz}

%if %{?_with_opt:0}%{!?_with_opt:1}
%define OPTTAG .O0
%endif

Summary: The ADAPTIVE Communication Environment (ACE) and The ACE ORB (TAO)
Name: ace-tao
Version: %{ACEVER}
Release: 1%{?OPTTAG}%{?dist}
Group: Development/Libraries
URL: http://www.cs.wustl.edu/~schmidt/ACE.html
License: DOC License
Source0: http://download.dre.vanderbilt.edu/previous_versions/ACE+TAO+CIAO-%{ACEVER}.tar.bz2
## Source1: ace-tao-etc.tar.gz
## Source2: ace-tao-macros.patch
## Patch0: ace-tao-config.patch
## Patch5: ace-tao-orbsvcs-daemon.patch
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

Requires(post): /sbin/install-info, /sbin/install-info
Requires(preun):  /sbin/install-info
Requires(postun): /sbin/ldconfig

BuildRequires: openssl-devel

# Read: true if _with_guilibs is defined, false if not defined.
%if %{?_with_guilibs:1}%{!?_with_guilibs:0}
BuildRequires: perl
BuildRequires: fltk-devel
BuildRequires: tcl-devel
BuildRequires: tk-devel
BuildRequires: tk

# qt3 has a name change in F9
%if 0%{?fedora} > 8
BuildRequires: qt3-devel
%else
BuildRequires: qt-devel
%endif

# The xorg package naming scheme changed, use specific files for now.
# old -> BuildRequires: xorg-x11-devel
# new -> BuildRequires: libX11-devel
# BuildRequires: %{_libdir}/libX11.so
# BuildRequires: %{_libdir}/libXt.so
%endif

%if %make_nosrc
# Leave out the distro for now
NoSource: 0
%endif

%description

The ADAPTIVE Communication Environment (ACE) is a freely available,
open-source object-oriented (OO) framework that implements many core
patterns for concurrent communication software. ACE provides a rich
set of reusable C++ wrapper facades and framework components that
perform common communication software tasks across a range of OS
platforms. The communication software tasks provided by ACE include
event demultiplexing and event handler dispatching, signal handling,
service initialization, interprocess communication, shared memory
management, message routing, dynamic (re)configuration of distributed
services, concurrent execution and synchronization.

TAO is a real-time implementation of CORBA built using the framework
components and patterns provided by ACE. TAO contains the network
interface, OS, communication protocol, and CORBA middleware components
and features. TAO is based on the standard OMG CORBA reference model,
with the enhancements designed to overcome the shortcomings of
conventional ORBs for high-performance and real-time applications.

# ---------------- ace ----------------

%package -n ace
Summary: The ADAPTIVE Communication Environment (ACE)
Version: %{ACEVER}
Group: Development/Libraries
Requires: openssl

%description -n ace

The ADAPTIVE Communication Environment (ACE) is a freely available,
open-source object-oriented (OO) framework that implements many core
patterns for concurrent communication software. ACE provides a rich
set of reusable C++ wrapper facades and framework components that
perform common communication software tasks across a range of OS
platforms. The communication software tasks provided by ACE include
event demultiplexing and event handler dispatching, signal handling,
service initialization, interprocess communication, shared memory
management, message routing, dynamic (re)configuration of distributed
services, concurrent execution and synchronization.

# ---------------- ace-devel ----------------

%package -n ace-devel
Summary: Header files and development components for ACE
Version: %{ACEVER}
Group: Development/Libraries
Requires: ace = %{ACEVER}
Requires: openssl-devel

%description -n ace-devel

This package contains the components needed for developing programs
using ACE.

# ---------------- ace-xml ----------------

%package -n ace-xml
Summary:  ACE XML Runtime Support
Version:  %{ACEVER}
Group: Development/Libraries
Requires: ace = %{ACEVER}

%description -n ace-xml

ACE XML Parser interfaces follows the the design of SAX 2.0, which is
a public domain specification for Java.  The major difference between
ACE XML Parser interfaces and SAX is that we added an reference of
ACEXML_Env to every SAX method to accommodate platforms/compilers that
don't support C++ exceptions.  SAX is defined by David Megginson
<david@megginson.com>

# ---------------- ace-xml-devel ----------------

%package -n ace-xml-devel
Summary:  Header files and development components for ACE XML
Version:  %{ACEVER}
Group: Development/Libraries
Requires: ace-devel = %{ACEVER}
Requires: ace-xml = %{ACEVER}

%description -n ace-xml-devel

This package contains the components needed for developing programs
using ACEXML.

# ---------------- ace-kokyu ----------------

%package -n ace-kokyu
Summary: Kokyu scheduling framework for ACE
Version: %{ACEVER}
Group: Development/Libraries
Requires: ace = %{ACEVER}

%description -n ace-kokyu

Kokyu is a portable middleware scheduling framework designed to
provide flexible scheduling and dispatching services within the
context of higher-level middleware. Kokyu currently provides real-time
scheduling and dispatching services for TAO's real-time Event Service
which mediates supplier-consumer relationships between application
operations. Kokyu also provides a scheduling and dispatching framework
for threads. This is being used by the TAO RTCORBA 2.0 scheduler
implementations.

# ---------------- ace-kokyu-devel ----------------

%package -n ace-kokyu-devel
Summary: Header files and development components for the ACE Kokyu scheduler
Version: %{ACEVER}
Group: Development/Libraries
Requires: ace-devel = %{ACEVER}
Requires: ace-kokyu = %{ACEVER}

%description -n ace-kokyu-devel

This package contains the components needed for developing programs
using Kokyu.


# Read: true if _with_guilibs is defined, false if not defined.
%if %{?_with_guilibs:1}%{!?_with_guilibs:0}
# ---------------- ace-flreactor ----------------

%package -n ace-flreactor
Summary: ACE_FlReactor for use with the Fast-Light toolkit
Version: %{ACEVER}
Group: Development/Libraries
Requires: ace = %{ACEVER}
Requires: fltk

%description -n ace-flreactor

A Reactor implementation that uses the Fast-Light (FL) toolkit for
event demultiplexing.  This will let us integrate the FL toolkit with
ACE and/or TAO.

# ---------------- ace-flreactor-devel ----------------

%package -n ace-flreactor-devel
Summary: Header files for development with ACE_FlReactor
Version: %{ACEVER}
Group: Development/Libraries
Requires: ace-devel = %{ACEVER}
Requires: ace-flreactor = %{ACEVER}
Requires: fltk-devel

%description -n ace-flreactor-devel

This package contains the components needed for developing programs
using the ACE_FlReactor.

# ---------------- ace-qtreactor ----------------

%package -n ace-qtreactor
Summary: ACE_QtReactor for use with Qt library
Version: %{ACEVER}
Group: Development/Libraries
Requires: ace = %{ACEVER}
Requires: qt

%description -n ace-qtreactor

A Reactor implementation that uses the Qt toolkit for event
demultiplexing.  This will let us integrate the Qt toolkit with ACE
and/or TAO.

# ---------------- ace-qtreactor-devel ----------------

%package -n ace-qtreactor-devel
Summary: Header files for development with ACE_QtReactor
Version: %{ACEVER}
Group: Development/Libraries
Requires: ace-devel = %{ACEVER}
Requires: ace-qtreactor = %{ACEVER}
Requires: qt-devel

%description -n ace-qtreactor-devel

This package contains the components needed for developing programs
using the ACE_QtReactor.

# ---------------- ace-tkreactor ----------------

%package -n ace-tkreactor
Summary: ACE_TkReactor for use with Tk toolkit
Version: %{ACEVER}
Group: Development/Libraries
Requires: ace = %{ACEVER}
Requires: tk

%description -n ace-tkreactor

A Reactor implementation that uses the Tk toolkit for event
demultiplexing.  This will let us integrate the Tk toolkit with ACE
and/or TAO.

# ---------------- ace-tkreactor-devel ----------------

%package -n ace-tkreactor-devel
Summary: Header files for development with ACE_TkReactor
Version: %{ACEVER}
Group: Development/Libraries
Requires: ace-devel = %{ACEVER}
Requires: ace-tkreactor = %{ACEVER}
Requires: tk-devel

%description -n ace-tkreactor-devel

This package contains the components needed for developing programs
using the ACE_TkReactor.

# ---------------- ace-xtreactor ----------------

%package -n ace-xtreactor
Summary: ACE_XtReactor for use with the X Toolkit
Version: %{ACEVER}
Group: Development/Libraries
Requires: ace = %{ACEVER}
# The xorg packaging scheme changed, let autoreq to the job for now.
# Requires: xorg-x11-libs

%description -n ace-xtreactor

A Reactor implementation that uses the X Toolkit for event
demultiplexing.  This will let us integrate the X Toolkit with ACE
and/or TAO.

# ---------------- ace-xtreactor-devel ----------------

%package -n ace-xtreactor-devel
Summary: Header files for development with ACE_XtReactor
Version: %{ACEVER}
Group: Development/Libraries
Requires: ace-devel = %{ACEVER}
Requires: ace-xtreactor = %{ACEVER}
# The xorg package naming scheme changed, use specific files for now.
# old -> Requires: xorg-x11-devel
# new -> Requires: libX11-devel
Requires: %{_libdir}/libX11.so
Requires: %{_libdir}/libXt.so

%description -n ace-xtreactor-devel

This package contains the components needed for developing programs
using the ACE_XtReactor.
%endif

## FIXME - namespace issues need to be resolved
##
## # ---------------- MPC ----------------
##
## %package -n ace-mpc
## Summary: Make Project Creator
## Version: %{ACEVER}
## Group: Development/Libraries
##
## %description -n ace-mpc
##
## The Makefile, Project and Workspace Creator.
## Designed by Justin Michel (michel_j@ociweb.com) and Chad Elliott.
## Implemented by Chad Elliott (elliott_c@ociweb.com).
##
## A single tool (MPC) can be used to generate tool specific input (i.e.
## Makefile, dsp, vcproj, etc). The generator takes platform and building
## tool generic files (mpc files) as input which describe basic information
## needed to generate a "project" file for various build tools. These tools
## include Make, NMake, Visual C++ 6, Visual C++ 7, etc.

# ---------------- tao ----------------

%package -n tao
Summary: The ACE ORB (TAO)
Version: %{TAOVER}
Group: Development/Libraries
Requires: ace = %{ACEVER}
Requires: ace-xml = %{ACEVER}
Requires: ace-kokyu = %{ACEVER}

%description -n tao

TAO is a real-time implementation of CORBA built using the framework
components and patterns provided by ACE. TAO contains the network
interface, OS, communication protocol, and CORBA middleware components
and features. TAO is based on the standard OMG CORBA reference model,
with the enhancements designed to overcome the shortcomings of
conventional ORBs for high-performance and real-time applications.

# ---------------- tao-devel ----------------

%package -n tao-devel
Summary: Header files and development components for TAO
Version: %{TAOVER}
Group: Development/Libraries
Requires: tao = %{TAOVER}
Requires: ace-devel = %{ACEVER}
Requires: ace-xml-devel = %{ACEVER}
Requires: ace-kokyu-devel = %{ACEVER}

%description -n tao-devel

This package contains the components needed for developing programs
using TAO.

# ---------------- tao-cosnaming ----------------

%package -n tao-cosnaming
Summary: The TAO CORBA Naming Service (CosNaming) and Interoperable Naming Service (INS)
Version: %{TAOVER}
Group: Development/Libraries
Requires: tao = %{TAOVER}

%description -n tao-cosnaming

OMG defined CORBA Naming Service to provide a basic service location
mechanism for CORBA systems. CosNaming manages a hierarchy of
name-to-object-reference mappings. Anything, but typically the server
process hosting an object, may bind an object reference with a name in
the Naming Service by providing the name and object
reference. Interested parties (typically clients) can then use the
Naming Service to resolve a name to an object reference.

More recently, CORBA Naming Service was subsumed/extended by the CORBA
Interoperable Naming Service, a.k.a. INS. INS inherits all the
functionality from the original Naming Service specification in
addition to addressing some its shortcomings. In particular, INS
defines a standard way for clients and servers to locate the Naming
Service itself. It also allows the ORB to be administratively
configured for bootstrapping to services not set up with the orb at
install time.

# ---------------- tao-cosevent ----------------

%package -n tao-cosevent
Summary: The TAO CORBA CosEvent Service
Version: %{TAOVER}
Group: Development/Libraries
Requires: tao = %{TAOVER}

%description -n tao-cosevent

The CosEvent_Service is a COS compilant Event Service.

The service is registered with the naming service with the name
"CosEventService" . It exposes the <EventChannel> interface which can be
used by suppliers and consumers to send and receive events.

# ---------------- tao-cosnotification ----------------

%package -n tao-cosnotification
Summary: The TAO CORBA Notification Service
Version: %{TAOVER}
Group: Development/Libraries
Requires: tao = %{TAOVER}

%description -n tao-cosnotification

The Notify_Service is a COS compilant Notification Service.

The Notify_Service executable starts up a Notification Service factory
and registers it with the Naming Service under the name
"NotifyEventChannelFactory"

# ---------------- tao-costrading ----------------

%package -n tao-costrading
Summary: The TAO CORBA Trading Service
Version: %{TAOVER}
Group: Development/Libraries
Requires: tao = %{TAOVER}

%description -n tao-costrading

The Trading_Service is a COS compilant Trading Service.

# ---------------- tao-rtevent ----------------

%package -n tao-rtevent
Summary: The TAO Real-time Event Service
Version: %{TAOVER}
Group: Development/Libraries
Requires: tao = %{TAOVER}

%description -n tao-rtevent

The TAO Real-time Event Service.

# ---------------- tao-cosconcurrency ----------------

%package -n tao-cosconcurrency
Summary: The TAO CORBA Concurrency Service
Version: %{TAOVER}
Group: Development/Libraries
Requires: tao = %{TAOVER}

%description -n tao-cosconcurrency

The CORBA Concurrency Service.

# ---------------- tao-flresource ----------------

# Read: true if _with_guilibs is defined, false if not defined.
%if %{?_with_guilibs:1}%{!?_with_guilibs:0}

%package -n tao-flresource
Summary: FlResource_Factory for creating FlReactor
Version: %{TAOVER}
Group: Development/Libraries
Requires: ace-flreactor = %{ACEVER}
Requires: tao = %{TAOVER}

%description -n tao-flresource

This factory is intended for creating FlReactor for ORB. This factory
can be feed into ORB using TAO_ORB_Core::set_gui_resource_factory
method which is usually done by TAO_FlResource_Loader.

# ---------------- tao-flresource-devel ----------------

%package -n tao-flresource-devel
Summary: Header files for development with FlResource_Factory
Version: %{TAOVER}
Group: Development/Libraries
Requires: ace-flreactor-devel = %{ACEVER}
Requires: tao-devel = %{TAOVER}
Requires: tao-flresource = %{TAOVER}

%description -n tao-flresource-devel

This package contains the components needed for developing programs
using the FlResource_Factory.

# ---------------- tao-qtresource ----------------

%package -n tao-qtresource
Summary: QtResource_Factory for creating QtReactor
Version: %{TAOVER}
Group: Development/Libraries
Requires: ace-qtreactor = %{ACEVER}
Requires: tao = %{TAOVER}

%description -n tao-qtresource

This factory is intended for creating QtReactor for ORB. This factory
can be feed into ORB using TAO_ORB_Core::set_gui_resource_factory
method which is usually done by TAO_QtResource_Loader.

# ---------------- tao-qtresource-devel ----------------

%package -n tao-qtresource-devel
Summary: Header files for development with QtResource_Factory
Version: %{TAOVER}
Group: Development/Libraries
Requires: ace-qtreactor-devel = %{ACEVER}
Requires: tao-devel = %{TAOVER}
Requires: tao-qtresource = %{TAOVER}

%description -n tao-qtresource-devel

This package contains the components needed for developing programs
using the QtResource_Factory.

# ---------------- tao-tkresource ----------------

%package -n tao-tkresource
Summary: TkResource_Factory for creating TkReactor
Version: %{TAOVER}
Group: Development/Libraries
Requires: ace-tkreactor = %{ACEVER}
Requires: tao = %{TAOVER}

%description -n tao-tkresource

This factory is intended for creating TkReactor for ORB. This factory
can be feed into ORB using TAO_ORB_Core::set_gui_resource_factory
method which is usually done by TAO_TkResource_Loader.

# ---------------- tao-tkresource-devel ----------------

%package -n tao-tkresource-devel
Summary: Header files for development with TkResource_Factory
Version: %{TAOVER}
Group: Development/Libraries
Requires: ace-tkreactor-devel = %{ACEVER}
Requires: tao-devel = %{TAOVER}
Requires: tao-tkresource = %{TAOVER}

%description -n tao-tkresource-devel

This package contains the components needed for developing programs
using the TkResource_Factory.

# ---------------- tao-xtresource ----------------

%package -n tao-xtresource
Summary: XtResource_Factory for creating XtReactor
Version: %{TAOVER}
Group: Development/Libraries
Requires: ace-xtreactor = %{ACEVER}
Requires: tao = %{TAOVER}

%description -n tao-xtresource

This factory is intended for creating XtReactor for ORB. This factory
can be feed into ORB using TAO_ORB_Core::set_gui_resource_factory
method which is usually done by TAO_XtResource_Loader.

# ---------------- tao-xtresource-devel ----------------

%package -n tao-xtresource-devel
Summary: Header files for development with XtResource_Factory
Version: %{TAOVER}
Group: Development/Libraries
Requires: ace-xtreactor-devel = %{ACEVER}
Requires: tao-devel = %{TAOVER}
Requires: tao-xtresource = %{TAOVER}

%description -n tao-xtresource-devel

This package contains the components needed for developing programs
using the XtResource_Factory.
%endif

# ================================================================
# prep
# ================================================================

%prep
%setup -q -n ACE_wrappers

%if ! %skip_make

export ACE_ROOT=`pwd`
export TAO_ROOT=$ACE_ROOT/TAO
export CIAO_ROOT=$TAO_ROOT/CIAO

# patch0 and patch1 are applied a bit later

#patch5 -p 1
cat ${ACE_ROOT}/rpmbuild/ace-tao-orbsvcs-daemon.patch | patch -p 1

# don't use patch8 until we verify wether needed

# config.h
( cd $ACE_ROOT/ace
rm -f config.h
cp config-linux.h config.h
)

#patch0 -p 1
cat ${ACE_ROOT}/rpmbuild/ace-tao-config.patch | patch -p 1

# If ipv6 support is indicated insert some lines into the confi.h
# file, right before the ace/post.h include.
#
%if %{?_with_ipv6:1}%{!?_with_ipv6:0}
rm -f $ACE_ROOT/ace/config.h.tmp
awk 'BEGIN { FLG = 1 }; /ace\/post/ { FLG = 0 }; { if (FLG==1) print }' \
$ACE_ROOT/ace/config.h >> $ACE_ROOT/ace/config.h.tmp
cat >> $ACE_ROOT/ace/config.h.tmp <<"EOF"
#define ACE_HAS_IPV6 // Ken Sedgwick 2006-06-14
#define ACE_USES_IPV4_IPV6_MIGRATION // Ken Sedgwick 2006-06-14
EOF
awk 'BEGIN { FLG = 0 }; /ace\/post/ { FLG = 1 }; { if (FLG==1) print }' \
$ACE_ROOT/ace/config.h >> $ACE_ROOT/ace/config.h.tmp
mv $ACE_ROOT/ace/config.h.tmp $ACE_ROOT/ace/config.h
%endif

# If rnq support is indicated insert some lines into the confi.h
# file, right before the ace/post.h include.
#
%if %{?_with_rnq:1}%{!?_with_rnq:0}
# Insert into the confi.h file, right before the ace/post.h include.
rm -f $ACE_ROOT/ace/config.h.tmp
awk 'BEGIN { FLG = 1 }; /ace\/post/ { FLG = 0 }; { if (FLG==1) print }' \
$ACE_ROOT/ace/config.h >> $ACE_ROOT/ace/config.h.tmp
cat >> $ACE_ROOT/ace/config.h.tmp <<"EOF"
#define ACE_HAS_REACTOR_NOTIFICATION_QUEUE  // Ken Sedgwick 2006-04-19
EOF
awk 'BEGIN { FLG = 0 }; /ace\/post/ { FLG = 1 }; { if (FLG==1) print }' \
$ACE_ROOT/ace/config.h >> $ACE_ROOT/ace/config.h.tmp
mv $ACE_ROOT/ace/config.h.tmp $ACE_ROOT/ace/config.h
%endif

# For template instantiation visibility and icmp support insert some
# lines into the confi.h file, right before the ace/post.h include.
#
rm -f $ACE_ROOT/ace/config.h.tmp
awk 'BEGIN { FLG = 1 }; /ace\/post/ { FLG = 0 }; { if (FLG==1) print }' \
$ACE_ROOT/ace/config.h >> $ACE_ROOT/ace/config.h.tmp
cat >> $ACE_ROOT/ace/config.h.tmp <<"EOF"
#define ACE_GCC_HAS_TEMPLATE_INSTANTIATION_VISIBILITY_ATTRS 1  // Ken Sedgwick 2007-05-05
#define ACE_HAS_ICMP_SUPPORT 1 // Ken Sedgwick 2006-09-05
EOF
awk 'BEGIN { FLG = 0 }; /ace\/post/ { FLG = 1 }; { if (FLG==1) print }' \
$ACE_ROOT/ace/config.h >> $ACE_ROOT/ace/config.h.tmp
mv $ACE_ROOT/ace/config.h.tmp $ACE_ROOT/ace/config.h

# platform_macros.GNU
cat > $ACE_ROOT/include/makeinclude/platform_macros.GNU <<EOF
ssl = 1
include \$(ACE_ROOT)/include/makeinclude/platform_linux.GNU
EOF

%if %{?_with_guilibs:1}%{!?_with_guilibs:0}
cat >> $ACE_ROOT/include/makeinclude/platform_macros.GNU <<EOF
x11 = 1
xt = 1
tk = 1
fl = 1
qt = 1
gl = 1
ace_qtreactor = 1
ace_xtreactor = 1
ace_tkreactor = 1
ace_flreactor = 1
tao_qtresource = 1
tao_xtresource = 1
tao_tkresource = 1
tao_flresource = 1
EOF
%endif

# 64 bit machines need additional macro.
%ifarch x86_64 ia64 ppc64 s390x
cat >> $ACE_ROOT/include/makeinclude/platform_macros.GNU <<EOF
buildbits = 64
EOF
%endif

%if %{?_with_opt:0}%{!?_with_opt:1}
cat >> $ACE_ROOT/include/makeinclude/platform_macros.GNU <<EOF
# Disable optimization
OCFLAGS = -O0
OCCFLAGS = -O0
EOF
%else
cat >> $ACE_ROOT/include/makeinclude/platform_macros.GNU <<EOF
# Enable Fedora optimization
OCFLAGS = %{optflags}
OCCFLAGS = %{optflags}
EOF
%endif

cat > $ACE_ROOT/bin/MakeProjectCreator/config/default.features <<EOF
ssl=1
qos=0
cidl=0
rwho=0
sctp=0
EOF

# We don't use default.features to enable ipv6 cause it conflicts w/
# the config.h generated version.  Config.h is superior because it is
# shipped to the end-user machines and also defines
# ACE_USES_IPV4_IPV6_MIGRATION which the default.features technique
# does not seem to set.

# Read: true if _with_guilibs is defined, false if not defined.
%if %{?_with_guilibs:1}%{!?_with_guilibs:0}
cat >> $ACE_ROOT/bin/MakeProjectCreator/config/default.features <<EOF
motif=1
athena=1
x11=1
xt=1
tk=1
fl=1
qt=1
gl=1
ace_qtreactor=1
ace_xtreactor=1
ace_tkreactor=1
ace_flreactor=1
tao_qtresource=1
tao_xtresource=1
tao_tkresource=1
tao_flresource=1
EOF
%endif

# Need to regenerate all of the Makefiles ...
(cd $ACE_ROOT && $ACE_ROOT/bin/mwc.pl -type gnuace)

# Fix source file permissions
find $TAO_ROOT/orbsvcs/FT_ReplicationManager -type f -exec chmod a-x {} \;
find $TAO_ROOT/orbsvcs/orbsvcs/FaultTolerance -type f -exec chmod a-x {} \;

%endif

# ================================================================
# build
# ================================================================

%build

export ACE_ROOT=`pwd`
export TAO_ROOT=$ACE_ROOT/TAO
export LD_LIBRARY_PATH=$ACE_ROOT/lib

%if %skip_make
cd .. && rm -rf ACE_wrappers && ln -s ACE_wrappers-BUILT ACE_wrappers
%else

MAKECMD="make %{?_smp_mflags}"

# build ACE components
for ace_comp in \
    ace \
    Kokyu \
    ACEXML \
    apps/gperf \
    protocols \
    websvcs;
do
    $MAKECMD -C $ACE_ROOT/$ace_comp;
done

# build TAO components
$MAKECMD -C $TAO_ROOT/TAO_IDL
$MAKECMD -C $TAO_ROOT/tao

# Instead of "$MAKECMD -C $TAO_ROOT/orbsvcs" use the list from
# $ACE_ROOT/orbsvcs/GNUmakefile less the performance-tests, tests and
# examples.
for orbsvcs_comp in \
    TAO_Service \
    orbsvcs \
    Trading_Service \
    Time_Service \
    Scheduling_Service \
    Notify_Service \
    Naming_Service \
    Logging_Service \
    LoadBalancer \
    LifeCycle_Service \
    ImplRepo_Service \
    IFR_Service \
    Fault_Notifier \
    Fault_Detector \
    FT_ReplicationManager \
    FTRT_Event_Service \
    Event_Service \
    Dump_Schedule \
    CosEvent_Service \
    Concurrency_Service;
do
    $MAKECMD -C $TAO_ROOT/orbsvcs/$orbsvcs_comp;
done

$MAKECMD -C $TAO_ROOT/utils

%endif

# ================================================================
# install
# ================================================================

# For major releases the package version will be the shortened version
# tuple and the shared-object version needs a placeholder '.0'
%if %is_major_ver
%define ACEVERSO %{ACEVER}.0
%define TAOVERSO %{TAOVER}.0
%define CIAOVERSO %{CIAOVER}.0
%else
%define ACEVERSO %{ACEVER}
%define TAOVERSO %{TAOVER}
%define CIAOVERSO %{CIAOVER}
%endif

%install

export ACE_ROOT=`pwd`
export TAO_ROOT=$ACE_ROOT/TAO

%if ! %skip_make
# cat %{SOURCE2} | patch -p 1
cat ${ACE_ROOT}/rpmbuild/ace-tao-macros.patch | patch -p 1
%endif

rm -rf $RPM_BUILD_ROOT

# make a new build root dir
mkdir -p $RPM_BUILD_ROOT

# ---------------- Runtime Components ----------------

# install shared libraries
install -d $RPM_BUILD_ROOT%{_libdir}

# ACE + XML libraries
INSTLIBS=`ls ${ACE_ROOT}/lib/libACE*.so.%{ACEVERSO}`
install $INSTLIBS $RPM_BUILD_ROOT%{_libdir}

# ACE-Kokyu libraries
INSTLIBS=`ls ${ACE_ROOT}/lib/libKokyu.so.%{ACEVERSO}`
install $INSTLIBS $RPM_BUILD_ROOT%{_libdir}

# TAO libraries
INSTLIBS=`ls ${ACE_ROOT}/lib/libTAO*.so.%{TAOVERSO}`
install $INSTLIBS $RPM_BUILD_ROOT%{_libdir}

# Create un-versioned symbolic links for libraries
(cd $RPM_BUILD_ROOT%{_libdir} && \
 ls *.so.* | awk 'BEGIN{FS="."}{print "ln -sf " $0 " " $1 "." $2;}' | sh)

# install binaries
install -d $RPM_BUILD_ROOT%{_sbindir}

# Rename the service binaries:

install ${ACE_ROOT}/TAO/orbsvcs/Naming_Service/Naming_Service \
    $RPM_BUILD_ROOT%{_sbindir}/tao-cosnaming

install ${ACE_ROOT}/TAO/orbsvcs/CosEvent_Service/CosEvent_Service \
    $RPM_BUILD_ROOT%{_sbindir}/tao-cosevent

install ${ACE_ROOT}/TAO/orbsvcs/Notify_Service/Notify_Service \
    $RPM_BUILD_ROOT%{_sbindir}/tao-cosnotification

install ${ACE_ROOT}/TAO/orbsvcs/Trading_Service/Trading_Service \
    $RPM_BUILD_ROOT%{_sbindir}/tao-costrading

install ${ACE_ROOT}/TAO/orbsvcs/Event_Service/Event_Service \
    $RPM_BUILD_ROOT%{_sbindir}/tao-rtevent

install ${ACE_ROOT}/TAO/orbsvcs/Concurrency_Service/Concurrency_Service \
    $RPM_BUILD_ROOT%{_sbindir}/tao-cosconcurrency

# Create the tao cache directory.
mkdir -p $RPM_BUILD_ROOT%{_localstatedir}/cache/tao

# Create data files which will be ghosted.
touch $RPM_BUILD_ROOT%{_localstatedir}/cache/tao/tao-cosnaming.dat

# Create the tao log directory.
mkdir -p $RPM_BUILD_ROOT%{_localstatedir}/log/tao

# Create data files which will be ghosted.
touch $RPM_BUILD_ROOT%{_localstatedir}/log/tao/tao-cosnaming.log
touch $RPM_BUILD_ROOT%{_localstatedir}/log/tao/tao-cosconcurrency.log
touch $RPM_BUILD_ROOT%{_localstatedir}/log/tao/tao-cosevent.log
touch $RPM_BUILD_ROOT%{_localstatedir}/log/tao/tao-cosnotification.log
touch $RPM_BUILD_ROOT%{_localstatedir}/log/tao/tao-costrading.log
touch $RPM_BUILD_ROOT%{_localstatedir}/log/tao/tao-rtevent.log

# ---------------- Development Components ----------------

# INSTHDR="cp --preserve=timestamps"
INSTHDR="install -m 0644 -p"

# install headers
install -d $RPM_BUILD_ROOT%{_includedir}
( set +x
echo "Building list of headers..."

# Generate raw dependency output
BASEHDR=`find \
    ace \
    ACEXML/common \
    ACEXML/parser/parser \
    Kokyu \
    TAO/tao \
    TAO/orbsvcs/orbsvcs \
    -name '*.h' -not -name 'config-*'`
for j in $BASEHDR; do
        echo $j >> rawhdrs.log
        echo '#include <'$j'>' | \
        g++ -I . \
            -I protocols \
            -I TAO \
            -I TAO/orbsvcs \
            -I TAO/orbsvcs/orbsvcs \
            -x c++ - -MM -MF mmout 2>> rawhdrs.log && cat mmout || true;
done > mmraw.list

# Append IDL headers to the raw list.
find \
    TAO/tao \
    TAO/orbsvcs/orbsvcs \
    -regex '.*\.p?idl$' >> mmraw.list

# Cleanup dependency output:
#   remove '-:' sequences
#   change all sequences of whitespace into \n
#   remove leading './'
#   cannonicalize up to two levels of '/../../'
#   remove duplicates
cat mmraw.list |\
        sed -e 's/^-://g' -e 's/\\//g' | \
        tr -s [:space:] "\n" | \
        sed -e 's#^./##g' | \
        sed -e 's#/[^/]\+/\.\./#/#g' -e 's#/[^/]\+/\.\./#/#g' | \
        sort -u > allhdrs.list

# Add missing headers.
echo ace/QtReactor/QtReactor.h >> allhdrs.list
echo TAO/tao/QtResource/QtResource_Factory.h >> allhdrs.list
echo TAO/tao/QtResource/QtResource_Loader.h >> allhdrs.list

# Install headers and create header lists
rm -f ace-headers.tmp
rm -f acexml-headers.tmp
rm -f kokyu-headers.tmp
rm -f tao-headers.tmp

for i in `cat allhdrs.list`; do
    case "$i" in
    protocols/ace/*)
        mkdir -p `dirname $RPM_BUILD_ROOT%{_includedir}/${i/protocols\/}`
        $INSTHDR $i $RPM_BUILD_ROOT%{_includedir}/${i/protocols/}
        echo '%dir %{_includedir}/'`dirname ${i/protocols/}` >> ace-headers.tmp
        echo '%{_includedir}/'${i/protocols/} >> ace-headers.tmp
        ;;
    ace/*)
        mkdir -p `dirname $RPM_BUILD_ROOT%{_includedir}/$i`
        $INSTHDR $i $RPM_BUILD_ROOT%{_includedir}/$i
        echo '%dir %{_includedir}/'`dirname $i` >> ace-headers.tmp
        echo '%{_includedir}/'$i >> ace-headers.tmp
        ;;
    ACEXML/*)
        mkdir -p `dirname $RPM_BUILD_ROOT%{_includedir}/$i`
        $INSTHDR $i $RPM_BUILD_ROOT%{_includedir}/$i
        echo '%dir %{_includedir}/'`dirname $i` >> acexml-headers.tmp
        echo '%{_includedir}/'$i >> acexml-headers.tmp
        ;;
    Kokyu/*)
        mkdir -p `dirname $RPM_BUILD_ROOT%{_includedir}/$i`
        $INSTHDR $i $RPM_BUILD_ROOT%{_includedir}/$i
        echo '%dir %{_includedir}/'`dirname $i` >> kokyu-headers.tmp
        echo '%{_includedir}/'$i >> kokyu-headers.tmp
        ;;
    TAO/tao/*)
        mkdir -p `dirname $RPM_BUILD_ROOT%{_includedir}/${i/TAO\/}`
        $INSTHDR $i $RPM_BUILD_ROOT%{_includedir}/${i/TAO\/}
        echo '%dir %{_includedir}/'`dirname ${i/TAO\/}` >> tao-headers.tmp
        echo '%{_includedir}/'${i/TAO\/} >> tao-headers.tmp
        ;;
    TAO/orbsvcs/orbsvcs/*)
        mkdir -p `dirname $RPM_BUILD_ROOT%{_includedir}/${i/TAO\/orbsvcs\/}`
        $INSTHDR $i $RPM_BUILD_ROOT%{_includedir}/${i/TAO\/orbsvcs\/}
        echo '%dir %{_includedir}/'`dirname ${i/TAO\/orbsvcs\/}` >> tao-headers.tmp
        echo '%{_includedir}/'${i/TAO\/orbsvcs\/} >> tao-headers.tmp
        ;;
    *)
        # mkdir -p `dirname $RPM_BUILD_ROOT%{_includedir}/$i`
        # $INSTHDR $i $RPM_BUILD_ROOT%{_includedir}/$i
        echo $i
        ;;
    esac
done

echo '%defattr(-,root,root,-)' > ace-headers.list
sort -u < ace-headers.tmp >> ace-headers.list
rm -f ace-headers.tmp

echo '%defattr(-,root,root,-)' > acexml-headers.list
sort -u < acexml-headers.tmp >> acexml-headers.list
rm -f acexml-headers.tmp

echo '%defattr(-,root,root,-)' > kokyu-headers.list
sort -u < kokyu-headers.tmp >> kokyu-headers.list
rm -f kokyu-headers.tmp

echo '%defattr(-,root,root,-)' > tao-headers.list
sort -u < tao-headers.tmp >> tao-headers.list
rm -f tao-headers.tmp
)

# install the TAO_IDL compiler
install -d $RPM_BUILD_ROOT%{_libdir}

# NOTE - it appears that when TAO's build copies the files to the
# ACE_ROOT/ace directory they get versioned with ACE's version number
# rather then TAO's.  Use the ACEVERSO macro for now ...
install \
        ${ACE_ROOT}/lib/libTAO_IDL_BE.so.%{ACEVERSO} \
        ${ACE_ROOT}/lib/libTAO_IDL_FE.so.%{ACEVERSO} \
        $RPM_BUILD_ROOT%{_libdir}

(cd $RPM_BUILD_ROOT%{_libdir} && ln -sf libTAO_IDL_BE.so.%{ACEVERSO} libTAO_IDL_BE.so)
(cd $RPM_BUILD_ROOT%{_libdir} && ln -sf libTAO_IDL_FE.so.%{ACEVERSO} libTAO_IDL_FE.so)

install -d $RPM_BUILD_ROOT%{_bindir}
install ${ACE_ROOT}/bin/ace_gperf $RPM_BUILD_ROOT%{_bindir}
install ${ACE_ROOT}/bin/tao_idl $RPM_BUILD_ROOT%{_bindir}
install ${ACE_ROOT}/bin/tao_imr $RPM_BUILD_ROOT%{_bindir}
install ${ACE_ROOT}/bin/tao_ifr $RPM_BUILD_ROOT%{_bindir}
install ${ACE_ROOT}/bin/catior $RPM_BUILD_ROOT%{_bindir}/tao_catior
install ${ACE_ROOT}/bin/nsadd $RPM_BUILD_ROOT%{_bindir}/tao_nsadd
install ${ACE_ROOT}/bin/nsdel $RPM_BUILD_ROOT%{_bindir}/tao_nsdel
install ${ACE_ROOT}/bin/nslist $RPM_BUILD_ROOT%{_bindir}/tao_nslist

# ================================================================
# Config & Options
# ================================================================

install -d $RPM_BUILD_ROOT%{_sysconfdir}
tar -xzvf ${ACE_ROOT}/rpmbuild/ace-tao-etc.tar.gz \
	-C $RPM_BUILD_ROOT%{_sysconfdir}

## # FIXME - namespace issues need to be resolved
##
## # ================================================================
## # Makefiles
## # ================================================================
##
## install -d $RPM_BUILD_ROOT%{_includedir}/makeinclude
##
## for mk_macros in \
##     all_in_one.GNU \
##     component_check.GNU \
##     macros.GNU \
##     platform_g++_common.GNU \
##     platform_linux.GNU \
##     platform_macros.GNU \
##     rules.bin.GNU \
##     rules.common.GNU \
##     rules.lib.GNU \
##     rules.local.GNU \
##     rules.nested.GNU \
##     rules.nolocal.GNU \
##     rules.nonested.GNU \
##     wrapper_macros.GNU; do (
## install ${ACE_ROOT}/include/makeinclude/$mk_macros $RPM_BUILD_ROOT%{_includedir}/makeinclude
## ); done
##
## install ${TAO_ROOT}/rules.tao.GNU $RPM_BUILD_ROOT%{_includedir}/makeinclude
##
## install -d $RPM_BUILD_ROOT%{_datadir}
## install -d $RPM_BUILD_ROOT%{_datadir}/ace
## install -d $RPM_BUILD_ROOT%{_datadir}/ace/MPC
## cp -a ${ACE_ROOT}/MPC $RPM_BUILD_ROOT%{_datadir}/ace/
##
## install -d $RPM_BUILD_ROOT%{_datadir}/ace/bin
## cp -a ${ACE_ROOT}/bin/DependencyGenerator $RPM_BUILD_ROOT%{_datadir}/ace/bin
## cp -a ${ACE_ROOT}/bin/MakeProjectCreator $RPM_BUILD_ROOT%{_datadir}/ace/bin
## install -d $RPM_BUILD_ROOT%{_datadir}/ace/bin/PerlACE
## install ${ACE_ROOT}/bin/PerlACE/{ConfigList,Process,Process_Unix}.pm $RPM_BUILD_ROOT%{_datadir}/ace/bin/PerlACE
## install ${ACE_ROOT}/bin/mpc.pl $RPM_BUILD_ROOT%{_datadir}/ace/bin
## install ${ACE_ROOT}/bin/mwc.pl $RPM_BUILD_ROOT%{_datadir}/ace/bin
## install ${ACE_ROOT}/bin/g++dep $RPM_BUILD_ROOT%{_datadir}/ace/bin
## install ${ACE_ROOT}/bin/depgen.pl $RPM_BUILD_ROOT%{_datadir}/ace/bin
## install ${ACE_ROOT}/bin/generate_export_file.pl $RPM_BUILD_ROOT%{_datadir}/ace/bin
## install ${ACE_ROOT}/bin/{ACEutils,Process,Process_Unix,Uniqueid}.pm $RPM_BUILD_ROOT%{_datadir}/ace/bin

rm -rf $RPM_BUILD_ROOT%{_sysconfdir}/profile.d

# ================================================================
# Manuals
# ================================================================
install -d $RPM_BUILD_ROOT%{_mandir}
install -d $RPM_BUILD_ROOT%{_mandir}/man1
install ${TAO_ROOT}/TAO_IDL/tao_idl.1 $RPM_BUILD_ROOT%{_mandir}/man1
install ${ACE_ROOT}/apps/gperf/ace_gperf.1 $RPM_BUILD_ROOT%{_mandir}/man1
install -d  $RPM_BUILD_ROOT%{_infodir}
install ${ACE_ROOT}/apps/gperf/ace_gperf.info $RPM_BUILD_ROOT%{_infodir}

# ================================================================
# Create lists of symlinked so's.  We need two lists because we need
# the unversioned symlinks in the runtime package for so's that can
# be loaded in the svc.conf.
# ================================================================

# Make a list of all shared objects.
(cd $RPM_BUILD_ROOT/%{_libdir} && ls *.so | \
        awk '{ print "%{_libdir}/"$1; }' | \
        sort) > all-so.list

# Make a list of likely svc.conf targets.
(cd $RPM_BUILD_ROOT/%{_libdir} && ls *.so | \
    nm --print-file-name *.so | \
    grep _make_ | \
    awk 'BEGIN { FS=":"} /^[^:]+:/ { print "%{_libdir}/"$1; }' | \
    sort -u) > rough-svc-so.list

# Remove false positives (IMPORTANT keep this list sorted!)
cat > falsepos-svc-so.list <<EOF
%{_libdir}/libACE.so
%{_libdir}/libTAO.so
EOF
comm -2 -3 rough-svc-so.list falsepos-svc-so.list > svc-so.list

# Find the list of non-sv.conf target files.
comm -2 -3 all-so.list svc-so.list > nonsvc-so.list

# Generate file lists.
grep libACE svc-so.list > ace-svc-so.list
grep libACE nonsvc-so.list > ace-nonsvc-so.list
grep libTAO svc-so.list > tao-svc-so.list
grep libTAO nonsvc-so.list > tao-nonsvc-so.list

# Concatenate file lists as neccessary
cat tao-headers.list tao-nonsvc-so.list > tao-devel-files.list

# ================================================================
# clean
# ================================================================

%clean
rm -rf $RPM_BUILD_ROOT

# ================================================================
# pre install
# ================================================================

# ---------------- tao-cosnaming ----------------

%pre -n tao-cosnaming

getent group tao >/dev/null || /usr/sbin/groupadd -r tao
getent passwd tao >/dev/null || \
/usr/sbin/useradd -r -g tao -d %{_sysconfdir}/tao -s /sbin/nologin \
    -c "TAO Services" tao
exit 0

# ---------------- tao-cosevent ----------------

%pre -n tao-cosevent

getent group tao >/dev/null || /usr/sbin/groupadd -r tao
getent passwd tao >/dev/null || \
/usr/sbin/useradd -r -g tao -d %{_sysconfdir}/tao -s /sbin/nologin \
    -c "TAO Services" tao
exit 0

# ---------------- tao-cosnotification ----------------

%pre -n tao-cosnotification

getent group tao >/dev/null || /usr/sbin/groupadd -r tao
getent passwd tao >/dev/null || \
/usr/sbin/useradd -r -g tao -d %{_sysconfdir}/tao -s /sbin/nologin \
    -c "TAO Services" tao
exit 0

# ---------------- tao-costrading ----------------

%pre -n tao-costrading

getent group tao >/dev/null || /usr/sbin/groupadd -r tao
getent passwd tao >/dev/null || \
/usr/sbin/useradd -r -g tao -d %{_sysconfdir}/tao -s /sbin/nologin \
    -c "TAO Services" tao
exit 0

# ---------------- tao-rtevent ----------------

%pre -n tao-rtevent

getent group tao >/dev/null || /usr/sbin/groupadd -r tao
getent passwd tao >/dev/null || \
/usr/sbin/useradd -r -g tao -d %{_sysconfdir}/tao -s /sbin/nologin \
    -c "TAO Services" tao
exit 0

# ---------------- tao-cosconcurrency ----------------

%pre -n tao-cosconcurrency

getent group tao >/dev/null || /usr/sbin/groupadd -r tao
getent passwd tao >/dev/null || \
/usr/sbin/useradd -r -g tao -d %{_sysconfdir}/tao -s /sbin/nologin \
    -c "TAO Services" tao
exit 0

# ================================================================
# post install
# ================================================================

# ---------------- ace ----------------

%post -n ace

/sbin/ldconfig

# ---------------- ace-devel ----------------

%post -n ace-devel

/sbin/install-info %{_infodir}/ace_gperf.info%{_extension} %{_infodir}/dir

# ---------------- ace-xml ----------------

%post -n ace-xml

/sbin/ldconfig

# ---------------- ace-kokyu ----------------

%post -n ace-kokyu

/sbin/ldconfig

# Read: true if _with_guilibs is defined, false if not defined.
%if %{?_with_guilibs:1}%{!?_with_guilibs:0}
# ---------------- ace-flreactor ----------------

%post -n ace-flreactor

/sbin/ldconfig

# ---------------- ace-qtreactor ----------------

%post -n ace-qtreactor

/sbin/ldconfig

# ---------------- ace-tkreactor ----------------

%post -n ace-tkreactor

/sbin/ldconfig

# ---------------- ace-xtreactor ----------------

%post -n ace-xtreactor

/sbin/ldconfig
%endif

# ---------------- tao ----------------

%post -n tao

/sbin/ldconfig

# ---------------- tao-devel ----------------

%post -n tao-devel

/sbin/ldconfig

# ---------------- tao-cosnaming ----------------

%post -n tao-cosnaming

/sbin/chkconfig --add tao-cosnaming

# ---------------- tao-cosevent ----------------

%post -n tao-cosevent

/sbin/chkconfig --add tao-cosevent

# ---------------- tao-cosnotification ----------------

%post -n tao-cosnotification

/sbin/chkconfig --add tao-cosnotification

# ---------------- tao-costrading ----------------

%post -n tao-costrading

/sbin/chkconfig --add tao-costrading

# ---------------- tao-rtevent ----------------

%post -n tao-rtevent

/sbin/chkconfig --add tao-rtevent

# ---------------- tao-cosconcurrency ----------------

%post -n tao-cosconcurrency

/sbin/chkconfig --add tao-cosconcurrency

# Read: true if _with_guilibs is defined, false if not defined.
%if %{?_with_guilibs:1}%{!?_with_guilibs:0}
# ---------------- tao-flresource ----------------

%post -n tao-flresource

/sbin/ldconfig

# ---------------- tao-qtresource ----------------

%post -n tao-qtresource

/sbin/ldconfig

# ---------------- tao-tkresource ----------------

%post -n tao-tkresource

/sbin/ldconfig

# ---------------- tao-xtresource ----------------

%post -n tao-xtresource

/sbin/ldconfig
%endif

# ================================================================
# pre uninstall
# ================================================================

# ---------------- ace-devel ----------------

%preun -n ace-devel

if [ $1 = 0 ]; then
    /sbin/install-info --delete %{_infodir}/ace_gperf.info.gz %{_infodir}/dir
fi

# ---------------- tao-cosnaming ----------------

%preun -n tao-cosnaming

if [ $1 = 0 ]; then
    /sbin/service tao-cosnaming stop > /dev/null 2>&1
    /sbin/chkconfig --del tao-cosnaming
fi

# ---------------- tao-cosevent ----------------

%preun -n tao-cosevent

if [ $1 = 0 ]; then
    /sbin/service tao-cosevent stop > /dev/null 2>&1
    /sbin/chkconfig --del tao-cosevent
fi

# ---------------- tao-cosnotification ----------------

%preun -n tao-cosnotification

if [ $1 = 0 ]; then
    /sbin/service tao-cosnotification stop > /dev/null 2>&1
    /sbin/chkconfig --del tao-cosnotification
fi

# ---------------- tao-costrading ----------------

%preun -n tao-costrading

if [ $1 = 0 ]; then
    /sbin/service tao-costrading stop > /dev/null 2>&1
    /sbin/chkconfig --del tao-costrading
fi

# ---------------- tao-rtevent ----------------

%preun -n tao-rtevent

if [ $1 = 0 ]; then
    /sbin/service tao-rtevent stop > /dev/null 2>&1
    /sbin/chkconfig --del tao-rtevent
fi

# ---------------- tao-cosconcurrency ----------------

%preun -n tao-cosconcurrency

if [ $1 = 0 ]; then
    /sbin/service tao-cosconcurrency stop > /dev/null 2>&1
    /sbin/chkconfig --del tao-cosconcurrency
fi

# ================================================================
# post uninstall
# ================================================================

# ---------------- ace ----------------

%postun -n ace

/sbin/ldconfig

# ---------------- ace-xml ----------------

%postun -n ace-xml

/sbin/ldconfig

# ---------------- ace-kokyu ----------------

%postun -n ace-kokyu

/sbin/ldconfig

# Read: true if _with_guilibs is defined, false if not defined.
%if %{?_with_guilibs:1}%{!?_with_guilibs:0}
# ---------------- ace-flreactor ----------------

%postun -n ace-flreactor

/sbin/ldconfig

# ---------------- ace-qtreactor ----------------

%postun -n ace-qtreactor

/sbin/ldconfig

# ---------------- ace-tkreactor ----------------

%postun -n ace-tkreactor

/sbin/ldconfig

# ---------------- ace-xtreactor ----------------

%postun -n ace-xtreactor

/sbin/ldconfig
%endif

# ---------------- tao ----------------

%postun -n tao

/sbin/ldconfig

# ---------------- tao-devel ----------------

%postun -n tao-devel

/sbin/ldconfig

# ---------------- tao-cosnaming ----------------

%postun -n tao-cosnaming

if [ "$1" -ge "1" ]; then
    /sbin/service tao-cosnaming condrestart > /dev/null 2>&1
fi

# ---------------- tao-cosevent ----------------

%postun -n tao-cosevent

if [ "$1" -ge "1" ]; then
    /sbin/service tao-cosevent condrestart > /dev/null 2>&1
fi

# ---------------- tao-cosnotification ----------------

%postun -n tao-cosnotification

if [ "$1" -ge "1" ]; then
    /sbin/service tao-cosnotification condrestart > /dev/null 2>&1
fi

# ---------------- tao-costrading ----------------

%postun -n tao-costrading

if [ "$1" -ge "1" ]; then
    /sbin/service tao-costrading condrestart > /dev/null 2>&1
fi

# ---------------- tao-rtevent ----------------

%postun -n tao-rtevent

if [ "$1" -ge "1" ]; then
    /sbin/service tao-rtevent condrestart > /dev/null 2>&1
fi

# ---------------- tao-cosconcurrency ----------------

%postun -n tao-cosconcurrency

if [ "$1" -ge "1" ]; then
    /sbin/service tao-cosconcurrency condrestart > /dev/null 2>&1
fi

# Read: true if _with_guilibs is defined, false if not defined.
%if %{?_with_guilibs:1}%{!?_with_guilibs:0}
# ---------------- tao-flresource ----------------

%postun -n tao-flresource

/sbin/ldconfig

# ---------------- tao-qtresource ----------------

%postun -n tao-qtresource

/sbin/ldconfig

# ---------------- tao-tkresource ----------------

%postun -n tao-tkresource

/sbin/ldconfig

# ---------------- tao-xtresource ----------------

%postun -n tao-xtresource

/sbin/ldconfig
%endif

# ================================================================
# files
# ================================================================

# ---------------- ace ----------------

%files -n ace
%defattr(-,root,root,-)
%{_libdir}/libACE.so.%{ACEVERSO}
%{_libdir}/libACE_ETCL_Parser.so.%{ACEVERSO}
%{_libdir}/libACE_ETCL.so.%{ACEVERSO}
%{_libdir}/libACE_HTBP.so.%{ACEVERSO}
%{_libdir}/libACE_Monitor_Control.so.%{ACEVERSO}
%{_libdir}/libACE_RMCast.so.%{ACEVERSO}
%{_libdir}/libACE_SSL.so.%{ACEVERSO}
%{_libdir}/libACE_TMCast.so.%{ACEVERSO}

%doc ACE-INSTALL.html
%doc AUTHORS
%doc COPYING
%doc FAQ
%doc PROBLEM-REPORT-FORM
%doc README
%doc THANKS
%doc VERSION

# ---------------- ace-devel ----------------

%files -n ace-devel -f ace-headers.list
%defattr(-,root,root,-)
%{_bindir}/ace_gperf
%{_libdir}/libACE.so
%{_libdir}/libACE_ETCL_Parser.so
%{_libdir}/libACE_ETCL.so
%{_libdir}/libACE_HTBP.so
%{_libdir}/libACE_Monitor_Control.so
%{_libdir}/libACE_RMCast.so
%{_libdir}/libACE_SSL.so
%{_libdir}/libACE_TMCast.so
%attr(0644,root,root) %{_mandir}/man1/ace_gperf.1%{_extension}
%attr(0644,root,root) %{_infodir}/ace_gperf.info%{_extension}

%if %{?_with_guilibs:1}%{!?_with_guilibs:0}
%exclude %{_includedir}/ace/FlReactor/FlReactor.h
%exclude %{_includedir}/ace/FlReactor/ACE_FlReactor_export.h
%exclude %{_includedir}/ace/QtReactor/QtReactor.h
%exclude %{_includedir}/ace/QtReactor/ACE_QtReactor_export.h
%exclude %{_includedir}/ace/TkReactor/TkReactor.h
%exclude %{_includedir}/ace/TkReactor/ACE_TkReactor_export.h
%exclude %{_includedir}/ace/XtReactor/XtReactor.h
%exclude %{_includedir}/ace/XtReactor/ACE_XtReactor_export.h
%endif

%doc AUTHORS
%doc COPYING
%doc PROBLEM-REPORT-FORM
%doc README
%doc VERSION

# ---------------- ace-xml ----------------

%files -n ace-xml
%defattr(-,root,root,-)
%{_libdir}/libACEXML*.so.%{ACEVERSO}

%doc AUTHORS
%doc COPYING
%doc PROBLEM-REPORT-FORM
%doc README
%doc VERSION

# ---------------- ace-xml-devel ----------------

%files -n ace-xml-devel -f acexml-headers.list
%defattr(-,root,root,-)
%{_libdir}/libACEXML*.so

# These get missed by the automatic list generator because they
# contain no immediate files.
%dir %{_includedir}/ACEXML/parser
%dir %{_includedir}/ACEXML

%doc AUTHORS
%doc COPYING
%doc PROBLEM-REPORT-FORM
%doc README
%doc VERSION

# ---------------- ace-kokyu ----------------

%files -n ace-kokyu
%defattr(-,root,root,-)
%{_libdir}/libKokyu.so.%{ACEVERSO}

%doc AUTHORS
%doc COPYING
%doc PROBLEM-REPORT-FORM
%doc README
%doc VERSION

# ---------------- ace-kokyu-devel ----------------

%files -n ace-kokyu-devel -f kokyu-headers.list
%defattr(-,root,root,-)
%{_libdir}/libKokyu.so

%doc AUTHORS
%doc COPYING
%doc PROBLEM-REPORT-FORM
%doc README
%doc VERSION

# ---------------- ace-flreactor ----------------

# Read: true if _with_guilibs is defined, false if not defined.
%if %{?_with_guilibs:1}%{!?_with_guilibs:0}

%files -n ace-flreactor
%defattr(-,root,root,-)
%{_libdir}/libACE_FlReactor.so.%{ACEVERSO}

%doc AUTHORS
%doc COPYING
%doc PROBLEM-REPORT-FORM
%doc README
%doc VERSION

%endif

# ---------------- ace-flreactor-devel ----------------

# Read: true if _with_guilibs is defined, false if not defined.
%if %{?_with_guilibs:1}%{!?_with_guilibs:0}

%files -n ace-flreactor-devel
%defattr(-,root,root,-)
%dir %{_includedir}/ace/FlReactor
%{_libdir}/libACE_FlReactor.so
%{_includedir}/ace/FlReactor/FlReactor.h
%{_includedir}/ace/FlReactor/ACE_FlReactor_export.h

%doc AUTHORS
%doc COPYING
%doc PROBLEM-REPORT-FORM
%doc README
%doc VERSION

%endif

# ---------------- ace-qtreactor ----------------

# Read: true if _with_guilibs is defined, false if not defined.
%if %{?_with_guilibs:1}%{!?_with_guilibs:0}

%files -n ace-qtreactor
%defattr(-,root,root,-)
%{_libdir}/libACE_QtReactor.so.%{ACEVERSO}

%doc AUTHORS
%doc COPYING
%doc PROBLEM-REPORT-FORM
%doc README
%doc VERSION

%endif

# ---------------- ace-qtreactor-devel ----------------

# Read: true if _with_guilibs is defined, false if not defined.
%if %{?_with_guilibs:1}%{!?_with_guilibs:0}

%files -n ace-qtreactor-devel
%defattr(-,root,root,-)
%{_libdir}/libACE_QtReactor.so
%dir %{_includedir}/ace/QtReactor
%{_includedir}/ace/QtReactor/QtReactor.h
%{_includedir}/ace/QtReactor/ACE_QtReactor_export.h

%doc AUTHORS
%doc COPYING
%doc PROBLEM-REPORT-FORM
%doc README
%doc VERSION

%endif

# ---------------- ace-tkreactor ----------------

# Read: true if _with_guilibs is defined, false if not defined.
%if %{?_with_guilibs:1}%{!?_with_guilibs:0}

%files -n ace-tkreactor
%defattr(-,root,root,-)
%{_libdir}/libACE_TkReactor.so.%{ACEVERSO}

%doc AUTHORS
%doc COPYING
%doc PROBLEM-REPORT-FORM
%doc README
%doc VERSION

%endif

# ---------------- ace-tkreactor-devel ----------------

# Read: true if _with_guilibs is defined, false if not defined.
%if %{?_with_guilibs:1}%{!?_with_guilibs:0}

%files -n ace-tkreactor-devel
%defattr(-,root,root,-)
%{_libdir}/libACE_TkReactor.so
%dir %{_includedir}/ace/TkReactor
%{_includedir}/ace/TkReactor/TkReactor.h
%{_includedir}/ace/TkReactor/ACE_TkReactor_export.h

%doc AUTHORS
%doc COPYING
%doc PROBLEM-REPORT-FORM
%doc README
%doc VERSION

%endif

# ---------------- ace-xtreactor ----------------

# Read: true if _with_guilibs is defined, false if not defined.
%if %{?_with_guilibs:1}%{!?_with_guilibs:0}

%files -n ace-xtreactor
%defattr(-,root,root,-)
%{_libdir}/libACE_XtReactor.so.%{ACEVERSO}

%doc AUTHORS
%doc COPYING
%doc PROBLEM-REPORT-FORM
%doc README
%doc VERSION

%endif

# ---------------- ace-xtreactor-devel ----------------

# Read: true if _with_guilibs is defined, false if not defined.
%if %{?_with_guilibs:1}%{!?_with_guilibs:0}

%files -n ace-xtreactor-devel
%defattr(-,root,root,-)
%{_libdir}/libACE_XtReactor.so
%dir %{_includedir}/ace/XtReactor
%{_includedir}/ace/XtReactor/XtReactor.h
%{_includedir}/ace/XtReactor/ACE_XtReactor_export.h

%doc AUTHORS
%doc COPYING
%doc PROBLEM-REPORT-FORM
%doc README
%doc VERSION

%endif

# ---------------- tao ----------------

# NOTE - Some of the TAO service modules need to be found by dlopen at
# runtime.  Currently this means these specific .so files need to be
# shipped in the runtime package instead of the devel package.

%files -n tao -f tao-svc-so.list
%defattr(-,root,root,-)

%{_libdir}/libTAO*.so.%{TAOVERSO}

# The libTAO_IFR_BE needs the IDL front-end
%{_libdir}/libTAO_IDL_FE.so.%{ACEVERSO}

%exclude %{_libdir}/libTAO_IDL_BE*

%if %{?_with_guilibs:1}%{!?_with_guilibs:0}
%exclude %{_libdir}/libTAO_FlResource.so*
%exclude %{_libdir}/libTAO_QtResource.so*
%exclude %{_libdir}/libTAO_TkResource.so*
%exclude %{_libdir}/libTAO_XtResource.so*
%endif

%doc TAO/COPYING
%doc TAO/PROBLEM-REPORT-FORM
%doc TAO/TAO-INSTALL.html
%doc TAO/VERSION
%doc TAO/README

# ---------------- tao-devel ----------------

# NOTE - Some of the TAO service modules need to be found by dlopen at
# runtime.  Currently this means these specific .so files need to be
# shipped in the runtime package instead of the devel package.

%files -n tao-devel -f tao-devel-files.list
%defattr(-,root,root,-)

%{_libdir}/libTAO_IDL_BE.so.%{ACEVERSO}

%{_bindir}/tao_idl
%{_bindir}/tao_imr
%{_bindir}/tao_ifr
%{_bindir}/tao_catior
%{_bindir}/tao_nsadd
%{_bindir}/tao_nsdel
%{_bindir}/tao_nslist
%attr(0644,root,root) %{_mandir}/man1/tao_idl.1%{_extension}

# These get missed by the automatic list generator because they
# contain no immediate files.
%dir %{_includedir}/orbsvcs/FtRtEvent

%if %{?_with_guilibs:1}%{!?_with_guilibs:0}
%exclude %{_includedir}/tao/FlResource/FlResource_Factory.h
%exclude %{_includedir}/tao/FlResource/FlResource_Loader.h
%exclude %{_includedir}/tao/FlResource/TAO_FlResource_Export.h
%exclude %{_includedir}/tao/QtResource/QtResource_Factory.h
%exclude %{_includedir}/tao/QtResource/QtResource_Loader.h
%exclude %{_includedir}/tao/QtResource/TAO_QtResource_Export.h
%exclude %{_includedir}/tao/TkResource/TkResource_Factory.h
%exclude %{_includedir}/tao/TkResource/TkResource_Loader.h
%exclude %{_includedir}/tao/TkResource/TAO_TkResource_Export.h
%exclude %{_includedir}/tao/XtResource/XtResource_Factory.h
%exclude %{_includedir}/tao/XtResource/XtResource_Loader.h
%exclude %{_includedir}/tao/XtResource/TAO_XtResource_Export.h
%exclude %{_libdir}/libTAO_FlResource.so
%exclude %{_libdir}/libTAO_QtResource.so
%exclude %{_libdir}/libTAO_TkResource.so
%exclude %{_libdir}/libTAO_XtResource.so
%endif

%doc TAO/COPYING
%doc TAO/PROBLEM-REPORT-FORM
%doc TAO/VERSION
%doc TAO/README

# ---------------- tao-cosnaming ----------------

%files -n tao-cosnaming
%defattr(-,root,root,-)

%{_sbindir}/tao-cosnaming
%{_sysconfdir}/rc.d/init.d/tao-cosnaming
%dir %{_sysconfdir}/tao
%config(noreplace) %{_sysconfdir}/tao/tao-cosnaming.opt
%config(noreplace) %{_sysconfdir}/tao/tao-cosnaming.conf
%config(noreplace) %{_sysconfdir}/logrotate.d/tao-cosnaming
%attr(-,tao,tao) %dir %{_localstatedir}/cache/tao
%attr(0644,tao,tao) %ghost %{_localstatedir}/cache/tao/tao-cosnaming.dat
%attr(-,tao,tao) %dir %{_localstatedir}/log/tao
%attr(0644,tao,tao) %ghost %{_localstatedir}/log/tao/tao-cosnaming.log

%doc TAO/COPYING
%doc TAO/PROBLEM-REPORT-FORM
%doc TAO/VERSION
%doc TAO/README

# ---------------- tao-cosevent ----------------

%files -n tao-cosevent
%defattr(-,root,root,-)

%{_sbindir}/tao-cosevent
%{_sysconfdir}/rc.d/init.d/tao-cosevent
%dir %{_sysconfdir}/tao
%config(noreplace) %{_sysconfdir}/tao/tao-cosevent.opt
%config(noreplace) %{_sysconfdir}/tao/tao-cosevent.conf
%config(noreplace) %{_sysconfdir}/logrotate.d/tao-cosevent
%attr(-,tao,tao) %dir %{_localstatedir}/log/tao
%attr(0644,tao,tao) %ghost %{_localstatedir}/log/tao/tao-cosevent.log

%doc TAO/COPYING
%doc TAO/PROBLEM-REPORT-FORM
%doc TAO/VERSION
%doc TAO/README

# ---------------- tao-cosnotification ----------------

%files -n tao-cosnotification
%defattr(-,root,root,-)

%{_sbindir}/tao-cosnotification
%{_sysconfdir}/rc.d/init.d/tao-cosnotification
%dir %{_sysconfdir}/tao
%config(noreplace) %{_sysconfdir}/tao/tao-cosnotification.opt
%config(noreplace) %{_sysconfdir}/tao/tao-cosnotification.conf
%config(noreplace) %{_sysconfdir}/logrotate.d/tao-cosnotification
%attr(-,tao,tao) %dir %{_localstatedir}/log/tao
%attr(0644,tao,tao) %ghost %{_localstatedir}/log/tao/tao-cosnotification.log

%doc TAO/COPYING
%doc TAO/PROBLEM-REPORT-FORM
%doc TAO/VERSION
%doc TAO/README

# ---------------- tao-costrading ----------------

%files -n tao-costrading
%defattr(-,root,root,-)

%{_sbindir}/tao-costrading
%{_sysconfdir}/rc.d/init.d/tao-costrading
%dir %{_sysconfdir}/tao
%config(noreplace) %{_sysconfdir}/tao/tao-costrading.opt
%config(noreplace) %{_sysconfdir}/tao/tao-costrading.conf
%config(noreplace) %{_sysconfdir}/logrotate.d/tao-costrading
%attr(-,tao,tao) %dir %{_localstatedir}/log/tao
%attr(0644,tao,tao) %ghost %{_localstatedir}/log/tao/tao-costrading.log

%doc TAO/COPYING
%doc TAO/PROBLEM-REPORT-FORM
%doc TAO/VERSION
%doc TAO/README

# ---------------- tao-rtevent ----------------

%files -n tao-rtevent
%defattr(-,root,root,-)

%{_sbindir}/tao-rtevent
%{_sysconfdir}/rc.d/init.d/tao-rtevent
%dir %{_sysconfdir}/tao
%config(noreplace) %{_sysconfdir}/tao/tao-rtevent.opt
%config(noreplace) %{_sysconfdir}/tao/tao-rtevent.conf
%config(noreplace) %{_sysconfdir}/logrotate.d/tao-rtevent
%attr(-,tao,tao) %dir %{_localstatedir}/log/tao
%attr(0644,tao,tao) %ghost %{_localstatedir}/log/tao/tao-rtevent.log

%doc TAO/COPYING
%doc TAO/PROBLEM-REPORT-FORM
%doc TAO/VERSION
%doc TAO/README

# ---------------- tao-cosconcurrency ----------------

%files -n tao-cosconcurrency
%defattr(-,root,root,-)

%{_sbindir}/tao-cosconcurrency
%{_sysconfdir}/rc.d/init.d/tao-cosconcurrency
%dir %{_sysconfdir}/tao
%config(noreplace) %{_sysconfdir}/tao/tao-cosconcurrency.opt
%config(noreplace) %{_sysconfdir}/tao/tao-cosconcurrency.conf
%config(noreplace) %{_sysconfdir}/logrotate.d/tao-cosconcurrency
%attr(-,tao,tao) %dir %{_localstatedir}/log/tao
%attr(0644,tao,tao) %ghost %{_localstatedir}/log/tao/tao-cosconcurrency.log

%doc TAO/COPYING
%doc TAO/PROBLEM-REPORT-FORM
%doc TAO/VERSION
%doc TAO/README

# ---------------- tao-flresource ----------------

# Read: true if _with_guilibs is defined, false if not defined.
%if %{?_with_guilibs:1}%{!?_with_guilibs:0}

%files -n tao-flresource
%defattr(-,root,root,-)
%{_libdir}/libTAO_FlResource.so.%{TAOVERSO}

%doc TAO/COPYING
%doc TAO/PROBLEM-REPORT-FORM
%doc TAO/VERSION
%doc TAO/README

%endif

# ---------------- tao-flresource-devel ----------------

# Read: true if _with_guilibs is defined, false if not defined.
%if %{?_with_guilibs:1}%{!?_with_guilibs:0}

%files -n tao-flresource-devel
%defattr(-,root,root,-)
%{_libdir}/libTAO_FlResource.so
%dir %{_includedir}/tao
%{_includedir}/tao/FlResource/FlResource_Factory.h
%{_includedir}/tao/FlResource/FlResource_Loader.h
%{_includedir}/tao/FlResource/TAO_FlResource_Export.h

%doc TAO/COPYING
%doc TAO/PROBLEM-REPORT-FORM
%doc TAO/VERSION
%doc TAO/README

%endif

# ---------------- tao-qtresource ----------------

# Read: true if _with_guilibs is defined, false if not defined.
%if %{?_with_guilibs:1}%{!?_with_guilibs:0}

%files -n tao-qtresource
%defattr(-,root,root,-)
%{_libdir}/libTAO_QtResource.so.%{TAOVERSO}

%doc TAO/COPYING
%doc TAO/PROBLEM-REPORT-FORM
%doc TAO/VERSION
%doc TAO/README

%endif

# ---------------- tao-qtresource-devel ----------------

# Read: true if _with_guilibs is defined, false if not defined.
%if %{?_with_guilibs:1}%{!?_with_guilibs:0}

%files -n tao-qtresource-devel
%defattr(-,root,root,-)
%{_libdir}/libTAO_QtResource.so
%dir %{_includedir}/tao
%{_includedir}/tao/QtResource/QtResource_Factory.h
%{_includedir}/tao/QtResource/QtResource_Loader.h
%{_includedir}/tao/QtResource/TAO_QtResource_Export.h

%doc TAO/COPYING
%doc TAO/PROBLEM-REPORT-FORM
%doc TAO/VERSION
%doc TAO/README

%endif

# ---------------- tao-tkresource ----------------

# Read: true if _with_guilibs is defined, false if not defined.
%if %{?_with_guilibs:1}%{!?_with_guilibs:0}

%files -n tao-tkresource
%defattr(-,root,root,-)
%{_libdir}/libTAO_TkResource.so.%{TAOVERSO}

%doc TAO/COPYING
%doc TAO/PROBLEM-REPORT-FORM
%doc TAO/VERSION
%doc TAO/README

%endif

# ---------------- tao-tkresource-devel ----------------

# Read: true if _with_guilibs is defined, false if not defined.
%if %{?_with_guilibs:1}%{!?_with_guilibs:0}

%files -n tao-tkresource-devel
%defattr(-,root,root,-)
%{_libdir}/libTAO_TkResource.so
%dir %{_includedir}/tao
%{_includedir}/tao/TkResource/TkResource_Factory.h
%{_includedir}/tao/TkResource/TkResource_Loader.h
%{_includedir}/tao/TkResource/TAO_TkResource_Export.h

%doc TAO/COPYING
%doc TAO/PROBLEM-REPORT-FORM
%doc TAO/VERSION
%doc TAO/README

%endif

# ---------------- tao-xtresource ----------------

# Read: true if _with_guilibs is defined, false if not defined.
%if %{?_with_guilibs:1}%{!?_with_guilibs:0}

%files -n tao-xtresource
%defattr(-,root,root,-)
%{_libdir}/libTAO_XtResource.so.%{TAOVERSO}

%doc TAO/COPYING
%doc TAO/PROBLEM-REPORT-FORM
%doc TAO/VERSION
%doc TAO/README

%endif

# ---------------- tao-xtresource-devel ----------------

# Read: true if _with_guilibs is defined, false if not defined.
%if %{?_with_guilibs:1}%{!?_with_guilibs:0}

%files -n tao-xtresource-devel
%defattr(-,root,root,-)
%{_libdir}/libTAO_XtResource.so
%dir %{_includedir}/tao
%{_includedir}/tao/XtResource/XtResource_Factory.h
%{_includedir}/tao/XtResource/XtResource_Loader.h
%{_includedir}/tao/XtResource/TAO_XtResource_Export.h

%doc TAO/COPYING
%doc TAO/PROBLEM-REPORT-FORM
%doc TAO/VERSION
%doc TAO/README

%endif

# ================================================================
# changelog
# ================================================================

%changelog
* Thu Jul 31 2008 Johnny Willemsen  <jwillemsen@remedy.nl> - 5.6.6-2
- Removed ace-tao-unusedarg.patch (related to bug #3270).

* Sat Jul  5 2008 Ken Sedgwick <ken+5a4@bonsai.com> - 5.6.6-1
- Updated for latest DOC Group release (x.6.6).
- Tracked libACE_Monitor_Control name change.
- Added idl and pidl include files to tao-devel package.

* Wed Jun 25 2008 Johnny Willemsen  <jwillemsen@remedy.nl> - 5.6.5-6
- Removed gperf to gperf_ace rename, ACE ships now ace_gperf by default
- Removed ace-tao-strrecvfd.patch
- Removed gperf macro from ace-tao-config.patch
- Removed gperf.info patch

* Mon Jun  2 2008 Ken Sedgwick <ken+5a4@bonsai.com> - 5.6.5-5
- Added ace-tao-strrecvfd.patch (related to bug #3291).
- Changed make loop construct to abort when subcomponent fails.
- Removed PSS from TAO build list.
- Added ace-tao-unusedarg.patch (related to bug #3270).
- Made qt3 BuildRequires conditional on Fedora version.

* Wed May 28 2008 Ken Sedgwick <ken+5a4@bonsai.com> - 5.6.5-4
- Added ace-tao-orbsvcs-daemon.patch.
- Fixed tao-cosconcurrency command line arguments.

* Sat May 24 2008 Ken Sedgwick <ken+5a4@bonsai.com> - 5.6.5-3
- Removed obstack patch, no longer needed.
- Converted ace-tao-config-ipv6.patch into conditional rpm script.
- Converted ace-tao-rnq.patch into conditional rpm script.
- Converted ace-tao-config-tmplvis.patch and ace-tao-hasicmp.patch
  into rpm script.

* Thu May 22 2008 Johnny Willemsen  <jwillemsen@remedy.nl> - 5.6.5-3
- Removed codeset patch, merged into the distribution

* Wed May 21 2008 Ken Sedgwick <ken+5a4@bonsai.com> - 5.6.5-2
- Fixed without opt processing.

* Mon May 19 2008 Ken Sedgwick <ken+5a4@bonsai.com> - 5.6.5-1
- Updated to latest DOC Group release (x.6.5).
- Updated ace-tao-codeset.patch.
- Removed ace-tao-x86_64.patch (fixed in repository.)
- Added libACE_ETCL, libACE_ETCL_Parser and libACE_MonitorControl.

* Sun May 18 2008 Ken Sedgwick <ken+5a4@bonsai.com> - 5.6.4-4
- Fixed tao-cosnaming.dat path bug.

* Sun May 18 2008 Ken Sedgwick <ken+5a4@bonsai.com> - 5.6.4-3
- Improved to use _localstatedir where appropriate.
- Made adjustments to file/dir permissions for services.
- Added service logrotate scripts.

* Wed May 14 2008 Ken Sedgwick <ken+5a4@bonsai.com> - 5.6.4-2
- Renamed tao services again per Fedora service spec.

* Thu May  1 2008 Ken Sedgwick <ken+5a4@bonsai.com> - 5.6.4-1
- Updated to latest DOC Group release (x.6.4).

* Thu May  1 2008 Ken Sedgwick <ken+5a4@bonsai.com> - 5.6.3-3
- Install service binaries in /usr/sbin instead of /usr/bin.

* Sun Mar 30 2008 Ken Sedgwick <ken+5a4@bonsai.com> - 5.6.3-2
- Added dist macro to Release tag.
- Miscellaneous rpmlint fixes.
- Renamed service executables.

* Sat Feb 23 2008 Ken Sedgwick <ken+5a4@bonsai.com> - 5.6.3-1
- Updated to latest DOC group release (x.6.3).
- Removed ace-tao-conf-epoll patch, no longer needed.

* Sat Dec 15 2007 Ken Sedgwick <ken+5a4@bonsai.com> - 5.6.2-1
- Updated to latest DOC group release (x.6.2).

* Sat Dec 15 2007 Ken Sedgwick <ken+5a4@bonsai.com> - 5.6.1-3
- Added service modules back into runtime packages.

* Mon Dec  3 2007 Ken Sedgwick <ken+5a4@bonsai.com> - 5.6.1-2
- Added QtResource_Loader to explicit headers.

* Sun Oct 28 2007 Ken Sedgwick <ken+5a4@bonsai.com> - 5.6.1-2
- Applied Tom Callaway's Fedora fixes.

* Sat Sep 22 2007 Ken Sedgwick <ken+5a4@bonsai.com> - 5.6.1-1
- Updated to latest DOC group release (x.6.1).

* Mon Sep  3 2007 Ken Sedgwick <ken+5a4@bonsai.com> - 5.6-1
- Updated to latest DOC group release (x.6).
- Combined 64bit patches into ace-tao-x86_64.patch.

* Wed Aug  1 2007 Ken Sedgwick <ken+5a4@bonsai.com> - 5.5.10-1
- Updated to latest DOC group release (x.5.10).

* Sun Jul  1 2007 Ken Sedgwick <ken+5a4@bonsai.com> - 5.5.9-1
- Updated to latest DOC group release (x.5.9).

* Sat May  5 2007 Ken Sedgwick <ken+5a4@bonsai.com> - 5.5.8-1
- Updated to latest DOC group release (x.5.8).
- Added ACE_GCC_HAS_TEMPLATE_INSTANTIATION_VISIBILITY_ATTRS.

* Sat Mar 24 2007 Ken Sedgwick <ken+5a4@bonsai.com> - 5.5.7-1
- Updated to latest DOC group release (x.5.7).

* Mon Feb 19 2007 Ken Sedgwick <ken+5a4@bonsai.com> - 5.5.6-2
- Added SSLIOP refcount patch (DOC BUG 1506).

* Fri Feb  2 2007 Ken Sedgwick <ken+5a4@bonsai.com> - 5.5.6-1
- Updated to latest DOC group release (x.5.6).

* Thu Jan 25 2007 Ken Sedgwick <ken+5a4@bonsai.com> - 5.5.5-1
- Updated to latest DOC group release (x.5.5).
- Adjusted paths to {Fl,Qt,Tk,Xt}Reactor headers.

* Sat Dec 16 2006 Ken Sedgwick <ken+5a4@bonsai.com> - 5.5.4-2
- Defined ACE_HAS_EVENT_POLL config parameter.

* Wed Nov 29 2006 Ken Sedgwick <ken+5a4@bonsai.com> - 5.5.4-1
- Updated to latest DOC group release (x.5.4).

* Mon Nov 20 2006 Ken Sedgwick <ken+5a4@bonsai.com> - 5.5.3-3
- Improved obstack patch per Johnny Willemsen.

* Sat Nov 18 2006 Ken Sedgwick <ken+5a4@bonsai.com> - 5.5.3-2
- Added obstack workaround for FC6 builds.
- Fixed IPV6 configuration patch.

* Tue Oct 10 2006 Ken Sedgwick <ken+5a4@bonsai.com> - 5.5.3-1
- Updated to latest DOC group release (x.5.3).

* Tue Sep  5 2006 Ken Sedgwick <ken+5a4@bonsai.com> - 5.5.2-2
- Added ACE_HAS_ICMP_SUPPORT to config.h
- Added patch to re-enable setting ssl verify mode.

* Wed Jul 12 2006 Ken Sedgwick <ken+5a4@bonsai.com> - 5.5.2-1
- Updated to latest DOC group release (x.5.2).

* Sat Jun 24 2006 Ken Sedgwick <ken+5a4@bonsai.com> - 5.5.1-4
- Added IPv6 support by default.

* Wed Jun 14 2006 Ken Sedgwick <ken+5a4@bonsai.com> - 5.5.1-3
- Added x86_64 patch back in (required for GUI builds).

* Sun May 21 2006 Ken Sedgwick <ken+5a4@bonsai.com> - 5.5.1-2
- Added aio64 patch for librt.so detection.

* Wed Apr 19 2006 Ken Sedgwick <ken+5a4@bonsai.com> - 5.5.1-1
- Updated to latest DOC group release (x.5.1).
- Added conditional (on by default) addition of
  ACE_HAS_REACTOR_NOTIFICATION_QUEUE to config.h.

* Sat Mar 25 2006 Ken Sedgwick <ken+5a4@bonsai.com> - 5.5-2
- Added Doug Schmidt's patches for gcc 4.1.0 type-punned warnings.
- Removed X11 build dependencies due to xorg package renaming.

* Tue Mar  7 2006 Ken Sedgwick <ken+5a4@bonsai.com> - 5.5-1
- Updated to latest DOC group release (x.5)

* Wed Feb 22 2006 Ken Sedgwick <ken+5a4@bonsai.com> - 5.4.10-1
- Updated to latest DOC group release (x.4.10)

* Sat Feb 11 2006 Ken Sedgwick <ken+5a4@bonsai.com> - 5.4.9-2
- Created unversioned symlinks for svc.conf loadable shared objects
  in runtime packages.
- Now run mwc during rpmbuild even if _with_guilibs is not defined.
- Removed patch for fixed namespace bugs.

* Thu Feb  9 2006 Ken Sedgwick <ken+5a4@bonsai.com> - 5.4.9-1
- Updated to latest DOC group release (x.4.9)

* Fri Dec 23 2005 Ken Sedgwick <ken+5a4@bonsai.com> - 5.4.8-1
- Added ace-tao-idl-preproc patch for IDL include path bug.

* Thu Dec 22 2005 Ken Sedgwick <ken+5a4@bonsai.com> - 5.4.8-0
- Updated to the latest DOC group release (5.4.8)

* Fri Dec  9 2005 Ken Sedgwick <ken+5a4@bonsai.com> - 5.4.7-1
- Added buildbits=64 macro for 64 bit archs.

* Mon Aug  8 2005 Ken Sedgwick <ken+5a4@bonsai.com> - 5.4.7-0
- Updated to latest DOC group release (5.4.7)

* Fri Jun 10 2005 Ken Sedgwick <ken+5a4@bonsai.com> - 5.4.6-0
- Updated to latest DOC group release (5.4.6)

* Sun May 15 2005 Ken Sedgwick <ken+5a4@bonsai.com> - 5.4.5-0
- Updated to latest DOC group release (5.4.5)
- Removed TSS Cleanup patch (bug fixed in release).

* Sat Mar  5 2005 Ken Sedgwick <ken+5a4@bonsai.com> - 5.4.4-5
- Fixed include directory uninstall cleanup for GUI reactor packages.
- Fixed installation to preserve header timestamps.

* Wed Mar  2 2005 Ken Sedgwick <ken+5a4@bonsai.com> - 5.4.4-4
- Integrated Marek Brudka's GUI library additions.

* Thu Feb 24 2005 Ken Sedgwick <ken+5a4@bonsai.com> - 5.4.4-3
- Changed name of spec, source rpm and patches to remove '+'.

* Sat Feb 19 2005 Ken Sedgwick <ken+5a4@bonsai.com> - 5.4.4-2
- Fixed installed doc, header and debug source file permissions.

* Fri Feb 18 2005 Ken Sedgwick <ken+5a4@bonsai.com> - 5.4.4-2
- Updated spec tags per Fedora PackagingHints wiki document.

* Thu Feb 17 2005 Ken Sedgwick <ken+5a4@bonsai.com> - 5.4.4-1
- Added patch to rename gperf info menu entry to gperf-ace.
- Replaced TSS-Cleanup patch with candidate fixes from Dale Wilson.

* Wed Feb 16 2005 Ken Sedgwick <ken+5a4@bonsai.com> - 5.4.4-1
- Macroed some absolute paths in the files section.
- Added post and preun calls to install-info.

* Mon Feb 14 2005 Ken Sedgwick <ken+5a4@bonsai.com> - 5.4.4-1
- Fixed BuildRequires entries.

* Sun Feb 13 2005 Ken Sedgwick <ken+5a4@bonsai.com> - 5.4.4-1
- Added Giovanni Ferro's _extension macro for info and man pages.

* Thu Feb 10 2005 Ken Sedgwick <ken+5a4@bonsai.com> - 5.4.4-0
- Updated to latest DOC group release (5.4.4).
- Removed idl-rootinc patch (fixed in distro).
- Removed orbdaemon patch (fixed in distro).
- Added libTAO_TMCast.so to ace package.

* Mon Feb  7 2005 Ken Sedgwick <ken+5a4@bonsai.com> - 5.4.3-5
- Added /usr/include/ACEXML to explicit directory list.

* Sun Feb  6 2005 Ken Sedgwick <ken+5a4@bonsai.com> - 5.4.3-4
- Fixed unfulfilled dependency in tao package for libTAO_IDL_FE.so
- Removed bidir-noassert patch, not needed.

* Mon Jan 31 2005 Ken Sedgwick <ken+5a4@bonsai.com> - 5.4.3-4
- Added TSS Cleanup fixes patch (DOC bugzilla #2044)

* Mon Jan 17 2005 Andrew L. Shwaika <als@solvo.ru> - 5.4.3-3
- Add CORBA services

* Sat Jan 15 2005 Ken Sedgwick <ken+5a4@bonsai.com> - 5.4.3-2
- Used ACE_GPERF to change name of installed gperf to gperf-ace.

* Mon Jan 10 2005 Ken Sedgwick <ken+5a4@bonsai.com> - 5.4.3-1
- Added bidir-noassert2 patch.

* Sat Dec 18 2004 Ken Sedgwick <ken+5a4@bonsai.com> - 5.4.2-0
- Initial build.
