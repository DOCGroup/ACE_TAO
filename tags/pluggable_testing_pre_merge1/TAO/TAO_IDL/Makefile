#----------------------------------------------------------------------------
#       $Id$
#
#       Top-level Makefile for the TAO IDL compiler
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

BIN     = tao_idl
LDLIBS = -lACE
BUILD   = $(BIN)

VPATH=.:driver:fe:ast:util:narrow:be

FILES = tao_idl \
	driver/drv_init \
	driver/drv_private \
	driver/drv_args \
	driver/drv_fork \
	driver/drv_link \
	driver/drv_preproc \
	fe/fe_declarator \
	fe/fe_lookup	\
	fe/fe_private \
	fe/fe_init \
	fe/fe_extern \
	fe/fe_interface_header \
	fe/y.tab \
	fe/lex.yy \
	ast/ast_array \
	ast/ast_argument \
	ast/ast_attribute \
	ast/ast_check \
	ast/ast_concrete_type \
	ast/ast_constant \
	ast/ast_decl \
	ast/ast_enum \
	ast/ast_enum_val \
	ast/ast_exception \
	ast/ast_expression \
	ast/ast_field \
	ast/ast_interface \
	ast/ast_interface_fwd \
	ast/ast_module \
	ast/ast_native \
	ast/ast_operation \
	ast/ast_predefined_type \
	ast/ast_root \
	ast/ast_sequence \
	ast/ast_string \
	ast/ast_structure \
	ast/ast_type \
	ast/ast_typedef \
	ast/ast_union \
	ast/ast_union_branch \
	ast/ast_union_label \
	ast/ast_generator \
	ast/ast_redef \
	ast/ast_recursive \
	util/utl_scope \
	util/utl_stack \
	util/utl_string \
	util/utl_decllist \
	util/utl_labellist \
	util/utl_namelist \
	util/utl_exceptlist \
	util/utl_strlist \
	util/utl_list \
	util/utl_exprlist \
	util/utl_error \
	util/utl_indenter \
	util/utl_identifier \
	util/utl_idlist \
	util/utl_global \
	narrow/narrow \
	be/be_array \
	be/be_argument \
	be/be_attribute \
	be/be_constant \
	be/be_enum \
	be/be_enum_val \
	be/be_exception \
	be/be_expression \
	be/be_field \
	be/be_interface \
	be/be_interface_fwd \
	be/be_valuetype \
	be/be_valuetype_fwd\
	be/be_module \
	be/be_native \
	be/be_operation \
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
	be/be_init \
	be/be_produce \
	be/be_args  \
	be/be_helper \
	be/be_codegen \
	be/be_sunsoft \
	be/be_decl \
	be/be_scope \
	be/be_stream_factory \
	be/be_visitor_factory \
	be/be_visitor \
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

SRC     = $(addsuffix .cpp,$(FILES))
#VOBJS = $(LSRC:%.cpp=$(VDIR)%.$(OBJEXT))
#$(BIN): $(VOBJS)

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

ifeq ($(CROSS-COMPILE),)
  include $(ACE_ROOT)/include/makeinclude/macros.GNU
  include $(TAO_ROOT)/rules.tao.GNU
  include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
  include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
  include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
  include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
  include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
endif # ! CROSS-COMPILE

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

ifneq ($(CROSS-COMPILE),)
  all default_embedded:
	@echo The IDL compiler won\'t run or build on CROSS-COMPILE platforms.

  clean realclean:
	@echo Nothing to clean in TAO_IDL on CROSS-COMPILE platforms.
else  # ! CROSS-COMPILE

  ifeq ($(findstring g++,$(CXX)),g++)
    CCFLAGS += -Wno-switch
  endif # g++

  CPPFLAGS += -Iinclude -Ibe_include -Ife \
              -DCPP_LOCATION=\"$(strip $(CXX))\"

  realclean:
	rm -f tao_idl TAGS
endif # ! CROSS-COMPILE

fe/fe_lookup.cpp: fe/keywords.dat
	rm -f fe/fe_lookup.cpp
	echo "// \$$Id\$$" > fe/fe_lookup.cpp
	echo "#include	\"idl.h\"" >> fe/fe_lookup.cpp
	echo "#include	\"idl_extern.h\"" >> fe/fe_lookup.cpp
	echo "#include	\"fe_private.h\"" >> fe/fe_lookup.cpp
	$(ACE_ROOT)/bin/gperf -M -J -c -C -D -E -T -f 0 -a -o -t -p -K keyword_  \
			-L C++ -Z TAO_IDL_CPP_Keyword_Table -N lookup -k1,2,$$ fe/keywords.dat \
			 >> fe/fe_lookup.cpp

fe/lex.yy.cpp:	fe/idl.ll
	@echo "WARNING: Potentially outdated fe/lex.yy.cpp"
	@echo "         either touch(1) the file or explicitly"
	@echo "         generate it using:"
	@echo "    make lex.yy.cpp.target"

.PHONY: lex.yy.cpp.target

# For DOC group use
YACC = /home/cs/faculty/schmidt/bin/SunOS5/yacc

lex.yy.cpp.target:
		$(LEX) -t fe/idl.ll | \
		sed -e "s/yy/tao_yy/g" \
		    -e "s/YY/TAO_YY/g" \
		    -e "s@#include <unistd\.h>@#include \"ace/OS.h\"@" \
		    -e "/#include </d" \
		    -e "s@ECHO@TAO_YY_ECHO@" \
		    -e 's/\$$Hea''der.*\$$/$$I''d$$/' > fe/lex.yy.cpp
		(cd fe ; patch < lex.yy.cpp.diff)

fe/y.tab.cpp:	fe/idl.yy
	@echo "WARNING: Potentially outdated fe/y.tab.cpp"
	@echo "         either touch(1) the file or explicitly"
	@echo "         generate it using:"
	@echo "    make y.tab"

.PHONY: y.tab

y.tab: fe/idl.yy
		$(YACC) -d fe/idl.yy
		sed -e "s/char \*getenv/char *ace_foo/g" \
		    -e "s/= getenv/= ACE_OS::getenv/g" \
		    -e "s/yynewerror://g" \
		    -e "s/yy/tao_yy/g" \
		    -e "s/YY/TAO_YY/g" \
		    -e "s/y\.tab\.c/y.tab.cpp/g" \
		    -e "s%\(#pragma ident\)%// \1%" < y.tab.c > /tmp/$(USER).Y
		sed -e "s/yy/tao_yy/g" \
		    -e "s/YY/TAO_YY/g" < y.tab.h > /tmp/$(USER).YH
		echo '// $$I''d$$' > fe/y.tab.cpp
		cat /tmp/$(USER).Y >> fe/y.tab.cpp
		echo '// $$I''d$$' > fe/y.tab.h
		cat /tmp/$(USER).YH >> fe/y.tab.h
		$(RM) -f /tmp/$(USER).Y /tmp/$(USER).YH y.tab.c y.tab.h
		(cd fe ; patch < y.tab.cpp.diff)

tags:
	rm -f TAGS
	find . -name "*.h" -print | etags -a --c++ -
	find . -name "*.cpp" -print | etags -a -


#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/tao_idl.o .obj/tao_idl.so .shobj/tao_idl.o .shobj/tao_idl.so: tao_idl.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
.obj/drv_init.o .obj/drv_init.so .shobj/drv_init.o .shobj/drv_init.so: driver/drv_init.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
.obj/drv_private.o .obj/drv_private.so .shobj/drv_private.o .shobj/drv_private.so: driver/drv_private.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/drv_private.h
.obj/drv_args.o .obj/drv_args.so .shobj/drv_args.o .shobj/drv_args.so: driver/drv_args.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/drv_private.h include/drv_link.h be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h
.obj/drv_fork.o .obj/drv_fork.so .shobj/drv_fork.o .shobj/drv_fork.so: driver/drv_fork.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/drv_private.h \
 $(ACE_ROOT)/ace/Process_Manager.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 $(ACE_ROOT)/ace/Process_Manager.i
.obj/drv_link.o .obj/drv_link.so .shobj/drv_link.o .shobj/drv_link.so: driver/drv_link.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
.obj/drv_preproc.o .obj/drv_preproc.so .shobj/drv_preproc.o .shobj/drv_preproc.so: driver/drv_preproc.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/drv_private.h include/drv_link.h \
 $(ACE_ROOT)/ace/Process_Manager.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 $(ACE_ROOT)/ace/Process_Manager.i \
 $(ACE_ROOT)/ace/Env_Value_T.h \
 $(ACE_ROOT)/ace/Env_Value_T.i \
 $(ACE_ROOT)/ace/Env_Value_T.cpp
