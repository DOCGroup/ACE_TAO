# Microsoft Developer Studio Project File - Name="SSLIOP" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=SSLIOP - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SSLIOP.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SSLIOP.mak" CFG="SSLIOP - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SSLIOP - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SSLIOP - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SSLIOP - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Release\SSLIOP"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SSLIOP_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "TAO_SSLIOP_BUILD_DLL" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 ACE_SSL.lib ace.lib TAO.lib TAO_Security.lib TAO_PortableServer.lib ssleay32.lib libeay32.lib /nologo /dll /machine:I386 /out:"..\..\..\bin\TAO_SSLIOP.dll" /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao" /libpath:"..\..\..\ace" /libpath:"..\..\..\ace\SSL"

!ELSEIF  "$(CFG)" == "SSLIOP - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Debug\SSLIOP"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SSLIOP_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "TAO_SSLIOP_BUILD_DLL" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ACE_SSLd.lib TAOd.lib aced.lib TAO_Securityd.lib TAO_PortableServerd.lib libeay32.lib ssleay32.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_SSLIOPd.dll" /pdbtype:sept /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao" /libpath:"..\..\..\ace" /libpath:"..\..\..\ace\SSL"
# SUBTRACT LINK32 /profile

!ENDIF 

# Begin Target

# Name "SSLIOP - Win32 Release"
# Name "SSLIOP - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\SSLIOP\IIOP_SSL_Acceptor.cpp
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\IIOP_SSL_Connection_Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\IIOP_SSL_Connector.cpp
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\IIOP_SSL_Transport.cpp
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\ssl_endpoints.cpp
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_Acceptor.cpp
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_Connection_Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_Connector.cpp
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_Credentials.cpp
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_Current.cpp
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_Current_Impl.cpp
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_Endpoint.cpp
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_EVP_PKEY.cpp
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_Invocation_Interceptor.cpp
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_ORBInitializer.cpp
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_Profile.cpp
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_ReceivedCredentials.cpp
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_TargetCredentials.cpp
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_Transport.cpp
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_Util.cpp
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_Vault.cpp
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_X509.cpp
# End Source File
# Begin Source File

SOURCE=.\SSLIOPC.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm"
# Begin Source File

SOURCE=.\SSLIOP\IIOP_SSL_Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\IIOP_SSL_Connection_Handler.h
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\IIOP_SSL_Connector.h
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\IIOP_SSL_Transport.h
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\ssl_endpoints.h
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_Connection_Handler.h
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_Connector.h
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_Credentials.h
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_Current.h
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_Current_Impl.h
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_Endpoint.h
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_EVP_PKEY.h
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_Export.h
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_Factory.h
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_Invocation_Interceptor.h
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_ORBInitializer.h
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_Profile.h
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_ReceivedCredentials.h
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_TargetCredentials.h
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_Transport.h
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_Util.h
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_Vault.h
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_X509.h
# End Source File
# Begin Source File

SOURCE=.\SSLIOPC.h
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter ".idl"
# Begin Source File

SOURCE=.\SSLIOP.idl

!IF  "$(CFG)" == "SSLIOP - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__SSLIO="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\SSLIOP.idl
InputName=SSLIOP

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -I../../ -I../../tao  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_SSLIOP_Export -Wb,export_include=SSLIOP\SSLIOP_Export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "SSLIOP - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__SSLIO="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\SSLIOP.idl
InputName=SSLIOP

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -I../../tao  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_SSLIOP_Export -Wb,export_include=SSLIOP\SSLIOP_Export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
# Begin Source File

SOURCE=.\SSLIOP\IIOP_SSL_Connection_Handler.inl
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\ssl_endpoints.i
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_Acceptor.i
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_Connection_Handler.i
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_Current.inl
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_Current_Impl.inl
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_Endpoint.i
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_EVP_PKEY.inl
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_Profile.i
# End Source File
# Begin Source File

SOURCE=.\SSLIOP\SSLIOP_X509.inl
# End Source File
# Begin Source File

SOURCE=.\SSLIOPC.i
# End Source File
# End Group
# Begin Group "Template Files"

# PROP Default_Filter ""
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "rc"
# Begin Source File

SOURCE=.\SSLIOP.rc
# End Source File
# End Group
# End Target
# End Project
