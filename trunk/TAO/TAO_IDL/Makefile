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

VPATH=.:driver:fe:ast:util:narrow:be

FILES = tao_idl \
        driver/drv_init \
        driver/drv_private \
        driver/drv_args \
        driver/drv_fork \
        driver/drv_link \
        driver/drv_preproc \
        fe/fe_declarator \
        fe/fe_lookup    \
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
        util/utl_err \
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

#  ifeq ($(findstring g++,$(CXX)),g++)
#    CCFLAGS += -Wno-switch
#  endif # g++

CPPFLAGS += -Iinclude -Ibe_include -Ife
#ifneq ($(CPP_LOCATION),)
#	echo "Warning: CPP_LOCATION is deprecated and will be removed in the future.  Please"
#	echo "use TAO_IDL_PREPROCESSOR instead"
#	TAO_IDL_PREPROCESSOR = $(CPP_LOCATION)
#endif # !def CPP_LOCATION

ifeq ($(TAO_IDL_PREPROCESSOR),)
	CPPFLAGS += -DTAO_IDL_PREPROCESSOR=\"$(strip $(CXX))\"
else
	CPPFLAGS += -DTAO_IDL_PREPROCESSOR=\"$(TAO_IDL_PREPROCESSOR)\"
endif # !def TAO_IDL_PREPROCESSOR

  realclean:
	$(RM) tao_idl TAGS
endif # ! CROSS-COMPILE

fe/fe_lookup.cpp: fe/keywords.dat
	$(RM) fe/fe_lookup.cpp
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
		sed -e 's/char \*getenv/char *ace_foo/g' \
		    -e 's/= getenv/= ACE_OS::getenv/g' \
		    -e 's/int yynew_state,/int,/g' \
		    -e 's/int yyold_state,/int,/g' \
		    -e 's/int yyrule_num,/int,/g' \
		    -e 's/\(YYDEBUG_DISCARD_TOKEN.*int\) yytoken_num/\1/g' \
		    -e 's/yynewerror://g' \
		    -e 's/yy/tao_yy/g' \
		    -e 's/YY/TAO_YY/g' \
		    -e 's/y\.tab\.c/y.tab.cpp/g' \
		    -e 's%\(#pragma ident\)%// \1%' < y.tab.c > /tmp/$(USER).Y
		sed -e 's/yy/tao_yy/g' \
		    -e 's/YY/TAO_YY/g' < y.tab.h > /tmp/$(USER).YH
		echo '// $$I''d$$' > fe/y.tab.cpp
		cat /tmp/$(USER).Y >> fe/y.tab.cpp
		echo '// $$I''d$$' > fe/y.tab.h
		cat /tmp/$(USER).YH >> fe/y.tab.h
		$(RM) /tmp/$(USER).Y /tmp/$(USER).YH y.tab.c y.tab.h
		(cd fe ; patch < y.tab.cpp.diff)

tags:
	$(RM) TAGS
	find . -name "*.h" -print | etags -a --c++ -
	find . -name "*.cpp" -print | etags -a -


#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/tao_idl.o .obj/tao_idl.o .obj/tao_idl.o .obj/tao_idl.o: tao_idl.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 include/drv_private.h include/drv_link.h

.obj/drv_init.o .obj/drv_init.o .obj/drv_init.o .obj/drv_init.o: driver/drv_init.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 include/drv_private.h include/drv_link.h

.obj/drv_private.o .obj/drv_private.o .obj/drv_private.o .obj/drv_private.o: driver/drv_private.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 include/drv_private.h

.obj/drv_args.o .obj/drv_args.o .obj/drv_args.o .obj/drv_args.o: driver/drv_args.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 include/drv_private.h include/drv_link.h be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h

.obj/drv_fork.o .obj/drv_fork.o .obj/drv_fork.o .obj/drv_fork.o: driver/drv_fork.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 include/drv_private.h \
 $(ACE_ROOT)/ace/Process_Manager.h \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.cpp \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 $(ACE_ROOT)/ace/Process_Manager.i

.obj/drv_link.o .obj/drv_link.o .obj/drv_link.o .obj/drv_link.o: driver/drv_link.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 include/drv_private.h include/drv_link.h

.obj/drv_preproc.o .obj/drv_preproc.o .obj/drv_preproc.o .obj/drv_preproc.o: driver/drv_preproc.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 include/drv_private.h include/drv_link.h \
 $(ACE_ROOT)/ace/Process_Manager.h \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.cpp \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 $(ACE_ROOT)/ace/Process_Manager.i \
 $(ACE_ROOT)/ace/Env_Value_T.h \
 $(ACE_ROOT)/ace/Env_Value_T.i \
 $(ACE_ROOT)/ace/Env_Value_T.cpp \
 $(ACE_ROOT)/ace/ARGV.h \
 $(ACE_ROOT)/ace/ARGV.i

.obj/fe_declarator.o .obj/fe_declarator.o .obj/fe_declarator.o .obj/fe_declarator.o: fe/fe_declarator.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 include/fe_private.h include/fe_interface_header.h \
 include/fe_declarator.h

.obj/fe_lookup.o .obj/fe_lookup.o .obj/fe_lookup.o .obj/fe_lookup.o: fe/fe_lookup.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 include/fe_private.h include/fe_interface_header.h \
 include/fe_declarator.h

.obj/fe_private.o .obj/fe_private.o .obj/fe_private.o .obj/fe_private.o: fe/fe_private.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 include/fe_private.h include/fe_interface_header.h \
 include/fe_declarator.h

.obj/fe_init.o .obj/fe_init.o .obj/fe_init.o .obj/fe_init.o: fe/fe_init.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 include/fe_private.h include/fe_interface_header.h \
 include/fe_declarator.h

.obj/fe_extern.o .obj/fe_extern.o .obj/fe_extern.o .obj/fe_extern.o: fe/fe_extern.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 include/fe_private.h include/fe_interface_header.h \
 include/fe_declarator.h

.obj/fe_interface_header.o .obj/fe_interface_header.o .obj/fe_interface_header.o .obj/fe_interface_header.o: fe/fe_interface_header.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 include/fe_private.h include/fe_interface_header.h \
 include/fe_declarator.h

.obj/y.tab.o .obj/y.tab.o .obj/y.tab.o .obj/y.tab.o: fe/y.tab.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 include/fe_private.h include/fe_interface_header.h \
 include/fe_declarator.h

.obj/lex.yy.o .obj/lex.yy.o .obj/lex.yy.o .obj/lex.yy.o: fe/lex.yy.cpp \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 include/fe_private.h include/fe_interface_header.h \
 include/fe_declarator.h fe/y.tab.h

