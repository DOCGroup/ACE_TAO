##----------------------------------------------------------------------------
##       $Id$
##
##       Makefile.am for TAO
##----------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_builddir) -I$(top_srcdir)

POA_FILES = \
	Object_Adapter.cpp \
	Key_Adapters.cpp \
	POA.cpp \
	POAC.cpp \
	POAS.cpp \
	POAManager.cpp \
	Active_Object_Map.cpp \
	Servant_Base.cpp \
	Forwarding_Servant.cpp

PLUGGABLE_PROTOCOLS_FILES = \
	Pluggable.cpp \
	Connector_Registry.cpp \
	Acceptor_Registry.cpp \
	Protocol_Factory.cpp \
	IIOP_Factory.cpp \
	IIOP_Profile.cpp \
	IIOP_Transport.cpp \
	IIOP_Connector.cpp \
	IIOP_Acceptor.cpp \
	IIOP_Connect.cpp \
	UIOP_Factory.cpp \
	UIOP_Profile.cpp \
	UIOP_Transport.cpp \
	UIOP_Connector.cpp \
	UIOP_Acceptor.cpp \
	UIOP_Connect.cpp \
	IORC.cpp \
	IORS.cpp \
	IORManipulation.cpp

DEFAULT_RESOURCES_FILES = \
	default_client.cpp \
	default_server.cpp \
	default_resource

INTERPRETIVE_MARSHALING_FILES = \
	append.cpp \
	decode.cpp \
	deep_free.cpp \
	encode.cpp \
	CDR_Interpreter.cpp \
	Marshal.cpp \
	skip.cpp

IDL_COMPILER_FILES = \
	Managed_Types.cpp \
	Union.cpp \
	Operation_Table.cpp

ORB_CORE_FILES = \
	Any.cpp \
	CurrentC.cpp \
	CurrentS.cpp \
	Exception.cpp \
	WrongTransactionC.cpp \
	Environment.cpp \
	Object.cpp \
	ORB.cpp \
	corbafwd.cpp \
	PolicyC.cpp \
	PolicyS.cpp \
	DomainC.cpp \
	DomainS.cpp \
	TimeBaseC.cpp \
	TimeBaseS.cpp \
	Principal.cpp \
	Sequence.cpp \
	Server_Request.cpp \
	MProfile.cpp \
	Stub.cpp \
	Typecode.cpp \
	ValueBase.cpp \
	ValueFactory.cpp \
	ValueFactory_Map.cpp \
	CDR.cpp \
	Client_Strategy_Factory.cpp \
	debug.cpp \
	Object_KeyC.cpp \
	GIOP.cpp \
	GIOP_Server_Request.cpp \
	Invocation.cpp \
	ORB_Core.cpp \
	params.cpp \
	Resource_Factory.cpp \
	Server_Strategy_Factory.cpp \
	TAO_Internal.cpp \
	Typecode_Constants.cpp \
	IOR_LookupTable.cpp \
	TAO.cpp \
	Wait_Strategy.cpp \
	Transport_Mux_Strategy.cpp \
	Reply_Dispatcher.cpp \
	IOPC.cpp \
	IOPS.cpp \
	CONV_FRAMEC.cpp \
	CONV_FRAMES.cpp \
	Tagged_Components.cpp \
	Context.cpp \
	Request.cpp \
	Services.cpp \
	NVList.cpp \
	ObjectIDList.cpp \
	MessagingC.cpp \
	MessagingS.cpp \
	Policy_Manager.cpp \
	Messaging_Policy_i.cpp

DYNAMIC_ANY_FILES = \
	DynAnyC.cpp \
	DynAnyS.cpp \
	DynAny_i.cpp \
	DynArray_i.cpp \
	DynEnum_i.cpp \
	DynSequence_i.cpp \
	DynStruct_i.cpp \
	DynUnion_i.cpp \
	InconsistentTypeCodeC.cpp

# Build a libtool library, libTAO.la for installation in libdir.
lib_LTLIBRARIES = libTAO.la
libTAO_la_SOURCES = \
	$(POA_FILES) \
	$(PLUGGABLE_PROTOCOLS_FILES) \
	$(DEFAULT_RESOURCES_FILES) \
	$(INTERPRETIVE_MARSHALING_FILES) \
	$(IDL_COMPILER_FILES) \
	$(ORB_CORE_FILES) \
	$(DYNAMIC_ANY_FILES)

