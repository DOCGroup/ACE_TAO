tao_idl -o orig -Ge 1 -GT -Wb,export_macro=TAO_RTCORBA_Export -Wb,export_include="rtcorba_export.h" -Wb,pre_include="ace/pre.h" -Wb,post_include="ace/post.h" -I..\..\.. -I..\.. -I.. RTCORBA.pidl

copy orig\*C.cpp .
copy orig\*C.h .
copy orig\*C.inl .

patch < diffs/RTCORBA.diff