.obj/ast_array.o .obj/ast_array.o .obj/ast_array.o .obj/ast_array.o: ast/ast_array.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/ast_argument.o .obj/ast_argument.o .obj/ast_argument.o .obj/ast_argument.o: ast/ast_argument.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/ast_attribute.o .obj/ast_attribute.o .obj/ast_attribute.o .obj/ast_attribute.o: ast/ast_attribute.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/ast_check.o .obj/ast_check.o .obj/ast_check.o .obj/ast_check.o: ast/ast_check.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/ast_concrete_type.o .obj/ast_concrete_type.o .obj/ast_concrete_type.o .obj/ast_concrete_type.o: ast/ast_concrete_type.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/ast_constant.o .obj/ast_constant.o .obj/ast_constant.o .obj/ast_constant.o: ast/ast_constant.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/ast_decl.o .obj/ast_decl.o .obj/ast_decl.o .obj/ast_decl.o: ast/ast_decl.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/ast_enum.o .obj/ast_enum.o .obj/ast_enum.o .obj/ast_enum.o: ast/ast_enum.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/ast_enum_val.o .obj/ast_enum_val.o .obj/ast_enum_val.o .obj/ast_enum_val.o: ast/ast_enum_val.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/ast_exception.o .obj/ast_exception.o .obj/ast_exception.o .obj/ast_exception.o: ast/ast_exception.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/ast_expression.o .obj/ast_expression.o .obj/ast_expression.o .obj/ast_expression.o: ast/ast_expression.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/ast_field.o .obj/ast_field.o .obj/ast_field.o .obj/ast_field.o: ast/ast_field.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/ast_interface.o .obj/ast_interface.o .obj/ast_interface.o .obj/ast_interface.o: ast/ast_interface.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/ast_interface_fwd.o .obj/ast_interface_fwd.o .obj/ast_interface_fwd.o .obj/ast_interface_fwd.o: ast/ast_interface_fwd.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/ast_module.o .obj/ast_module.o .obj/ast_module.o .obj/ast_module.o: ast/ast_module.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/ast_native.o .obj/ast_native.o .obj/ast_native.o .obj/ast_native.o: ast/ast_native.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/ast_operation.o .obj/ast_operation.o .obj/ast_operation.o .obj/ast_operation.o: ast/ast_operation.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/ast_predefined_type.o .obj/ast_predefined_type.o .obj/ast_predefined_type.o .obj/ast_predefined_type.o: ast/ast_predefined_type.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/ast_root.o .obj/ast_root.o .obj/ast_root.o .obj/ast_root.o: ast/ast_root.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/ast_sequence.o .obj/ast_sequence.o .obj/ast_sequence.o .obj/ast_sequence.o: ast/ast_sequence.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/ast_string.o .obj/ast_string.o .obj/ast_string.o .obj/ast_string.o: ast/ast_string.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/ast_structure.o .obj/ast_structure.o .obj/ast_structure.o .obj/ast_structure.o: ast/ast_structure.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/ast_type.o .obj/ast_type.o .obj/ast_type.o .obj/ast_type.o: ast/ast_type.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/ast_typedef.o .obj/ast_typedef.o .obj/ast_typedef.o .obj/ast_typedef.o: ast/ast_typedef.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/ast_union.o .obj/ast_union.o .obj/ast_union.o .obj/ast_union.o: ast/ast_union.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/ast_union_branch.o .obj/ast_union_branch.o .obj/ast_union_branch.o .obj/ast_union_branch.o: ast/ast_union_branch.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/ast_union_label.o .obj/ast_union_label.o .obj/ast_union_label.o .obj/ast_union_label.o: ast/ast_union_label.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/ast_generator.o .obj/ast_generator.o .obj/ast_generator.o .obj/ast_generator.o: ast/ast_generator.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/ast_redef.o .obj/ast_redef.o .obj/ast_redef.o .obj/ast_redef.o: ast/ast_redef.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/ast_recursive.o .obj/ast_recursive.o .obj/ast_recursive.o .obj/ast_recursive.o: ast/ast_recursive.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/utl_scope.o .obj/utl_scope.o .obj/utl_scope.o .obj/utl_scope.o: util/utl_scope.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/utl_stack.o .obj/utl_stack.o .obj/utl_stack.o .obj/utl_stack.o: util/utl_stack.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/utl_string.o .obj/utl_string.o .obj/utl_string.o .obj/utl_string.o: util/utl_string.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/utl_decllist.o .obj/utl_decllist.o .obj/utl_decllist.o .obj/utl_decllist.o: util/utl_decllist.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/utl_labellist.o .obj/utl_labellist.o .obj/utl_labellist.o .obj/utl_labellist.o: util/utl_labellist.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/utl_namelist.o .obj/utl_namelist.o .obj/utl_namelist.o .obj/utl_namelist.o: util/utl_namelist.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/utl_exceptlist.o .obj/utl_exceptlist.o .obj/utl_exceptlist.o .obj/utl_exceptlist.o: util/utl_exceptlist.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/utl_strlist.o .obj/utl_strlist.o .obj/utl_strlist.o .obj/utl_strlist.o: util/utl_strlist.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/utl_list.o .obj/utl_list.o .obj/utl_list.o .obj/utl_list.o: util/utl_list.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/utl_exprlist.o .obj/utl_exprlist.o .obj/utl_exprlist.o .obj/utl_exprlist.o: util/utl_exprlist.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/utl_err.o .obj/utl_err.o .obj/utl_err.o .obj/utl_err.o: util/utl_err.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/utl_indenter.o .obj/utl_indenter.o .obj/utl_indenter.o .obj/utl_indenter.o: util/utl_indenter.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/utl_identifier.o .obj/utl_identifier.o .obj/utl_identifier.o .obj/utl_identifier.o: util/utl_identifier.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/utl_idlist.o .obj/utl_idlist.o .obj/utl_idlist.o .obj/utl_idlist.o: util/utl_idlist.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/utl_global.o .obj/utl_global.o .obj/utl_global.o .obj/utl_global.o: util/utl_global.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/narrow.o .obj/narrow.o .obj/narrow.o .obj/narrow.o: narrow/narrow.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h

.obj/be_array.o .obj/be_array.o .obj/be_array.o .obj/be_array.o: be/be_array.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h

.obj/be_argument.o .obj/be_argument.o .obj/be_argument.o .obj/be_argument.o: be/be_argument.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h

.obj/be_attribute.o .obj/be_attribute.o .obj/be_attribute.o .obj/be_attribute.o: be/be_attribute.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h

.obj/be_constant.o .obj/be_constant.o .obj/be_constant.o .obj/be_constant.o: be/be_constant.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h

.obj/be_enum.o .obj/be_enum.o .obj/be_enum.o .obj/be_enum.o: be/be_enum.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h

.obj/be_enum_val.o .obj/be_enum_val.o .obj/be_enum_val.o .obj/be_enum_val.o: be/be_enum_val.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h

.obj/be_exception.o .obj/be_exception.o .obj/be_exception.o .obj/be_exception.o: be/be_exception.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h

.obj/be_expression.o .obj/be_expression.o .obj/be_expression.o .obj/be_expression.o: be/be_expression.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h

.obj/be_field.o .obj/be_field.o .obj/be_field.o .obj/be_field.o: be/be_field.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h

.obj/be_interface.o .obj/be_interface.o .obj/be_interface.o .obj/be_interface.o: be/be_interface.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be_visitor_interface.h be_include/be_visitor_scope.h \
 be_include/be_visitor.h be_include/be_visitor_decl.h \
 be_include/be_codegen.h be_include/be_visitor_interface/interface.h \
 be_include/be_visitor_interface/interface_ch.h \
 be_include/be_visitor_interface/interface_ci.h \
 be_include/be_visitor_interface/interface_cs.h \
 be_include/be_visitor_interface/interface_sh.h \
 be_include/be_visitor_interface/interface_ih.h \
 be_include/be_visitor_interface/interface_si.h \
 be_include/be_visitor_interface/interface_ss.h \
 be_include/be_visitor_interface/interface_is.h \
 be_include/be_visitor_interface/thru_poa_collocated_sh.h \
 be_include/be_visitor_interface/thru_poa_collocated_ss.h \
 be_include/be_visitor_interface/direct_collocated_sh.h \
 be_include/be_visitor_interface/direct_collocated_ss.h \
 be_include/be_visitor_interface/any_op_ch.h \
 be_include/be_visitor_interface/any_op_cs.h \
 be_include/be_visitor_interface/cdr_op_ch.h \
 be_include/be_visitor_interface/cdr_op_ci.h \
 be_include/be_visitor_interface/cdr_op_cs.h \
 be_include/be_visitor_interface/tie_sh.h \
 be_include/be_visitor_interface/tie_si.h \
 be_include/be_visitor_interface/ami_interface_ch.h \
 be_include/be_visitor_interface/smart_proxy_ch.h \
 be_include/be_visitor_interface/smart_proxy_cs.h be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_interface_fwd.h be_include/be_valuetype.h \
 be_include/be_valuetype_fwd.h be_include/be_structure.h \
 be_include/be_exception.h be_include/be_expression.h \
 be_include/be_enum.h be_include/be_operation.h \
 be_include/be_operation_strategy.h be_include/be_field.h \
 be_include/be_argument.h be_include/be_attribute.h \
 be_include/be_union.h be_include/be_union_branch.h \
 be_include/be_union_label.h be_include/be_constant.h \
 be_include/be_enum_val.h be_include/be_array.h \
 be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor_factory.h be_include/be_visitor_context.h \
 be_include/be_visitor_ami_pre_proc.h \
 be_include/be_interface_strategy.h