.obj/fe_declarator.o .obj/fe_declarator.so .shobj/fe_declarator.o .shobj/fe_declarator.so: fe/fe_declarator.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/fe_private.h include/fe_interface_header.h \
 include/fe_declarator.h
.obj/fe_lookup.o .obj/fe_lookup.so .shobj/fe_lookup.o .shobj/fe_lookup.so: fe/fe_lookup.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/fe_private.h include/fe_interface_header.h \
 include/fe_declarator.h
.obj/fe_private.o .obj/fe_private.so .shobj/fe_private.o .shobj/fe_private.so: fe/fe_private.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/fe_private.h include/fe_interface_header.h \
 include/fe_declarator.h
.obj/fe_init.o .obj/fe_init.so .shobj/fe_init.o .shobj/fe_init.so: fe/fe_init.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/fe_private.h include/fe_interface_header.h \
 include/fe_declarator.h
.obj/fe_extern.o .obj/fe_extern.so .shobj/fe_extern.o .shobj/fe_extern.so: fe/fe_extern.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/fe_private.h include/fe_interface_header.h \
 include/fe_declarator.h
.obj/fe_interface_header.o .obj/fe_interface_header.so .shobj/fe_interface_header.o .shobj/fe_interface_header.so: fe/fe_interface_header.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/fe_private.h include/fe_interface_header.h \
 include/fe_declarator.h
.obj/y.tab.o .obj/y.tab.so .shobj/y.tab.o .shobj/y.tab.so: fe/y.tab.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/fe_private.h include/fe_interface_header.h \
 include/fe_declarator.h
.obj/lex.yy.o .obj/lex.yy.so .shobj/lex.yy.o .shobj/lex.yy.so: fe/lex.yy.cpp \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
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
 $(ACE_ROOT)/ace/Log_Record.i \
 include/idl.h include/intlmacros.h include/idl_narrow.h \
 include/idl_defines.h include/idl_fwd.h include/idl_bool.h \
 include/utl_string.h include/utl_identifier.h include/ast.h \
 include/utl_scoped_name.h include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/fe_private.h include/fe_interface_header.h \
 include/fe_declarator.h fe/y.tab.h
.obj/ast_array.o .obj/ast_array.so .shobj/ast_array.o .shobj/ast_array.so: ast/ast_array.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/ast_argument.o .obj/ast_argument.so .shobj/ast_argument.o .shobj/ast_argument.so: ast/ast_argument.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/ast_attribute.o .obj/ast_attribute.so .shobj/ast_attribute.o .shobj/ast_attribute.so: ast/ast_attribute.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/ast_check.o .obj/ast_check.so .shobj/ast_check.o .shobj/ast_check.so: ast/ast_check.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/ast_concrete_type.o .obj/ast_concrete_type.so .shobj/ast_concrete_type.o .shobj/ast_concrete_type.so: ast/ast_concrete_type.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/ast_constant.o .obj/ast_constant.so .shobj/ast_constant.o .shobj/ast_constant.so: ast/ast_constant.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/ast_decl.o .obj/ast_decl.so .shobj/ast_decl.o .shobj/ast_decl.so: ast/ast_decl.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/ast_enum.o .obj/ast_enum.so .shobj/ast_enum.o .shobj/ast_enum.so: ast/ast_enum.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/ast_enum_val.o .obj/ast_enum_val.so .shobj/ast_enum_val.o .shobj/ast_enum_val.so: ast/ast_enum_val.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/ast_exception.o .obj/ast_exception.so .shobj/ast_exception.o .shobj/ast_exception.so: ast/ast_exception.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/ast_expression.o .obj/ast_expression.so .shobj/ast_expression.o .shobj/ast_expression.so: ast/ast_expression.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/ast_field.o .obj/ast_field.so .shobj/ast_field.o .shobj/ast_field.so: ast/ast_field.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/ast_interface.o .obj/ast_interface.so .shobj/ast_interface.o .shobj/ast_interface.so: ast/ast_interface.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/ast_interface_fwd.o .obj/ast_interface_fwd.so .shobj/ast_interface_fwd.o .shobj/ast_interface_fwd.so: ast/ast_interface_fwd.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/ast_module.o .obj/ast_module.so .shobj/ast_module.o .shobj/ast_module.so: ast/ast_module.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/ast_native.o .obj/ast_native.so .shobj/ast_native.o .shobj/ast_native.so: ast/ast_native.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/ast_operation.o .obj/ast_operation.so .shobj/ast_operation.o .shobj/ast_operation.so: ast/ast_operation.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/ast_predefined_type.o .obj/ast_predefined_type.so .shobj/ast_predefined_type.o .shobj/ast_predefined_type.so: ast/ast_predefined_type.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/ast_root.o .obj/ast_root.so .shobj/ast_root.o .shobj/ast_root.so: ast/ast_root.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/ast_sequence.o .obj/ast_sequence.so .shobj/ast_sequence.o .shobj/ast_sequence.so: ast/ast_sequence.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/ast_string.o .obj/ast_string.so .shobj/ast_string.o .shobj/ast_string.so: ast/ast_string.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/ast_structure.o .obj/ast_structure.so .shobj/ast_structure.o .shobj/ast_structure.so: ast/ast_structure.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/ast_type.o .obj/ast_type.so .shobj/ast_type.o .shobj/ast_type.so: ast/ast_type.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/ast_typedef.o .obj/ast_typedef.so .shobj/ast_typedef.o .shobj/ast_typedef.so: ast/ast_typedef.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/ast_union.o .obj/ast_union.so .shobj/ast_union.o .shobj/ast_union.so: ast/ast_union.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/ast_union_branch.o .obj/ast_union_branch.so .shobj/ast_union_branch.o .shobj/ast_union_branch.so: ast/ast_union_branch.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/ast_union_label.o .obj/ast_union_label.so .shobj/ast_union_label.o .shobj/ast_union_label.so: ast/ast_union_label.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/ast_generator.o .obj/ast_generator.so .shobj/ast_generator.o .shobj/ast_generator.so: ast/ast_generator.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/ast_redef.o .obj/ast_redef.so .shobj/ast_redef.o .shobj/ast_redef.so: ast/ast_redef.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/ast_recursive.o .obj/ast_recursive.so .shobj/ast_recursive.o .shobj/ast_recursive.so: ast/ast_recursive.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/utl_scope.o .obj/utl_scope.so .shobj/utl_scope.o .shobj/utl_scope.so: util/utl_scope.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/utl_stack.o .obj/utl_stack.so .shobj/utl_stack.o .shobj/utl_stack.so: util/utl_stack.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/utl_string.o .obj/utl_string.so .shobj/utl_string.o .shobj/utl_string.so: util/utl_string.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/utl_decllist.o .obj/utl_decllist.so .shobj/utl_decllist.o .shobj/utl_decllist.so: util/utl_decllist.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/utl_labellist.o .obj/utl_labellist.so .shobj/utl_labellist.o .shobj/utl_labellist.so: util/utl_labellist.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/utl_namelist.o .obj/utl_namelist.so .shobj/utl_namelist.o .shobj/utl_namelist.so: util/utl_namelist.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/utl_exceptlist.o .obj/utl_exceptlist.so .shobj/utl_exceptlist.o .shobj/utl_exceptlist.so: util/utl_exceptlist.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/utl_strlist.o .obj/utl_strlist.so .shobj/utl_strlist.o .shobj/utl_strlist.so: util/utl_strlist.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/utl_list.o .obj/utl_list.so .shobj/utl_list.o .shobj/utl_list.so: util/utl_list.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/utl_exprlist.o .obj/utl_exprlist.so .shobj/utl_exprlist.o .shobj/utl_exprlist.so: util/utl_exprlist.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/utl_error.o .obj/utl_error.so .shobj/utl_error.o .shobj/utl_error.so: util/utl_error.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/utl_indenter.o .obj/utl_indenter.so .shobj/utl_indenter.o .shobj/utl_indenter.so: util/utl_indenter.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/utl_identifier.o .obj/utl_identifier.so .shobj/utl_identifier.o .shobj/utl_identifier.so: util/utl_identifier.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/utl_idlist.o .obj/utl_idlist.so .shobj/utl_idlist.o .shobj/utl_idlist.so: util/utl_idlist.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/utl_global.o .obj/utl_global.so .shobj/utl_global.o .shobj/utl_global.so: util/utl_global.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/narrow.o .obj/narrow.so .shobj/narrow.o .shobj/narrow.so: narrow/narrow.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/ast_extern.h include/nr_extern.h include/global_extern.h
.obj/be_array.o .obj/be_array.so .shobj/be_array.o .shobj/be_array.so: be/be_array.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h
.obj/be_argument.o .obj/be_argument.so .shobj/be_argument.o .shobj/be_argument.so: be/be_argument.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h
.obj/be_attribute.o .obj/be_attribute.so .shobj/be_attribute.o .shobj/be_attribute.so: be/be_attribute.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h
.obj/be_constant.o .obj/be_constant.so .shobj/be_constant.o .shobj/be_constant.so: be/be_constant.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h
.obj/be_enum.o .obj/be_enum.so .shobj/be_enum.o .shobj/be_enum.so: be/be_enum.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h
.obj/be_enum_val.o .obj/be_enum_val.so .shobj/be_enum_val.o .shobj/be_enum_val.so: be/be_enum_val.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h
.obj/be_exception.o .obj/be_exception.so .shobj/be_exception.o .shobj/be_exception.so: be/be_exception.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h
.obj/be_expression.o .obj/be_expression.so .shobj/be_expression.o .shobj/be_expression.so: be/be_expression.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h
.obj/be_field.o .obj/be_field.so .shobj/be_field.o .shobj/be_field.so: be/be_field.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h
.obj/be_interface.o .obj/be_interface.so .shobj/be_interface.o .shobj/be_interface.so: be/be_interface.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be_visitor_interface.h be_include/be_visitor_scope.h \
 be_include/be_visitor.h be_include/be_visitor_decl.h \
 be_include/be_visitor_interface/interface.h \
 be_include/be_visitor_interface/interface_ch.h \
 be_include/be_visitor_interface/interface_ci.h \
 be_include/be_visitor_interface/interface_cs.h \
 be_include/be_visitor_interface/interface_sh.h \
 be_include/be_visitor_interface/interface_ih.h \
 be_include/be_visitor_interface/interface_si.h \
 be_include/be_visitor_interface/interface_ss.h \
 be_include/be_visitor_interface/interface_is.h \
 be_include/be_visitor_interface/collocated_sh.h \
 be_include/be_visitor_interface/collocated_ss.h \
 be_include/be_visitor_interface/any_op_ch.h \
 be_include/be_visitor_interface/any_op_cs.h \
 be_include/be_visitor_interface/cdr_op_ch.h \
 be_include/be_visitor_interface/cdr_op_cs.h \
 be_include/be_visitor_interface/tie_sh.h \
 be_include/be_visitor_interface/tie_si.h be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h
