tao_idl -o orig -Gp -Gd -Ge 1 -GT -Sa -St -Sc -GT -I..\.. -Wb,export_macro=TAO_IORInterceptor_Export -Wb,export_include="iorinterceptor_export.h" -Wb,pre_include="ace/pre.h" -Wb,post_include="ace/post.h" IORInterceptor.pidl
tao_idl -o orig -Gp -Gd -Ge 1 -GT -Sa -St -I..\.. -Wb,export_macro=TAO_IORInterceptor_Export -Wb,export_include="iorinterceptor_export.h" -Wb,pre_include="ace/pre.h" -Wb,post_include="ace/post.h" IORInfo.pidl

copy orig\*A.cpp .
copy orig\*C.h .
copy orig\*C.cpp .
copy orig\*C.inl .

patch < diff/IORInterceptor.diff

