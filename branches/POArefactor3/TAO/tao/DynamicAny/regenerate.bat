tao_idl -o orig -Gp -Gd -Ge 1 -Sa -Sc -St -GT -I../.. -Wb,export_macro=TAO_DynamicAny_Export -Wb,export_include=dynamicany_export.h -Wb,pre_include="ace/pre.h" -Wb,post_include="ace/post.h" DynamicAny.pidl

copy orig\*C.cpp .
copy orig\*C.h .
copy orig\*C.inl .

patch < diffs/DynamicAny.diff