.obj/be_interface_fwd.o .obj/be_interface_fwd.so .shobj/be_interface_fwd.o .shobj/be_interface_fwd.so: be/be_interface_fwd.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h
.obj/be_valuetype.o .obj/be_valuetype.so .shobj/be_valuetype.o .shobj/be_valuetype.so: be/be_valuetype.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h
.obj/be_valuetype_fwd.o .obj/be_valuetype_fwd.so .shobj/be_valuetype_fwd.o .shobj/be_valuetype_fwd.so: be/be_valuetype_fwd.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h
.obj/be_module.o .obj/be_module.so .shobj/be_module.o .shobj/be_module.so: be/be_module.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h
.obj/be_native.o .obj/be_native.so .shobj/be_native.o .shobj/be_native.so: be/be_native.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h
.obj/be_operation.o .obj/be_operation.so .shobj/be_operation.o .shobj/be_operation.so: be/be_operation.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h
.obj/be_predefined_type.o .obj/be_predefined_type.so .shobj/be_predefined_type.o .shobj/be_predefined_type.so: be/be_predefined_type.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h
.obj/be_root.o .obj/be_root.so .shobj/be_root.o .shobj/be_root.so: be/be_root.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h
.obj/be_sequence.o .obj/be_sequence.so .shobj/be_sequence.o .shobj/be_sequence.so: be/be_sequence.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h \
 be_include/be_visitor_sequence.h be_include/be_visitor_decl.h \
 be_include/be_visitor_sequence/sequence_ch.h \
 be_include/be_visitor_sequence/sequence_ci.h \
 be_include/be_visitor_sequence/sequence_cs.h \
 be_include/be_visitor_sequence/sequence_base.h \
 be_include/be_visitor_sequence/buffer_type.h \
 be_include/be_visitor_sequence/elemtype.h \
 be_include/be_visitor_sequence/any_op_ch.h \
 be_include/be_visitor_sequence/any_op_cs.h \
 be_include/be_visitor_sequence/cdr_op_ch.h \
 be_include/be_visitor_sequence/cdr_op_cs.h
.obj/be_string.o .obj/be_string.so .shobj/be_string.o .shobj/be_string.so: be/be_string.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h
.obj/be_structure.o .obj/be_structure.so .shobj/be_structure.o .shobj/be_structure.so: be/be_structure.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h
.obj/be_type.o .obj/be_type.so .shobj/be_type.o .shobj/be_type.so: be/be_type.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h
.obj/be_typedef.o .obj/be_typedef.so .shobj/be_typedef.o .shobj/be_typedef.so: be/be_typedef.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h
.obj/be_union.o .obj/be_union.so .shobj/be_union.o .shobj/be_union.so: be/be_union.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h
.obj/be_union_branch.o .obj/be_union_branch.so .shobj/be_union_branch.o .shobj/be_union_branch.so: be/be_union_branch.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h
.obj/be_union_label.o .obj/be_union_label.so .shobj/be_union_label.o .shobj/be_union_label.so: be/be_union_label.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h
.obj/be_generator.o .obj/be_generator.so .shobj/be_generator.o .shobj/be_generator.so: be/be_generator.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h
.obj/be_init.o .obj/be_init.so .shobj/be_init.o .shobj/be_init.so: be/be_init.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h \
 be/../../tao/Version.h
.obj/be_produce.o .obj/be_produce.so .shobj/be_produce.o .shobj/be_produce.so: be/be_produce.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h
.obj/be_args.o .obj/be_args.so .shobj/be_args.o .shobj/be_args.so: be/be_args.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h
.obj/be_helper.o .obj/be_helper.so .shobj/be_helper.o .shobj/be_helper.so: be/be_helper.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h
.obj/be_codegen.o .obj/be_codegen.so .shobj/be_codegen.o .shobj/be_codegen.so: be/be_codegen.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h
.obj/be_sunsoft.o .obj/be_sunsoft.so .shobj/be_sunsoft.o .shobj/be_sunsoft.so: be/be_sunsoft.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h \
 be_include/be_sunsoft.h
.obj/be_decl.o .obj/be_decl.so .shobj/be_decl.o .shobj/be_decl.so: be/be_decl.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h
.obj/be_scope.o .obj/be_scope.so .shobj/be_scope.o .shobj/be_scope.so: be/be_scope.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h
.obj/be_stream_factory.o .obj/be_stream_factory.so .shobj/be_stream_factory.o .shobj/be_stream_factory.so: be/be_stream_factory.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h \
 be_include/be_sunsoft.h