.obj/be_interface_strategy.o .obj/be_interface_strategy.o .obj/be_interface_strategy.o .obj/be_interface_strategy.o: be/be_interface_strategy.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h

.obj/be_interface_fwd.o .obj/be_interface_fwd.o .obj/be_interface_fwd.o .obj/be_interface_fwd.o: be/be_interface_fwd.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h

.obj/be_valuetype.o .obj/be_valuetype.o .obj/be_valuetype.o .obj/be_valuetype.o: be/be_valuetype.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h

.obj/be_valuetype_fwd.o .obj/be_valuetype_fwd.o .obj/be_valuetype_fwd.o .obj/be_valuetype_fwd.o: be/be_valuetype_fwd.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h

.obj/be_module.o .obj/be_module.o .obj/be_module.o .obj/be_module.o: be/be_module.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h

.obj/be_native.o .obj/be_native.o .obj/be_native.o .obj/be_native.o: be/be_native.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h

.obj/be_operation.o .obj/be_operation.o .obj/be_operation.o .obj/be_operation.o: be/be_operation.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h

.obj/be_operation_strategy.o .obj/be_operation_strategy.o .obj/be_operation_strategy.o .obj/be_operation_strategy.o: be/be_operation_strategy.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h

.obj/be_predefined_type.o .obj/be_predefined_type.o .obj/be_predefined_type.o .obj/be_predefined_type.o: be/be_predefined_type.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h

.obj/be_root.o .obj/be_root.o .obj/be_root.o .obj/be_root.o: be/be_root.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h

.obj/be_sequence.o .obj/be_sequence.o .obj/be_sequence.o .obj/be_sequence.o: be/be_sequence.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h be_include/be_visitor_sequence.h \
 be_include/be_visitor_sequence/sequence_ch.h \
 be_include/be_visitor_sequence/sequence_ci.h \
 be_include/be_visitor_sequence/sequence_cs.h \
 be_include/be_visitor_sequence/sequence_base.h \
 be_include/be_visitor_sequence/buffer_type.h \
 be_include/be_visitor_sequence/elemtype.h \
 be_include/be_visitor_sequence/any_op_ch.h \
 be_include/be_visitor_sequence/any_op_cs.h \
 be_include/be_visitor_sequence/cdr_op_ch.h \
 be_include/be_visitor_sequence/cdr_op_ci.h \
 be_include/be_visitor_sequence/cdr_op_cs.h

.obj/be_string.o .obj/be_string.o .obj/be_string.o .obj/be_string.o: be/be_string.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h

.obj/be_structure.o .obj/be_structure.o .obj/be_structure.o .obj/be_structure.o: be/be_structure.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h

.obj/be_type.o .obj/be_type.o .obj/be_type.o .obj/be_type.o: be/be_type.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h

.obj/be_typedef.o .obj/be_typedef.o .obj/be_typedef.o .obj/be_typedef.o: be/be_typedef.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h

.obj/be_union.o .obj/be_union.o .obj/be_union.o .obj/be_union.o: be/be_union.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h

.obj/be_union_branch.o .obj/be_union_branch.o .obj/be_union_branch.o .obj/be_union_branch.o: be/be_union_branch.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h

.obj/be_union_label.o .obj/be_union_label.o .obj/be_union_label.o .obj/be_union_label.o: be/be_union_label.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h

.obj/be_generator.o .obj/be_generator.o .obj/be_generator.o .obj/be_generator.o: be/be_generator.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h

.obj/be_init.o .obj/be_init.o .obj/be_init.o .obj/be_init.o: be/be_init.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h be/../../tao/Version.h

.obj/be_produce.o .obj/be_produce.o .obj/be_produce.o .obj/be_produce.o: be/be_produce.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h

.obj/be_args.o .obj/be_args.o .obj/be_args.o .obj/be_args.o: be/be_args.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h

.obj/be_helper.o .obj/be_helper.o .obj/be_helper.o .obj/be_helper.o: be/be_helper.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h

.obj/be_codegen.o .obj/be_codegen.o .obj/be_codegen.o .obj/be_codegen.o: be/be_codegen.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h

.obj/be_sunsoft.o .obj/be_sunsoft.o .obj/be_sunsoft.o .obj/be_sunsoft.o: be/be_sunsoft.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h be_include/be_sunsoft.h

.obj/be_decl.o .obj/be_decl.o .obj/be_decl.o .obj/be_decl.o: be/be_decl.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h

.obj/be_scope.o .obj/be_scope.o .obj/be_scope.o .obj/be_scope.o: be/be_scope.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h

.obj/be_stream_factory.o .obj/be_stream_factory.o .obj/be_stream_factory.o .obj/be_stream_factory.o: be/be_stream_factory.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h be_include/be_sunsoft.h

