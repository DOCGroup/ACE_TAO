# Microsoft Developer Studio Project File - Name="SSLIOP_Static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=SSLIOP_Static - Win32 Static Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SSLIOP_Static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SSLIOP_Static.mak" CFG="SSLIOP_Static - Win32 Static Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SSLIOP_Static - Win32 Static Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "SSLIOP_Static - Win32 Static Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SSLIOP_Static - Win32 Static Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SSLIOP_Static___Win32_Static_Debug"
# PROP BASE Intermediate_Dir "SSLIOP_Static___Win32_Static_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Debug\SSLIOP"
# PROP Target_Dir ""
MTL=midl.exe
LINK32=link.exe -lib
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "ACE_AS_STATIC_LIBS" /D "TAO_AS_STATIC_LIBS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"TAO_SSLIOPsd.lib"

!ELSEIF  "$(CFG)" == "SSLIOP_Static - Win32 Static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SSLIOP_Static___Win32_Static_Release"
# PROP BASE Intermediate_Dir "SSLIOP_Static___Win32_Static_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Release\SSLIOP"
# PROP Target_Dir ""
MTL=midl.exe
LINK32=link.exe -lib
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "ACE_AS_STATIC_LIBS" /D "TAO_AS_STATIC_LIBS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"TAO_SSLIOPs.lib"

!ENDIF 

# Begin Target

# Name "SSLIOP_Static - Win32 Static Debug"
# Name "SSLIOP_Static - Win32 Static Release"
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

SOURCE=.\SSLIOP\SSLIOP_Accept_Strategy.cpp
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
# ADD CPP /GR
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
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

SOURCE=.\SSLIOP\SSLIOP_Accept_Strategy.h
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
# Begin Group "Inline Files"

# PROP Default_Filter "inl"
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

SOURCE=.\SSLIOP\SSLIOP_Credentials.inl
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
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter ".idl"
# Begin Source File

SOURCE=.\SSLIOP.idl

!IF  "$(CFG)" == "SSLIOP_Static - Win32 Static Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=.\SSLIOP.idl
InputName=SSLIOP

BuildCmds= \
	..\..\..\bin\tao_idl_static -Ge 1 -I../..  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_SSLIOP_Export -Wb,export_include=SSLIOP\SSLIOP_Export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "SSLIOP_Static - Win32 Static Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=.\SSLIOP.idl
InputName=SSLIOP

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -Ge 1 -I../..  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_SSLIOP_Export -Wb,export_include=SSLIOP\SSLIOP_Export.h $(InputName).idl

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
