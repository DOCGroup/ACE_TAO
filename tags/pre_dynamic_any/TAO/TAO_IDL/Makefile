#----------------------------------------------------------------------------
#       $Id$
#
#       Top-level Makefile for the TAO IDL compiler
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

MAKEFILE = Makefile

#----------------------------------------------------------------------------
#       Include macros
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
#       Define local rules and targets
#---------------------------------------------------------------------------

ifneq ($(CROSS-COMPILE),)
  all default_embedded:
	@echo The IDL compiler won\'t run or build on CROSS-COMPILE platforms.

  clean realclean:
	@echo Nothing to clean in TAO_IDL on CROSS-COMPILE platforms.
else  # ! CROSS-COMPILE

MKLIST = \
	Makefile.FE \
	Makefile.BE \
	Makefile.EXE

all debug profile optimize install deinstall clean realclean clobber depend:
ifeq (Windows,$(findstring Windows,$(OS)))
	@cmd /c "FOR /D %m IN ($(MKLIST)) DO $(MAKE) -f %m MAKEFILE=$$m $(@:.nested=)"
else  # ! Windows
	@for m in $(MKLIST); do \
	echo $$m; \
        $(MAKE) -f $$m MAKEFILE=$$m $(@:.nested=); \
        done
endif # ! Windows

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


.obj/tao_idl.o .obj/tao_idl.so .shobj/tao_idl.o .shobj/tao_idl.so: tao_idl.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/config-sunos5.6.h \
 $(ACE_ROOT)/ace/config-sunos5.5.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
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
 include/TAO_IDL_FE_Export.h include/idl_narrow.h \
 include/idl_defines.h include/idl_fwd.h include/idl_bool.h \
 include/utl_string.h include/utl_identifier.h include/ast.h \
 include/utl_scoped_name.h include/ast_decl.h include/ast_expression.h \
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
 include/utl_err.h include/utl_indenter.h include/global_extern.h \
 include/fe_extern.h include/drv_private.h include/drv_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/TAO_IDL_BE_Export.h be_include/be_global.h \
 be_include/be_extern.h be_include/be_type.h \
 be_include/be_predefined_type.h be_include/be_module.h \
 be_include/be_interface.h be_include/be_codegen.h \
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
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 include/idl_extern.h include/ast_extern.h include/nr_extern.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h include/../../tao/Version.h

.obj/drv_init.o .obj/drv_init.so .shobj/drv_init.o .shobj/drv_init.so: driver/drv_init.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/config-sunos5.6.h \
 $(ACE_ROOT)/ace/config-sunos5.5.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
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
 include/TAO_IDL_FE_Export.h include/idl_narrow.h \
 include/idl_defines.h include/idl_fwd.h include/idl_bool.h \
 include/utl_string.h include/utl_identifier.h include/ast.h \
 include/utl_scoped_name.h include/ast_decl.h include/ast_expression.h \
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
 include/fe_extern.h include/ast_extern.h include/nr_extern.h \
 include/global_extern.h include/drv_private.h be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/TAO_IDL_BE_Export.h be_include/be_global.h \
 be_include/be_extern.h be_include/be_type.h \
 be_include/be_predefined_type.h be_include/be_module.h \
 be_include/be_interface.h be_include/be_codegen.h \
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
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h

.obj/drv_private.o .obj/drv_private.so .shobj/drv_private.o .shobj/drv_private.so: driver/drv_private.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/config-sunos5.6.h \
 $(ACE_ROOT)/ace/config-sunos5.5.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
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
 include/TAO_IDL_FE_Export.h include/idl_narrow.h \
 include/idl_defines.h include/idl_fwd.h include/idl_bool.h \
 include/utl_string.h include/utl_identifier.h include/ast.h \
 include/utl_scoped_name.h include/ast_decl.h include/ast_expression.h \
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
 include/fe_extern.h include/ast_extern.h include/nr_extern.h \
 include/global_extern.h include/drv_private.h

.obj/drv_args.o .obj/drv_args.so .shobj/drv_args.o .shobj/drv_args.so: driver/drv_args.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/config-sunos5.6.h \
 $(ACE_ROOT)/ace/config-sunos5.5.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
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
 include/TAO_IDL_FE_Export.h include/idl_narrow.h \
 include/idl_defines.h include/idl_fwd.h include/idl_bool.h \
 include/utl_string.h include/utl_identifier.h include/ast.h \
 include/utl_scoped_name.h include/ast_decl.h include/ast_expression.h \
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
 include/fe_extern.h include/ast_extern.h include/nr_extern.h \
 include/global_extern.h include/drv_private.h include/drv_extern.h \
 be_include/be.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 be_include/be_decl.h be_include/be_scope.h be_include/be_generator.h \
 be_include/TAO_IDL_BE_Export.h be_include/be_global.h \
 be_include/be_extern.h be_include/be_type.h \
 be_include/be_predefined_type.h be_include/be_module.h \
 be_include/be_interface.h be_include/be_codegen.h \
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
 be_include/be_visitor.h be_include/be_visitor_factory.h \
 be_include/be_visitor_context.h be_include/be_visitor_ami_pre_proc.h \
 be_include/be_visitor_scope.h be_include/be_visitor_decl.h \
 be_include/be_interface_strategy.h

.obj/drv_preproc.o .obj/drv_preproc.so .shobj/drv_preproc.o .shobj/drv_preproc.so: driver/drv_preproc.cpp include/idl.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/config-sunos5.6.h \
 $(ACE_ROOT)/ace/config-sunos5.5.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
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
 include/TAO_IDL_FE_Export.h include/idl_narrow.h \
 include/idl_defines.h include/idl_fwd.h include/idl_bool.h \
 include/utl_string.h include/utl_identifier.h include/ast.h \
 include/utl_scoped_name.h include/ast_decl.h include/ast_expression.h \
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
 include/fe_extern.h include/ast_extern.h include/nr_extern.h \
 include/global_extern.h include/drv_private.h include/drv_extern.h \
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

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
