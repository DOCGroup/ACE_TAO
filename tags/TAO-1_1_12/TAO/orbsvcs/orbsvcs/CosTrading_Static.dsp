# Microsoft Developer Studio Project File - Name="CosTrading_Static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=CosTrading_Static - Win32 Static Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CosTrading_Static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CosTrading_Static.mak" CFG="CosTrading_Static - Win32 Static Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CosTrading_Static - Win32 Static Release" (based on "Win32 (x86) Static Library")
!MESSAGE "CosTrading_Static - Win32 Static Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CosTrading_Static - Win32 Static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Release\CosTrading"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "_MBCS" /D "_LIB" /D "TAO_AS_STATIC_LIBS" /D "NDEBUG" /D "ACE_AS_STATIC_LIBS" /D "WIN32" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"TAO_CosTradings.lib"

!ELSEIF  "$(CFG)" == "CosTrading_Static - Win32 Static Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Debug\CosTrading"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "_MBCS" /D "_LIB" /D "TAO_AS_STATIC_LIBS" /D "_DEBUG" /D "ACE_AS_STATIC_LIBS" /D "WIN32" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"TAO_CosTradingsd.lib"

!ENDIF 

# Begin Target

# Name "CosTrading_Static - Win32 Static Release"
# Name "CosTrading_Static - Win32 Static Debug"
# Begin Group "Template Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CosTradingDynamicS_T.cpp

!IF  "$(CFG)" == "CosTrading_Static - Win32 Static Release"

!ELSEIF  "$(CFG)" == "CosTrading_Static - Win32 Static Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosTradingReposS_T.cpp

!IF  "$(CFG)" == "CosTrading_Static - Win32 Static Release"

!ELSEIF  "$(CFG)" == "CosTrading_Static - Win32 Static Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosTradingS_T.cpp

!IF  "$(CFG)" == "CosTrading_Static - Win32 Static Release"

!ELSEIF  "$(CFG)" == "CosTrading_Static - Win32 Static Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Trader\Interpreter_Utils_T.cpp

!IF  "$(CFG)" == "CosTrading_Static - Win32 Static Release"

!ELSEIF  "$(CFG)" == "CosTrading_Static - Win32 Static Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Trader\Offer_Iterators_T.cpp

!IF  "$(CFG)" == "CosTrading_Static - Win32 Static Release"

!ELSEIF  "$(CFG)" == "CosTrading_Static - Win32 Static Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Trader\Trader_T.cpp

!IF  "$(CFG)" == "CosTrading_Static - Win32 Static Release"

!ELSEIF  "$(CFG)" == "CosTrading_Static - Win32 Static Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter "idl"
# Begin Source File

SOURCE=.\CosTrading.idl

!IF  "$(CFG)" == "CosTrading_Static - Win32 Static Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL on $(InputPath)
InputPath=.\CosTrading.idl
InputName=CosTrading

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -Ge 1 -I../../ -I../../tao  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_Trading_Export -Wb,export_include=Trader/trading_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "CosTrading_Static - Win32 Static Debug"

# PROP Ignore_Default_Tool 1
USERDEP__COSTR="..\..\..\bin\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL on $(InputPath)
InputPath=.\CosTrading.idl
InputName=CosTrading

BuildCmds= \
	..\..\..\bin\tao_idl_static -Ge 1 -I../../ -I../../tao  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_Trading_Export -Wb,export_include=Trader/trading_export.h $(InputName).idl

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
# Begin Source File

SOURCE=.\CosTradingDynamic.idl

!IF  "$(CFG)" == "CosTrading_Static - Win32 Static Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL on $(InputPath)
InputPath=.\CosTradingDynamic.idl
InputName=CosTradingDynamic

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -Ge 1 -I../../ -I../../tao  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_Trading_Export -Wb,export_include=Trader/trading_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "CosTrading_Static - Win32 Static Debug"

# PROP Ignore_Default_Tool 1
USERDEP__COSTRA="..\..\..\bin\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL on $(InputPath)
InputPath=.\CosTradingDynamic.idl
InputName=CosTradingDynamic

BuildCmds= \
	..\..\..\bin\tao_idl_static -Ge 1 -I../../ -I../../tao  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_Trading_Export -Wb,export_include=Trader/trading_export.h $(InputName).idl

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
# Begin Source File

SOURCE=.\CosTradingRepos.idl

!IF  "$(CFG)" == "CosTrading_Static - Win32 Static Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL on $(InputPath)
InputPath=.\CosTradingRepos.idl
InputName=CosTradingRepos

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -Ge 1 -I../../ -I../../tao  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_Trading_Export -Wb,export_include=Trader/trading_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "CosTrading_Static - Win32 Static Debug"

