<?xml version="1.0"?> <!-- -*- SGML -*-  -->

<!-- If this file is moved to a different directory, make sure to change the
     path to the DTD below. Otherwise the examples won't work. -->

<!DOCTYPE softpkg SYSTEM "http://www.dre.vanderbilt.edu/CIAO/DTD/softpkg.dtd">

<!-- UCAV's Software Package Descriptor  -->
<!-- This file describes various UCAV executor -->
<!-- implementations. -->

<softpkg name="BBN-UAV-TEST" version="1.0">
  <pkgtype>CORBA Component</pkgtype>
  <title>UCAV</title>
  <author>
    <company>ISIS at Vanderbilt University</company>
    <webpage href="http://www.dre.vanderbilt.edu/"/>
  </author>
  <description>A UCAV component exec implementation.</description>
  <license href="http://www.cs.wustl.edu/~schmidt/ACE-copying.html"/>
  <idl id="IDL:BBN_UAV/UCAV:1.0" homeid="IDL:BBN_UAV/UCAVHome:1.0">
    <fileinarchive name="UCAV.idl"/>
  </idl>
  <descriptor type="CORBA Component">
    <fileinarchive name="UCAV.ccd"/>
  </descriptor>
  <implementation id="DCE:7dbce0f0-8fde-4928-9af0-555908f9e0e9">

    <!-- CIAO extension -->
  <dependency type="CIAODLL">
    <softpkgref>
    <!-- .ssd stands for Servant Software Descriptors which is a CIAO extension -->
    <fileinarchive name="UCAV.ssd"/>
    <implref idref="DCE:9c49fa76-a420-4014-ac0f-942e3e1326c8"/>
    </softpkgref>
  </dependency>

  <code type="DLL">
  <!-- No need to specify extension below since ACE takes care of that, -->
  <fileinarchive name="UCAV_exec"/>
  <entrypoint>createUCAVHome_Impl</entrypoint>
  </code>

  </implementation>

</softpkg>
