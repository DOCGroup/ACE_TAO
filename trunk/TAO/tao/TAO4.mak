# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=TAO - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to TAO - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "TAO - Win32 Release" && "$(CFG)" != "TAO - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "TAO4.mak" CFG="TAO - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TAO - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TAO - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "TAO - Win32 Debug"
RSC=rc.exe
CPP=cl.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "TAO - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.
INTDIR=.\Release

ALL : "$(OUTDIR)\TAO4.dll"

CLEAN : 
	-@erase "$(INTDIR)\Any.obj"
	-@erase "$(INTDIR)\append.obj"
	-@erase "$(INTDIR)\CDR.obj"
	-@erase "$(INTDIR)\Client_Strategy_Factory.obj"
	-@erase "$(INTDIR)\Connect.obj"
	-@erase "$(INTDIR)\debug.obj"
	-@erase "$(INTDIR)\decode.obj"
	-@erase "$(INTDIR)\deep_copy.obj"
	-@erase "$(INTDIR)\deep_free.obj"
	-@erase "$(INTDIR)\default_client.obj"
	-@erase "$(INTDIR)\default_server.obj"
	-@erase "$(INTDIR)\encode.obj"
	-@erase "$(INTDIR)\Exception.obj"
	-@erase "$(INTDIR)\GIOP.obj"
	-@erase "$(INTDIR)\IIOP_Interpreter.obj"
	-@erase "$(INTDIR)\IIOP_Object.obj"
	-@erase "$(INTDIR)\IIOP_ORB.obj"
	-@erase "$(INTDIR)\Marshal.obj"
	-@erase "$(INTDIR)\NVList.obj"
	-@erase "$(INTDIR)\Object.obj"
	-@erase "$(INTDIR)\Object_KeyC.obj"
	-@erase "$(INTDIR)\Object_Table.obj"
	-@erase "$(INTDIR)\Operation_Table.obj"
	-@erase "$(INTDIR)\ORB.obj"
	-@erase "$(INTDIR)\ORB_Core.obj"
	-@erase "$(INTDIR)\params.obj"
	-@erase "$(INTDIR)\POA.obj"
	-@erase "$(INTDIR)\POAC.obj"
	-@erase "$(INTDIR)\POAS.obj"
	-@erase "$(INTDIR)\PolicyC.obj"
	-@erase "$(INTDIR)\PolicyS.obj"
	-@erase "$(INTDIR)\Principal.obj"
	-@erase "$(INTDIR)\Request.obj"
	-@erase "$(INTDIR)\Sequence.obj"
	-@erase "$(INTDIR)\Servant_Base.obj"
	-@erase "$(INTDIR)\Server_Request.obj"
	-@erase "$(INTDIR)\Server_Strategy_Factory.obj"
	-@erase "$(INTDIR)\skip.obj"
	-@erase "$(INTDIR)\Stub.obj"
	-@erase "$(INTDIR)\TAO.obj"
	-@erase "$(INTDIR)\TAO_Internal.obj"
	-@erase "$(INTDIR)\Timeprobe.obj"
	-@erase "$(INTDIR)\Typecode.obj"
	-@erase "$(INTDIR)\Typecode_Constants.obj"
	-@erase "$(OUTDIR)\TAO4.dll"
	-@erase "$(OUTDIR)\TAO4.exp"
	-@erase "$(OUTDIR)\TAO4.lib"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "." /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "__IIOP_BUILD" /D "ACE_BUILD_SVC_DLL" /D "__ACE_INLINE__" /YX /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "." /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "__IIOP_BUILD" /D "ACE_BUILD_SVC_DLL" /D "__ACE_INLINE__"\
 /Fp"$(INTDIR)/TAO4.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/TAO4.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
LINK32_FLAGS=ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)/TAO4.pdb" /machine:I386 /out:"$(OUTDIR)/TAO4.dll"\
 /implib:"$(OUTDIR)/TAO4.lib" 
