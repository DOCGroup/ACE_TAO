# Microsoft Developer Studio Project File - Name="RTCosScheduling_Client" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=RTCosScheduling_Client - Win32 Static Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RTCosScheduling_Client_Static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RTCosScheduling_Client_Static.mak" CFG="RTCosScheduling_Client - Win32 Static Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RTCosScheduling_Client - Win32 Static Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "RTCosScheduling_Client - Win32 Static Release" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RTCosScheduling_Client - Win32 Static Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "RTCosScheduling_Client___Win32_Static_Debug"
# PROP BASE Intermediate_Dir "RTCosScheduling_Client___Win32_Static_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Gy /I "../../../" /I "../../../tao" /I "../../../orbsvcs" /I "..\..\..\.." /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /FD /D /D /D /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Gy /I "../../../" /I "../../../tao" /I "../../../orbsvcs" /I "..\..\..\.." /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /FD /D /D /D /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /i "../../../" /i "../../../tao" /i "../../../orbsvcs" /i "..\..\..\.." /d "_DEBUG"
# ADD RSC /l 0x409 /i "../../../" /i "../../../tao" /i "../../../orbsvcs" /i "..\..\..\.." /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 advapi32.lib user32.lib TAO_RTCosSchedulingsd.lib TAO_RTPortableServersd.lib TAO_RTCORBAsd.lib TAO_PortableServersd.lib TAO_IORInterceptorsd.lib TAO_Valuetypesd.lib TAO_ObjRefTemplatesd.lib TAOsd.lib ACEsd.lib /nologo /subsystem:console /incremental:no /debug /machine:I386 /out:"Debug\client.exe" /libpath:"..\..\..\lib" /libpath:"..\..\tao\RTPortableServer" /libpath:"..\..\tao\RTCORBA" /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao\IORInterceptor" /libpath:"..\..\tao\ValueType" /libpath:"..\..\tao\ObjRefTemplate" /libpath:"..\..\tao" /libpath:"..\..\..\ace" /libpath:"..\..\..\..\lib" /version:1.3.4
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 advapi32.lib user32.lib TAO_RTCosSchedulingsd.lib TAO_RTPortableServersd.lib TAO_RTCORBAsd.lib TAO_PortableServersd.lib TAO_IORInterceptorsd.lib TAO_Valuetypesd.lib TAO_ObjRefTemplatesd.lib TAOsd.lib ACEsd.lib /nologo /subsystem:console /incremental:no /debug /machine:I386 /out:"Debug\client.exe" /libpath:"..\..\..\lib" /libpath:"..\..\tao\RTPortableServer" /libpath:"..\..\tao\RTCORBA" /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao\IORInterceptor" /libpath:"..\..\tao\ValueType" /libpath:"..\..\tao\ObjRefTemplate" /libpath:"..\..\tao" /libpath:"..\..\..\ace" /libpath:"..\..\..\..\lib" /version:1.3.4
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "RTCosScheduling_Client - Win32 Static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "RTCosScheduling_Client___Win32_Static_Release"
# PROP BASE Intermediate_Dir "RTCosScheduling_Client___Win32_Static_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GR /GX /O2 /I "../../../" /I "../../../tao" /I "../../../orbsvcs" /I "..\..\..\.." /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /FD /D /D /D /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "../../../" /I "../../../tao" /I "../../../orbsvcs" /I "..\..\..\.." /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /FD /D /D /D /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /i "../../../" /i "../../../tao" /i "../../../orbsvcs" /i "..\..\..\.." /d "NDEBUG"
# ADD RSC /l 0x409 /i "../../../" /i "../../../tao" /i "../../../orbsvcs" /i "..\..\..\.." /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 advapi32.lib user32.lib TAO_RTCosSchedulings.lib TAO_RTPortableServers.lib TAO_RTCORBAs.lib TAO_PortableServers.lib TAO_IORInterceptors.lib TAO_Valuetypes.lib TAO_ObjRefTemplates.lib TAOs.lib ACEs.lib /nologo /subsystem:console /machine:I386 /out:"Release\client.exe" /libpath:"..\..\..\..\lib" /libpath:"..\..\tao\RTPortableServer" /libpath:"..\..\tao\RTCORBA" /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao\IORInterceptor" /libpath:"..\..\tao\ValueType" /libpath:"..\..\tao\ObjRefTemplate" /libpath:"..\..\tao" /libpath:"..\..\..\ace" /version:1.3.4
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 advapi32.lib user32.lib TAO_RTCosSchedulings.lib TAO_RTPortableServers.lib TAO_RTCORBAs.lib TAO_PortableServers.lib TAO_IORInterceptors.lib TAO_Valuetypes.lib TAO_ObjRefTemplates.lib TAOs.lib ACEs.lib /nologo /subsystem:console /machine:I386 /out:"Release\client.exe" /libpath:"..\..\..\..\lib" /libpath:"..\..\tao\RTPortableServer" /libpath:"..\..\tao\RTCORBA" /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao\IORInterceptor" /libpath:"..\..\tao\ValueType" /libpath:"..\..\tao\ObjRefTemplate" /libpath:"..\..\tao" /libpath:"..\..\..\ace" /version:1.3.4
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "RTCosScheduling_Client - Win32 Static Debug"
# Name "RTCosScheduling_Client - Win32 Static Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE=".\client.cpp"
# End Source File
# Begin Source File

SOURCE=".\testSchedC.cpp"
# End Source File
# Begin Source File

SOURCE=".\testSchedS.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE=".\testSchedC.h"
# End Source File
# Begin Source File

SOURCE=".\testSchedS.h"
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
# Begin Source File

SOURCE=".\testSchedC.i"
# End Source File
# Begin Source File

SOURCE=".\testSchedS.i"
# End Source File
# End Group
# Begin Group "Documentation"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\README"
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter "idl"
# Begin Source File

SOURCE=".\testSched.idl"

!IF  "$(CFG)" == "RTCosScheduling_Client - Win32 Static Debug"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking IDL Compiler on $(InputPath)
InputPath=".\testSched.idl"
InputName=testSched

BuildCmds= \
	PATH=%PATH%;..\..\..\..\lib \
	..\..\..\..\bin\tao_idl -o . -Ge 1 -Sc -Wb,pre_include=ace\pre.h -Wb,post_include=ace\post.h -Gv $(InputPath) \
	

".\$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "RTCosScheduling_Client - Win32 Static Release"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking IDL Compiler on $(InputPath)
InputPath=".\testSched.idl"
InputName=testSched

BuildCmds= \
	PATH=%PATH%;..\..\..\..\lib \
	..\..\..\..\bin\Release\tao_idl -o . -Ge 1 -Sc -Wb,pre_include=ace\pre.h -Wb,post_include=ace\post.h -Gv $(InputPath) \
	

".\$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
