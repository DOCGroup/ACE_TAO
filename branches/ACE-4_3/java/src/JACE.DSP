# Microsoft Developer Studio Project File - Name="JACE" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Java Virtual Machine Java Project" 0x0809

CFG=JACE - Java Virtual Machine Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "JACE.MAK".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "JACE.MAK" CFG="JACE - Java Virtual Machine Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "JACE - Java Virtual Machine Release" (based on\
 "Java Virtual Machine Java Project")
!MESSAGE "JACE - Java Virtual Machine Debug" (based on\
 "Java Virtual Machine Java Project")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
JAVA=jvc.exe

!IF  "$(CFG)" == "JACE - Java Virtual Machine Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\classes-r\"
# PROP Intermediate_Dir ""
# PROP Target_Dir ""
# ADD BASE JAVA /O
# ADD JAVA /O

!ELSEIF  "$(CFG)" == "JACE - Java Virtual Machine Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\classes\"
# PROP Intermediate_Dir ""
# PROP Target_Dir ""
# ADD BASE JAVA /g
# ADD JAVA /g

!ENDIF 

# Begin Target

# Name "JACE - Java Virtual Machine Release"
# Name "JACE - Java Virtual Machine Debug"
# Begin Source File

SOURCE=.\Acceptor.java
# End Source File
# Begin Source File

SOURCE=.\AcceptStrategy.java
# End Source File
# Begin Source File

SOURCE=.\ACE.java
# End Source File
# Begin Source File

SOURCE=.\ActivateStrategy.java
# End Source File
# Begin Source File

SOURCE=.\Blob.java
# End Source File
# Begin Source File

SOURCE=.\BlobHandler.java
# End Source File
# Begin Source File

SOURCE=.\BlobReader.java
# End Source File
# Begin Source File

SOURCE=.\BlobWriter.java
# End Source File
# Begin Source File

SOURCE=.\Condition.java
# End Source File
# Begin Source File

SOURCE=.\Connector.java
# End Source File
# Begin Source File

SOURCE=.\CreationStrategy.java
# End Source File
# Begin Source File

SOURCE=.\EventHandler.java
# End Source File
# Begin Source File

SOURCE=.\GetOpt.java
# End Source File
# Begin Source File

SOURCE=.\INETAddr.java
# End Source File
# Begin Source File

SOURCE=.\IOCntlCmds.java
# End Source File
# Begin Source File

SOURCE=.\IOCntlMsg.java
# End Source File
# Begin Source File

SOURCE=.\MessageBlock.java
# End Source File
# Begin Source File

SOURCE=.\MessageQueue.java
# End Source File
# Begin Source File

SOURCE=.\MessageType.java
# End Source File
# Begin Source File

SOURCE=.\Module.java
# End Source File
# Begin Source File

SOURCE=.\Mutex.java
# End Source File
# Begin Source File

SOURCE=.\OS.java
# End Source File
# Begin Source File

SOURCE=.\ProfileTimer.java
# End Source File
# Begin Source File

SOURCE=.\RWMutex.java
# End Source File
# Begin Source File

SOURCE=.\Semaphore.java
# End Source File
# Begin Source File

SOURCE=.\ServiceConfig.java
# End Source File
# Begin Source File

SOURCE=.\ServiceObject.java
# End Source File
# Begin Source File

SOURCE=.\ServiceRepository.java
# End Source File
# Begin Source File

SOURCE=.\SOCKAcceptor.java
# End Source File
# Begin Source File

SOURCE=.\SOCKConnector.java
# End Source File
# Begin Source File

SOURCE=.\SOCKStream.java
# End Source File
# Begin Source File

SOURCE=.\StrategyAcceptor.java
# End Source File
# Begin Source File

SOURCE=.\Stream.java
# End Source File
# Begin Source File

SOURCE=.\StreamHead.java
# End Source File
# Begin Source File

SOURCE=.\StreamTail.java
# End Source File
# Begin Source File

SOURCE=.\SvcHandler.java
# End Source File
# Begin Source File

SOURCE=.\Task.java
# End Source File
# Begin Source File

SOURCE=.\TaskFlags.java
# End Source File
# Begin Source File

SOURCE=.\ThreadManager.java
# End Source File
# Begin Source File

SOURCE=.\ThruTask.java
# End Source File
# Begin Source File

SOURCE=.\TimedWait.java
# End Source File
# Begin Source File

SOURCE=.\TimeoutException.java
# End Source File
# Begin Source File

SOURCE=.\TimerQueue.java
# End Source File
# Begin Source File

SOURCE=.\TimeValue.java
# End Source File
# Begin Source File

SOURCE=.\Token.java
# End Source File
# End Target
# End Project
