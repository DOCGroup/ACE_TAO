%ACE_ROOT%\bin\tao_idl -Ge -g %ACE_ROOT%\bin\gperf.exe -Wb,pch_include=pch.h Broadcaster.idl
%ACE_ROOT%\bin\tao_idl_bp -Ge -g %ACE_ROOT%\bin\gperf_bp.exe -Wb,pch_include=pch.h Receiver.idl
