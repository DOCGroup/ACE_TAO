REM This File generates the necessary classes for Visibroker and JDK1.2
REM call it like: make VB    or    make JDK


if %1 == VB goto VB


:JDK
echo JDK1.2
del RtecEventComm
del RtecEventChannelAdmin
del RtecScheduler
del CosNaming
del TimeBase
idltojava RtecEventComm.idl
idltojava RtecEventChannelAdmin.idl
idltojava RtecScheduler.idl
idltojava CosNaming.idl
idltojava CosTimeBase.idl
javac CosNaming\NamingContextPackage\*.java
javac CosNaming\*.java
javac TimeBase\*.java
javac RtecScheduler\*.java
javac RtecEventComm\*.java
javac RtecEventComm\EventChannelPackage\*.java
javac RtecEventChannelAdmin\*.java
javac *.java

goto end

:VB
echo Visibroker
del RtecEventComm
del RtecEventChannelAdmin
del RtecScheduler
del CosNaming
del TimeBase
idl2java RtecEventComm.idl
idl2java RtecEventChannelAdmin.idl
idl2java RtecScheduler.idl
idl2java CosNaming.idl
idl2java CosTimeBase.idl
vbjc *.java

:end