LINK32_OBJS= \
	"$(INTDIR)\Any.obj" \
	"$(INTDIR)\append.obj" \
	"$(INTDIR)\CDR.obj" \
	"$(INTDIR)\Client_Strategy_Factory.obj" \
	"$(INTDIR)\Connect.obj" \
	"$(INTDIR)\debug.obj" \
	"$(INTDIR)\decode.obj" \
	"$(INTDIR)\deep_copy.obj" \
	"$(INTDIR)\deep_free.obj" \
	"$(INTDIR)\default_client.obj" \
	"$(INTDIR)\default_server.obj" \
	"$(INTDIR)\encode.obj" \
	"$(INTDIR)\Exception.obj" \
	"$(INTDIR)\GIOP.obj" \
	"$(INTDIR)\IIOP_Interpreter.obj" \
	"$(INTDIR)\IIOP_Object.obj" \
	"$(INTDIR)\IIOP_ORB.obj" \
	"$(INTDIR)\Marshal.obj" \
	"$(INTDIR)\NVList.obj" \
	"$(INTDIR)\Object.obj" \
	"$(INTDIR)\Object_KeyC.obj" \
	"$(INTDIR)\Object_Table.obj" \
	"$(INTDIR)\Operation_Table.obj" \
	"$(INTDIR)\ORB.obj" \
	"$(INTDIR)\ORB_Core.obj" \
	"$(INTDIR)\params.obj" \
	"$(INTDIR)\POA.obj" \
	"$(INTDIR)\POAC.obj" \
	"$(INTDIR)\POAS.obj" \
	"$(INTDIR)\PolicyC.obj" \
	"$(INTDIR)\PolicyS.obj" \
	"$(INTDIR)\Principal.obj" \
	"$(INTDIR)\Request.obj" \
	"$(INTDIR)\Sequence.obj" \
	"$(INTDIR)\Servant_Base.obj" \
	"$(INTDIR)\Server_Request.obj" \
	"$(INTDIR)\Server_Strategy_Factory.obj" \
	"$(INTDIR)\skip.obj" \
	"$(INTDIR)\Stub.obj" \
	"$(INTDIR)\TAO.obj" \
	"$(INTDIR)\TAO_Internal.obj" \
	"$(INTDIR)\Timeprobe.obj" \
	"$(INTDIR)\Typecode.obj" \
	"$(INTDIR)\Typecode_Constants.obj"

"$(OUTDIR)\TAO4.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.
INTDIR=.\Debug

ALL : "$(OUTDIR)\TAO4.dll"

CLEAN : 
	-@erase "$(INTDIR)\Any.obj"
	-@erase "$(INTDIR)\append.obj"
	-@erase "$(INTDIR)\CDR.obj"
	-@erase "$(INTDIR)\Client_Strategy_Factory.obj"
	-@erase "$(INTDIR)\Connect.obj"
	-@erase "$(INTDIR)\debug.obj"
	-@erase "$(INTDIR)\decode.obj"
	-@erase "$(INTDIR)\deep_copy.obj"
	-@erase "$(INTDIR)\deep_free.obj"
	-@erase "$(INTDIR)\default_client.obj"
	-@erase "$(INTDIR)\default_server.obj"
	-@erase "$(INTDIR)\encode.obj"
	-@erase "$(INTDIR)\Exception.obj"
	-@erase "$(INTDIR)\GIOP.obj"
	-@erase "$(INTDIR)\IIOP_Interpreter.obj"
	-@erase "$(INTDIR)\IIOP_Object.obj"
	-@erase "$(INTDIR)\IIOP_ORB.obj"
	-@erase "$(INTDIR)\Marshal.obj"
	-@erase "$(INTDIR)\NVList.obj"
	-@erase "$(INTDIR)\Object.obj"
	-@erase "$(INTDIR)\Object_KeyC.obj"
	-@erase "$(INTDIR)\Object_Table.obj"
	-@erase "$(INTDIR)\Operation_Table.obj"
	-@erase "$(INTDIR)\ORB.obj"
	-@erase "$(INTDIR)\ORB_Core.obj"
	-@erase "$(INTDIR)\params.obj"
	-@erase "$(INTDIR)\POA.obj"
	-@erase "$(INTDIR)\POAC.obj"
	-@erase "$(INTDIR)\POAS.obj"
	-@erase "$(INTDIR)\PolicyC.obj"
	-@erase "$(INTDIR)\PolicyS.obj"
	-@erase "$(INTDIR)\Principal.obj"
	-@erase "$(INTDIR)\Request.obj"
	-@erase "$(INTDIR)\Sequence.obj"
	-@erase "$(INTDIR)\Servant_Base.obj"
	-@erase "$(INTDIR)\Server_Request.obj"
	-@erase "$(INTDIR)\Server_Strategy_Factory.obj"
	-@erase "$(INTDIR)\skip.obj"
	-@erase "$(INTDIR)\Stub.obj"
	-@erase "$(INTDIR)\TAO.obj"
	-@erase "$(INTDIR)\TAO_Internal.obj"
	-@erase "$(INTDIR)\Timeprobe.obj"
	-@erase "$(INTDIR)\Typecode.obj"
	-@erase "$(INTDIR)\Typecode_Constants.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\TAO4.dll"
	-@erase "$(OUTDIR)\TAO4.exp"
	-@erase "$(OUTDIR)\TAO4.ilk"
	-@erase "$(OUTDIR)\TAO4.lib"
	-@erase "$(OUTDIR)\TAO4.pdb"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "." /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "__IIOP_BUILD" /D "ACE_BUILD_SVC_DLL" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "." /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "__IIOP_BUILD" /D "ACE_BUILD_SVC_DLL" /Fp"$(INTDIR)/TAO4.pch" /YX\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/TAO4.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 aced.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
