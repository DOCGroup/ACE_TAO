#----------------------------------------------------------------------------
#	$Id$
#
#	Makefile for libbe
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIBNAME	 = libbe
LIB	 = $(LIBNAME).a
SHLIB	 = $(LIBNAME).$(SOEXT)
IDL_CFE_VERSION	= \"1.3.0\"

# These are components that are sources of
BE_FILES	= \
		be_array \
		be_argument \
		be_attribute \
		be_constant \
		be_enum \
		be_enum_val \
		be_exception \
		be_expression \
		be_field \
		be_interface \
		be_interface_fwd \
		be_module \
		be_native \
		be_operation \
		be_predefined_type \
		be_root \
		be_sequence \
		be_string \
		be_structure \
		be_type \
		be_typedef \
		be_union \
		be_union_branch \
		be_union_label \
		be_generator \
		be_init \
		be_produce \
		be_args  \
		be_helper \
		be_state \
		be_state_array \
		be_state_argument \
		be_state_attribute \
		be_state_exception \
		be_state_operation \
		be_state_sequence \
		be_state_structure \
		be_state_typedef \
		be_state_union \
		be_codegen \
		be_factory \
		be_sunsoft \
		be_decl \
		be_scope \
		be_visitor \
		be_visitor_args \
		be_visitor_attribute \
		be_visitor_constant \
		be_visitor_context \
		be_visitor_decl \
		be_visitor_enum \
		be_visitor_exception \
		be_visitor_field \
		be_visitor_interface \
		be_visitor_interface_fwd \
		be_visitor_module \
		be_visitor_operation \
		be_visitor_root \
		be_visitor_scope \
		be_visitor_sequence \
		be_visitor_structure \
		be_visitor_typedef \
		be_visitor_union \
		be_visitor_union_branch \
		be_interpretive

FILES	 = $(BE_FILES)

LSRC	= $(addsuffix .cpp,$(FILES))
LOBJ    = $(addsuffix .o,$(FILES))
SHOBJ   = $(addsuffix .so,$(FILES))


LDLIBS  =  -lACE
LIBS	= -lACE
VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD	= $(VSHLIB) #$(VLIB)

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Local targets (and local hacks)
#----------------------------------------------------------------------------

ifeq ($(CXX),g++)
CCFLAGS += -Wno-switch
PIC = -fPIC
endif

CPPFLAGS += -I../include -I../be_include -I. -DCPP_LOCATION=\"$(CXX)\" \
            -DIDL_CFE_VERSION=$(IDL_CFE_VERSION)

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/be_array.o .obj/be_array.so .shobj/be_array.o .shobj/be_array.so: be_array.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h
.obj/be_argument.o .obj/be_argument.so .shobj/be_argument.o .shobj/be_argument.so: be_argument.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h
.obj/be_attribute.o .obj/be_attribute.so .shobj/be_attribute.o .shobj/be_attribute.so: be_attribute.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h
.obj/be_constant.o .obj/be_constant.so .shobj/be_constant.o .shobj/be_constant.so: be_constant.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h
.obj/be_enum.o .obj/be_enum.so .shobj/be_enum.o .shobj/be_enum.so: be_enum.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h
.obj/be_enum_val.o .obj/be_enum_val.so .shobj/be_enum_val.o .shobj/be_enum_val.so: be_enum_val.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h
.obj/be_exception.o .obj/be_exception.so .shobj/be_exception.o .shobj/be_exception.so: be_exception.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h
.obj/be_expression.o .obj/be_expression.so .shobj/be_expression.o .shobj/be_expression.so: be_expression.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h
.obj/be_field.o .obj/be_field.so .shobj/be_field.o .shobj/be_field.so: be_field.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h
.obj/be_interface.o .obj/be_interface.so .shobj/be_interface.o .shobj/be_interface.so: be_interface.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be_visitor_interface.h \
 ../be_include/be_visitor_scope.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_decl.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor_context.h
.obj/be_interface_fwd.o .obj/be_interface_fwd.so .shobj/be_interface_fwd.o .shobj/be_interface_fwd.so: be_interface_fwd.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h
.obj/be_module.o .obj/be_module.so .shobj/be_module.o .shobj/be_module.so: be_module.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h
.obj/be_native.o .obj/be_native.so .shobj/be_native.o .shobj/be_native.so: be_native.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h
.obj/be_operation.o .obj/be_operation.so .shobj/be_operation.o .shobj/be_operation.so: be_operation.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h
.obj/be_predefined_type.o .obj/be_predefined_type.so .shobj/be_predefined_type.o .shobj/be_predefined_type.so: be_predefined_type.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h
.obj/be_root.o .obj/be_root.so .shobj/be_root.o .shobj/be_root.so: be_root.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h
.obj/be_sequence.o .obj/be_sequence.so .shobj/be_sequence.o .shobj/be_sequence.so: be_sequence.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h \
 ../be_include/be_visitor_sequence.h ../be_include/be_visitor_decl.h
