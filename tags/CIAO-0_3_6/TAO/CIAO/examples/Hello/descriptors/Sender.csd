<?xml version="1.0"?> <!-- -*- SGML -*-  -->

<!-- If this file is moved to a different directory, make sure to change the
     path to the DTD below. Otherwise the examples won't work. -->
<!DOCTYPE softpkg SYSTEM "../../../docs/XML/softpkg.dtd">

<softpkg name="Sender" version="1.0">
  <pkgtype>CORBA Component</pkgtype>
  <title>Sender</title>
  <author>
    <name>Tao Lu (lu@dre.vanderbilt.edu)</name>
    <company>ISIS Vanderbilt University</company>
    <webpage href="http://www.dre.vanderbilt.edu/"/>
  </author>
  <description>A component that generates (image) data for distribution.</description>
  <license href="http://www.dre.vanderbilt.edu"/>
  <idl id="IDL:Hello/Sender:1.0"
       homeid="IDL:Hello/SenderHome:1.0">
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
      <fileinarchive name="Sender_exec"/>
      <entrypoint>createSenderHome_Impl</entrypoint>
    </code>

  </implementation>
</softpkg>
