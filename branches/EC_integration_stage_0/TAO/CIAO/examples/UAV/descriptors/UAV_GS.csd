<?xml version="1.0"?> <!-- -*- SGML -*-  -->

<!-- If this file is moved to a different directory, make sure to change the
     path to the DTD below. Otherwise the examples won't work. -->

<!DOCTYPE softpkg SYSTEM "http://www.dre.vanderbilt.edu/CIAO/DTD/softpkg.dtd">

<!-- UAV_GS's Software Package Descriptor  -->
<!-- This file describes various UAV_GS executor -->
<!-- implementations. -->

<softpkg name="BBN-UAV-TEST" version="1.0">
  <pkgtype>CORBA Component</pkgtype>
  <title>UAV_GS</title>
  <author>
    <company>ISIS at Vanderbilt University</company>
    <webpage href="http://www.dre.vanderbilt.edu/"/>
  </author>
  <description>A UAV_GS component exec implementation.</description>
  <license href="http://www.cs.wustl.edu/~schmidt/ACE-copying.html"/>
  <idl id="IDL:BBN_UAV/UAV_GS:1.0" homeid="IDL:BBN_UAV/UAV_GSHome:1.0">
    <fileinarchive name="UAV_GS.idl"/>
  </idl>
  <descriptor type="CORBA Component">
    <fileinarchive name="UAV_GS.ccd"/>
  </descriptor>
  <implementation id="DCE:9cc8bebc-22a5-46d1-9cd4-701e97465c80">

    <!-- CIAO extension -->
  <dependency type="CIAODLL">
    <softpkgref>
    <!-- .ssd stands for Servant Software Descriptors which is a CIAO extension -->
    <fileinarchive name="UAV_GS.ssd"/>
    <implref idref="DCE:e3bb3bad-39dc-4808-b736-17f0f0ebad28"/>
    </softpkgref>
  </dependency>

  <code type="DLL">
  <!-- No need to specify extension below since ACE takes care of that, -->
  <fileinarchive name="UAV_GS_exec"/>
  <entrypoint>createUAV_GSHome_Impl</entrypoint>
  </code>

  </implementation>

</softpkg>
