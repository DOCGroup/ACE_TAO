<?xml version="1.0"?> <!-- -*- SGML -*-  -->

<!-- $Id$ -->
<!-- If this file is moved to a different directory, make sure to change the
     path to the DTD below. Otherwise the examples won't work. -->
<!DOCTYPE softpkg SYSTEM "../../../docs/XML/softpkg.dtd">

<softpkg name="Sender" version="1.0">

  <pkgtype>CORBA Component</pkgtype>

  <title>Sender</title>

  <idl id="IDL:Protocols/Sender:1.0"
       homeid="IDL:Protocols/SenderHome:1.0">
   <fileinarchive name="Sender.idl"/>
  </idl>

  <descriptor type="CORBA Component">
    <fileinarchive name="Sender.ccd"/>
  </descriptor>

  <implementation id="DCE:83d9348a-3248-445c-82c6-e38294943d65">

   <!-- CIAO extension -->
   <dependency type="CIAODLL">
     <softpkgref>
       <fileinarchive name="Sender.ssd"/>
       <implref idref="DCE:2c16cd81-a364-4422-b9e0-ee2ebe084954"/>
     </softpkgref>
   </dependency>

   <code type="DLL">
     <fileinarchive name="Protocols_Sender_exec"/>
     <entrypoint>createSenderHome_Impl</entrypoint>
   </code>

  </implementation>

</softpkg>
