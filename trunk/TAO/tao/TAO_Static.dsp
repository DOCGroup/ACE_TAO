# Microsoft Developer Studio Project File - Name="TAO Static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=TAO Static - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TAO_Static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TAO_Static.mak" CFG="TAO Static - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TAO Static - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "TAO Static - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath "Desktop"
# PROP WCE_FormatVersion ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TAO Static - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "TAO_Stat"
# PROP BASE Intermediate_Dir "TAO_Stat"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\\" /I "..\\" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D ACE_HAS_DLL=0 /D TAO_HAS_DLL=0 /D "ACE_NO_INLINE" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"TAOs.lib"

!ELSEIF  "$(CFG)" == "TAO Static - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "TAO_Sta0"
# PROP BASE Intermediate_Dir "TAO_Sta0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /GX /Z7 /Od /I "..\..\\" /I "..\\" /D TAO_HAS_DLL=0 /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D ACE_HAS_DLL=0 /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"TAOsd.lib"

!ENDIF 

# Begin Target

# Name "TAO Static - Win32 Release"
# Name "TAO Static - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\Active_Object_Map.cpp
# End Source File
# Begin Source File

SOURCE=.\Any.cpp
# End Source File
# Begin Source File

SOURCE=.\append.cpp
# End Source File
# Begin Source File

SOURCE=.\CDR.cpp
# End Source File
# Begin Source File

SOURCE=.\CDR_Interpreter.cpp
# End Source File
# Begin Source File

SOURCE=.\Client_Strategy_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\Connect.cpp
# End Source File
# Begin Source File

SOURCE=.\Context.cpp
# End Source File
# Begin Source File

SOURCE=.\CurrentC.cpp
# End Source File
# Begin Source File

SOURCE=.\CurrentS.cpp
# End Source File
# Begin Source File

SOURCE=.\debug.cpp
# End Source File
# Begin Source File

SOURCE=.\decode.cpp
# End Source File
# Begin Source File

SOURCE=.\deep_free.cpp
# End Source File
# Begin Source File

SOURCE=.\default_client.cpp
# End Source File
# Begin Source File

SOURCE=.\default_resource.cpp
# End Source File
# Begin Source File

SOURCE=.\default_server.cpp
# End Source File
# Begin Source File

SOURCE=.\DomainC.cpp
# End Source File
# Begin Source File

SOURCE=.\DomainS.cpp
# End Source File
# Begin Source File

SOURCE=.\DynAny_i.cpp
# End Source File
# Begin Source File

SOURCE=.\DynAnyC.cpp
# End Source File
# Begin Source File

SOURCE=.\DynAnyS.cpp
# End Source File
# Begin Source File

SOURCE=.\DynArray_i.cpp
# End Source File
# Begin Source File

SOURCE=.\DynEnum_i.cpp
# End Source File
# Begin Source File

SOURCE=.\DynSequence_i.cpp
# End Source File
# Begin Source File

SOURCE=.\DynStruct_i.cpp
# End Source File
# Begin Source File

SOURCE=.\DynUnion_i.cpp
# End Source File
# Begin Source File

SOURCE=.\encode.cpp
# End Source File
# Begin Source File

SOURCE=.\Environment.cpp
# End Source File
# Begin Source File

SOURCE=.\Exception.cpp
# End Source File
# Begin Source File

SOURCE=.\Forwarding_Servant.cpp
# End Source File
# Begin Source File

SOURCE=.\GIOP.cpp
# End Source File
# Begin Source File

SOURCE=.\GIOP_Server_Request.cpp
# End Source File
# Begin Source File

SOURCE=.\IIOP_Acceptor.cpp
# End Source File
# Begin Source File

SOURCE=.\IIOP_Connector.cpp
# End Source File
# Begin Source File

SOURCE=.\IIOP_Profile.cpp
# End Source File
# Begin Source File

SOURCE=.\IIOP_Transport.cpp
# End Source File
# Begin Source File

SOURCE=.\InconsistentTypeCodeC.cpp
# End Source File
# Begin Source File

SOURCE=.\Invocation.cpp
# End Source File
# Begin Source File

