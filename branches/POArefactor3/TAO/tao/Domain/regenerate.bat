tao_idl.exe -o orig -Gp -Gd -Ge 1 -GA -Wb,export_macro=TAO_Domain_Export -Wb,skel_export_include=domain_export.h -Wb,pre_include="ace/pre.h" -Wb,post_include="ace/post.h" -I %TAO_ROOT% -I %TAO_ROOT%\tao -I .. ..\Domain.pidl

copy orig\*S.cpp .
copy orig\*S.h .
copy orig\*S.inl .
copy orig\*S_T.cpp .
copy orig\*S_T.h .
copy orig\*S_T.inl .

patch < diffs/Domain.diff