LINK32_FLAGS=aced.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /incremental:yes\
 /pdb:"$(OUTDIR)/TAO4.pdb" /debug /machine:I386 /out:"$(OUTDIR)/TAO4.dll"\
 /implib:"$(OUTDIR)/TAO4.lib" 
LINK32_OBJS= \
	"$(INTDIR)\Any.obj" \
	"$(INTDIR)\append.obj" \
	"$(INTDIR)\CDR.obj" \
	"$(INTDIR)\Client_Strategy_Factory.obj" \
	"$(INTDIR)\Connect.obj" \
	"$(INTDIR)\debug.obj" \
	"$(INTDIR)\decode.obj" \
	"$(INTDIR)\deep_copy.obj" \
	"$(INTDIR)\deep_free.obj" \
	"$(INTDIR)\default_client.obj" \
	"$(INTDIR)\default_server.obj" \
	"$(INTDIR)\encode.obj" \
	"$(INTDIR)\Exception.obj" \
	"$(INTDIR)\GIOP.obj" \
	"$(INTDIR)\IIOP_Interpreter.obj" \
	"$(INTDIR)\IIOP_Object.obj" \
	"$(INTDIR)\IIOP_ORB.obj" \
	"$(INTDIR)\Marshal.obj" \
	"$(INTDIR)\NVList.obj" \
	"$(INTDIR)\Object.obj" \
	"$(INTDIR)\Object_KeyC.obj" \
	"$(INTDIR)\Object_Table.obj" \
	"$(INTDIR)\Operation_Table.obj" \
	"$(INTDIR)\ORB.obj" \
	"$(INTDIR)\ORB_Core.obj" \
	"$(INTDIR)\params.obj" \
	"$(INTDIR)\POA.obj" \
	"$(INTDIR)\POAC.obj" \
	"$(INTDIR)\POAS.obj" \
	"$(INTDIR)\PolicyC.obj" \
	"$(INTDIR)\PolicyS.obj" \
	"$(INTDIR)\Principal.obj" \
	"$(INTDIR)\Request.obj" \
	"$(INTDIR)\Sequence.obj" \
	"$(INTDIR)\Servant_Base.obj" \
	"$(INTDIR)\Server_Request.obj" \
	"$(INTDIR)\Server_Strategy_Factory.obj" \
	"$(INTDIR)\skip.obj" \
	"$(INTDIR)\Stub.obj" \
	"$(INTDIR)\TAO.obj" \
	"$(INTDIR)\TAO_Internal.obj" \
	"$(INTDIR)\Timeprobe.obj" \
	"$(INTDIR)\Typecode.obj" \
	"$(INTDIR)\Typecode_Constants.obj"

"$(OUTDIR)\TAO4.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "TAO - Win32 Release"
# Name "TAO - Win32 Debug"

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\Typecode_Constants.cpp
NODEP_CPP_TYPEC=\
	".\tao\corba.h"\
	

"$(INTDIR)\Typecode_Constants.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\append.cpp
NODEP_CPP_APPEN=\
	".\tao\corba.h"\
	

"$(INTDIR)\append.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\CDR.cpp
NODEP_CPP_CDR_C=\
	".\tao\CDR.i"\
	".\tao\corba.h"\
	

"$(INTDIR)\CDR.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Client_Strategy_Factory.cpp
NODEP_CPP_CLIEN=\
	".\tao\corba.h"\
	

