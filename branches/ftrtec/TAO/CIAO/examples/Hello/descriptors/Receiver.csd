<?xml version="1.0"?> <!-- -*- SGML -*-  -->

<!-- If this file is moved to a different directory, make sure to change the
     path to the DTD below. Otherwise the examples won't work. -->
<!DOCTYPE softpkg SYSTEM "../../../docs/XML/softpkg.dtd">

<softpkg name="CIAO-Receiver" version="1.0">
  <pkgtype>CORBA Component</pkgtype>
  <title>Receiver</title>
  <author>
    <name>Tao Lu (lu@dre.vanderbilt.edu)</name>
    <company>ISIS, Vanderbilt University</company>
    <webpage href="http://www.dre.vanderbilt.edu/"/>
  </author>
  <description>A Receiver executor implementation.</description>
  <license href="http://www.dre.vanderbilt.edu/"/>
  <idl id="IDL:Hello/Receiver:1.0"
    homeid="IDL:Hello/ReceiverHome:1.0">
  <fileinarchive name="Receiver.idl"/>
  </idl>

  <descriptor type="CORBA Component">
    <fileinarchive name="Receiver.ccd"/>
  </descriptor>

  <!-- id="DCE:8E92655E-CA07-46C8-B127-0F0872A8CC29" -->
  <implementation id="DCE:34b61767-2b7d-459d-985d-ece6255275d5">

    <!-- CIAO extension -->
    <dependency type="CIAODLL">
      <softpkgref>
        <fileinarchive name="Receiver.ssd"/>
        <implref idref="DCE:6b6d29ca-c6e7-4823-806d-157113767331"/>
      </softpkgref>
    </dependency>

    <code type="DLL">
      <fileinarchive name="Receiver_exec"/>
      <entrypoint>createReceiverHome_Impl</entrypoint>
    </code>

  </implementation>

</softpkg>
