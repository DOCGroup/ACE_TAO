# Microsoft Developer Studio Project File - Name="java_cup" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Java Virtual Machine Java Project" 0x0809

CFG=java_cup - Java Virtual Machine Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "java_cup.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "java_cup.mak" CFG="java_cup - Java Virtual Machine Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "java_cup - Java Virtual Machine Release" (based on\
 "Java Virtual Machine Java Project")
!MESSAGE "java_cup - Java Virtual Machine Debug" (based on\
 "Java Virtual Machine Java Project")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
JAVA=jvc.exe

!IF  "$(CFG)" == "java_cup - Java Virtual Machine Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir ""
# PROP Target_Dir ""
# ADD BASE JAVA /O
# ADD JAVA /O

!ELSEIF  "$(CFG)" == "java_cup - Java Virtual Machine Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../"
# PROP Intermediate_Dir ""
# PROP Target_Dir ""
# ADD BASE JAVA /g
# ADD JAVA /g

!ENDIF 

# Begin Target

# Name "java_cup - Java Virtual Machine Release"
# Name "java_cup - Java Virtual Machine Debug"
# Begin Source File

SOURCE=.\action_part.java
# End Source File
# Begin Source File

SOURCE=.\action_production.java
# End Source File
# Begin Source File

SOURCE=.\runtime\char_token.java
# End Source File
# Begin Source File

SOURCE=.\runtime\double_token.java
# End Source File
# Begin Source File

SOURCE=.\emit.java
# End Source File
# Begin Source File

SOURCE=.\runtime\float_token.java
# End Source File
# Begin Source File

SOURCE=.\runtime\int_token.java
# End Source File
# Begin Source File

SOURCE=.\internal_error.java
# End Source File
# Begin Source File

SOURCE=.\lalr_item.java
# End Source File
# Begin Source File

SOURCE=.\lalr_item_set.java
# End Source File
# Begin Source File

SOURCE=.\lalr_state.java
# End Source File
# Begin Source File

SOURCE=.\lalr_transition.java
# End Source File
# Begin Source File

SOURCE=.\lexer.java
# End Source File
# Begin Source File

SOURCE=.\runtime\long_token.java
# End Source File
# Begin Source File

SOURCE=.\lr_item_core.java
# End Source File
# Begin Source File

SOURCE=.\runtime\lr_parser.java
# End Source File
# Begin Source File

SOURCE=.\Main.java
# End Source File
# Begin Source File

SOURCE=.\non_terminal.java
# End Source File
# Begin Source File

SOURCE=.\parse_action.java
# End Source File
# Begin Source File

SOURCE=.\parse_action_row.java
# End Source File
# Begin Source File

SOURCE=.\parse_action_table.java
# End Source File
# Begin Source File

SOURCE=.\parse_reduce_row.java
# End Source File
# Begin Source File

SOURCE=.\parse_reduce_table.java
# End Source File
# Begin Source File

SOURCE=.\parser.java
# End Source File
# Begin Source File

SOURCE=.\production.java
# End Source File
# Begin Source File

SOURCE=.\production_part.java
# End Source File
# Begin Source File

SOURCE=.\reduce_action.java
# End Source File
# Begin Source File

SOURCE=.\shift_action.java
# End Source File
# Begin Source File

SOURCE=.\runtime\str_token.java
# End Source File
# Begin Source File

SOURCE=.\sym.java
# End Source File
# Begin Source File

SOURCE=.\runtime\symbol.java
# End Source File
# Begin Source File

SOURCE=.\symbol.java
# End Source File
# Begin Source File

SOURCE=.\symbol_part.java
# End Source File
# Begin Source File

SOURCE=.\symbol_set.java
# End Source File
# Begin Source File

SOURCE=.\terminal.java
# End Source File
# Begin Source File

SOURCE=.\terminal_set.java
# End Source File
# Begin Source File

SOURCE=.\runtime\token.java
# End Source File
# Begin Source File

SOURCE=.\version.java
# End Source File
# Begin Source File

SOURCE=.\runtime\virtual_parse_stack.java
# End Source File
# End Target
# End Project