.obj/be_visitor_factory.o .obj/be_visitor_factory.so .shobj/be_visitor_factory.o .shobj/be_visitor_factory.so: be/be_visitor_factory.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h \
 be_include/be_visitor_argument.h be_include/be_visitor_decl.h \
 be_include/be_visitor_scope.h \
 be_include/be_visitor_argument/argument.h \
 be_include/be_visitor_argument/arglist.h \
 be_include/be_visitor_argument/pre_docall_cs.h \
 be_include/be_visitor_argument/docall_cs.h \
 be_include/be_visitor_argument/post_docall_cs.h \
 be_include/be_visitor_argument/post_docall_compiled_cs.h \
 be_include/be_visitor_argument/vardecl_ss.h \
 be_include/be_visitor_argument/pre_upcall_ss.h \
 be_include/be_visitor_argument/upcall_ss.h \
 be_include/be_visitor_argument/post_upcall_ss.h \
 be_include/be_visitor_argument/marshal_ss.h \
 be_include/be_visitor_argument/post_marshal_ss.h \
 be_include/be_visitor_argument/compiled_marshal_cs.h \
 be_include/be_visitor_argument/compiled_marshal_ss.h \
 be_include/be_visitor_argument/pre_invoke_cs.h \
 be_include/be_visitor_array.h be_include/be_visitor_array/array.h \
 be_include/be_visitor_array/array_ch.h \
 be_include/be_visitor_array/array_ci.h \
 be_include/be_visitor_array/array_cs.h \
 be_include/be_visitor_array/any_op_ch.h \
 be_include/be_visitor_array/any_op_cs.h \
 be_include/be_visitor_array/cdr_op_ch.h \
 be_include/be_visitor_array/cdr_op_cs.h \
 be_include/be_visitor_attribute.h \
 be_include/be_visitor_attribute/attribute.h \
 be_include/be_visitor_constant.h \
 be_include/be_visitor_constant/constant_ch.h \
 be_include/be_visitor_constant/constant_cs.h \
 be_include/be_visitor_enum.h be_include/be_visitor_enum/enum_ch.h \
 be_include/be_visitor_enum/enum_cs.h \
 be_include/be_visitor_enum/any_op_ch.h \
 be_include/be_visitor_enum/any_op_cs.h \
 be_include/be_visitor_enum/cdr_op_ch.h \
 be_include/be_visitor_enum/cdr_op_cs.h \
 be_include/be_visitor_exception.h \
 be_include/be_visitor_exception/exception.h \
 be_include/be_visitor_exception/exception_ch.h \
 be_include/be_visitor_exception/exception_ci.h \
 be_include/be_visitor_exception/exception_cs.h \
 be_include/be_visitor_exception/ctor.h \
 be_include/be_visitor_exception/ctor_assign.h \
 be_include/be_visitor_exception/any_op_ch.h \
 be_include/be_visitor_exception/any_op_cs.h \
 be_include/be_visitor_exception/cdr_op_ch.h \
 be_include/be_visitor_exception/cdr_op_cs.h \
 be_include/be_visitor_field.h be_include/be_visitor_field/field_ch.h \
 be_include/be_visitor_field/field_ci.h \
 be_include/be_visitor_field/field_cs.h \
 be_include/be_visitor_field/cdr_op_ch.h \
 be_include/be_visitor_field/cdr_op_cs.h \
 be_include/be_visitor_interface.h \
 be_include/be_visitor_interface/interface.h \
 be_include/be_visitor_interface/interface_ch.h \
 be_include/be_visitor_interface/interface_ci.h \
 be_include/be_visitor_interface/interface_cs.h \
 be_include/be_visitor_interface/interface_sh.h \
 be_include/be_visitor_interface/interface_ih.h \
 be_include/be_visitor_interface/interface_si.h \
 be_include/be_visitor_interface/interface_ss.h \
 be_include/be_visitor_interface/interface_is.h \
 be_include/be_visitor_interface/collocated_sh.h \
 be_include/be_visitor_interface/collocated_ss.h \
 be_include/be_visitor_interface/any_op_ch.h \
 be_include/be_visitor_interface/any_op_cs.h \
 be_include/be_visitor_interface/cdr_op_ch.h \
 be_include/be_visitor_interface/cdr_op_cs.h \
 be_include/be_visitor_interface/tie_sh.h \
 be_include/be_visitor_interface/tie_si.h \
 be_include/be_visitor_interface_fwd.h \
 be_include/be_visitor_interface_fwd/interface_fwd_ch.h \
 be_include/be_visitor_interface_fwd/interface_fwd_ci.h \
 be_include/be_visitor_interface_fwd/cdr_op_ci.h \
 be_include/be_visitor_valuetype.h \
 be_include/be_visitor_valuetype/valuetype.h \
 be_include/be_visitor_valuetype/valuetype_ch.h \
 be_include/be_visitor_valuetype/valuetype_obv_ch.h \
 be_include/be_visitor_valuetype/valuetype_obv_cs.h \
 be_include/be_visitor_valuetype/valuetype_ci.h \
 be_include/be_visitor_valuetype/valuetype_cs.h \
 be_include/be_visitor_valuetype/cdr_op_ch.h \
 be_include/be_visitor_valuetype/cdr_op_cs.h \
 be_include/be_visitor_valuetype/marshal_ch.h \
 be_include/be_visitor_valuetype/marshal_cs.h \
 be_include/be_visitor_valuetype/arglist.h \
 be_include/be_visitor_valuetype/field_ch.h \
 be_include/be_visitor_valuetype/field_cs.h \
 be_include/be_visitor_valuetype/field_cdr_ci.h \
 be_include/be_visitor_valuetype/obv_module.h \
 be_include/be_visitor_valuetype_fwd.h \
 be_include/be_visitor_valuetype_fwd/valuetype_fwd_ch.h \
 be_include/be_visitor_valuetype_fwd/valuetype_fwd_ci.h \
 be_include/be_visitor_valuetype_fwd/cdr_op_ci.h \
 be_include/be_visitor_module.h be_include/be_visitor_module/module.h \
 be_include/be_visitor_module/module_ch.h \
 be_include/be_visitor_module/module_sh.h \
 be_include/be_visitor_module/module_ih.h \
 be_include/be_visitor_module/any_op.h \
 be_include/be_visitor_module/cdr_op.h \
 be_include/be_visitor_operation.h \
 be_include/be_visitor_operation/operation.h \
 be_include/be_visitor_operation/operation_ch.h \
 be_include/be_visitor_operation/operation_cs.h \
 be_include/be_visitor_operation/operation_sh.h \
 be_include/be_visitor_operation/operation_ih.h \
 be_include/be_visitor_operation/operation_ss.h \
 be_include/be_visitor_operation/operation_is.h \
 be_include/be_visitor_operation/tie_sh.h \
 be_include/be_visitor_operation/tie_si.h \
 be_include/be_visitor_operation/collocated_sh.h \
 be_include/be_visitor_operation/collocated_ss.h \
 be_include/be_visitor_operation/arglist.h \
 be_include/be_visitor_operation/rettype.h \
 be_include/be_visitor_operation/rettype_is.h \
 be_include/be_visitor_operation/rettype_vardecl_cs.h \
 be_include/be_visitor_operation/rettype_pre_docall_cs.h \
 be_include/be_visitor_operation/rettype_docall_cs.h \
 be_include/be_visitor_operation/rettype_post_docall_cs.h \
 be_include/be_visitor_operation/rettype_return_cs.h \
 be_include/be_visitor_operation/exceptlist_cs.h \
 be_include/be_visitor_operation/argument.h \
 be_include/be_visitor_operation/argument_invoke.h \
 be_include/be_visitor_operation/argument_marshal.h \
 be_include/be_visitor_operation/rettype_vardecl_ss.h \
 be_include/be_visitor_operation/rettype_assign_ss.h \
 be_include/be_visitor_operation/rettype_post_upcall_ss.h \
 be_include/be_visitor_operation/rettype_marshal_ss.h \
 be_include/be_visitor_operation/compiled_marshal.h \
 be_include/be_visitor_operation/rettype_pre_invoke_cs.h \
 be_include/be_visitor_root.h be_include/be_visitor_root/root.h \
 be_include/be_visitor_root/root_ch.h \
 be_include/be_visitor_root/root_ci.h \
 be_include/be_visitor_root/root_cs.h \
 be_include/be_visitor_root/root_sh.h \
 be_include/be_visitor_root/root_si.h \
 be_include/be_visitor_root/root_ss.h \
 be_include/be_visitor_root/root_is.h \
 be_include/be_visitor_root/root_ih.h \
 be_include/be_visitor_root/any_op.h \
 be_include/be_visitor_root/cdr_op.h be_include/be_visitor_sequence.h \
 be_include/be_visitor_sequence/sequence_ch.h \
 be_include/be_visitor_sequence/sequence_ci.h \
 be_include/be_visitor_sequence/sequence_cs.h \
 be_include/be_visitor_sequence/sequence_base.h \
 be_include/be_visitor_sequence/buffer_type.h \
 be_include/be_visitor_sequence/elemtype.h \
 be_include/be_visitor_sequence/any_op_ch.h \
 be_include/be_visitor_sequence/any_op_cs.h \
 be_include/be_visitor_sequence/cdr_op_ch.h \
 be_include/be_visitor_sequence/cdr_op_cs.h \
 be_include/be_visitor_structure.h \
 be_include/be_visitor_structure/structure.h \
 be_include/be_visitor_structure/structure_ch.h \
 be_include/be_visitor_structure/structure_ci.h \
 be_include/be_visitor_structure/structure_cs.h \
 be_include/be_visitor_structure/any_op_ch.h \
 be_include/be_visitor_structure/any_op_cs.h \
 be_include/be_visitor_structure/cdr_op_ch.h \
 be_include/be_visitor_structure/cdr_op_cs.h \
 be_include/be_visitor_typecode.h \
 be_include/be_visitor_typecode/typecode_decl.h \
 be_include/be_visitor_typecode/typecode_defn.h \
 be_include/be_visitor_typedef.h \
 be_include/be_visitor_typedef/typedef.h \
 be_include/be_visitor_typedef/typedef_ch.h \
 be_include/be_visitor_typedef/typedef_ci.h \
 be_include/be_visitor_typedef/typedef_cs.h \
 be_include/be_visitor_typedef/any_op_ch.h \
 be_include/be_visitor_typedef/any_op_cs.h \
 be_include/be_visitor_typedef/cdr_op_ch.h \
 be_include/be_visitor_typedef/cdr_op_cs.h \
 be_include/be_visitor_union.h be_include/be_visitor_union/union.h \
 be_include/be_visitor_union/union_ch.h \
 be_include/be_visitor_union/union_ci.h \
 be_include/be_visitor_union/union_cs.h \
 be_include/be_visitor_union/discriminant_ch.h \
 be_include/be_visitor_union/discriminant_ci.h \
 be_include/be_visitor_union/discriminant_cs.h \
 be_include/be_visitor_union/any_op_ch.h \
 be_include/be_visitor_union/any_op_cs.h \
 be_include/be_visitor_union/cdr_op_ch.h \
 be_include/be_visitor_union/cdr_op_cs.h \
 be_include/be_visitor_union_branch.h \
 be_include/be_visitor_union_branch/public_ch.h \
 be_include/be_visitor_union_branch/public_ci.h \
 be_include/be_visitor_union_branch/public_cs.h \
 be_include/be_visitor_union_branch/public_assign_cs.h \
 be_include/be_visitor_union_branch/public_reset_cs.h \
 be_include/be_visitor_union_branch/public_access_cs.h \
 be_include/be_visitor_union_branch/private_ch.h \
 be_include/be_visitor_union_branch/cdr_op_ch.h \
 be_include/be_visitor_union_branch/cdr_op_cs.h