.obj/be_visitor_factory.o .obj/be_visitor_factory.o .obj/be_visitor_factory.o .obj/be_visitor_factory.o: be/be_visitor_factory.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h be_include/be_visitor_argument.h \
 be_include/be_visitor_argument/argument.h \
 be_include/be_visitor_argument/arglist.h \
 be_include/be_visitor_argument/vardecl_ss.h \
 be_include/be_visitor_argument/upcall_ss.h \
 be_include/be_visitor_argument/post_upcall_ss.h \
 be_include/be_visitor_argument/marshal_ss.h \
 be_include/be_visitor_argument/pre_invoke_cs.h \
 be_include/be_visitor_argument/invoke_cs.h \
 be_include/be_visitor_argument/post_invoke_cs.h \
 be_include/be_visitor_array.h be_include/be_visitor_array/array.h \
 be_include/be_visitor_array/array_ch.h \
 be_include/be_visitor_array/array_ci.h \
 be_include/be_visitor_array/array_cs.h \
 be_include/be_visitor_array/any_op_ch.h \
 be_include/be_visitor_array/any_op_cs.h \
 be_include/be_visitor_array/cdr_op_ch.h \
 be_include/be_visitor_array/cdr_op_ci.h \
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
 be_include/be_visitor_enum/cdr_op_ci.h \
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
 be_include/be_visitor_exception/cdr_op_ci.h \
 be_include/be_visitor_exception/cdr_op_cs.h \
 be_include/be_visitor_field.h be_include/be_visitor_field/field_ch.h \
 be_include/be_visitor_field/field_ci.h \
 be_include/be_visitor_field/field_cs.h \
 be_include/be_visitor_field/cdr_op_ch.h \
 be_include/be_visitor_field/cdr_op_ci.h \
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
 be_include/be_visitor_interface/thru_poa_collocated_sh.h \
 be_include/be_visitor_interface/thru_poa_collocated_ss.h \
 be_include/be_visitor_interface/direct_collocated_sh.h \
 be_include/be_visitor_interface/direct_collocated_ss.h \
 be_include/be_visitor_interface/any_op_ch.h \
 be_include/be_visitor_interface/any_op_cs.h \
 be_include/be_visitor_interface/cdr_op_ch.h \
 be_include/be_visitor_interface/cdr_op_ci.h \
 be_include/be_visitor_interface/cdr_op_cs.h \
 be_include/be_visitor_interface/tie_sh.h \
 be_include/be_visitor_interface/tie_si.h \
 be_include/be_visitor_interface/ami_interface_ch.h \
 be_include/be_visitor_interface/smart_proxy_ch.h \
 be_include/be_visitor_interface/smart_proxy_cs.h \
 be_include/be_visitor_interface_fwd.h \
 be_include/be_visitor_interface_fwd/interface_fwd_ch.h \
 be_include/be_visitor_interface_fwd/interface_fwd_ci.h \
 be_include/be_visitor_interface_fwd/cdr_op_ch.h \
 be_include/be_visitor_interface_fwd/cdr_op_ci.h \
 be_include/be_visitor_valuetype.h \
 be_include/be_visitor_valuetype/valuetype.h \
 be_include/be_visitor_valuetype/valuetype_ch.h \
 be_include/be_visitor_valuetype/valuetype_obv_ch.h \
 be_include/be_visitor_valuetype/valuetype_obv_ci.h \
 be_include/be_visitor_valuetype/valuetype_obv_cs.h \
 be_include/be_visitor_valuetype/valuetype_ci.h \
 be_include/be_visitor_valuetype/valuetype_cs.h \
 be_include/be_visitor_valuetype/cdr_op_ch.h \
 be_include/be_visitor_valuetype/cdr_op_ci.h \
 be_include/be_visitor_valuetype/marshal_ch.h \
 be_include/be_visitor_valuetype/marshal_cs.h \
 be_include/be_visitor_valuetype/arglist.h \
 be_include/be_visitor_valuetype/field_ch.h \
 be_include/be_visitor_valuetype/field_ci.h \
 be_include/be_visitor_valuetype/field_cs.h \
 be_include/be_visitor_valuetype/field_cdr_ch.h \
 be_include/be_visitor_valuetype/field_cdr_ci.h \
 be_include/be_visitor_valuetype/obv_module.h \
 be_include/be_visitor_valuetype/ami_exception_holder_ch.h \
 be_include/be_visitor_valuetype/ami_exception_holder_cs.h \
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
 be_include/be_visitor_operation/thru_poa_collocated_sh.h \
 be_include/be_visitor_operation/thru_poa_collocated_ss.h \
 be_include/be_visitor_operation/direct_collocated_sh.h \
 be_include/be_visitor_operation/direct_collocated_ss.h \
 be_include/be_visitor_operation/arglist.h \
 be_include/be_visitor_operation/rettype.h \
 be_include/be_visitor_operation/rettype_is.h \
 be_include/be_visitor_operation/rettype_vardecl_cs.h \
 be_include/be_visitor_operation/rettype_post_invoke_cs.h \
 be_include/be_visitor_operation/rettype_return_cs.h \
 be_include/be_visitor_operation/exceptlist_cs.h \
 be_include/be_visitor_operation/argument.h \
 be_include/be_visitor_operation/argument_invoke.h \
 be_include/be_visitor_operation/argument_marshal.h \
 be_include/be_visitor_operation/rettype_vardecl_ss.h \
 be_include/be_visitor_operation/rettype_assign_ss.h \
 be_include/be_visitor_operation/rettype_post_upcall_ss.h \
 be_include/be_visitor_operation/rettype_marshal_ss.h \
 be_include/be_visitor_operation/rettype_pre_invoke_cs.h \
 be_include/be_visitor_operation/ami_ch.h \
 be_include/be_visitor_operation/ami_cs.h \
 be_include/be_visitor_operation/ami_handler_reply_stub_operation_ch.h \
 be_include/be_visitor_operation/ami_handler_reply_stub_operation_cs.h \
 be_include/be_visitor_operation/ami_exception_holder_operation_cs.h \
 be_include/be_visitor_operation/smart_proxy_ch.h \
 be_include/be_visitor_operation/smart_proxy_cs.h \
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
 be_include/be_visitor_sequence/cdr_op_ci.h \
 be_include/be_visitor_sequence/cdr_op_cs.h \
 be_include/be_visitor_structure.h \
 be_include/be_visitor_structure/structure.h \
 be_include/be_visitor_structure/structure_ch.h \
 be_include/be_visitor_structure/structure_ci.h \
 be_include/be_visitor_structure/structure_cs.h \
 be_include/be_visitor_structure/any_op_ch.h \
 be_include/be_visitor_structure/any_op_cs.h \
 be_include/be_visitor_structure/cdr_op_ch.h \
 be_include/be_visitor_structure/cdr_op_ci.h \
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
 be_include/be_visitor_typedef/cdr_op_ci.h \
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
 be_include/be_visitor_union/cdr_op_ci.h \
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
 be_include/be_visitor_union_branch/cdr_op_ci.h \
 be_include/be_visitor_union_branch/cdr_op_cs.h

.obj/be_visitor.o .obj/be_visitor.o .obj/be_visitor.o .obj/be_visitor.o: be/be_visitor.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 include/idl_extern.h include/fe_extern.h include/be_extern.h \
 include/drv_extern.h include/ast_extern.h include/nr_extern.h \
 include/global_extern.h be_include/be_visitor_scope.h \
 be_include/be_visitor_decl.h be_include/be_interface_strategy.h

.obj/be_visitor_ami_pre_proc.o .obj/be_visitor_ami_pre_proc.o .obj/be_visitor_ami_pre_proc.o .obj/be_visitor_ami_pre_proc.o: be/be_visitor_ami_pre_proc.cpp \
 be_include/be_visitor_ami_pre_proc.h include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_interface_strategy.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h

.obj/be_visitor_argument.o .obj/be_visitor_argument.o .obj/be_visitor_argument.o .obj/be_visitor_argument.o: be/be_visitor_argument.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 include/idl_extern.h include/fe_extern.h include/be_extern.h \
 include/drv_extern.h include/ast_extern.h include/nr_extern.h \
 include/global_extern.h be_include/be_visitor_scope.h \
 be_include/be_visitor_decl.h be_include/be_interface_strategy.h \
 be_include/be_visitor_argument.h \
 be_include/be_visitor_argument/argument.h \
 be_include/be_visitor_argument/arglist.h \
 be_include/be_visitor_argument/vardecl_ss.h \
 be_include/be_visitor_argument/upcall_ss.h \
 be_include/be_visitor_argument/post_upcall_ss.h \
 be_include/be_visitor_argument/marshal_ss.h \
 be_include/be_visitor_argument/pre_invoke_cs.h \
 be_include/be_visitor_argument/invoke_cs.h \
 be_include/be_visitor_argument/post_invoke_cs.h \
 be/be_visitor_argument/arglist.cpp \
 be/be_visitor_argument/argument.cpp \
 be/be_visitor_argument/marshal_ss.cpp \
 be/be_visitor_argument/post_upcall_ss.cpp \
 be/be_visitor_argument/upcall_ss.cpp \
 be/be_visitor_argument/vardecl_ss.cpp \
 be/be_visitor_argument/pre_invoke_cs.cpp \
 be/be_visitor_argument/invoke_cs.cpp \
 be/be_visitor_argument/post_invoke_cs.cpp

.obj/be_visitor_array.o .obj/be_visitor_array.o .obj/be_visitor_array.o .obj/be_visitor_array.o: be/be_visitor_array.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h be_include/be_visitor_array.h \
 be_include/be_visitor_array/array.h \
 be_include/be_visitor_array/array_ch.h \
 be_include/be_visitor_array/array_ci.h \
 be_include/be_visitor_array/array_cs.h \
 be_include/be_visitor_array/any_op_ch.h \
 be_include/be_visitor_array/any_op_cs.h \
 be_include/be_visitor_array/cdr_op_ch.h \
 be_include/be_visitor_array/cdr_op_ci.h \
 be_include/be_visitor_array/cdr_op_cs.h be/be_visitor_array/array.cpp \
 be/be_visitor_array/array_ch.cpp be/be_visitor_array/array_ci.cpp \
 be/be_visitor_array/array_cs.cpp be/be_visitor_array/any_op_ch.cpp \
 be/be_visitor_array/any_op_cs.cpp be/be_visitor_array/cdr_op_ch.cpp \
 be/be_visitor_array/cdr_op_ci.cpp be/be_visitor_array/cdr_op_cs.cpp