"$(INTDIR)\Client_Strategy_Factory.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Connect.cpp
NODEP_CPP_CONNE=\
	".\tao\Connect.i"\
	".\tao\corba.h"\
	".\tao\Timeprobe.h"\
	

"$(INTDIR)\Connect.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\debug.cpp
NODEP_CPP_DEBUG=\
	".\tao\corba.h"\
	

"$(INTDIR)\debug.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\decode.cpp
NODEP_CPP_DECOD=\
	".\tao\corba.h"\
	

"$(INTDIR)\decode.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\deep_copy.cpp
NODEP_CPP_DEEP_=\
	".\tao\corba.h"\
	

"$(INTDIR)\deep_copy.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\deep_free.cpp
NODEP_CPP_DEEP_F=\
	".\tao\corba.h"\
	

"$(INTDIR)\deep_free.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\default_client.cpp
NODEP_CPP_DEFAU=\
	".\tao\default_client.h"\
	".\tao\default_client.i"\
	".\tao\ORB_Core.h"\
	

"$(INTDIR)\default_client.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\default_server.cpp
NODEP_CPP_DEFAUL=\
	".\tao\default_client.i"\
	".\tao\default_server.h"\
	".\tao\ORB_Core.h"\
	

"$(INTDIR)\default_server.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\encode.cpp
NODEP_CPP_ENCOD=\
	".\tao\corba.h"\
	

"$(INTDIR)\encode.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Exception.cpp
NODEP_CPP_EXCEP=\
	".\tao\corba.h"\
	".\tao\Exception.i"\
	

"$(INTDIR)\Exception.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\GIOP.cpp
NODEP_CPP_GIOP_=\
	".\tao\corba.h"\
	".\tao\GIOP.i"\
	".\tao\Timeprobe.h"\
	

"$(INTDIR)\GIOP.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\IIOP_Interpreter.cpp
NODEP_CPP_IIOP_=\
	".\tao\corba.h"\
	

"$(INTDIR)\IIOP_Interpreter.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\IIOP_Object.cpp
NODEP_CPP_IIOP_O=\
	".\tao\corba.h"\
	".\tao\IIOP_Object.i"\
	

"$(INTDIR)\IIOP_Object.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\IIOP_ORB.cpp
NODEP_CPP_IIOP_OR=\
	".\tao\corba.h"\
	".\tao\IIOP_ORB.i"\
	

"$(INTDIR)\IIOP_ORB.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Marshal.cpp
NODEP_CPP_MARSH=\
	".\tao\corba.h"\
	".\tao\Marshal.i"\
	

"$(INTDIR)\Marshal.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\NVList.cpp
NODEP_CPP_NVLIS=\
	".\tao\corba.h"\
	".\tao\NVList.i"\
	

"$(INTDIR)\NVList.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Object.cpp
NODEP_CPP_OBJEC=\
	".\tao\corba.h"\
	".\tao\Object.i"\
	

"$(INTDIR)\Object.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Object_KeyC.cpp
DEP_CPP_OBJECT=\
	".\Object_KeyC.i"\
	
NODEP_CPP_OBJECT=\
	".\tao\corba.h"\
	

"$(INTDIR)\Object_KeyC.obj" : $(SOURCE) $(DEP_CPP_OBJECT) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Object_Table.cpp
NODEP_CPP_OBJECT_=\
	".\ace\Auto_Ptr.h"\
	".\tao\corba.h"\
	".\tao\Object_Table.h"\
	

"$(INTDIR)\Object_Table.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Operation_Table.cpp
NODEP_CPP_OPERA=\
	".\tao\corba.h"\
	

"$(INTDIR)\Operation_Table.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ORB.cpp
NODEP_CPP_ORB_C=\
	".\ace\Dynamic_Service.h"\
	".\ace\Object_Manager.h"\
	".\ace\Service_Repository.h"\
	".\ace\SOCK_Dgram_Mcast.h"\
	".\tao\corba.h"\
	".\tao\ORB.i"\
	".\tao\TAO_Internal.h"\
	".\tao\Timeprobe.h"\
	

"$(INTDIR)\ORB.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ORB_Core.cpp
NODEP_CPP_ORB_CO=\
	".\ace\Arg_Shifter.h"\
	".\ace\Dynamic_Service.h"\
	".\ace\Env_Value_T.h"\
	".\ace\Select_Reactor.h"\
	".\ace\Service_Repository.h"\
	".\tao\corba.h"\
	".\tao\ORB_Core.i"\
	".\tao\TAO_Internal.h"\
	