## The following is an excerpt from the "libtool" manual, written by
## Gordon Matzigkeit:
##
## This flag accepts an argument of the form `current[:revision[:age]]'. So,
## passing `-version-info 3:12:1' sets current to 3, revision to 12, and age
## to 1. 
## If either revision or age are omitted, they default to 0. Also note that
## age must be less than or equal to the current interface number. 
##
## Here are a set of rules to help you update your library version
## information: 
##
##     1.Start with version information of `0:0:0' for each libtool library. 
##     2.Update the version information only immediately before a public
##       release of your software. More frequent updates are unnecessary, and
##       only guarantee that the current interface number gets larger faster. 
##     3.If the library source code has changed at all since the last update,
##       then increment revision (`c:r:a' becomes `c:r+1:a'). 
##     4.If any interfaces have been added, removed, or changed since the last
##       update, increment current, and set revision to 0. 
##     5.If any interfaces have been added since the last public release, then
##       increment age. 
##     6.If any interfaces have been removed since the last public release,
##       then set age to 0. 
##
## Never try to set the interface numbers so that they correspond to the
## release number of your package. This is an abuse that only fosters
## misunderstanding of the purpose of library versions. Instead, use the
## `-release' flag (see section 6.4 Managing release information), but be
## warned that every release of your package will not be binary compatibility
## with any other release. 

## The below "-version-info" isn't being used the way libtool intends
## it to be used but we do it that way to make the version numbers
## that TAO uses match those created by libtool from "-version-info."
##libTAO_la_LDFLAGS = -version-info 2:41:2   ## 2-2=0 --> libTAO.so.0.2.41
libTAO_la_LDFLAGS = -version-info @TAO_CURRENT@:@TAO_REVISION@:@TAO_AGE@

## These are source files that are needed by the header files if the
## ACE_TEMPLATES_REQUIRE_SOURCE or ACE_TEMPLATES_REQUIRE_PRAGMA
## macro is defined.
if TEMPLATES_REQUIRE_SOURCE
TEMPLATE_FILES = \
	Acceptor_Impl.cpp \
	CONV_FRAMES_T.cpp \
	DomainS_T.cpp \
	DynUnion_i_T.cpp \
	IOPS_T.cpp \
	MessagingS_T.cpp \
	PollableS_T.cpp \
	Sequence_T.cpp \
	TimeBaseS_T.cpp \
	varout.cpp
else
TEMPLATE_FILES = 
endif