.obj/be_visitor_attribute.o .obj/be_visitor_attribute.o .obj/be_visitor_attribute.o .obj/be_visitor_attribute.o: be/be_visitor_attribute.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h be_include/be_visitor_attribute.h \
 be_include/be_visitor_attribute/attribute.h \
 be/be_visitor_attribute/attribute.cpp

.obj/be_visitor_constant.o .obj/be_visitor_constant.o .obj/be_visitor_constant.o .obj/be_visitor_constant.o: be/be_visitor_constant.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h be_include/be_visitor_constant.h \
 be_include/be_visitor_constant/constant_ch.h \
 be_include/be_visitor_constant/constant_cs.h \
 be/be_visitor_constant/constant_ch.cpp \
 be/be_visitor_constant/constant_cs.cpp

.obj/be_visitor_context.o .obj/be_visitor_context.o .obj/be_visitor_context.o .obj/be_visitor_context.o: be/be_visitor_context.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h

.obj/be_visitor_decl.o .obj/be_visitor_decl.o .obj/be_visitor_decl.o .obj/be_visitor_decl.o: be/be_visitor_decl.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h

.obj/be_visitor_enum.o .obj/be_visitor_enum.o .obj/be_visitor_enum.o .obj/be_visitor_enum.o: be/be_visitor_enum.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h be_include/be_visitor_enum.h \
 be_include/be_visitor_enum/enum_ch.h \
 be_include/be_visitor_enum/enum_cs.h \
 be_include/be_visitor_enum/any_op_ch.h \
 be_include/be_visitor_enum/any_op_cs.h \
 be_include/be_visitor_enum/cdr_op_ch.h \
 be_include/be_visitor_enum/cdr_op_ci.h be/be_visitor_enum/enum_ch.cpp \
 be/be_visitor_enum/enum_cs.cpp be/be_visitor_enum/any_op_ch.cpp \
 be/be_visitor_enum/any_op_cs.cpp be/be_visitor_enum/cdr_op_ch.cpp \
 be/be_visitor_enum/cdr_op_ci.cpp

.obj/be_visitor_exception.o .obj/be_visitor_exception.o .obj/be_visitor_exception.o .obj/be_visitor_exception.o: be/be_visitor_exception.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h be_include/be_visitor_exception.h \
 be_include/be_visitor_exception/exception.h \
 be_include/be_visitor_exception/exception_ch.h \
 be_include/be_visitor_exception/exception_ci.h \
 be_include/be_visitor_exception/exception_cs.h \
 be_include/be_visitor_exception/ctor.h \
 be_include/be_visitor_exception/ctor_assign.h \
 be_include/be_visitor_exception/any_op_ch.h \
 be_include/be_visitor_exception/any_op_cs.h \
 be_include/be_visitor_exception/cdr_op_ch.h \
 be_include/be_visitor_exception/cdr_op_ci.h \
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
 be/be_visitor_exception/cdr_op_ci.cpp be_include/be_visitor_field.h \
 be_include/be_visitor_field/field_ch.h \
 be_include/be_visitor_field/field_ci.h \
 be_include/be_visitor_field/field_cs.h \
 be_include/be_visitor_field/cdr_op_ch.h \
 be_include/be_visitor_field/cdr_op_ci.h \
 be_include/be_visitor_field/cdr_op_cs.h \
 be/be_visitor_exception/cdr_op_cs.cpp

.obj/be_visitor_field.o .obj/be_visitor_field.o .obj/be_visitor_field.o .obj/be_visitor_field.o: be/be_visitor_field.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h be_include/be_visitor_field.h \
 be_include/be_visitor_field/field_ch.h \
 be_include/be_visitor_field/field_ci.h \
 be_include/be_visitor_field/field_cs.h \
 be_include/be_visitor_field/cdr_op_ch.h \
 be_include/be_visitor_field/cdr_op_ci.h \
 be_include/be_visitor_field/cdr_op_cs.h \
 be/be_visitor_field/field_ch.cpp be/be_visitor_field/field_ci.cpp \
 be/be_visitor_field/field_cs.cpp be/be_visitor_field/cdr_op_ch.cpp \
 be/be_visitor_field/cdr_op_ci.cpp be/be_visitor_field/cdr_op_cs.cpp

.obj/be_visitor_interface.o .obj/be_visitor_interface.o .obj/be_visitor_interface.o .obj/be_visitor_interface.o: be/be_visitor_interface.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h be_include/be_visitor_interface.h \
 be_include/be_visitor_interface/interface.h \
 be_include/be_visitor_interface/interface_ch.h \
 be_include/be_visitor_interface/interface_ci.h \
 be_include/be_visitor_interface/interface_cs.h \
 be_include/be_visitor_interface/interface_sh.h \
 be_include/be_visitor_interface/interface_ih.h \
 be_include/be_visitor_interface/interface_si.h \
 be_include/be_visitor_interface/interface_ss.h \
 be_include/be_visitor_interface/interface_is.h \
 be_include/be_visitor_interface/thru_poa_collocated_sh.h \
 be_include/be_visitor_interface/thru_poa_collocated_ss.h \
 be_include/be_visitor_interface/direct_collocated_sh.h \
 be_include/be_visitor_interface/direct_collocated_ss.h \
 be_include/be_visitor_interface/any_op_ch.h \
 be_include/be_visitor_interface/any_op_cs.h \
 be_include/be_visitor_interface/cdr_op_ch.h \
 be_include/be_visitor_interface/cdr_op_ci.h \
 be_include/be_visitor_interface/cdr_op_cs.h \
 be_include/be_visitor_interface/tie_sh.h \
 be_include/be_visitor_interface/tie_si.h \
 be_include/be_visitor_interface/ami_interface_ch.h \
 be_include/be_visitor_interface/smart_proxy_ch.h \
 be_include/be_visitor_interface/smart_proxy_cs.h \
 be/be_visitor_interface/interface.cpp \
 be/be_visitor_interface/interface_ch.cpp \
 be/be_visitor_interface/interface_ci.cpp \
 be/be_visitor_interface/interface_cs.cpp \
 be/be_visitor_interface/interface_sh.cpp \
 be/be_visitor_interface/interface_ih.cpp \
 be/be_visitor_interface/interface_si.cpp \
 be/be_visitor_interface/interface_ss.cpp \
 be/be_visitor_interface/interface_is.cpp \
 be/be_visitor_interface/thru_poa_collocated_sh.cpp \
 be/be_visitor_interface/thru_poa_collocated_ss.cpp \
 be/be_visitor_interface/direct_collocated_sh.cpp \
 be/be_visitor_interface/direct_collocated_ss.cpp \
 be/be_visitor_interface/tie_sh.cpp be/be_visitor_interface/tie_si.cpp \
 be/be_visitor_interface/any_op_ch.cpp \
 be/be_visitor_interface/any_op_cs.cpp \
 be/be_visitor_interface/cdr_op_ch.cpp \
 be/be_visitor_interface/cdr_op_ci.cpp \
 be/be_visitor_interface/cdr_op_cs.cpp \
 be/be_visitor_interface/smart_proxy_ch.cpp \
 be/be_visitor_interface/smart_proxy_cs.cpp \
 be/be_visitor_interface/ami_interface_ch.cpp

