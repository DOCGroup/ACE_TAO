REM $Id$

..\..\..\bin\generate_export_file.pl %1_STUB > %1_stub_export.h
..\..\..\bin\generate_export_file.pl %1_SERVANT > %1_servant_export.h
..\..\..\bin\generate_export_file.pl %1_EXECUTOR > %1_executor_export.h