pkgincludedir = $(prefix)/include/tao
pkginclude_HEADERS = \
        Acceptor_Impl.h \
        Acceptor_Impl.i \
        Acceptor_Registry.h \
        Acceptor_Registry.i \
        Active_Object_Map.h \
        Active_Object_Map.i \
        Any.h \
        Any.i \
        CDR.h \
        CDR.i \
        CDR_Interpreter.h \
        CONV_FRAMEC.h \
        CONV_FRAMEC.i \
        CONV_FRAMES.h \
        CONV_FRAMES.i \
        CONV_FRAMES_T.h \
        CONV_FRAMES_T.i \
        Client_Strategy_Factory.h \
        Connector_Registry.h \
        Context.h \
        Context.i \
        CurrentC.h \
        CurrentC.i \
        DomainC.h \
        DomainC.i \
        DomainS_T.h \
        DomainS_T.i \
        DynAnyC.h \
        DynAnyC.i \
        DynAny_i.h \
        DynArray_i.h \
        DynEnum_i.h \
        DynSequence_i.h \
        DynStruct_i.h \
        DynUnion_i.h \
        DynUnion_i_T.h \
        Environment.h \
        Environment.i \
        Exception.h \
        Exception.i \
        Forwarding_Servant.h \
        GIOP.h \
        GIOP.i \
        GIOP_Server_Request.h \
        GIOP_Server_Request.i \
        IIOP_Acceptor.h \
        IIOP_Acceptor.i \
        IIOP_Connect.h \
        IIOP_Connect.i \
        IIOP_Connector.h \
        IIOP_Factory.h \
        IIOP_Profile.h \
        IIOP_Profile.i \
        IIOP_Transport.h \
        IOPC.h \
        IOPC.i \
        IOPS.h \
        IOPS.i \
        IOPS_T.h \
        IOPS_T.i \
        IORC.h \
        IORC.i \
        IORManipulation.h \
        IORS.h \
        IORS.i \
        IOR_LookupTable.h \
        InconsistentTypeCodeC.h \
        Invocation.h \
        Invocation.i \
        Key_Adapters.h \
        Key_Adapters.i \
        MProfile.h \
        MProfile.i \
        Managed_Types.h \
        Managed_Types.i \
        Marshal.h \
        Marshal.i \
        MessagingC.h \
        MessagingC.i \
        MessagingS.h \
        MessagingS.i \
        MessagingS_T.h \
        MessagingS_T.i \
        Messaging_Policy_i.h \
        Messaging_Policy_i.i \
        NVList.h \
        NVList.i \
        ORB.h \
        ORB.i \
        ORB_Core.h \
        ORB_Core.i \
        Object.h \
        Object.i \
        ObjectIDList.h \
        ObjectIDList.i \
        Object_Adapter.h \
        Object_Adapter.i \
        Object_KeyC.h \
        Object_KeyC.i \
        Operation_Table.h \
        POA.h \
        POA.i \
        POAC.h \
        POAC.i \
        POAManager.h \
        POAManager.i \
        POAS.h \
        POAS.i \
        POA_CORBA.h \
        Pluggable.h \
        Pluggable.i \
        PolicyC.h \
        PolicyC.i \
        Policy_Manager.h \
        Policy_Manager.i \
        PollableC.h \
        PollableC.i \
        PollableS.h \
        PollableS.i \
        PollableS_T.h \
        PollableS_T.i \
        Principal.h \
        Principal.i \
        Protocol_Factory.h \
        Reply_Dispatcher.h \
        Reply_Dispatcher.i \
        Request.h \
        Request.i \
        Resource_Factory.h \
        Sequence.h \
        Sequence.i \
        Sequence_T.h \
        Sequence_T.i \
        Servant_Base.h \
        Servant_Base.i \
        Server_Request.h \
        Server_Request.i \
        Server_Strategy_Factory.h \
        Services.h \
        Services.i \
        Stub.h \
        Stub.i \
        TAO.h \
        TAO_Internal.h \
        Tagged_Components.h \
        Tagged_Components.i \
        TimeBaseC.h \
        TimeBaseC.i \
        TimeBaseS.h \
        TimeBaseS.i \
        TimeBaseS_T.h \
        TimeBaseS_T.i \
        Timeprobe.h \
        Transport_Mux_Strategy.h \
        Typecode.h \
        Typecode.i \
        UIOP_Acceptor.h \
        UIOP_Connect.h \
        UIOP_Connect.i \
        UIOP_Connector.h \
        UIOP_Factory.h \
        UIOP_Profile.h \
        UIOP_Profile.i \
        UIOP_Transport.h \
        Union.h \
        ValueBase.h \
        ValueBase.i \
        ValueFactory.h \
        ValueFactory.i \
        ValueFactory_Map.h \
        ValueFactory_Map.i \
        Version.h \
        Wait_Strategy.h \
        WrongTransactionC.h \
        corba.h \
        corbafwd.h \
        corbafwd.i \
        debug.h \
        default_client.h \
        default_client.i \
        default_resource.h \
        default_resource.i \
        default_server.h \
        default_server.i \
        orbconf.h \
        params.h \
        params.i \
        poa_macros.h \
        singletons.h \
        try_macros.h \
        varout.h \
        varout.i \
	$(TEMPLATE_FILES)
## Make sure $(TEMPLATE_FILES) is in the above header list!


## noinst_PROGRAMS = t-sizes

## t_sizes_SOURCES = t-sizes.cpp

EXTRA_DIST = \
        t-sizes.cpp

## Clean up some additional files/directories possibly created during
## the configure script tests.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -rf ptrepository Templates.DB gcctemp.c gcctemp so_locations

