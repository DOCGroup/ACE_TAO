<?xml version="1.0"?> <!-- -*- SGML -*-  -->

<!-- If this file is moved to a different directory, make sure to change the
     path to the DTD below. Otherwise the examples won't work. -->

<!DOCTYPE softpkg SYSTEM "http://www.dre.vanderbilt.edu/CIAO/DTD/softpkg.dtd">

<!-- Satellite's Software Package Descriptor  -->
<!-- This file describes various Satellite executor -->
<!-- implementations. -->

<softpkg name="BBN-UAV-TEST" version="1.0">
  <pkgtype>CORBA Component</pkgtype>
  <title>Satellite</title>
  <author>
    <company>ISIS at Vanderbilt University</company>
    <webpage href="http://www.dre.vanderbilt.edu/"/>
  </author>
  <description>A Satellite component exec implementation.</description>
  <license href="http://www.cs.wustl.edu/~schmidt/ACE-copying.html"/>
  <idl id="IDL:BBN_UAV/Satellite:1.0" homeid="IDL:BBN_UAV/SatelliteHome:1.0">
    <fileinarchive name="Satellite.idl"/>
  </idl>
  <descriptor type="CORBA Component">
    <fileinarchive name="Satellite.ccd"/>
  </descriptor>
  <implementation id="DCE:3e142583-5d79-4e5d-b502-59aed9dc26d6">

    <!-- CIAO extension -->
  <dependency type="CIAODLL">
    <softpkgref>
    <!-- .ssd stands for Servant Software Descriptors which is a CIAO extension -->
    <fileinarchive name="Satellite.ssd"/>
    <implref idref="DCE:bea9902f-1377-4a9e-9a89-c376e2bbdc87"/>
    </softpkgref>
  </dependency>

  <code type="DLL">
  <!-- No need to specify extension below since ACE takes care of that, -->
  <fileinarchive name="Satellite_exec"/>
  <entrypoint>createSatelliteHome_Impl</entrypoint>
  </code>

  </implementation>

</softpkg>