.obj/be_string.o .obj/be_string.so .shobj/be_string.o .shobj/be_string.so: be_string.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h
.obj/be_structure.o .obj/be_structure.so .shobj/be_structure.o .shobj/be_structure.so: be_structure.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h
.obj/be_type.o .obj/be_type.so .shobj/be_type.o .shobj/be_type.so: be_type.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h
.obj/be_typedef.o .obj/be_typedef.so .shobj/be_typedef.o .shobj/be_typedef.so: be_typedef.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h
.obj/be_union.o .obj/be_union.so .shobj/be_union.o .shobj/be_union.so: be_union.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h
.obj/be_union_branch.o .obj/be_union_branch.so .shobj/be_union_branch.o .shobj/be_union_branch.so: be_union_branch.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h
.obj/be_union_label.o .obj/be_union_label.so .shobj/be_union_label.o .shobj/be_union_label.so: be_union_label.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h
.obj/be_generator.o .obj/be_generator.so .shobj/be_generator.o .shobj/be_generator.so: be_generator.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h
.obj/be_init.o .obj/be_init.so .shobj/be_init.o .shobj/be_init.so: be_init.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h
.obj/be_produce.o .obj/be_produce.so .shobj/be_produce.o .shobj/be_produce.so: be_produce.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h ../be_include/be_interpretive.h
.obj/be_args.o .obj/be_args.so .shobj/be_args.o .shobj/be_args.so: be_args.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h
.obj/be_helper.o .obj/be_helper.so .shobj/be_helper.o .shobj/be_helper.so: be_helper.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h
.obj/be_state.o .obj/be_state.so .shobj/be_state.o .shobj/be_state.so: be_state.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h
.obj/be_state_array.o .obj/be_state_array.so .shobj/be_state_array.o .shobj/be_state_array.so: be_state_array.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h
.obj/be_state_argument.o .obj/be_state_argument.so .shobj/be_state_argument.o .shobj/be_state_argument.so: be_state_argument.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h
.obj/be_state_attribute.o .obj/be_state_attribute.so .shobj/be_state_attribute.o .shobj/be_state_attribute.so: be_state_attribute.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h
.obj/be_state_exception.o .obj/be_state_exception.so .shobj/be_state_exception.o .shobj/be_state_exception.so: be_state_exception.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h
.obj/be_state_operation.o .obj/be_state_operation.so .shobj/be_state_operation.o .shobj/be_state_operation.so: be_state_operation.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h
.obj/be_state_sequence.o .obj/be_state_sequence.so .shobj/be_state_sequence.o .shobj/be_state_sequence.so: be_state_sequence.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h
.obj/be_state_structure.o .obj/be_state_structure.so .shobj/be_state_structure.o .shobj/be_state_structure.so: be_state_structure.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h
.obj/be_state_typedef.o .obj/be_state_typedef.so .shobj/be_state_typedef.o .shobj/be_state_typedef.so: be_state_typedef.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h
.obj/be_state_union.o .obj/be_state_union.so .shobj/be_state_union.o .shobj/be_state_union.so: be_state_union.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h
.obj/be_codegen.o .obj/be_codegen.so .shobj/be_codegen.o .shobj/be_codegen.so: be_codegen.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h
.obj/be_factory.o .obj/be_factory.so .shobj/be_factory.o .shobj/be_factory.so: be_factory.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h ../be_include/be_sunsoft.h
.obj/be_sunsoft.o .obj/be_sunsoft.so .shobj/be_sunsoft.o .shobj/be_sunsoft.so: be_sunsoft.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h ../be_include/be_sunsoft.h
.obj/be_decl.o .obj/be_decl.so .shobj/be_decl.o .shobj/be_decl.so: be_decl.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h
.obj/be_scope.o .obj/be_scope.so .shobj/be_scope.o .shobj/be_scope.so: be_scope.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h
.obj/be_visitor.o .obj/be_visitor.so .shobj/be_visitor.o .shobj/be_visitor.so: be_visitor.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h
.obj/be_visitor_args.o .obj/be_visitor_args.so .shobj/be_visitor_args.o .shobj/be_visitor_args.so: be_visitor_args.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h ../be_include/be_visitor_args.h \
 ../be_include/be_visitor_decl.h
