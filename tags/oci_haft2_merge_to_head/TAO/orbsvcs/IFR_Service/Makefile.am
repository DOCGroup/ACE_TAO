##---------------------------------------------------------------------------
##       $Id$
##
##       Makefile for TAO's Interface Repository Service
##
##---------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_builddir) -I$(top_srcdir)

bin_PROGRAMS = IFR_Service

IFR_Service_LDADD = \
	$(top_builddir)/ace/libACE.la $(top_builddir)/TAO/tao/libTAO.la

IFR_Service_SOURCES = \
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
	IFR_BaseS.cpp \
	IFR_BasicS.cpp \
	IFR_ComponentsS.cpp \
	IFR_ExtendedS.cpp \
	IFR_Server.cpp \
	IFR_Service.cpp \
	InterfaceDef_i.cpp \
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

noinst_HEADERS = \
        AliasDef_i.h \
        ArrayDef_i.h \
        AttributeDef_i.h \
        ComponentDef_i.h \
	ComponentRepository_i.h \
	concrete_classes.h \
	ConstantDef_i.h \
	ConsumesDef_i.h \
	Contained_i.h \
	Container_i.h \
	EmitsDef_i.h \
	EnumDef_i.h \
	EventDef_i.h \
	ExceptionDef_i.h \
	FactoryDef_i.h \
	FinderDef_i.h \
	FixedDef_i.h \
	HomeDef_i.h \
	IDLType_i.h \
	IFR_BaseS.h \
	IFR_BasicS.h \
	IFR_ComponentsS.h \
	IFR_ExtendedS.h \
	IFR_Service.h \
	InterfaceDef_i.h \
	InterfaceS_T.h \
	IRObject_i.h \
	ModuleDef_i.h \
	NativeDef_i.cphp \
	OperationDef_i.h \
	Options.h \
	PrimaryKeyDef_i.h \
	PrimitiveDef_i.h \
	ProvidesDef_i.h \
	PublishesDef_i.h \
	Repository_i.h \
	SequenceDef_i.h \
	StringDef_i.h \
	StructDef_i.h \
	TypedefDef_i.h \
	UnionDef_i.h \
	UsesDef_i.h \
	ValueBoxDef_i.h \
	ValueDef_i.h \
	ValueMemberDef_i.h \
	WstringDef_i.h

## EXTRA_DIST = README

## Clean up some additional files/directories possibly created during
## the configure script tests.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -rf ptrepository Templates.DB gcctemp.c gcctemp so_locations

