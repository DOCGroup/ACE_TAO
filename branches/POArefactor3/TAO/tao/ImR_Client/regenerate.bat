
tao_idl.exe -o orig -Gp -Gd -Ge 1 -GT -Sc -GA -I../.. -Wb,export_macro=TAO_IMR_Client_Export -Wb,export_include="imr_client_export.h" -Wb,pre_include="ace/pre.h" -Wb,post_include="ace/post.h" ImplRepo.pidl
tao_idl.exe -o orig -Gp -Gd -Ge 1 -GT -Sc -GA -I../.. -Wb,export_macro=TAO_IMR_Client_Export -Wb,export_include="imr_client_export.h" -Wb,pre_include="ace/pre.h" -Wb,post_include="ace/post.h" ServerObject.pidl

copy orig\*A.cpp .
copy orig\*C.cpp .
copy orig\*C.h .
copy orig\*C.inl .
copy orig\ImplRepoS.* .
copy orig\ServerObjectS.* .