# PROP Ignore_Default_Tool 1
USERDEP__COSTRAD="..\..\..\bin\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL on $(InputPath)
InputPath=.\CosTradingRepos.idl
InputName=CosTradingRepos

BuildCmds= \
	..\..\..\bin\tao_idl_static -Ge 1 -I../../ -I../../tao  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_Trading_Export -Wb,export_include=Trader/trading_export.h $(InputName).idl

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

# PROP Default_Filter "i"
# Begin Source File

SOURCE=.\CosTradingC.i
# End Source File
# Begin Source File

SOURCE=.\CosTradingDynamicC.i
# End Source File
# Begin Source File

SOURCE=.\CosTradingDynamicS.i
# End Source File
# Begin Source File

SOURCE=.\CosTradingReposC.i
# End Source File
# Begin Source File

SOURCE=.\CosTradingReposS.i
# End Source File
# Begin Source File

SOURCE=.\CosTradingS.i
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\Trader\Constraint_Interpreter.h
# End Source File
# Begin Source File

SOURCE=.\Trader\Constraint_Nodes.h
# End Source File
# Begin Source File

SOURCE=.\Trader\Constraint_Tokens.h
# End Source File
# Begin Source File

SOURCE=.\Trader\Constraint_Visitors.h
# End Source File
# Begin Source File

SOURCE=.\CosTradingC.h
# End Source File
# Begin Source File

SOURCE=.\CosTradingS.h
# End Source File
# Begin Source File

SOURCE=.\Trader\Interpreter.h
# End Source File
# Begin Source File

SOURCE=.\Trader\Interpreter_Utils.h
# End Source File
# Begin Source File

SOURCE=.\Trader\Interpreter_Utils_T.h
# End Source File
# Begin Source File

SOURCE=.\Trader\Locking.h
# End Source File
# Begin Source File

SOURCE=.\Trader\Offer_Database.h
# End Source File
# Begin Source File

SOURCE=.\Trader\Offer_Iterators.h
# End Source File
# Begin Source File

SOURCE=.\Trader\Offer_Iterators_T.h
# End Source File
# Begin Source File

SOURCE=.\Trader\Service_Type_Repository.h
# End Source File
# Begin Source File

SOURCE=.\Trader\Trader.h
# End Source File
# Begin Source File

SOURCE=.\Trader\Trader_Constraint_Visitors.h
# End Source File
# Begin Source File

SOURCE=.\Trader\Trader_Interfaces.h
# End Source File
# Begin Source File

SOURCE=.\Trader\Trader_T.h
# End Source File
# Begin Source File

SOURCE=.\Trader\Trader_Utils.h
# End Source File
# Begin Source File

SOURCE=.\Trader\trading_export.h
# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\Trader\Constraint_Interpreter.cpp
# End Source File
# Begin Source File

SOURCE=.\Trader\Constraint_l.cpp
# End Source File
# Begin Source File

SOURCE=.\Trader\Constraint_Nodes.cpp
# End Source File
# Begin Source File

SOURCE=.\Trader\Constraint_Visitors.cpp
# End Source File
# Begin Source File

SOURCE=.\Trader\Constraint_y.cpp
# End Source File
# Begin Source File

SOURCE=.\CosTradingC.cpp
# End Source File
# Begin Source File

SOURCE=.\CosTradingDynamicC.cpp
# End Source File
# Begin Source File

SOURCE=.\CosTradingDynamicS.cpp
# End Source File
# Begin Source File

SOURCE=.\CosTradingReposC.cpp
# End Source File
# Begin Source File

SOURCE=.\CosTradingReposS.cpp
# End Source File
# Begin Source File

SOURCE=.\CosTradingS.cpp
# End Source File
# Begin Source File

SOURCE=.\Trader\Interpreter.cpp
# End Source File
# Begin Source File

SOURCE=.\Trader\Interpreter_Utils.cpp
# End Source File
# Begin Source File

SOURCE=.\Trader\Offer_Database.cpp
# End Source File
# Begin Source File

SOURCE=.\Trader\Offer_Iterators.cpp
# End Source File
# Begin Source File

SOURCE=.\Trader\Service_Type_Repository.cpp
# End Source File
# Begin Source File

SOURCE=.\Trader\Trader.cpp
# End Source File
# Begin Source File

SOURCE=.\Trader\Trader_Constraint_Visitors.cpp
# End Source File
# Begin Source File

SOURCE=.\Trader\Trader_Interfaces.cpp
# End Source File
# Begin Source File

SOURCE=.\Trader\Trader_Utils.cpp
# End Source File
# End Group
# End Target
# End Project