.obj/be_visitor.o .obj/be_visitor.so .shobj/be_visitor.o .shobj/be_visitor.so: be/be_visitor.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/utl_error.h include/utl_indenter.h be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h
.obj/be_visitor_argument.o .obj/be_visitor_argument.so .shobj/be_visitor_argument.o .shobj/be_visitor_argument.so: be/be_visitor_argument.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 include/utl_error.h include/utl_indenter.h be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h \
 be_include/be_visitor_argument.h be_include/be_visitor_decl.h \
 be_include/be_visitor_scope.h \
 be_include/be_visitor_argument/argument.h \
 be_include/be_visitor_argument/arglist.h \
 be_include/be_visitor_argument/pre_docall_cs.h \
 be_include/be_visitor_argument/docall_cs.h \
 be_include/be_visitor_argument/post_docall_cs.h \
 be_include/be_visitor_argument/post_docall_compiled_cs.h \
 be_include/be_visitor_argument/vardecl_ss.h \
 be_include/be_visitor_argument/pre_upcall_ss.h \
 be_include/be_visitor_argument/upcall_ss.h \
 be_include/be_visitor_argument/post_upcall_ss.h \
 be_include/be_visitor_argument/marshal_ss.h \
 be_include/be_visitor_argument/post_marshal_ss.h \
 be_include/be_visitor_argument/compiled_marshal_cs.h \
 be_include/be_visitor_argument/compiled_marshal_ss.h \
 be_include/be_visitor_argument/pre_invoke_cs.h \
 be/be_visitor_argument/arglist.cpp \
 be/be_visitor_argument/argument.cpp \
 be/be_visitor_argument/docall_cs.cpp \
 be/be_visitor_argument/marshal_ss.cpp \
 be/be_visitor_argument/post_docall_cs.cpp \
 be/be_visitor_argument/post_docall_compiled_cs.cpp \
 be/be_visitor_argument/post_marshal_ss.cpp \
 be/be_visitor_argument/post_upcall_ss.cpp \
 be/be_visitor_argument/pre_docall_cs.cpp \
 be/be_visitor_argument/pre_upcall_ss.cpp \
 be/be_visitor_argument/upcall_ss.cpp \
 be/be_visitor_argument/vardecl_ss.cpp \
 be/be_visitor_argument/compiled_marshal_cs.cpp \
 be/be_visitor_argument/compiled_marshal_ss.cpp \
 be/be_visitor_argument/pre_invoke_cs.cpp
.obj/be_visitor_array.o .obj/be_visitor_array.so .shobj/be_visitor_array.o .shobj/be_visitor_array.so: be/be_visitor_array.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h \
 be_include/be_visitor_array.h be_include/be_visitor_decl.h \
 be_include/be_visitor_array/array.h \
 be_include/be_visitor_array/array_ch.h \
 be_include/be_visitor_array/array_ci.h \
 be_include/be_visitor_array/array_cs.h \
 be_include/be_visitor_array/any_op_ch.h \
 be_include/be_visitor_array/any_op_cs.h \
 be_include/be_visitor_array/cdr_op_ch.h \
 be_include/be_visitor_array/cdr_op_cs.h be/be_visitor_array/array.cpp \
 be/be_visitor_array/array_ch.cpp be/be_visitor_array/array_ci.cpp \
 be/be_visitor_array/array_cs.cpp be/be_visitor_array/any_op_ch.cpp \
 be/be_visitor_array/any_op_cs.cpp be/be_visitor_array/cdr_op_ch.cpp \
 be/be_visitor_array/cdr_op_cs.cpp
.obj/be_visitor_attribute.o .obj/be_visitor_attribute.so .shobj/be_visitor_attribute.o .shobj/be_visitor_attribute.so: be/be_visitor_attribute.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h \
 be_include/be_visitor_attribute.h be_include/be_visitor_scope.h \
 be_include/be_visitor_decl.h \
 be_include/be_visitor_attribute/attribute.h \
 be/be_visitor_attribute/attribute.cpp
.obj/be_visitor_constant.o .obj/be_visitor_constant.so .shobj/be_visitor_constant.o .shobj/be_visitor_constant.so: be/be_visitor_constant.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h \
 be_include/be_visitor_constant.h be_include/be_visitor_scope.h \
 be_include/be_visitor_decl.h \
 be_include/be_visitor_constant/constant_ch.h \
 be_include/be_visitor_constant/constant_cs.h \
 be/be_visitor_constant/constant_ch.cpp \
 be/be_visitor_constant/constant_cs.cpp
.obj/be_visitor_context.o .obj/be_visitor_context.so .shobj/be_visitor_context.o .shobj/be_visitor_context.so: be/be_visitor_context.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h
.obj/be_visitor_decl.o .obj/be_visitor_decl.so .shobj/be_visitor_decl.o .shobj/be_visitor_decl.so: be/be_visitor_decl.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h \
 be_include/be_visitor_decl.h
.obj/be_visitor_enum.o .obj/be_visitor_enum.so .shobj/be_visitor_enum.o .shobj/be_visitor_enum.so: be/be_visitor_enum.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h \
 be_include/be_visitor_enum.h be_include/be_visitor_scope.h \
 be_include/be_visitor_decl.h be_include/be_visitor_enum/enum_ch.h \
 be_include/be_visitor_enum/enum_cs.h \
 be_include/be_visitor_enum/any_op_ch.h \
 be_include/be_visitor_enum/any_op_cs.h \
 be_include/be_visitor_enum/cdr_op_ch.h \
 be_include/be_visitor_enum/cdr_op_cs.h be/be_visitor_enum/enum_ch.cpp \
 be/be_visitor_enum/enum_cs.cpp be/be_visitor_enum/any_op_ch.cpp \
 be/be_visitor_enum/any_op_cs.cpp be/be_visitor_enum/cdr_op_ch.cpp \
 be/be_visitor_enum/cdr_op_cs.cpp
.obj/be_visitor_exception.o .obj/be_visitor_exception.so .shobj/be_visitor_exception.o .shobj/be_visitor_exception.so: be/be_visitor_exception.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h \
 be_include/be_visitor_exception.h be_include/be_visitor_scope.h \
 be_include/be_visitor_decl.h \
 be_include/be_visitor_exception/exception.h \
 be_include/be_visitor_exception/exception_ch.h \
 be_include/be_visitor_exception/exception_ci.h \
 be_include/be_visitor_exception/exception_cs.h \
 be_include/be_visitor_exception/ctor.h \
 be_include/be_visitor_exception/ctor_assign.h \
 be_include/be_visitor_exception/any_op_ch.h \
 be_include/be_visitor_exception/any_op_cs.h \
 be_include/be_visitor_exception/cdr_op_ch.h \
 be_include/be_visitor_exception/cdr_op_cs.h \
 be/be_visitor_exception/exception.cpp \
 be/be_visitor_exception/exception_ch.cpp \
 be/be_visitor_exception/exception_ci.cpp \
 be/be_visitor_exception/exception_cs.cpp \
 be/be_visitor_exception/exception_ctor.cpp \
 be/be_visitor_exception/ctor_assign.cpp \
 be/be_visitor_exception/any_op_ch.cpp \
 be/be_visitor_exception/any_op_cs.cpp \
 be/be_visitor_exception/cdr_op_ch.cpp \
 be/be_visitor_exception/cdr_op_cs.cpp be_include/be_visitor_field.h \
 be_include/be_visitor_field/field_ch.h \
 be_include/be_visitor_field/field_ci.h \
 be_include/be_visitor_field/field_cs.h \
 be_include/be_visitor_field/cdr_op_ch.h \
 be_include/be_visitor_field/cdr_op_cs.h
