#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif

LDLIBS = -lTAO_Svc_Utils -lTAO -lACE

PROG_SRCS = \
        AliasDef_i.cpp \
        ArrayDef_i.cpp \
        AttributeDef_i.cpp \
        ComponentDef_i.cpp \
	ComponentRepository_i.cpp \
	ConstantDef_i.cpp \
	ConsumesDef_i.cpp \
	Contained_i.cpp \
	Container_i.cpp \
	EmitsDef_i.cpp \
	EnumDef_i.cpp \
	EventDef_i.cpp \
	ExceptionDef_i.cpp \
	FactoryDef_i.cpp \
	FinderDef_i.cpp \
	FixedDef_i.cpp \
	HomeDef_i.cpp \
	IDLType_i.cpp \
	IFR_Server.cpp \
	IFR_Service.cpp \
	InterfaceDef_i.cpp \
	InterfaceS.cpp \
	IRObject_i.cpp \
	ModuleDef_i.cpp \
	NativeDef_i.cpp \
	OperationDef_i.cpp \
	Options.cpp \
	PrimaryKeyDef_i.cpp \
	PrimitiveDef_i.cpp \
	ProvidesDef_i.cpp \
	PublishesDef_i.cpp \
	Repository_i.cpp \
	SequenceDef_i.cpp \
	Servant_Factory.cpp \
	Servant_Locator.cpp \
	StringDef_i.cpp \
	StructDef_i.cpp \
	tmplinst.cpp \
	TypedefDef_i.cpp \
	UnionDef_i.cpp \
	UsesDef_i.cpp \
	ValueBoxDef_i.cpp \
	ValueDef_i.cpp \
	ValueMemberDef_i.cpp \
	WstringDef_i.cpp

SRC = $(PROG_SRCS)

IFR_SVR_OBJS = \
        AliasDef_i.o \
        ArrayDef_i.o \
        AttributeDef_i.o \
        ComponentDef_i.o \
	ComponentRepository_i.o \
	ConstantDef_i.o \
	ConsumesDef_i.o \
	Contained_i.o \
	Container_i.o \
	EmitsDef_i.o \
	EnumDef_i.o \
	EventDef_i.o \
	ExceptionDef_i.o \
	FactoryDef_i.o \
	FinderDef_i.o \
	FixedDef_i.o \
	HomeDef_i.o \
	IDLType_i.o \
	IFR_Server.o \
	IFR_Service.o \
	InterfaceDef_i.o \
	InterfaceS.o \
	IRObject_i.o \
	ModuleDef_i.o \
	NativeDef_i.o \
	OperationDef_i.o \
	Options.o \
	PrimaryKeyDef_i.o \
	PrimitiveDef_i.o \
	ProvidesDef_i.o \
	PublishesDef_i.o \
	Repository_i.o \
	SequenceDef_i.o \
	Servant_Factory.o \
	Servant_Locator.o \
	StringDef_i.o \
	StructDef_i.o \
	tmplinst.o \
	TypedefDef_i.o \
	UnionDef_i.o \
	UsesDef_i.o \
	ValueBoxDef_i.o \
	ValueDef_i.o \
	ValueMemberDef_i.o \
	WstringDef_i.o

BIN_UNCHECKED = IFR_Service

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

ifeq ($(interface_repo),1)
  BIN = $(BIN_UNCHECKED)
endif # interface_repo

include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------


LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs

IFR_Service: $(addprefix $(VDIR),$(IFR_SVR_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


