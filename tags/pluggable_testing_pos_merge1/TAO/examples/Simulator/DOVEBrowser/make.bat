@REM This File generates the necessary classes for Visibroker and JDK1.2
@REM call it like: 
@REM
@REM make VB     (makes all VB classes)
@REM make JDK    (makes all JDK classes)
@REM make clean  (removes classes, IDL generated files)
@REM make realclean (updates IDL files, does make clean)
@REM

@if %1. == VB. goto VB
@if %1. == vB. goto VB
@if %1. == Vb. goto VB
@if %1. == vb. goto VB
@if %1. == JDK. goto JDK
@if %1. == jDK. goto JDK
@if %1. == JdK. goto JDK
@if %1. == JDk. goto JDK
@if %1. == jdK. goto JDK
@if %1. == jDk. goto JDK
@if %1. == Jdk. goto JDK
@if %1. == jdk. goto JDK
@if %1. == setup. goto setup
@if %1. == realclean. goto realclean
@if %1. == clean. goto clean
@if %1. == vbjava. goto VBJAVA


@echo Usage:
@echo make setup     (sets up IDL files)
@echo make vb        (makes all VB classes)
@echo make jdk       (makes all JDK classes)
@echo make clean     (removes old classes, IDL generated files)
@echo make realclean (updates IDL files, calls make clean)
@goto end

REM This part build with JDK
:JDK
@echo JDK1.2
idltojava NavWeap.idl
idltojava RtecEventComm.idl
idltojava RtecEventChannelAdmin.idl
idltojava RtecScheduler.idl
idltojava CosNaming.idl
idltojava CosTimeBase.idl
javac CosNaming\NamingContextPackage\*.java
javac CosNaming\*.java
javac CosTimeBase\*.java
javac RtecScheduler\*.java
javac RtecEventComm\*.java
javac RtecEventComm\EventChannelPackage\*.java
javac RtecEventChannelAdmin\*.java
javac *.java

@goto end

:setup

@REM This part updates IDL files

@del *.idl
@xcopy ..\NavWeap.idl
@xcopy ..\..\..\orbsvcs\orbsvcs\CosTimeBase.idl
@xcopy ..\..\..\orbsvcs\orbsvcs\CosNaming.idl
@xcopy ..\..\..\orbsvcs\orbsvcs\RtecEventComm.idl
@xcopy ..\..\..\orbsvcs\orbsvcs\RtecEventChannelAdmin.idl
@xcopy ..\..\..\orbsvcs\orbsvcs\RtecScheduler.idl

@goto end

@REM This part cleans up everything, and updates IDL files

:realclean

@del *.idl
@xcopy ..\NavWeap.idl
@xcopy ..\..\..\orbsvcs\orbsvcs\CosTimeBase.idl
@xcopy ..\..\..\orbsvcs\orbsvcs\CosNaming.idl
@xcopy ..\..\..\orbsvcs\orbsvcs\RtecEventComm.idl
@xcopy ..\..\..\orbsvcs\orbsvcs\RtecEventChannelAdmin.idl
@xcopy ..\..\..\orbsvcs\orbsvcs\RtecScheduler.idl

:clean

@del *.class
@del Weapons.java
@del WeaponsHelper.java
@del WeaponsHolder.java
@del *NavWeapTerminator*.java
@del Navigation.java
@del NavigationHelper.java
@del NavigationHolder.java
@del RtecEventComm\*.java
@del RtecEventComm\*.class
@REM del RtecEventComm\EventChannelPackage\*.java
@REM del RtecEventComm\EventChannelPackage\*.class
@REM rd RtecEventComm\EventChannelPackage
@rd RtecEventComm

@del RtecEventChannelAdmin\*.java
@del RtecEventChannelAdmin\*.class
@del RtecEventChannelAdmin\EventChannelPackage\*.java
@del RtecEventChannelAdmin\EventChannelPackage\*.class
@rd RtecEventChannelAdmin\EventChannelPackage
@rd RtecEventChannelAdmin

@del RtecScheduler\*.java
@del RtecScheduler\*.class
@rd RtecScheduler

@del CosNaming\*.java
@del CosNaming\*.class
@del CosNaming\NamingContextPackage\*.java
@del CosNaming\NamingContextPackage\*.class
@rd CosNaming\NamingContextPackage
@rd CosNaming

@REM del CosTimeBase\*.java
@REM del CosTimeBase\*.class
@REM rd CosTimeBase

@del TimeBase\*.java
@del TimeBase\*.class
@rd TimeBase

@goto end


REM This part build with VB
:VB
@echo Visibroker
@echo idl2java NavWeap.idl
@idl2java NavWeap.idl
@echo idl2java RtecEventComm.idl
@idl2java RtecEventComm.idl
@echo idl2java RtecEventChannelAdmin.idl
@idl2java RtecEventChannelAdmin.idl
@echo idl2java RtecScheduler.idl
@idl2java RtecScheduler.idl
@echo idl2java CosNaming.idl
@idl2java CosNaming.idl
@echo idl2java CosTimeBase.idl
@idl2java CosTimeBase.idl

:VBJAVA

@echo vbjc *.java
@vbjc *.java

:end