.obj/be_visitor_field.o .obj/be_visitor_field.so .shobj/be_visitor_field.o .shobj/be_visitor_field.so: be/be_visitor_field.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h \
 be_include/be_visitor_field.h be_include/be_visitor_decl.h \
 be_include/be_visitor_field/field_ch.h \
 be_include/be_visitor_field/field_ci.h \
 be_include/be_visitor_field/field_cs.h \
 be_include/be_visitor_field/cdr_op_ch.h \
 be_include/be_visitor_field/cdr_op_cs.h be_include/be_visitor_scope.h \
 be/be_visitor_field/field_ch.cpp be/be_visitor_field/field_ci.cpp \
 be/be_visitor_field/field_cs.cpp be/be_visitor_field/cdr_op_ch.cpp \
 be/be_visitor_field/cdr_op_cs.cpp
.obj/be_visitor_interface.o .obj/be_visitor_interface.so .shobj/be_visitor_interface.o .shobj/be_visitor_interface.so: be/be_visitor_interface.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h \
 be_include/be_visitor_interface.h be_include/be_visitor_scope.h \
 be_include/be_visitor_decl.h \
 be_include/be_visitor_interface/interface.h \
 be_include/be_visitor_interface/interface_ch.h \
 be_include/be_visitor_interface/interface_ci.h \
 be_include/be_visitor_interface/interface_cs.h \
 be_include/be_visitor_interface/interface_sh.h \
 be_include/be_visitor_interface/interface_ih.h \
 be_include/be_visitor_interface/interface_si.h \
 be_include/be_visitor_interface/interface_ss.h \
 be_include/be_visitor_interface/interface_is.h \
 be_include/be_visitor_interface/collocated_sh.h \
 be_include/be_visitor_interface/collocated_ss.h \
 be_include/be_visitor_interface/any_op_ch.h \
 be_include/be_visitor_interface/any_op_cs.h \
 be_include/be_visitor_interface/cdr_op_ch.h \
 be_include/be_visitor_interface/cdr_op_cs.h \
 be_include/be_visitor_interface/tie_sh.h \
 be_include/be_visitor_interface/tie_si.h \
 be/be_visitor_interface/interface.cpp \
 be/be_visitor_interface/interface_ch.cpp \
 be/be_visitor_interface/interface_ci.cpp \
 be/be_visitor_interface/interface_cs.cpp \
 be/be_visitor_interface/interface_sh.cpp \
 be/be_visitor_interface/interface_ih.cpp \
 be/be_visitor_interface/interface_si.cpp \
 be/be_visitor_interface/interface_ss.cpp \
 be/be_visitor_interface/interface_is.cpp \
 be/be_visitor_interface/collocated_sh.cpp \
 be/be_visitor_interface/collocated_ss.cpp \
 be/be_visitor_interface/tie_sh.cpp be/be_visitor_interface/tie_si.cpp \
 be/be_visitor_interface/any_op_ch.cpp \
 be/be_visitor_interface/any_op_cs.cpp \
 be/be_visitor_interface/cdr_op_ch.cpp \
 be/be_visitor_interface/cdr_op_cs.cpp
.obj/be_visitor_interface_fwd.o .obj/be_visitor_interface_fwd.so .shobj/be_visitor_interface_fwd.o .shobj/be_visitor_interface_fwd.so: be/be_visitor_interface_fwd.cpp \
 include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h \
 be_include/be_visitor_interface_fwd.h be_include/be_visitor_scope.h \
 be_include/be_visitor_decl.h \
 be_include/be_visitor_interface_fwd/interface_fwd_ch.h \
 be_include/be_visitor_interface_fwd/interface_fwd_ci.h \
 be_include/be_visitor_interface_fwd/cdr_op_ci.h \
 be/be_visitor_interface_fwd/interface_fwd_ch.cpp \
 be/be_visitor_interface_fwd/interface_fwd_ci.cpp \
 be/be_visitor_interface_fwd/cdr_op_ci.cpp
.obj/be_visitor_valuetype.o .obj/be_visitor_valuetype.so .shobj/be_visitor_valuetype.o .shobj/be_visitor_valuetype.so: be/be_visitor_valuetype.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h \
 be_include/be_visitor_valuetype.h be_include/be_visitor_scope.h \
 be_include/be_visitor_decl.h \
 be_include/be_visitor_valuetype/valuetype.h \
 be_include/be_visitor_valuetype/valuetype_ch.h \
 be_include/be_visitor_valuetype/valuetype_obv_ch.h \
 be_include/be_visitor_valuetype/valuetype_obv_cs.h \
 be_include/be_visitor_valuetype/valuetype_ci.h \
 be_include/be_visitor_valuetype/valuetype_cs.h \
 be_include/be_visitor_valuetype/cdr_op_ch.h \
 be_include/be_visitor_valuetype/cdr_op_cs.h \
 be_include/be_visitor_valuetype/marshal_ch.h \
 be_include/be_visitor_valuetype/marshal_cs.h \
 be_include/be_visitor_valuetype/arglist.h \
 be_include/be_visitor_valuetype/field_ch.h \
 be_include/be_visitor_valuetype/field_cs.h \
 be_include/be_visitor_valuetype/field_cdr_ci.h \
 be_include/be_visitor_valuetype/obv_module.h \
 be/be_visitor_valuetype/valuetype.cpp \
 be/be_visitor_valuetype/valuetype_ch.cpp \
 be/be_visitor_valuetype/valuetype_obv_ch.cpp \
 be/be_visitor_valuetype/valuetype_obv_cs.cpp \
 be/be_visitor_valuetype/valuetype_ci.cpp \
 be/be_visitor_valuetype/valuetype_cs.cpp \
 be/be_visitor_valuetype/cdr_op_ch.cpp \
 be/be_visitor_valuetype/cdr_op_cs.cpp \
 be/be_visitor_valuetype/marshal_ch.cpp \
 be/be_visitor_valuetype/marshal_cs.cpp \
 be/be_visitor_valuetype/arglist.cpp be_include/be_visitor_operation.h \
 be_include/be_visitor_operation/operation.h \
 be_include/be_visitor_operation/operation_ch.h \
 be_include/be_visitor_operation/operation_cs.h \
 be_include/be_visitor_operation/operation_sh.h \
 be_include/be_visitor_operation/operation_ih.h \
 be_include/be_visitor_operation/operation_ss.h \
 be_include/be_visitor_operation/operation_is.h \
 be_include/be_visitor_operation/tie_sh.h \
 be_include/be_visitor_operation/tie_si.h \
 be_include/be_visitor_operation/collocated_sh.h \
 be_include/be_visitor_operation/collocated_ss.h \
 be_include/be_visitor_operation/arglist.h \
 be_include/be_visitor_operation/rettype.h \
 be_include/be_visitor_operation/rettype_is.h \
 be_include/be_visitor_operation/rettype_vardecl_cs.h \
 be_include/be_visitor_operation/rettype_pre_docall_cs.h \
 be_include/be_visitor_operation/rettype_docall_cs.h \
 be_include/be_visitor_operation/rettype_post_docall_cs.h \
 be_include/be_visitor_operation/rettype_return_cs.h \
 be_include/be_visitor_operation/exceptlist_cs.h \
 be_include/be_visitor_operation/argument.h \
 be_include/be_visitor_operation/argument_invoke.h \
 be_include/be_visitor_operation/argument_marshal.h \
 be_include/be_visitor_operation/rettype_vardecl_ss.h \
 be_include/be_visitor_operation/rettype_assign_ss.h \
 be_include/be_visitor_operation/rettype_post_upcall_ss.h \
 be_include/be_visitor_operation/rettype_marshal_ss.h \
 be_include/be_visitor_operation/compiled_marshal.h \
 be_include/be_visitor_operation/rettype_pre_invoke_cs.h \
 be/be_visitor_valuetype/field_ch.cpp \
 be/be_visitor_valuetype/field_cs.cpp \
 be/be_visitor_valuetype/field_cdr_ci.cpp \
 be_include/be_visitor_field.h be_include/be_visitor_field/field_ch.h \
 be_include/be_visitor_field/field_ci.h \
 be_include/be_visitor_field/field_cs.h \
 be_include/be_visitor_field/cdr_op_ch.h \
 be_include/be_visitor_field/cdr_op_cs.h \
 be/be_visitor_valuetype/obv_module.cpp
