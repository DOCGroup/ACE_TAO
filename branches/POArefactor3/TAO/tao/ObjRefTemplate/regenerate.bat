tao_idl -o orig -Ge 1 -GT -GA -Sc -I..\.. -Wb,export_macro=TAO_ORT_Export -Wb,export_include="ort_export.h" -Wb,pre_include="ace/pre.h" -Wb,post_include="ace/post.h" Default_ORT.pidl
tao_idl -o orig -Gp -Gd -Ge 1 -GT -GA -Sc -I..\.. -Wb,export_macro=TAO_ORT_Export -Wb,export_include="ort_export.h" -Wb,pre_include="ace/pre.h" -Wb,post_include="ace/post.h" ObjectReferenceTemplate.pidl

copy orig\*A.cpp .
copy orig\*C.h .
copy orig\*C.cpp .
copy orig\*C.inl .

patch < diffs/ObjectReferenceTemplate.diff