.obj/be_visitor_interface_fwd.o .obj/be_visitor_interface_fwd.o .obj/be_visitor_interface_fwd.o .obj/be_visitor_interface_fwd.o: be/be_visitor_interface_fwd.cpp \
 include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h \
 be_include/be_visitor_interface_fwd.h \
 be_include/be_visitor_interface_fwd/interface_fwd_ch.h \
 be_include/be_visitor_interface_fwd/interface_fwd_ci.h \
 be_include/be_visitor_interface_fwd/cdr_op_ch.h \
 be_include/be_visitor_interface_fwd/cdr_op_ci.h \
 be/be_visitor_interface_fwd/interface_fwd_ch.cpp \
 be/be_visitor_interface_fwd/interface_fwd_ci.cpp \
 be/be_visitor_interface_fwd/cdr_op_ch.cpp \
 be/be_visitor_interface_fwd/cdr_op_ci.cpp

.obj/be_visitor_valuetype.o .obj/be_visitor_valuetype.o .obj/be_visitor_valuetype.o .obj/be_visitor_valuetype.o: be/be_visitor_valuetype.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h be_include/be_visitor_valuetype.h \
 be_include/be_visitor_valuetype/valuetype.h \
 be_include/be_visitor_valuetype/valuetype_ch.h \
 be_include/be_visitor_valuetype/valuetype_obv_ch.h \
 be_include/be_visitor_valuetype/valuetype_obv_ci.h \
 be_include/be_visitor_valuetype/valuetype_obv_cs.h \
 be_include/be_visitor_valuetype/valuetype_ci.h \
 be_include/be_visitor_valuetype/valuetype_cs.h \
 be_include/be_visitor_valuetype/cdr_op_ch.h \
 be_include/be_visitor_valuetype/cdr_op_ci.h \
 be_include/be_visitor_valuetype/marshal_ch.h \
 be_include/be_visitor_valuetype/marshal_cs.h \
 be_include/be_visitor_valuetype/arglist.h \
 be_include/be_visitor_valuetype/field_ch.h \
 be_include/be_visitor_valuetype/field_ci.h \
 be_include/be_visitor_valuetype/field_cs.h \
 be_include/be_visitor_valuetype/field_cdr_ch.h \
 be_include/be_visitor_valuetype/field_cdr_ci.h \
 be_include/be_visitor_valuetype/obv_module.h \
 be_include/be_visitor_valuetype/ami_exception_holder_ch.h \
 be_include/be_visitor_valuetype/ami_exception_holder_cs.h \
 be/be_visitor_valuetype/valuetype.cpp \
 be/be_visitor_valuetype/valuetype_ch.cpp \
 be/be_visitor_valuetype/valuetype_obv_ch.cpp \
 be/be_visitor_valuetype/valuetype_obv_ci.cpp \
 be/be_visitor_valuetype/valuetype_obv_cs.cpp \
 be/be_visitor_valuetype/valuetype_ci.cpp \
 be/be_visitor_valuetype/valuetype_cs.cpp \
 be/be_visitor_valuetype/cdr_op_ch.cpp \
 be/be_visitor_valuetype/cdr_op_ci.cpp \
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
 be_include/be_visitor_operation/thru_poa_collocated_sh.h \
 be_include/be_visitor_operation/thru_poa_collocated_ss.h \
 be_include/be_visitor_operation/direct_collocated_sh.h \
 be_include/be_visitor_operation/direct_collocated_ss.h \
 be_include/be_visitor_operation/arglist.h \
 be_include/be_visitor_operation/rettype.h \
 be_include/be_visitor_operation/rettype_is.h \
 be_include/be_visitor_operation/rettype_vardecl_cs.h \
 be_include/be_visitor_operation/rettype_post_invoke_cs.h \
 be_include/be_visitor_operation/rettype_return_cs.h \
 be_include/be_visitor_operation/exceptlist_cs.h \
 be_include/be_visitor_operation/argument.h \
 be_include/be_visitor_operation/argument_invoke.h \
 be_include/be_visitor_operation/argument_marshal.h \
 be_include/be_visitor_operation/rettype_vardecl_ss.h \
 be_include/be_visitor_operation/rettype_assign_ss.h \
 be_include/be_visitor_operation/rettype_post_upcall_ss.h \
 be_include/be_visitor_operation/rettype_marshal_ss.h \
 be_include/be_visitor_operation/rettype_pre_invoke_cs.h \
 be_include/be_visitor_operation/ami_ch.h \
 be_include/be_visitor_operation/ami_cs.h \
 be_include/be_visitor_operation/ami_handler_reply_stub_operation_ch.h \
 be_include/be_visitor_operation/ami_handler_reply_stub_operation_cs.h \
 be_include/be_visitor_operation/ami_exception_holder_operation_cs.h \
 be_include/be_visitor_operation/smart_proxy_ch.h \
 be_include/be_visitor_operation/smart_proxy_cs.h \
 be/be_visitor_valuetype/field_ch.cpp \
 be/be_visitor_valuetype/field_ci.cpp \
 be/be_visitor_valuetype/field_cs.cpp \
 be/be_visitor_valuetype/field_cdr_ch.cpp \
 be_include/be_visitor_field.h be_include/be_visitor_field/field_ch.h \
 be_include/be_visitor_field/field_ci.h \
 be_include/be_visitor_field/field_cs.h \
 be_include/be_visitor_field/cdr_op_ch.h \
 be_include/be_visitor_field/cdr_op_ci.h \
 be_include/be_visitor_field/cdr_op_cs.h \
 be/be_visitor_valuetype/field_cdr_ci.cpp \
 be/be_visitor_valuetype/obv_module.cpp \
 be/be_visitor_valuetype/ami_exception_holder_ch.cpp \
 be/be_visitor_valuetype/ami_exception_holder_cs.cpp

.obj/be_visitor_valuetype_fwd.o .obj/be_visitor_valuetype_fwd.o .obj/be_visitor_valuetype_fwd.o .obj/be_visitor_valuetype_fwd.o: be/be_visitor_valuetype_fwd.cpp \
 include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h \
 be_include/be_visitor_valuetype_fwd.h \
 be_include/be_visitor_valuetype_fwd/valuetype_fwd_ch.h \
 be_include/be_visitor_valuetype_fwd/valuetype_fwd_ci.h \
 be_include/be_visitor_valuetype_fwd/cdr_op_ci.h

.obj/be_visitor_module.o .obj/be_visitor_module.o .obj/be_visitor_module.o .obj/be_visitor_module.o: be/be_visitor_module.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h be_include/be_visitor_module.h \
 be_include/be_visitor_module/module.h \
 be_include/be_visitor_module/module_ch.h \
 be_include/be_visitor_module/module_sh.h \
 be_include/be_visitor_module/module_ih.h \
 be_include/be_visitor_module/any_op.h \
 be_include/be_visitor_module/cdr_op.h be/be_visitor_module/module.cpp \
 be/be_visitor_module/module_ch.cpp be/be_visitor_module/module_sh.cpp \
 be/be_visitor_module/module_ih.cpp be/be_visitor_module/any_op.cpp \
 be/be_visitor_module/cdr_op.cpp

