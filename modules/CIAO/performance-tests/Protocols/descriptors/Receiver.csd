<?xml version="1.0"?> <!-- -*- SGML -*-  -->

<!-- $Id$ -->
<!-- If this file is moved to a different directory, make sure to change the
     path to the DTD below. Otherwise the examples won't work. -->
<!DOCTYPE softpkg SYSTEM "../../../docs/XML/softpkg.dtd">

<softpkg name="Receiver" version="1.0">

  <pkgtype>CORBA Component</pkgtype>

  <title>Receiver</title>

  <idl id="IDL:Protocols/Receiver:1.0"
    homeid="IDL:Protocols/ReceiverHome:1.0">
   <fileinarchive name="Receiver.idl"/>
  </idl>

  <descriptor type="CORBA Component">
    <fileinarchive name="Receiver.ccd"/>
  </descriptor>

  <implementation id="DCE:34b61767-2b7d-459d-985d-ece6255275d5">

   <!-- CIAO extension -->
   <dependency type="CIAODLL">
     <softpkgref>
       <fileinarchive name="Receiver.ssd"/>
       <implref idref="DCE:6b6d29ca-c6e7-4823-806d-157113767331"/>
     </softpkgref>
   </dependency>

   <code type="DLL">
     <fileinarchive name="Protocols_Receiver_exec"/>
     <entrypoint>createReceiverHome_Impl</entrypoint>
   </code>

  </implementation>

</softpkg>
