# Microsoft Developer Studio Project File - Name="RTCosScheduling_Client" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=RTCosScheduling_Client - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE run the tool that generated this project file and specify the
!MESSAGE nmake output type.  You can then use the following command:
!MESSAGE
!MESSAGE NMAKE /f "RTCosScheduling_Client_Static.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "RTCosScheduling_Client_Static.mak" CFG="RTCosScheduling_Client - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "RTCosScheduling_Client - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "RTCosScheduling_Client - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RTCosScheduling_Client - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release\RTCosScheduling_Client_Static"
# PROP Target_Dir ""
# ADD CPP /nologo /O2 /W3 /GX /MD /GR /I "../../../" /I "../../../tao"  /I "../../../orbsvcs" /I "..\..\..\.." /D NDEBUG /D WIN32 /D _CONSOLE /D  /D  /D  /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d NDEBUG /i "../../../" /i "../../../tao" /i "../../../orbsvcs" /i "..\..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO TAO_RTCosSchedulings.lib TAO_RTPortableServers.lib TAO_RTCORBAs.lib TAO_PortableServers.lib TAO_IORInterceptors.lib TAO_Valuetypes.lib TAO_ObjRefTemplates.lib TAOs.lib ACEs.lib /libpath:"..\..\..\..\lib" /libpath:"..\..\tao\RTPortableServer" /libpath:"..\..\tao\RTCORBA" /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao\IORInterceptor" /libpath:"..\..\tao\ValueType" /libpath:"..\..\tao\ObjRefTemplate" /libpath:"..\..\tao" /libpath:"..\..\..\ace" /libpath:"..\..\..\..\lib" /nologo /version:1.3.4 /subsystem:console /pdb:"Release\client.pdb"  /machine:I386 /out:"Release\client.exe"

!ELSEIF  "$(CFG)" == "RTCosScheduling_Client - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug\RTCosScheduling_Client_Static"
# PROP Target_Dir ""
# ADD CPP /nologo /Ob0 /W3 /Gm /GX /Zi /MDd /GR /Gy /I "../../../" /I "../../../tao" /I "../../../orbsvcs" /I "..\..\..\.." /D _DEBUG /D WIN32 /D _CONSOLE /D  /D  /D  /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d _DEBUG /i "../../../" /i "../../../tao" /i "../../../orbsvcs" /i "..\..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO TAO_RTCosSchedulingsd.lib TAO_RTPortableServersd.lib TAO_RTCORBAsd.lib TAO_PortableServersd.lib TAO_IORInterceptorsd.lib TAO_Valuetypesd.lib TAO_ObjRefTemplatesd.lib TAOsd.lib ACEsd.lib /libpath:"..\..\..\lib" /libpath:"..\..\tao\RTPortableServer" /libpath:"..\..\tao\RTCORBA" /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao\IORInterceptor" /libpath:"..\..\tao\ValueType" /libpath:"..\..\tao\ObjRefTemplate" /libpath:"..\..\tao" /libpath:"..\..\..\ace" /libpath:"..\..\..\..\lib" /nologo /version:1.3.4 /subsystem:console /pdb:"Debug\client.pdb" /debug /machine:I386 /out:"Debug\client.exe"

!ENDIF

# Begin Target

# Name "RTCosScheduling_Client - Win32 Release"
# Name "RTCosScheduling_Client - Win32 Debug"
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

!IF  "$(CFG)" == "RTCosScheduling_Client - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking IDL Compiler on $(InputPath)
InputPath=.\testSched.idl
InputName=testSched
InputDir=.

BuildCmds= \
	PATH=%PATH%;..\..\..\..\lib \
	..\..\..\..\bin\Release\tao_idl -o . -Ge 1 -Sc -Wb,pre_include=ace\pre.h -Wb,post_include=ace\post.h -Gv $(InputPath)

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

!ELSEIF  "$(CFG)" == "RTCosScheduling_Client - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking IDL Compiler on $(InputPath)
InputPath=.\testSched.idl
InputName=testSched
InputDir=.

BuildCmds= \
	PATH=%PATH%;..\..\..\..\lib \
	..\..\..\..\bin\tao_idl -o . -Ge 1 -Sc -Wb,pre_include=ace\pre.h -Wb,post_include=ace\post.h -Gv $(InputPath)

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