.obj/be_visitor_operation.o .obj/be_visitor_operation.o .obj/be_visitor_operation.o .obj/be_visitor_operation.o: be/be_visitor_operation.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h be_include/be_visitor_operation.h \
 be_include/be_visitor_operation/operation.h \
 be_include/be_visitor_operation/operation_ch.h \
 be_include/be_visitor_operation/operation_cs.h \
 be_include/be_visitor_operation/operation_sh.h \
 be_include/be_visitor_operation/operation_ih.h \
 be_include/be_visitor_operation/operation_ss.h \
 be_include/be_visitor_operation/operation_is.h \
 be_include/be_visitor_operation/tie_sh.h \
 be_include/be_visitor_operation/tie_si.h \
 be_include/be_visitor_operation/thru_poa_collocated_sh.h \
 be_include/be_visitor_operation/thru_poa_collocated_ss.h \
 be_include/be_visitor_operation/direct_collocated_sh.h \
 be_include/be_visitor_operation/direct_collocated_ss.h \
 be_include/be_visitor_operation/arglist.h \
 be_include/be_visitor_operation/rettype.h \
 be_include/be_visitor_operation/rettype_is.h \
 be_include/be_visitor_operation/rettype_vardecl_cs.h \
 be_include/be_visitor_operation/rettype_post_invoke_cs.h \
 be_include/be_visitor_operation/rettype_return_cs.h \
 be_include/be_visitor_operation/exceptlist_cs.h \
 be_include/be_visitor_operation/argument.h \
 be_include/be_visitor_operation/argument_invoke.h \
 be_include/be_visitor_operation/argument_marshal.h \
 be_include/be_visitor_operation/rettype_vardecl_ss.h \
 be_include/be_visitor_operation/rettype_assign_ss.h \
 be_include/be_visitor_operation/rettype_post_upcall_ss.h \
 be_include/be_visitor_operation/rettype_marshal_ss.h \
 be_include/be_visitor_operation/rettype_pre_invoke_cs.h \
 be_include/be_visitor_operation/ami_ch.h \
 be_include/be_visitor_operation/ami_cs.h \
 be_include/be_visitor_operation/ami_handler_reply_stub_operation_ch.h \
 be_include/be_visitor_operation/ami_handler_reply_stub_operation_cs.h \
 be_include/be_visitor_operation/ami_exception_holder_operation_cs.h \
 be_include/be_visitor_operation/smart_proxy_ch.h \
 be_include/be_visitor_operation/smart_proxy_cs.h \
 be/be_visitor_operation/operation.cpp \
 be/be_visitor_operation/arglist.cpp \
 be/be_visitor_operation/argument.cpp \
 be/be_visitor_operation/argument_invoke.cpp \
 be/be_visitor_operation/argument_marshal.cpp \
 be/be_visitor_operation/thru_poa_collocated_sh.cpp \
 be/be_visitor_operation/thru_poa_collocated_ss.cpp \
 be/be_visitor_operation/direct_collocated_sh.cpp \
 be/be_visitor_operation/direct_collocated_ss.cpp \
 be/be_visitor_operation/exceptlist_cs.cpp \
 be/be_visitor_operation/operation_ch.cpp \
 be/be_visitor_operation/operation_cs.cpp \
 be/be_visitor_operation/operation_sh.cpp \
 be/be_visitor_operation/operation_ih.cpp \
 be/be_visitor_operation/operation_ss.cpp \
 be_include/be_visitor_argument.h \
 be_include/be_visitor_argument/argument.h \
 be_include/be_visitor_argument/arglist.h \
 be_include/be_visitor_argument/vardecl_ss.h \
 be_include/be_visitor_argument/upcall_ss.h \
 be_include/be_visitor_argument/post_upcall_ss.h \
 be_include/be_visitor_argument/marshal_ss.h \
 be_include/be_visitor_argument/pre_invoke_cs.h \
 be_include/be_visitor_argument/invoke_cs.h \
 be_include/be_visitor_argument/post_invoke_cs.h \
 be/be_visitor_operation/operation_is.cpp \
 be/be_visitor_operation/rettype.cpp \
 be/be_visitor_operation/rettype_is.cpp \
 be/be_visitor_operation/rettype_assign_ss.cpp \
 be/be_visitor_operation/rettype_marshal_ss.cpp \
 be/be_visitor_operation/rettype_post_invoke_cs.cpp \
 be/be_visitor_operation/rettype_post_upcall_ss.cpp \
 be/be_visitor_operation/rettype_return_cs.cpp \
 be/be_visitor_operation/rettype_vardecl_cs.cpp \
 be/be_visitor_operation/rettype_vardecl_ss.cpp \
 be/be_visitor_operation/tie_sh.cpp be/be_visitor_operation/tie_si.cpp \
 be/be_visitor_operation/rettype_pre_invoke_cs.cpp \
 be/be_visitor_operation/ami_ch.cpp be/be_visitor_operation/ami_cs.cpp \
 be/be_visitor_operation/ami_handler_reply_stub_operation_ch.cpp \
 be/be_visitor_operation/ami_handler_reply_stub_operation_cs.cpp \
 be/be_visitor_operation/ami_exception_holder_operation_cs.cpp \
 be/be_visitor_operation/smart_proxy_ch.cpp \
 be/be_visitor_operation/smart_proxy_cs.cpp

.obj/be_visitor_root.o .obj/be_visitor_root.o .obj/be_visitor_root.o .obj/be_visitor_root.o: be/be_visitor_root.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h be_include/be_visitor_root.h \
 be_include/be_visitor_root/root.h \
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

.obj/be_visitor_scope.o .obj/be_visitor_scope.o .obj/be_visitor_scope.o .obj/be_visitor_scope.o: be/be_visitor_scope.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h

.obj/be_visitor_sequence.o .obj/be_visitor_sequence.o .obj/be_visitor_sequence.o .obj/be_visitor_sequence.o: be/be_visitor_sequence.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h be_include/be_visitor_sequence.h \
 be_include/be_visitor_sequence/sequence_ch.h \
 be_include/be_visitor_sequence/sequence_ci.h \
 be_include/be_visitor_sequence/sequence_cs.h \
 be_include/be_visitor_sequence/sequence_base.h \
 be_include/be_visitor_sequence/buffer_type.h \
 be_include/be_visitor_sequence/elemtype.h \
 be_include/be_visitor_sequence/any_op_ch.h \
 be_include/be_visitor_sequence/any_op_cs.h \
 be_include/be_visitor_sequence/cdr_op_ch.h \
 be_include/be_visitor_sequence/cdr_op_ci.h \
 be_include/be_visitor_sequence/cdr_op_cs.h \
 be/be_visitor_sequence/any_op_ch.cpp \
 be/be_visitor_sequence/any_op_cs.cpp \
 be/be_visitor_sequence/cdr_op_ch.cpp \
 be/be_visitor_sequence/cdr_op_ci.cpp \
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
 be/be_visitor_sequence/gen_bounded_wstr_sequence_ch.cpp \
 be/be_visitor_sequence/gen_unbounded_sequence_ci.cpp \
 be/be_visitor_sequence/gen_bounded_sequence_ci.cpp \
 be/be_visitor_sequence/gen_unbounded_obj_sequence_ci.cpp \
 be/be_visitor_sequence/gen_bounded_obj_sequence_ci.cpp \
 be/be_visitor_sequence/gen_bounded_str_sequence_ci.cpp \
 be/be_visitor_sequence/gen_bounded_wstr_sequence_ci.cpp \
 be/be_visitor_sequence/gen_unbounded_sequence_cs.cpp \
 be/be_visitor_sequence/gen_bounded_sequence_cs.cpp \
 be/be_visitor_sequence/gen_unbounded_obj_sequence_cs.cpp \
 be/be_visitor_sequence/gen_bounded_obj_sequence_cs.cpp \
 be/be_visitor_sequence/gen_bounded_str_sequence_cs.cpp \
 be/be_visitor_sequence/gen_bounded_wstr_sequence_cs.cpp

