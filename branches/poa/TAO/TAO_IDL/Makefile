#----------------------------------------------------------------------------
#	$Id$
#
#	Top-level Makefile for the TAO IDL compiler
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN   	= tao_idl
LSRC   	= $(addsuffix .cpp,$(BIN))
LDLIBS = -ldrv -lfe -last -ltao_idl_util -lnarrow -lbe -lACE
BUILD 	= $(BIN)

DIRS	= driver fe ast util narrow be

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
ifndef VXWORKS
  include $(ACE_ROOT)/include/makeinclude/macros.GNU
  include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
  include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
  include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
  include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
endif # VXWORKS


#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

ifdef VXWORKS
	@echo The IDL compiler won\'t run or build for VxWorks.
else

IDL_CFE_VERSION	= \"1.3.0\"
CPPFLAGS += -Iinclude -Ibe_include -DCPP_LOCATION=\"$(CXX)\" \
            -DIDL_CFE_VERSION=$(IDL_CFE_VERSION)

clean:
	rm -f tao_idl

realclean:
	rm -f tao_idl TAGS

endif # VXWORKS

tags:
	rm -f TAGS
	etags -C include/*.h be_include/*.h
	etags -a ast/*.cpp util/*.cpp narrow/*.cpp \
		driver/*.cpp fe/*.cpp be/*.cpp

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/tao_idl.o .shobj/tao_idl.: tao_idl.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Version.h \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h include/utl_scope.h \
 include/ast_type.h include/ast_concrete_type.h \
 include/ast_predefined_type.h include/ast_module.h include/ast_root.h \
 include/ast_interface.h include/ast_interface_fwd.h \
 include/ast_structure.h include/ast_exception.h include/ast_enum.h \
 include/ast_operation.h include/utl_list.h include/ast_field.h \
 include/ast_argument.h include/ast_attribute.h include/ast_union.h \
 include/ast_union_branch.h include/ast_union_label.h \
 include/ast_constant.h include/ast_enum_val.h include/ast_array.h \
 include/ast_sequence.h include/ast_string.h include/ast_typedef.h \
 include/ast_native.h include/utl_strlist.h include/utl_exprlist.h \
 include/ast_generator.h include/util.h include/utl_stack.h \
 include/utl_idlist.h include/utl_exceptlist.h include/utl_namelist.h \
 include/utl_labellist.h include/utl_decllist.h include/idl_global.h \
 include/utl_error.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 include/drv_private.h include/drv_link.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