.obj/be_visitor_constant.o .obj/be_visitor_constant.so .shobj/be_visitor_constant.o .shobj/be_visitor_constant.so: be_visitor_constant.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h \
 ../be_include/be_visitor_constant.h ../be_include/be_visitor_scope.h \
 ../be_include/be_visitor_decl.h
.obj/be_visitor_context.o .obj/be_visitor_context.so .shobj/be_visitor_context.o .shobj/be_visitor_context.so: be_visitor_context.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h
.obj/be_visitor_decl.o .obj/be_visitor_decl.so .shobj/be_visitor_decl.o .shobj/be_visitor_decl.so: be_visitor_decl.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h ../be_include/be_visitor_decl.h
.obj/be_visitor_enum.o .obj/be_visitor_enum.so .shobj/be_visitor_enum.o .shobj/be_visitor_enum.so: be_visitor_enum.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h ../be_include/be_visitor_enum.h \
 ../be_include/be_visitor_scope.h ../be_include/be_visitor_decl.h
.obj/be_visitor_field.o .obj/be_visitor_field.so .shobj/be_visitor_field.o .shobj/be_visitor_field.so: be_visitor_field.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h ../be_include/be_visitor_field.h \
 ../be_include/be_visitor_decl.h
.obj/be_visitor_interface.o .obj/be_visitor_interface.so .shobj/be_visitor_interface.o .shobj/be_visitor_interface.so: be_visitor_interface.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h \
 ../be_include/be_visitor_interface.h ../be_include/be_visitor_scope.h \
 ../be_include/be_visitor_decl.h
.obj/be_visitor_interface_fwd.o .obj/be_visitor_interface_fwd.so .shobj/be_visitor_interface_fwd.o .shobj/be_visitor_interface_fwd.so: be_visitor_interface_fwd.cpp \
 ../include/idl.h $(ACE_ROOT)/ace/ACE.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h \
 ../be_include/be_visitor_interface_fwd.h \
 ../be_include/be_visitor_scope.h ../be_include/be_visitor_decl.h
.obj/be_visitor_module.o .obj/be_visitor_module.so .shobj/be_visitor_module.o .shobj/be_visitor_module.so: be_visitor_module.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h ../be_include/be_visitor_module.h \
 ../be_include/be_visitor_scope.h ../be_include/be_visitor_decl.h
.obj/be_visitor_operation.o .obj/be_visitor_operation.so .shobj/be_visitor_operation.o .shobj/be_visitor_operation.so: be_visitor_operation.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h \
 ../be_include/be_visitor_operation.h ../be_include/be_visitor_scope.h \
 ../be_include/be_visitor_decl.h
.obj/be_visitor_root.o .obj/be_visitor_root.so .shobj/be_visitor_root.o .shobj/be_visitor_root.so: be_visitor_root.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h ../be_include/be_visitor_root.h \
 ../be_include/be_visitor_scope.h ../be_include/be_visitor_decl.h
.obj/be_visitor_scope.o .obj/be_visitor_scope.so .shobj/be_visitor_scope.o .shobj/be_visitor_scope.so: be_visitor_scope.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h ../be_include/be_visitor_scope.h \
 ../be_include/be_visitor_decl.h
.obj/be_visitor_sequence.o .obj/be_visitor_sequence.so .shobj/be_visitor_sequence.o .shobj/be_visitor_sequence.so: be_visitor_sequence.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h \
 ../be_include/be_visitor_sequence.h ../be_include/be_visitor_decl.h
.obj/be_visitor_structure.o .obj/be_visitor_structure.so .shobj/be_visitor_structure.o .shobj/be_visitor_structure.so: be_visitor_structure.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h \
 ../be_include/be_visitor_structure.h ../be_include/be_visitor_scope.h \
 ../be_include/be_visitor_decl.h
.obj/be_visitor_typedef.o .obj/be_visitor_typedef.so .shobj/be_visitor_typedef.o .shobj/be_visitor_typedef.so: be_visitor_typedef.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h ../be_include/be_visitor_typedef.h \
 ../be_include/be_visitor_scope.h ../be_include/be_visitor_decl.h
