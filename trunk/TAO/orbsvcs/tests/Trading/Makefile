#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

BIN     = export_test import_test colocated_test

MAKEFILE = Makefile
LIBNAME  = libTTest
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).$(SOEXT)

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif

IDL_FILES = TTestC \
            TTestS \

FILES   = $(IDL_FILES) \
          TT_Info \
          Service_Type_Exporter \
          Offer_Exporter \
          Offer_Importer \
          Simple_Dynamic

LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs -I$(TAO_ROOT) $(TSS_ORB_FLAG)

LSRC    = $(addsuffix .cpp,$(FILES))
LIBS    = -lorbsvcs -lTAO
LDLIBS  = -lTTest -lorbsvcs -lTAO
VLDLIBS = $(LDLIBS:%=%$(VAR))

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

.PRECIOUS: TTestS.cpp TTestS.h TTestC.cpp TTestC.h

clean:
	-/bin/rm -rf *.o $(BIN) obj.* core Templates.DB .make.state

realclean: clean
	-/bin/rm -rf $(addsuffix .h, $(IDL_FILES)) $(addsuffix .i, $(IDL_FILES)) $(addsuffix .cpp, $(IDL_FILES)) TTestS_T.*


# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/TTestC.o .obj/TTestC.so .shobj/TTestC.o .shobj/TTestC.so: TTestC.cpp TTestC.h TTestC.i TTestS.h TTestS_T.h TTestS_T.i \
 TTestS_T.cpp TTestS.i
.obj/TTestS.o .obj/TTestS.so .shobj/TTestS.o .shobj/TTestS.so: TTestS.cpp TTestS.h TTestC.h TTestC.i TTestS_T.h TTestS_T.i \
 TTestS_T.cpp TTestS.i
.obj/TT_Info.o .obj/TT_Info.so .shobj/TT_Info.o .shobj/TT_Info.so: TT_Info.cpp TT_Info.h TTestS.h TTestC.h TTestC.i TTestS_T.h \
 TTestS_T.i TTestS_T.cpp TTestS.i \
 $(ACE_ROOT)/ace/Arg_Shifter.h \
 $(ACE_ROOT)/ace/Read_Buffer.h \
 $(ACE_ROOT)/ace/Read_Buffer.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/orbsvcs_export.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Trader/Trader_Utils.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Trader/Trader.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS.i
.obj/Service_Type_Exporter.o .obj/Service_Type_Exporter.so .shobj/Service_Type_Exporter.o .shobj/Service_Type_Exporter.so: Service_Type_Exporter.cpp \
 Service_Type_Exporter.h TT_Info.h TTestS.h TTestC.h TTestC.i \
 TTestS_T.h TTestS_T.i TTestS_T.cpp TTestS.i \
 $(ACE_ROOT)/ace/Arg_Shifter.h \
 $(ACE_ROOT)/ace/Read_Buffer.h \
 $(ACE_ROOT)/ace/Read_Buffer.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/orbsvcs_export.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingC.i
.obj/Offer_Exporter.o .obj/Offer_Exporter.so .shobj/Offer_Exporter.o .shobj/Offer_Exporter.so: Offer_Exporter.cpp Offer_Exporter.h TT_Info.h \
 TTestS.h TTestC.h TTestC.i TTestS_T.h TTestS_T.i TTestS_T.cpp \
 TTestS.i $(ACE_ROOT)/ace/Arg_Shifter.h \
 $(ACE_ROOT)/ace/Read_Buffer.h \
 $(ACE_ROOT)/ace/Read_Buffer.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/orbsvcs_export.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingC.i \
 Simple_Dynamic.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Trader/Trader_Utils.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Trader/Trader.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS.i \
 Simple_Dynamic.cpp
.obj/Offer_Importer.o .obj/Offer_Importer.so .shobj/Offer_Importer.o .shobj/Offer_Importer.so: Offer_Importer.cpp Offer_Importer.h TT_Info.h \
 TTestS.h TTestC.h TTestC.i TTestS_T.h TTestS_T.i TTestS_T.cpp \
 TTestS.i $(ACE_ROOT)/ace/Arg_Shifter.h \
 $(ACE_ROOT)/ace/Read_Buffer.h \
 $(ACE_ROOT)/ace/Read_Buffer.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/orbsvcs_export.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Trader/Trader_Utils.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Trader/Trader.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS.i
.obj/Simple_Dynamic.o .obj/Simple_Dynamic.so .shobj/Simple_Dynamic.o .shobj/Simple_Dynamic.so: Simple_Dynamic.cpp Simple_Dynamic.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Trader/Trader_Utils.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Trader/Trader.h \
 $(ACE_ROOT)/ace/Hash_Map_Manager.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-sunos5.5.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Functor.h \
 $(ACE_ROOT)/ace/Functor.i \
 $(ACE_ROOT)/ace/Functor_T.h \
 $(ACE_ROOT)/ace/Functor_T.i \
 $(ACE_ROOT)/ace/Functor_T.cpp \
 $(ACE_ROOT)/ace/Hash_Map_Manager.i \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.h \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.i \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Atomic_Op.i \
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.cpp \
 $(ACE_ROOT)/ace/Service_Config.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.i \
 $(ACE_ROOT)/ace/Service_Object.i \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Service_Config.i \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.cpp \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/orbsvcs_export.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS.i \
 Simple_Dynamic.cpp

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
