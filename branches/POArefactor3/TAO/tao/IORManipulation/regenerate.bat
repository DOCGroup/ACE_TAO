tao_idl -o orig -Ge 1 -GT -Sc -GA -Wb,export_macro=TAO_IORManip_Export -Wb,export_include="ior_manip_export.h" -Wb,pre_include="ace/pre.h" -Wb,post_include="ace/post.h" IOR.pidl -I..\..

copy orig\*A.cpp .
copy orig\*C.cpp .
copy orig\*C.h .
copy orig\*C.inl .

patch < diffs/IOR.diff