.obj/be_visitor_valuetype_fwd.o .obj/be_visitor_valuetype_fwd.so .shobj/be_visitor_valuetype_fwd.o .shobj/be_visitor_valuetype_fwd.so: be/be_visitor_valuetype_fwd.cpp \
 include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h \
 be_include/be_visitor_valuetype_fwd.h be_include/be_visitor_scope.h \
 be_include/be_visitor_decl.h \
 be_include/be_visitor_valuetype_fwd/valuetype_fwd_ch.h \
 be_include/be_visitor_valuetype_fwd/valuetype_fwd_ci.h \
 be_include/be_visitor_valuetype_fwd/cdr_op_ci.h
.obj/be_visitor_module.o .obj/be_visitor_module.so .shobj/be_visitor_module.o .shobj/be_visitor_module.so: be/be_visitor_module.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h \
 be_include/be_visitor_module.h be_include/be_visitor_scope.h \
 be_include/be_visitor_decl.h be_include/be_visitor_module/module.h \
 be_include/be_visitor_module/module_ch.h \
 be_include/be_visitor_module/module_sh.h \
 be_include/be_visitor_module/module_ih.h \
 be_include/be_visitor_module/any_op.h \
 be_include/be_visitor_module/cdr_op.h be/be_visitor_module/module.cpp \
 be/be_visitor_module/module_ch.cpp be/be_visitor_module/module_sh.cpp \
 be/be_visitor_module/module_ih.cpp be/be_visitor_module/any_op.cpp \
 be/be_visitor_module/cdr_op.cpp
.obj/be_visitor_operation.o .obj/be_visitor_operation.so .shobj/be_visitor_operation.o .shobj/be_visitor_operation.so: be/be_visitor_operation.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h \
 be_include/be_visitor_operation.h be_include/be_visitor_scope.h \
 be_include/be_visitor_decl.h \
 be_include/be_visitor_operation/operation.h \
 be_include/be_visitor_operation/operation_ch.h \
 be_include/be_visitor_operation/operation_cs.h \
 be_include/be_visitor_operation/operation_sh.h \
 be_include/be_visitor_operation/operation_ih.h \
 be_include/be_visitor_operation/operation_ss.h \
 be_include/be_visitor_operation/operation_is.h \
 be_include/be_visitor_operation/tie_sh.h \
 be_include/be_visitor_operation/tie_si.h \
 be_include/be_visitor_operation/collocated_sh.h \
 be_include/be_visitor_operation/collocated_ss.h \
 be_include/be_visitor_operation/arglist.h \
 be_include/be_visitor_operation/rettype.h \
 be_include/be_visitor_operation/rettype_is.h \
 be_include/be_visitor_operation/rettype_vardecl_cs.h \
 be_include/be_visitor_operation/rettype_pre_docall_cs.h \
 be_include/be_visitor_operation/rettype_docall_cs.h \
 be_include/be_visitor_operation/rettype_post_docall_cs.h \
 be_include/be_visitor_operation/rettype_return_cs.h \
 be_include/be_visitor_operation/exceptlist_cs.h \
 be_include/be_visitor_operation/argument.h \
 be_include/be_visitor_operation/argument_invoke.h \
 be_include/be_visitor_operation/argument_marshal.h \
 be_include/be_visitor_operation/rettype_vardecl_ss.h \
 be_include/be_visitor_operation/rettype_assign_ss.h \
 be_include/be_visitor_operation/rettype_post_upcall_ss.h \
 be_include/be_visitor_operation/rettype_marshal_ss.h \
 be_include/be_visitor_operation/compiled_marshal.h \
 be_include/be_visitor_operation/rettype_pre_invoke_cs.h \
 be/be_visitor_operation/operation.cpp \
 be/be_visitor_operation/arglist.cpp \
 be/be_visitor_operation/argument.cpp \
 be/be_visitor_operation/argument_invoke.cpp \
 be/be_visitor_operation/argument_marshal.cpp \
 be/be_visitor_operation/collocated_sh.cpp \
 be/be_visitor_operation/collocated_ss.cpp \
 be/be_visitor_operation/exceptlist_cs.cpp \
 be/be_visitor_operation/operation_ch.cpp \
 be/be_visitor_operation/operation_cs.cpp \
 be/be_visitor_operation/operation_sh.cpp \
 be/be_visitor_operation/operation_ih.cpp \
 be/be_visitor_operation/operation_ss.cpp \
 be_include/be_visitor_argument.h \
 be_include/be_visitor_argument/argument.h \
 be_include/be_visitor_argument/arglist.h \
 be_include/be_visitor_argument/pre_docall_cs.h \
 be_include/be_visitor_argument/docall_cs.h \
 be_include/be_visitor_argument/post_docall_cs.h \
 be_include/be_visitor_argument/post_docall_compiled_cs.h \
 be_include/be_visitor_argument/vardecl_ss.h \
 be_include/be_visitor_argument/pre_upcall_ss.h \
 be_include/be_visitor_argument/upcall_ss.h \
 be_include/be_visitor_argument/post_upcall_ss.h \
 be_include/be_visitor_argument/marshal_ss.h \
 be_include/be_visitor_argument/post_marshal_ss.h \
 be_include/be_visitor_argument/compiled_marshal_cs.h \
 be_include/be_visitor_argument/compiled_marshal_ss.h \
 be_include/be_visitor_argument/pre_invoke_cs.h \
 be/be_visitor_operation/operation_is.cpp \
 be/be_visitor_operation/rettype.cpp \
 be/be_visitor_operation/rettype_is.cpp \
 be/be_visitor_operation/rettype_assign_ss.cpp \
 be/be_visitor_operation/rettype_docall_cs.cpp \
 be/be_visitor_operation/rettype_marshal_ss.cpp \
 be/be_visitor_operation/rettype_post_docall_cs.cpp \
 be/be_visitor_operation/rettype_post_upcall_ss.cpp \
 be/be_visitor_operation/rettype_pre_docall_cs.cpp \
 be/be_visitor_operation/rettype_return_cs.cpp \
 be/be_visitor_operation/rettype_vardecl_cs.cpp \
 be/be_visitor_operation/rettype_vardecl_ss.cpp \
 be/be_visitor_operation/tie_sh.cpp be/be_visitor_operation/tie_si.cpp \
 be/be_visitor_operation/compiled_marshal.cpp \
 be/be_visitor_operation/rettype_pre_invoke_cs.cpp
.obj/be_visitor_root.o .obj/be_visitor_root.so .shobj/be_visitor_root.o .shobj/be_visitor_root.so: be/be_visitor_root.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h \
 be_include/be_visitor_root.h be_include/be_visitor_scope.h \
 be_include/be_visitor_decl.h be_include/be_visitor_root/root.h \
 be_include/be_visitor_root/root_ch.h \
 be_include/be_visitor_root/root_ci.h \
 be_include/be_visitor_root/root_cs.h \
 be_include/be_visitor_root/root_sh.h \
 be_include/be_visitor_root/root_si.h \
 be_include/be_visitor_root/root_ss.h \
 be_include/be_visitor_root/root_is.h \
 be_include/be_visitor_root/root_ih.h \
 be_include/be_visitor_root/any_op.h \
 be_include/be_visitor_root/cdr_op.h be/be_visitor_root/root.cpp \
 be/be_visitor_root/root_ch.cpp be/be_visitor_root/root_ci.cpp \
 be/be_visitor_root/root_cs.cpp be/be_visitor_root/root_sh.cpp \
 be/be_visitor_root/root_si.cpp be/be_visitor_root/root_ss.cpp \
 be/be_visitor_root/root_is.cpp be/be_visitor_root/root_ih.cpp \
 be/be_visitor_root/any_op.cpp be/be_visitor_root/cdr_op.cpp