SOURCE=.\IOR_LookupTable.cpp
# End Source File
# Begin Source File

SOURCE=.\Key_Adapters.cpp
# End Source File
# Begin Source File

SOURCE=.\Managed_Types.cpp
# End Source File
# Begin Source File

SOURCE=.\Marshal.cpp
# End Source File
# Begin Source File

SOURCE=.\MessagingC.cpp
# End Source File
# Begin Source File

SOURCE=.\MessagingS.cpp
# End Source File
# Begin Source File

SOURCE=.\MProfile.cpp
# End Source File
# Begin Source File

SOURCE=.\NVList.cpp
# End Source File
# Begin Source File

SOURCE=.\Object.cpp
# End Source File
# Begin Source File

SOURCE=.\Object_Adapter.cpp
# End Source File
# Begin Source File

SOURCE=.\Object_KeyC.cpp
# End Source File
# Begin Source File

SOURCE=.\Operation_Table.cpp
# End Source File
# Begin Source File

SOURCE=.\ORB.cpp
# End Source File
# Begin Source File

SOURCE=.\ORB_Core.cpp
# End Source File
# Begin Source File

SOURCE=.\params.cpp
# End Source File
# Begin Source File

SOURCE=.\Pluggable.cpp
# End Source File
# Begin Source File

SOURCE=.\POA.cpp
# End Source File
# Begin Source File

SOURCE=.\POAC.cpp
# End Source File
# Begin Source File

SOURCE=.\POAManager.cpp
# End Source File
# Begin Source File

SOURCE=.\POAS.cpp
# End Source File
# Begin Source File

SOURCE=.\Policy_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\PolicyC.cpp
# End Source File
# Begin Source File

SOURCE=.\PolicyS.cpp
# End Source File
# Begin Source File

SOURCE=.\Principal.cpp
# End Source File
# Begin Source File

SOURCE=.\Request.cpp
# End Source File
# Begin Source File

SOURCE=.\Resource_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\Sequence.cpp
# End Source File
# Begin Source File

SOURCE=.\Servant_Base.cpp
# End Source File
# Begin Source File

SOURCE=.\Server_Request.cpp
# End Source File
# Begin Source File

SOURCE=.\Server_Strategy_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\skip.cpp
# End Source File
# Begin Source File

SOURCE=.\Stub.cpp
# End Source File
# Begin Source File

SOURCE=.\TAO.cpp
# End Source File
# Begin Source File

SOURCE=.\TAO_Internal.cpp
# End Source File
# Begin Source File

SOURCE=.\TimeBaseC.cpp
# End Source File
# Begin Source File

SOURCE=.\TimeBaseS.cpp
# End Source File
# Begin Source File

SOURCE=.\Typecode.cpp
# End Source File
# Begin Source File

SOURCE=.\Typecode_Constants.cpp
# End Source File
# Begin Source File

SOURCE=.\Union.cpp
# End Source File
# Begin Source File

SOURCE=.\ValueBase.cpp
# End Source File
# Begin Source File

SOURCE=.\ValueFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\ValueFactory_Map.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\Active_Object_Map.h
# End Source File
# Begin Source File

SOURCE=.\align.h
# End Source File
# Begin Source File

SOURCE=.\any.h
# End Source File
# Begin Source File

SOURCE=.\arg_shifter.h
# End Source File
# Begin Source File

SOURCE=.\cdr.h
# End Source File
# Begin Source File

SOURCE=.\CDR_Interpreter.h
# End Source File
# Begin Source File

SOURCE=.\Client_Strategy_Factory.h
# End Source File
# Begin Source File

SOURCE=.\connect.h
# End Source File
# Begin Source File

SOURCE=.\corba.h
# End Source File
# Begin Source File

SOURCE=.\corbafwd.h
# End Source File
# Begin Source File

SOURCE=.\debug.h
# End Source File
# Begin Source File

SOURCE=.\default_client.h
# End Source File
# Begin Source File

SOURCE=.\default_resource.h
# End Source File
# Begin Source File

SOURCE=.\default_server.h
# End Source File
# Begin Source File