"$(INTDIR)\ORB_Core.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\params.cpp
NODEP_CPP_PARAM=\
	".\tao\corba.h"\
	".\tao\params.i"\
	

"$(INTDIR)\params.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\POA.cpp
NODEP_CPP_POA_C=\
	".\ace\Auto_Ptr.h"\
	".\tao\corba.h"\
	".\tao\poa_macros.h"\
	

"$(INTDIR)\POA.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\POAC.cpp
DEP_CPP_POAC_=\
	".\POAC.i"\
	
NODEP_CPP_POAC_=\
	".\tao\corba.h"\
	

"$(INTDIR)\POAC.obj" : $(SOURCE) $(DEP_CPP_POAC_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\POAS.cpp
DEP_CPP_POAS_=\
	".\POAS.i"\
	
NODEP_CPP_POAS_=\
	".\tao\corba.h"\
	

"$(INTDIR)\POAS.obj" : $(SOURCE) $(DEP_CPP_POAS_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\PolicyC.cpp
DEP_CPP_POLIC=\
	".\PolicyC.i"\
	
NODEP_CPP_POLIC=\
	".\tao\corba.h"\
	

"$(INTDIR)\PolicyC.obj" : $(SOURCE) $(DEP_CPP_POLIC) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\PolicyS.cpp
DEP_CPP_POLICY=\
	".\PolicyS.i"\
	
NODEP_CPP_POLICY=\
	".\tao\corba.h"\
	

"$(INTDIR)\PolicyS.obj" : $(SOURCE) $(DEP_CPP_POLICY) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Principal.cpp
NODEP_CPP_PRINC=\
	".\tao\corba.h"\
	

"$(INTDIR)\Principal.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Request.cpp
NODEP_CPP_REQUE=\
	".\tao\corba.h"\
	".\tao\Request.i"\
	

"$(INTDIR)\Request.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Sequence.cpp
NODEP_CPP_SEQUE=\
	".\tao\corba.h"\
	".\tao\Sequence.i"\
	

"$(INTDIR)\Sequence.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Servant_Base.cpp
NODEP_CPP_SERVA=\
	".\tao\corba.h"\
	

"$(INTDIR)\Servant_Base.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Server_Request.cpp
NODEP_CPP_SERVE=\
	".\tao\corba.h"\
	".\tao\Server_Request.i"\
	

"$(INTDIR)\Server_Request.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Server_Strategy_Factory.cpp
NODEP_CPP_SERVER=\
	".\tao\corba.h"\
	

"$(INTDIR)\Server_Strategy_Factory.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\skip.cpp
NODEP_CPP_SKIP_=\
	".\tao\corba.h"\
	

"$(INTDIR)\skip.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Stub.cpp
NODEP_CPP_STUB_=\
	".\tao\corba.h"\
	".\tao\Stub.i"\
	

"$(INTDIR)\Stub.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TAO.cpp
DEP_CPP_TAO_C=\
	".\TAO.h"\
	
NODEP_CPP_TAO_C=\
	".\tao\corba.h"\
	

"$(INTDIR)\TAO.obj" : $(SOURCE) $(DEP_CPP_TAO_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TAO_Internal.cpp
NODEP_CPP_TAO_I=\
	".\ace\Object_Manager.h"\
	".\ace\Service_Config.h"\
	".\ace\Service_Repository.h"\
	".\tao\default_client.h"\
	".\tao\default_server.h"\
	".\tao\TAO_Internal.h"\
	

"$(INTDIR)\TAO_Internal.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Timeprobe.cpp
DEP_CPP_TIMEP=\
	".\Timeprobe.h"\
	".\Timeprobe.i"\
	
NODEP_CPP_TIMEP=\
	".\ace\OS.h"\
	".\ace\Synch.h"\
	

"$(INTDIR)\Timeprobe.obj" : $(SOURCE) $(DEP_CPP_TIMEP) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Typecode.cpp
NODEP_CPP_TYPECO=\
	".\tao\corba.h"\
	".\tao\Typecode.i"\
	

"$(INTDIR)\Typecode.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Any.cpp
NODEP_CPP_ANY_C=\
	".\tao\Any.i"\
	".\tao\corba.h"\
	

"$(INTDIR)\Any.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
# End Target
# End Project
################################################################################
