# $Id$
# mcknerney, 11/7/2013

CXXLOCAL_DEFINES=

include			demo.mk
include 		cxx_demo.mk

LINK_WITH=		-lit_poa -lit_art -lit_ifc -lit_portable_interceptor

SOURCES=		server.cxx test_i.cxx server_interceptor.cxx Server_ORBInitializer.cxx testC.cxx testS.cxx

all:			server

server:		$(SOURCES) $(OBJS)
			$(RM) $@
			$(CXX) -o $@ $(CXXLDFLAGS) $(OBJS) $(LDLIBS)
			$(ITADMIN) scope create taodemo
			$(ITADMIN) variable create -scope taodemo -type list -value "Echo_Server_Interceptor, " binding:server_binding_list
			$(ITADMIN) variable create -scope taodemo -type list -value "local_log_stream,iiop_profile,giop,iiop,portable_interceptor" orb_plugins
# @see:
# http://community.microfocus.com/microfocus/corba/orbix/w/knowledge_base/22500.marshalexception-when-using-a-tao-based-application-to-obtain-a-consumeradmin-reference-for-the-orbix-notification-service.aspx
#			$(ITADMIN) variable create -scope taodemo -type bool -value true policies:giop:interop_policy:ignore_message_not_consumed

$(SOURCES):		test.hh

test.hh:		../idl/test.idl
			$(IDL) -base -poa -I$(ART_IDL_INCLUDE_DIR) ../idl/test.idl

clean:
			$(RM) server *.o core test.hh testS.hh testC.cxx testS.cxx test.ior
			$(ITADMIN) scope remove taodemo