SOURCE=.\DomainC.h
# End Source File
# Begin Source File

SOURCE=.\DynAny_i.h
# End Source File
# Begin Source File

SOURCE=.\DynAnyC.h
# End Source File
# Begin Source File

SOURCE=.\DynArray_i.h
# End Source File
# Begin Source File

SOURCE=.\DynEnum_i.h
# End Source File
# Begin Source File

SOURCE=.\DynSequence_i.h
# End Source File
# Begin Source File

SOURCE=.\DynStruct_i.h
# End Source File
# Begin Source File

SOURCE=.\DynUnion_i.h
# End Source File
# Begin Source File

SOURCE=.\Environment.h
# End Source File
# Begin Source File

SOURCE=.\Exception.h
# End Source File
# Begin Source File

SOURCE=.\Forwarding_Servant.h
# End Source File
# Begin Source File

SOURCE=.\giop.h
# End Source File
# Begin Source File

SOURCE=.\GIOP_Server_Request.h
# End Source File
# Begin Source File

SOURCE=.\IIOP_Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\IIOP_Connector.h
# End Source File
# Begin Source File

SOURCE=.\IIOP_Profile.h
# End Source File
# Begin Source File

SOURCE=.\IIOP_Transport.h
# End Source File
# Begin Source File

SOURCE=.\InconsistentTypeCodeC.h
# End Source File
# Begin Source File

SOURCE=.\Invocation.h
# End Source File
# Begin Source File

SOURCE=.\Managed_Types.h
# End Source File
# Begin Source File

SOURCE=.\marshal.h
# End Source File
# Begin Source File

SOURCE=.\MessagingC.h
# End Source File
# Begin Source File

SOURCE=.\MessagingS.h
# End Source File
# Begin Source File

SOURCE=.\MProfile.h
# End Source File
# Begin Source File

SOURCE=.\nvlist.h
# End Source File
# Begin Source File

SOURCE=.\object.h
# End Source File
# Begin Source File

SOURCE=.\Object_Adapter.h
# End Source File
# Begin Source File

SOURCE=.\Object_KeyC.h
# End Source File
# Begin Source File

SOURCE=.\Object_Table.h
# End Source File
# Begin Source File

SOURCE=.\Operation_Table.h
# End Source File
# Begin Source File

SOURCE=.\orb.h
# End Source File
# Begin Source File

SOURCE=.\orb_core.h
# End Source File
# Begin Source File

SOURCE=.\ORB_Strategies_T.h
# End Source File
# Begin Source File

SOURCE=.\orbconf.h
# End Source File
# Begin Source File

SOURCE=.\params.h
# End Source File
# Begin Source File

SOURCE=.\Pluggable.h
# End Source File
# Begin Source File

SOURCE=.\poa.h
# End Source File
# Begin Source File

SOURCE=.\poa_macros.h
# End Source File
# Begin Source File

SOURCE=.\poa_T.h
# End Source File
# Begin Source File

SOURCE=.\poaC.h
# End Source File
# Begin Source File

SOURCE=.\POAManager.h
# End Source File
# Begin Source File

SOURCE=.\poaS.h
# End Source File
# Begin Source File

SOURCE=.\Policy_Manager.h
# End Source File
# Begin Source File

SOURCE=.\PolicyC.h
# End Source File
# Begin Source File

SOURCE=.\Principal.h
# End Source File
# Begin Source File

SOURCE=.\request.h
# End Source File
# Begin Source File

SOURCE=.\Resource_Factory.h
# End Source File
# Begin Source File

SOURCE=.\sequence.h
# End Source File
# Begin Source File

SOURCE=.\Sequence_T.h
# End Source File
# Begin Source File

SOURCE=.\servant_base.h
# End Source File
# Begin Source File

SOURCE=.\Server_Request.h
# End Source File
# Begin Source File

SOURCE=.\Server_Strategy_Factory.h
# End Source File
# Begin Source File

SOURCE=.\singletons.h
# End Source File
# Begin Source File

SOURCE=.\stub.h
# End Source File
# Begin Source File

SOURCE=.\TAO.h
# End Source File
# Begin Source File