.obj/be_visitor_structure.o .obj/be_visitor_structure.o .obj/be_visitor_structure.o .obj/be_visitor_structure.o: be/be_visitor_structure.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h be_include/be_visitor_structure.h \
 be_include/be_visitor_structure/structure.h \
 be_include/be_visitor_structure/structure_ch.h \
 be_include/be_visitor_structure/structure_ci.h \
 be_include/be_visitor_structure/structure_cs.h \
 be_include/be_visitor_structure/any_op_ch.h \
 be_include/be_visitor_structure/any_op_cs.h \
 be_include/be_visitor_structure/cdr_op_ch.h \
 be_include/be_visitor_structure/cdr_op_ci.h \
 be_include/be_visitor_structure/cdr_op_cs.h \
 be/be_visitor_structure/structure.cpp \
 be/be_visitor_structure/structure_ch.cpp \
 be/be_visitor_structure/structure_ci.cpp \
 be/be_visitor_structure/structure_cs.cpp \
 be/be_visitor_structure/any_op_ch.cpp \
 be/be_visitor_structure/any_op_cs.cpp \
 be/be_visitor_structure/cdr_op_ch.cpp \
 be/be_visitor_structure/cdr_op_ci.cpp be_include/be_visitor_field.h \
 be_include/be_visitor_field/field_ch.h \
 be_include/be_visitor_field/field_ci.h \
 be_include/be_visitor_field/field_cs.h \
 be_include/be_visitor_field/cdr_op_ch.h \
 be_include/be_visitor_field/cdr_op_ci.h \
 be_include/be_visitor_field/cdr_op_cs.h \
 be/be_visitor_structure/cdr_op_cs.cpp

.obj/be_visitor_typecode.o .obj/be_visitor_typecode.o .obj/be_visitor_typecode.o .obj/be_visitor_typecode.o: be/be_visitor_typecode.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h be_include/be_visitor_typecode.h \
 be_include/be_visitor_typecode/typecode_decl.h \
 be_include/be_visitor_typecode/typecode_defn.h \
 be/be_visitor_typecode/typecode_decl.cpp \
 be/be_visitor_typecode/typecode_defn.cpp

.obj/be_visitor_typedef.o .obj/be_visitor_typedef.o .obj/be_visitor_typedef.o .obj/be_visitor_typedef.o: be/be_visitor_typedef.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h be_include/be_visitor_typedef.h \
 be_include/be_visitor_typedef/typedef.h \
 be_include/be_visitor_typedef/typedef_ch.h \
 be_include/be_visitor_typedef/typedef_ci.h \
 be_include/be_visitor_typedef/typedef_cs.h \
 be_include/be_visitor_typedef/any_op_ch.h \
 be_include/be_visitor_typedef/any_op_cs.h \
 be_include/be_visitor_typedef/cdr_op_ch.h \
 be_include/be_visitor_typedef/cdr_op_ci.h \
 be_include/be_visitor_typedef/cdr_op_cs.h \
 be/be_visitor_typedef/typedef.cpp \
 be/be_visitor_typedef/typedef_ch.cpp \
 be/be_visitor_typedef/typedef_ci.cpp \
 be/be_visitor_typedef/typedef_cs.cpp \
 be/be_visitor_typedef/any_op_ch.cpp \
 be/be_visitor_typedef/any_op_cs.cpp \
 be/be_visitor_typedef/cdr_op_ch.cpp \
 be/be_visitor_typedef/cdr_op_ci.cpp \
 be/be_visitor_typedef/cdr_op_cs.cpp

.obj/be_visitor_union.o .obj/be_visitor_union.o .obj/be_visitor_union.o .obj/be_visitor_union.o: be/be_visitor_union.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h be_include/be_visitor_union.h \
 be_include/be_visitor_union/union.h \
 be_include/be_visitor_union/union_ch.h \
 be_include/be_visitor_union/union_ci.h \
 be_include/be_visitor_union/union_cs.h \
 be_include/be_visitor_union/discriminant_ch.h \
 be_include/be_visitor_union/discriminant_ci.h \
 be_include/be_visitor_union/discriminant_cs.h \
 be_include/be_visitor_union/any_op_ch.h \
 be_include/be_visitor_union/any_op_cs.h \
 be_include/be_visitor_union/cdr_op_ch.h \
 be_include/be_visitor_union/cdr_op_ci.h \
 be_include/be_visitor_union/cdr_op_cs.h \
 be/be_visitor_union/discriminant_ch.cpp \
 be/be_visitor_union/discriminant_ci.cpp \
 be/be_visitor_union/discriminant_cs.cpp be/be_visitor_union/union.cpp \
 be/be_visitor_union/union_ch.cpp be/be_visitor_union/union_ci.cpp \
 be/be_visitor_union/union_cs.cpp be/be_visitor_union/any_op_ch.cpp \
 be/be_visitor_union/any_op_cs.cpp be/be_visitor_union/cdr_op_ch.cpp \
 be/be_visitor_union/cdr_op_ci.cpp be/be_visitor_union/cdr_op_cs.cpp

.obj/be_visitor_union_branch.o .obj/be_visitor_union_branch.o .obj/be_visitor_union_branch.o .obj/be_visitor_union_branch.o: be/be_visitor_union_branch.cpp \
 include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 include/intlmacros.h include/idl_narrow.h include/idl_defines.h \
 include/idl_fwd.h include/idl_bool.h include/utl_string.h \
 include/utl_identifier.h include/ast.h include/utl_scoped_name.h \
 include/ast_decl.h include/ast_expression.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.h \
 $(ACE_ROOT)/ace/Based_Pointer_T.i \
 $(ACE_ROOT)/ace/Based_Pointer_T.cpp \
 $(ACE_ROOT)/ace/Based_Pointer_Repository.h \
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
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 include/utl_err.h include/utl_indenter.h include/idl_extern.h \
 include/fe_extern.h include/be_extern.h include/drv_extern.h \
 include/ast_extern.h include/nr_extern.h include/global_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/be_type.h be_include/be_predefined_type.h \
 be_include/be_module.h be_include/be_interface.h \
 be_include/be_codegen.h be_include/be_interface_fwd.h \
 be_include/be_valuetype.h be_include/be_valuetype_fwd.h \
 be_include/be_structure.h be_include/be_exception.h \
 be_include/be_expression.h be_include/be_enum.h \
 be_include/be_operation.h be_include/be_operation_strategy.h \
 be_include/be_field.h be_include/be_argument.h \
 be_include/be_attribute.h be_include/be_union.h \
 be_include/be_union_branch.h be_include/be_union_label.h \
 be_include/be_constant.h be_include/be_enum_val.h \
 be_include/be_array.h be_include/be_sequence.h be_include/be_string.h \
 be_include/be_typedef.h be_include/be_root.h be_include/be_native.h \
 be_include/be_helper.h be_include/be_stream_factory.h \
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h \
 be_include/be_visitor_union_branch.h \
 be_include/be_visitor_union_branch/public_ch.h \
 be_include/be_visitor_union_branch/public_ci.h \
 be_include/be_visitor_union_branch/public_cs.h \
 be_include/be_visitor_union_branch/public_assign_cs.h \
 be_include/be_visitor_union_branch/public_reset_cs.h \
 be_include/be_visitor_union_branch/public_access_cs.h \
 be_include/be_visitor_union_branch/private_ch.h \
 be_include/be_visitor_union_branch/cdr_op_ch.h \
 be_include/be_visitor_union_branch/cdr_op_ci.h \
 be_include/be_visitor_union_branch/cdr_op_cs.h \
 be/be_visitor_union_branch/private_ch.cpp \
 be/be_visitor_union_branch/public_assign_cs.cpp \
 be/be_visitor_union_branch/public_reset_cs.cpp \
 be/be_visitor_union_branch/public_access_cs.cpp \
 be/be_visitor_union_branch/public_ch.cpp \
 be/be_visitor_union_branch/public_ci.cpp \
 be/be_visitor_union_branch/public_cs.cpp \
 be/be_visitor_union_branch/cdr_op_ch.cpp \
 be/be_visitor_union_branch/cdr_op_ci.cpp \
 be/be_visitor_union_branch/cdr_op_cs.cpp

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