.obj/be_visitor_union.o .obj/be_visitor_union.so .shobj/be_visitor_union.o .shobj/be_visitor_union.so: be_visitor_union.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h ../be_include/be_visitor_union.h \
 ../be_include/be_visitor_scope.h ../be_include/be_visitor_decl.h
.obj/be_visitor_union_branch.o .obj/be_visitor_union_branch.so .shobj/be_visitor_union_branch.o .shobj/be_visitor_union_branch.so: be_visitor_union_branch.cpp \
 ../include/idl.h $(ACE_ROOT)/ace/ACE.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h \
 ../be_include/be_visitor_union_branch.h \
 ../be_include/be_visitor_decl.h
.obj/be_interpretive.o .obj/be_interpretive.so .shobj/be_interpretive.o .shobj/be_interpretive.so: be_interpretive.cpp ../include/idl.h \
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
 ../include/intlmacros.h ../include/idl_narrow.h \
 ../include/idl_defines.h ../include/idl_fwd.h ../include/idl_bool.h \
 ../include/utl_string.h ../include/utl_identifier.h ../include/ast.h \
 ../include/utl_scoped_name.h ../include/ast_decl.h \
 ../include/ast_expression.h ../include/utl_scope.h \
 ../include/ast_type.h ../include/ast_concrete_type.h \
 ../include/ast_predefined_type.h ../include/ast_module.h \
 ../include/ast_root.h ../include/ast_interface.h \
 ../include/ast_interface_fwd.h ../include/ast_structure.h \
 ../include/ast_exception.h ../include/ast_enum.h \
 ../include/ast_operation.h ../include/utl_list.h \
 ../include/ast_field.h ../include/ast_argument.h \
 ../include/ast_attribute.h ../include/ast_union.h \
 ../include/ast_union_branch.h ../include/ast_union_label.h \
 ../include/ast_constant.h ../include/ast_enum_val.h \
 ../include/ast_array.h ../include/ast_sequence.h \
 ../include/ast_string.h ../include/ast_typedef.h \
 ../include/ast_native.h ../include/utl_strlist.h \
 ../include/utl_exprlist.h ../include/ast_generator.h \
 ../include/util.h ../include/utl_stack.h ../include/utl_idlist.h \
 ../include/utl_exceptlist.h ../include/utl_namelist.h \
 ../include/utl_labellist.h ../include/utl_decllist.h \
 ../include/idl_global.h ../include/utl_error.h \
 ../include/utl_indenter.h ../include/idl_extern.h \
 ../include/fe_extern.h ../include/be_extern.h ../include/drv_extern.h \
 ../include/ast_extern.h ../include/nr_extern.h \
 ../include/global_extern.h ../be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 ../be_include/be_decl.h ../be_include/be_scope.h \
 ../be_include/be_generator.h ../be_include/be_type.h \
 ../be_include/be_predefined_type.h ../be_include/be_module.h \
 ../be_include/be_interface.h ../be_include/be_interface_fwd.h \
 ../be_include/be_structure.h ../be_include/be_exception.h \
 ../be_include/be_expression.h ../be_include/be_enum.h \
 ../be_include/be_operation.h ../be_include/be_field.h \
 ../be_include/be_argument.h ../be_include/be_attribute.h \
 ../be_include/be_union.h ../be_include/be_union_branch.h \
 ../be_include/be_union_label.h ../be_include/be_constant.h \
 ../be_include/be_enum_val.h ../be_include/be_array.h \
 ../be_include/be_sequence.h ../be_include/be_string.h \
 ../be_include/be_typedef.h ../be_include/be_root.h \
 ../be_include/be_native.h ../be_include/be_helper.h \
 ../be_include/be_state.h ../be_include/be_codegen.h \
 ../be_include/be_factory.h ../be_include/be_visitor.h \
 ../be_include/be_visitor_context.h ../be_include/be_interpretive.h \
 ../be_include/be_visitor_args.h ../be_include/be_visitor_decl.h \
 ../be_include/be_visitor_constant.h ../be_include/be_visitor_scope.h \
 ../be_include/be_visitor_enum.h ../be_include/be_visitor_field.h \
 ../be_include/be_visitor_interface.h \
 ../be_include/be_visitor_interface_fwd.h \
 ../be_include/be_visitor_module.h \
 ../be_include/be_visitor_operation.h ../be_include/be_visitor_root.h \
 ../be_include/be_visitor_sequence.h \
 ../be_include/be_visitor_structure.h \
 ../be_include/be_visitor_typedef.h ../be_include/be_visitor_union.h \
 ../be_include/be_visitor_union_branch.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