SOURCE=.\TAO_Internal.h
# End Source File
# Begin Source File

SOURCE=.\Timeprobe.h
# End Source File
# Begin Source File

SOURCE=.\typecode.h
# End Source File
# Begin Source File

SOURCE=.\ValueBase.h
# End Source File
# Begin Source File

SOURCE=.\ValueFactory.h
# End Source File
# Begin Source File

SOURCE=.\ValueFactory_Map.h
# End Source File
# Begin Source File

SOURCE=.\varout.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "*.i"
# Begin Source File

SOURCE=.\Active_Object_Map.i
# End Source File
# Begin Source File

SOURCE=.\any.i
# End Source File
# Begin Source File

SOURCE=.\cdr.i
# End Source File
# Begin Source File

SOURCE=.\connect.i
# End Source File
# Begin Source File

SOURCE=.\CurrentC.i
# End Source File
# Begin Source File

SOURCE=.\default_client.i
# End Source File
# Begin Source File

SOURCE=.\default_resource.i
# End Source File
# Begin Source File

SOURCE=.\default_server.i
# End Source File
# Begin Source File

SOURCE=.\DomainC.i
# End Source File
# Begin Source File

SOURCE=.\DomainS_T.i
# End Source File
# Begin Source File

SOURCE=.\DynAnyC.i
# End Source File
# Begin Source File

SOURCE=.\Environment.i
# End Source File
# Begin Source File

SOURCE=.\Exception.i
# End Source File
# Begin Source File

SOURCE=.\giop.i
# End Source File
# Begin Source File

SOURCE=.\GIOP_Server_Request.i
# End Source File
# Begin Source File

SOURCE=.\IIOP_ORB.i
# End Source File
# Begin Source File

SOURCE=.\Invocation.i
# End Source File
# Begin Source File

SOURCE=.\Managed_Types.i
# End Source File
# Begin Source File

SOURCE=.\marshal.i
# End Source File
# Begin Source File

SOURCE=.\MessagingC.i
# End Source File
# Begin Source File

SOURCE=.\MessagingS.i
# End Source File
# Begin Source File

SOURCE=.\NVList.i
# End Source File
# Begin Source File

SOURCE=.\object.i
# End Source File
# Begin Source File

SOURCE=.\Object_KeyC.i
# End Source File
# Begin Source File

SOURCE=.\ORB.i
# End Source File
# Begin Source File

SOURCE=.\orb_core.i
# End Source File
# Begin Source File

SOURCE=.\ORB_Strategies_T.i
# End Source File
# Begin Source File

SOURCE=.\params.i
# End Source File
# Begin Source File

SOURCE=.\POA.i
# End Source File
# Begin Source File

SOURCE=.\poaC.i
# End Source File
# Begin Source File

SOURCE=.\POAManager.i
# End Source File
# Begin Source File

SOURCE=.\poaS.i
# End Source File
# Begin Source File

SOURCE=.\Policy_Manager.i
# End Source File
# Begin Source File

SOURCE=.\PolicyC.i
# End Source File
# Begin Source File

SOURCE=.\PolicyS.i
# End Source File
# Begin Source File

SOURCE=.\Principal.i
# End Source File
# Begin Source File

SOURCE=.\Request.i
# End Source File
# Begin Source File

SOURCE=.\sequence.i
# End Source File
# Begin Source File

SOURCE=.\Sequence_T.i
# End Source File
# Begin Source File

SOURCE=.\Servant_Base.i
# End Source File
# Begin Source File

SOURCE=.\Server_Request.i
# End Source File
# Begin Source File

SOURCE=.\stub.i
# End Source File
# Begin Source File

SOURCE=.\TAO_Internal.i
# End Source File
# Begin Source File

SOURCE=.\Timeprobe.i
# End Source File
# Begin Source File

SOURCE=.\typecode.i
# End Source File
# Begin Source File

SOURCE=.\ValueBase.i
# End Source File
# Begin Source File

SOURCE=.\ValueFactory.i
# End Source File
# Begin Source File

SOURCE=.\ValueFactory_Map.i
# End Source File
# Begin Source File

SOURCE=.\varout.i
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
