tao_idl -o orig -GA -Gp -Gd -Ge 1 -GT -Sc -Wb,export_macro=TAO_Messaging_Export -Wb,export_include=messaging_export.h -Wb,pre_include="ace/pre.h" -Wb,post_include="ace/post.h" -I$(TAO_ROOT) Messaging_No_Impl.pidl -I../..
tao_idl -o orig -GA -Gp -Gd -Ge 1 -GT -Sc -Wb,export_macro=TAO_Messaging_Export -Wb,export_include=messaging_export.h -Wb,pre_include="ace/pre.h" -Wb,post_include="ace/post.h" -I$(TAO_ROOT) Messaging_RT_Policy.pidl -I../..
tao_idl -o orig -GA -Gp -Gd -Ge 1 -GT -Sc -Wb,export_macro=TAO_Messaging_Export -Wb,export_include=messaging_export.h -Wb,pre_include="ace/pre.h" -Wb,post_include="ace/post.h" -I$(TAO_ROOT) Messaging_SyncScope_Policy.pidl -I../..
tao_idl -o orig -GA -Gp -Gd -Ge 1 -GT -Sc -Wb,export_macro=TAO_Messaging_Export -Wb,export_include=messaging_export.h -Wb,pre_include="ace/pre.h" -Wb,post_include="ace/post.h" -I$(TAO_ROOT) TAO_Ext.pidl -I../..
tao_idl -o orig -GA -Gp -Gd -Ge 1 -GT -Sc -Wb,export_macro=TAO_Messaging_Export -Wb,export_include=messaging_export.h -Wb,pre_include="ace/pre.h" -Wb,post_include="ace/post.h" -I$(TAO_ROOT) Messaging.pidl -I../..

copy orig\*A.cpp .
copy orig\*C.cpp .
copy orig\*C.h .
copy orig\*C.inl .
copy orig\MessagingS.* .
copy orig\MessagingS_T.* .

patch < diffs/Messaging.diff
patch < diffs/Messaging_No_Impl.diff
