tao_idl -o orig -Ge 1 -GT -Sa -St -Wb,export_macro=TAO_BiDirGIOP_Export -Wb,export_include="bidirgiop_export.h" -Wb,pre_include="ace/pre.h" -Wb,post_include="ace/post.h" BiDirPolicy.pidl

copy orig\*C.cpp .
copy orig\*C.h .
copy orig\*C.inl .

patch < diffs/BiDirPolicy.diff