.obj/be_visitor_scope.o .obj/be_visitor_scope.so .shobj/be_visitor_scope.o .shobj/be_visitor_scope.so: be/be_visitor_scope.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h
.obj/be_visitor_sequence.o .obj/be_visitor_sequence.so .shobj/be_visitor_sequence.o .shobj/be_visitor_sequence.so: be/be_visitor_sequence.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h \
 be_include/be_visitor_sequence.h be_include/be_visitor_decl.h \
 be_include/be_visitor_sequence/sequence_ch.h \
 be_include/be_visitor_sequence/sequence_ci.h \
 be_include/be_visitor_sequence/sequence_cs.h \
 be_include/be_visitor_sequence/sequence_base.h \
 be_include/be_visitor_sequence/buffer_type.h \
 be_include/be_visitor_sequence/elemtype.h \
 be_include/be_visitor_sequence/any_op_ch.h \
 be_include/be_visitor_sequence/any_op_cs.h \
 be_include/be_visitor_sequence/cdr_op_ch.h \
 be_include/be_visitor_sequence/cdr_op_cs.h \
 be/be_visitor_sequence/any_op_ch.cpp \
 be/be_visitor_sequence/any_op_cs.cpp \
 be/be_visitor_sequence/cdr_op_ch.cpp \
 be/be_visitor_sequence/cdr_op_cs.cpp \
 be/be_visitor_sequence/buffer_type.cpp \
 be/be_visitor_sequence/elemtype.cpp \
 be/be_visitor_sequence/sequence_base.cpp \
 be/be_visitor_sequence/sequence_ch.cpp \
 be/be_visitor_sequence/sequence_ci.cpp \
 be/be_visitor_sequence/sequence_cs.cpp \
 be/be_visitor_sequence/gen_unbounded_sequence_ch.cpp \
 be/be_visitor_sequence/gen_bounded_sequence_ch.cpp \
 be/be_visitor_sequence/gen_unbounded_obj_sequence_ch.cpp \
 be/be_visitor_sequence/gen_bounded_obj_sequence_ch.cpp \
 be/be_visitor_sequence/gen_bounded_str_sequence_ch.cpp \
 be/be_visitor_sequence/gen_object_manager_ch.cpp \
 be/be_visitor_sequence/gen_unbounded_sequence_ci.cpp \
 be/be_visitor_sequence/gen_bounded_sequence_ci.cpp \
 be/be_visitor_sequence/gen_unbounded_obj_sequence_ci.cpp \
 be/be_visitor_sequence/gen_bounded_obj_sequence_ci.cpp \
 be/be_visitor_sequence/gen_bounded_str_sequence_ci.cpp \
 be/be_visitor_sequence/gen_object_manager_ci.cpp \
 be/be_visitor_sequence/gen_unbounded_sequence_cs.cpp \
 be/be_visitor_sequence/gen_bounded_sequence_cs.cpp \
 be/be_visitor_sequence/gen_unbounded_obj_sequence_cs.cpp \
 be/be_visitor_sequence/gen_bounded_obj_sequence_cs.cpp \
 be/be_visitor_sequence/gen_bounded_str_sequence_cs.cpp
.obj/be_visitor_structure.o .obj/be_visitor_structure.so .shobj/be_visitor_structure.o .shobj/be_visitor_structure.so: be/be_visitor_structure.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h \
 be_include/be_visitor_structure.h be_include/be_visitor_scope.h \
 be_include/be_visitor_decl.h \
 be_include/be_visitor_structure/structure.h \
 be_include/be_visitor_structure/structure_ch.h \
 be_include/be_visitor_structure/structure_ci.h \
 be_include/be_visitor_structure/structure_cs.h \
 be_include/be_visitor_structure/any_op_ch.h \
 be_include/be_visitor_structure/any_op_cs.h \
 be_include/be_visitor_structure/cdr_op_ch.h \
 be_include/be_visitor_structure/cdr_op_cs.h \
 be/be_visitor_structure/structure.cpp \
 be/be_visitor_structure/structure_ch.cpp \
 be/be_visitor_structure/structure_ci.cpp \
 be/be_visitor_structure/structure_cs.cpp \
 be/be_visitor_structure/any_op_ch.cpp \
 be/be_visitor_structure/any_op_cs.cpp \
 be/be_visitor_structure/cdr_op_ch.cpp \
 be/be_visitor_structure/cdr_op_cs.cpp be_include/be_visitor_field.h \
 be_include/be_visitor_field/field_ch.h \
 be_include/be_visitor_field/field_ci.h \
 be_include/be_visitor_field/field_cs.h \
 be_include/be_visitor_field/cdr_op_ch.h \
 be_include/be_visitor_field/cdr_op_cs.h
.obj/be_visitor_typecode.o .obj/be_visitor_typecode.so .shobj/be_visitor_typecode.o .shobj/be_visitor_typecode.so: be/be_visitor_typecode.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h \
 be_include/be_visitor_typecode.h be_include/be_visitor_decl.h \
 be_include/be_visitor_scope.h \
 be_include/be_visitor_typecode/typecode_decl.h \
 be_include/be_visitor_typecode/typecode_defn.h \
 be/be_visitor_typecode/typecode_decl.cpp \
 be/be_visitor_typecode/typecode_defn.cpp
.obj/be_visitor_typedef.o .obj/be_visitor_typedef.so .shobj/be_visitor_typedef.o .shobj/be_visitor_typedef.so: be/be_visitor_typedef.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h \
 be_include/be_visitor_typedef.h be_include/be_visitor_scope.h \
 be_include/be_visitor_decl.h be_include/be_visitor_typedef/typedef.h \
 be_include/be_visitor_typedef/typedef_ch.h \
 be_include/be_visitor_typedef/typedef_ci.h \
 be_include/be_visitor_typedef/typedef_cs.h \
 be_include/be_visitor_typedef/any_op_ch.h \
 be_include/be_visitor_typedef/any_op_cs.h \
 be_include/be_visitor_typedef/cdr_op_ch.h \
 be_include/be_visitor_typedef/cdr_op_cs.h \
 be/be_visitor_typedef/typedef.cpp \
 be/be_visitor_typedef/typedef_ch.cpp \
 be/be_visitor_typedef/typedef_ci.cpp \
 be/be_visitor_typedef/typedef_cs.cpp \
 be/be_visitor_typedef/any_op_ch.cpp \
 be/be_visitor_typedef/any_op_cs.cpp \
 be/be_visitor_typedef/cdr_op_ch.cpp \
 be/be_visitor_typedef/cdr_op_cs.cpp
.obj/be_visitor_union.o .obj/be_visitor_union.so .shobj/be_visitor_union.o .shobj/be_visitor_union.so: be/be_visitor_union.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h \
 be_include/be_visitor_union.h be_include/be_visitor_scope.h \
 be_include/be_visitor_decl.h be_include/be_visitor_union/union.h \
 be_include/be_visitor_union/union_ch.h \
 be_include/be_visitor_union/union_ci.h \
 be_include/be_visitor_union/union_cs.h \
 be_include/be_visitor_union/discriminant_ch.h \
 be_include/be_visitor_union/discriminant_ci.h \
 be_include/be_visitor_union/discriminant_cs.h \
 be_include/be_visitor_union/any_op_ch.h \
 be_include/be_visitor_union/any_op_cs.h \
 be_include/be_visitor_union/cdr_op_ch.h \
 be_include/be_visitor_union/cdr_op_cs.h \
 be/be_visitor_union/discriminant_ch.cpp \
 be/be_visitor_union/discriminant_ci.cpp \
 be/be_visitor_union/discriminant_cs.cpp be/be_visitor_union/union.cpp \
 be/be_visitor_union/union_ch.cpp be/be_visitor_union/union_ci.cpp \
 be/be_visitor_union/union_cs.cpp be/be_visitor_union/any_op_ch.cpp \
 be/be_visitor_union/any_op_cs.cpp be/be_visitor_union/cdr_op_ch.cpp \
 be/be_visitor_union/cdr_op_cs.cpp
.obj/be_visitor_union_branch.o .obj/be_visitor_union_branch.so .shobj/be_visitor_union_branch.o .shobj/be_visitor_union_branch.so: be/be_visitor_union_branch.cpp \
 include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 include/utl_scope.h include/ast_type.h include/ast_concrete_type.h \
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
 be_include/be.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_codegen.h \
 be_include/be_stream_factory.h be_include/be_visitor.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h \
 be_include/be_visitor_union_branch.h be_include/be_visitor_decl.h \
 be_include/be_visitor_union_branch/public_ch.h \
 be_include/be_visitor_union_branch/public_ci.h \
 be_include/be_visitor_union_branch/public_cs.h \
 be_include/be_visitor_union_branch/public_assign_cs.h \
 be_include/be_visitor_union_branch/public_reset_cs.h \
 be_include/be_visitor_union_branch/public_access_cs.h \
 be_include/be_visitor_union_branch/private_ch.h \
 be_include/be_visitor_union_branch/cdr_op_ch.h \
 be_include/be_visitor_union_branch/cdr_op_cs.h \
 be/be_visitor_union_branch/private_ch.cpp \
 be/be_visitor_union_branch/public_assign_cs.cpp \
 be/be_visitor_union_branch/public_reset_cs.cpp \
 be/be_visitor_union_branch/public_access_cs.cpp \
 be/be_visitor_union_branch/public_ch.cpp \
 be/be_visitor_union_branch/public_ci.cpp \
 be/be_visitor_union_branch/public_cs.cpp \
 be/be_visitor_union_branch/cdr_op_ch.cpp \
 be/be_visitor_union_branch/cdr_op_cs.cpp

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
