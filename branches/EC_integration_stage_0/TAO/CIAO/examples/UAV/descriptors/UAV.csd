<?xml version="1.0"?> <!-- -*- SGML -*-  -->

<!-- If this file is moved to a different directory, make sure to change the
     path to the DTD below. Otherwise the examples won't work. -->

<!DOCTYPE softpkg SYSTEM "http://www.dre.vanderbilt.edu/CIAO/DTD/softpkg.dtd">

<!-- UAV's Software Package Descriptor  -->
<!-- This file describes various UAV executor -->
<!-- implementations. -->

<softpkg name="BBN-UAV-TEST" version="1.0">
  <pkgtype>CORBA Component</pkgtype>
  <title>UAV</title>
  <author>
    <company>ISIS at Vanderbilt University</company>
    <webpage href="http://www.dre.vanderbilt.edu/"/>
  </author>
  <description>A UAV component exec implementation.</description>
  <license href="http://www.cs.wustl.edu/~schmidt/ACE-copying.html"/>
  <idl id="IDL:BBN_UAV/UAV:1.0" homeid="IDL:BBN_UAV/UAVHome:1.0">
    <fileinarchive name="UAV.idl"/>
  </idl>
  <descriptor type="CORBA Component">
    <fileinarchive name="UAV.ccd"/>
  </descriptor>
  <implementation id="DCE:a7eff5fa-61b2-4432-a705-e0afb8c6425b">

    <!-- CIAO extension -->
  <dependency type="CIAODLL">
    <softpkgref>
    <!-- .ssd stands for Servant Software Descriptors which is a CIAO extension -->
    <fileinarchive name="UAV.ssd"/>
    <implref idref="DCE:d418affb-3723-4115-bfb8-d3c73631be63"/>
    </softpkgref>
  </dependency>

  <code type="DLL">
  <!-- No need to specify extension below since ACE takes care of that, -->
  <fileinarchive name="UAV_exec"/>
  <entrypoint>createUAVHome_Impl</entrypoint>
  </code>

  </implementation>

</softpkg>
