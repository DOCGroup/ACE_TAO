##---------------------------------------------------------------------------
##       $Id$
##
##       Makefile for TAO's Event Channel library
##
##---------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_builddir) -I$(top_srcdir)

lib_LTLIBRARIES = libTAO_Event.la

libTAO_Event_la_LDADD = \
	$(top_builddir)/ace/libACE.la $(top_builddir)/TAO/tao/libTAO.la

libTAO_Event_la_SOURCES = \
        BCU.cpp \
        Dispatching_Modules.cpp \
        EC_Basic_Factory.cpp \
        EC_Basic_Filter_Builder.cpp \
        EC_Busy_Lock.cpp \
        EC_Command.cpp \
        EC_Conjunction_Filter.cpp \
        EC_ConsumerAdmin.cpp \
        EC_Disjunction_Filter.cpp \
        EC_Dispatching.cpp \
        EC_Event_Channel.cpp \
        EC_Factory.cpp \
        EC_Filter.cpp \
        EC_Filter_Builder.cpp \
        EC_Gateway.cpp \
        EC_Gateway_UDP.cpp \
        EC_Null_Factory.cpp \
        EC_ObserverStrategy.cpp \
        EC_Per_Supplier_Filter.cpp \
        EC_ProxyConsumer.cpp \
        EC_ProxyPushSupplier_Set.cpp \
        EC_ProxyPushSupplier_Set_T.cpp \
        EC_ProxySupplier.cpp \
        EC_QOS_Info.cpp \
        EC_Reactive_Timeout_Generator.cpp \
        EC_SupplierAdmin.cpp \
        EC_SupplierFiltering.cpp \
        EC_Timeout_Filter.cpp \
        EC_Timeout_Generator.cpp \
        EC_Type_Filter.cpp \
        EC_UDP_Admin.cpp \
        Event_Channel.cpp \
        Event_Manip.cpp \
        GPlot_File.cpp \
        Local_ESTypes.cpp \
        Memory_Pools.cpp \
        Module_Factory.cpp \
        RT_Task.cpp \
        ReactorTask.cpp \
        Timer_Module.cpp

noinst_HEADERS = \
        BCU.h \
        Debug_Macros.h \
        Dispatching_Modules.h \
        Dispatching_Modules.i \
        EC_Basic_Factory.h \
        EC_Basic_Factory.i \
        EC_Basic_Filter_Builder.h \
        EC_Basic_Filter_Builder.i \
        EC_Busy_Lock.h \
        EC_Busy_Lock.i \
        EC_Command.h \
        EC_Command.i \
        EC_Conjunction_Filter.h \
        EC_Conjunction_Filter.i \
        EC_ConsumerAdmin.h \
        EC_ConsumerAdmin.i \
        EC_Disjunction_Filter.h \
        EC_Disjunction_Filter.i \
        EC_Dispatching.h \
        EC_Dispatching.i \
        EC_Event_Channel.h \
        EC_Event_Channel.i \
        EC_Factory.h \
        EC_Factory.i \
        EC_Filter.h \
        EC_Filter.i \
        EC_Filter_Builder.h \
        EC_Filter_Builder.i \
        EC_Gateway.h \
        EC_Gateway_UDP.h \
        EC_Gateway_UDP.i \
        EC_Null_Factory.h \
        EC_Null_Factory.i \
        EC_ObserverStrategy.h \
        EC_ObserverStrategy.i \
        EC_Per_Supplier_Filter.h \
        EC_Per_Supplier_Filter.i \
        EC_ProxyConsumer.h \
        EC_ProxyConsumer.i \
        EC_ProxyPushSupplier_Set.h \
        EC_ProxyPushSupplier_Set.i \
        EC_ProxyPushSupplier_Set_T.h \
        EC_ProxyPushSupplier_Set_T.i \
        EC_ProxySupplier.h \
        EC_ProxySupplier.i \
        EC_QOS_Info.h \
        EC_QOS_Info.i \
        EC_Reactive_Timeout_Generator.h \
        EC_Reactive_Timeout_Generator.i \
        EC_SupplierAdmin.h \
        EC_SupplierAdmin.i \
        EC_SupplierFiltering.h \
        EC_SupplierFiltering.i \
        EC_Timeout_Filter.h \
        EC_Timeout_Filter.i \
        EC_Timeout_Generator.h \
        EC_Timeout_Generator.i \
        EC_Type_Filter.h \
        EC_Type_Filter.i \
        EC_UDP_Admin.h \
        Event_Channel.h \
        Event_Channel.i \
        Fast_Reactor.h \
        GPlot_File.h \
        GPlot_File.i \
        Local_ESTypes.h \
        Local_ESTypes.i \
        Memory_Pools.h \
        Memory_Pools.i \
        Module_Factory.h \
        RT_Task.h \
        RT_Task.i \
        ReactorTask.h \
        Timer_Module.h \
        Timer_Module.i

EXTRA_DIST = README

## Clean up some additional files/directories possibly created during
## the configure script tests.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -rf ptrepository Templates.DB gcctemp.c gcctemp so_locations

