tao_idl -o orig -Ge 1 -GT -GA -Sa -Gp -Gd -Wb,export_macro=TAO_RTScheduler_Export -Wb,export_include="rtscheduler_export.h" -Wb,pre_include="ace/pre.h" -Wb,post_include="ace/post.h" RTScheduler.pidl -I..\..

copy orig\*A.cpp .
copy orig\*C.cpp .
copy orig\*C.h .
copy orig\*C.inl .

patch < diffs/RTScheduler.diff
