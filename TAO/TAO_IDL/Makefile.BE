#----------------------------------------------------------------------------
#       $Id$
#
#       Makefile for the TAO IDL compiler C++ back end library
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

LDLIBS = -lTAO_IDL_FE_DLL

VPATH = .:be

LIBNAME = libTAO_IDL_BE_DLL
LIB = $(LIBNAME).a
SHLIB = $(LIBNAME).$(SOEXT)

FILES = be/be_array \
        be/be_argument \
        be/be_attribute \
        be/be_constant \
        be/be_enum \
        be/be_enum_val \
        be/be_exception \
        be/be_expression \
        be/be_field \
	be/be_global \
        be/be_interface \
	be/be_interface_strategy \
        be/be_interface_fwd \
        be/be_valuetype \
        be/be_valuetype_fwd\
        be/be_module \
        be/be_native \
        be/be_operation \
	be/be_operation_strategy \
        be/be_predefined_type \
        be/be_root \
        be/be_sequence \
        be/be_string \
        be/be_structure \
        be/be_type \
        be/be_typedef \
        be/be_union \
        be/be_union_branch \
        be/be_union_label \
        be/be_generator \
        be/be_produce \
        be/be_helper \
        be/be_codegen \
        be/be_sunsoft \
        be/be_decl \
        be/be_scope \
        be/be_stream_factory \
        be/be_visitor_factory \
        be/be_visitor \
        be/be_visitor_ami_pre_proc \
        be/be_visitor_argument \
        be/be_visitor_array \
        be/be_visitor_attribute \
        be/be_visitor_constant \
        be/be_visitor_context \
        be/be_visitor_decl \
        be/be_visitor_enum \
        be/be_visitor_exception \
        be/be_visitor_field \
        be/be_visitor_interface \
        be/be_visitor_interface_fwd \
        be/be_visitor_valuetype \
        be/be_visitor_valuetype_fwd \
        be/be_visitor_module \
        be/be_visitor_operation \
        be/be_visitor_root \
        be/be_visitor_scope \
        be/be_visitor_sequence \
        be/be_visitor_structure \
        be/be_visitor_typecode \
        be/be_visitor_typedef \
        be/be_visitor_union \
        be/be_visitor_union_branch

LSRC = $(addsuffix .cpp,$(FILES))

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

CPPFLAGS += -Iinclude -Ibe_include -Ife

realclean: clean

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

