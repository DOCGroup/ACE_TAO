tao_idl -o orig -Ge 1 -GT -Sc -Sa -St -Wb,export_macro=TAO_IORTable_Export -Wb,export_include="iortable_export.h" -Wb,pre_include="ace/pre.h" -Wb,post_include="ace/post.h" IORTable.pidl -I..\..

copy orig\*C.cpp .
copy orig\*C.h .
copy orig\*C.inl .

patch < diff/IORTable.diff
