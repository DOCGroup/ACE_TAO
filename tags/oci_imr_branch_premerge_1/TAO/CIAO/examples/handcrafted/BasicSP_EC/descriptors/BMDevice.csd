<?xml version="1.0"?> <!-- -*- SGML -*-  -->

<!-- If this file is moved to a different directory, make sure to change the
     path to the DTD below. Otherwise the examples won't work. -->
<!DOCTYPE softpkg SYSTEM "../../../../docs/XML/softpkg.dtd">

<!-- GPS's Software Package Descriptor  -->
<!-- This file describes various GPS executor -->
<!-- implementations. -->


<softpkg name="CIAO-BMDevice" version="1.0">
  <pkgtype>CORBA Component</pkgtype>
  <title>BMDevice</title>
  <author>
    <company>ISIS at Vanderbilt University</company>
    <webpage href="http://www.cs.wustl.edu/~doc/"/>
  </author>
  <description>A BMDevice executor implementation.</description>
  <license href="http://www.cs.wustl.edu/~schmidt/ACE-copying.html"/>
  <idl id="IDL:BasicSP/BMDevice:1.0" homeid="IDL:BasicSP/BMDeviceHome:1.0">
  <fileinarchive name="BMDevice.idl"/>
  </idl>

  <descriptor type="CORBA Component">
    <fileinarchive name="BMDevice.ccd"/>
  </descriptor>

  <implementation id="DCE:82C2B032-37F0-4315-A59F-7020D3264E4D">

    <!-- CIAO extension -->
    <dependency type="CIAODLL">
      <softpkgref>
        <fileinarchive name="BMDevice.ssd"/>
        <implref idref="DCE:93D254CF-9538-44e8-BB98-AABCD134ADD3"/>
      </softpkgref>
    </dependency>

    <code type="DLL">
      <fileinarchive name="BMDevice_exec"/>
      <entrypoint>createBMDeviceHome_Impl</entrypoint>
    </code>

  </implementation>

</softpkg>
