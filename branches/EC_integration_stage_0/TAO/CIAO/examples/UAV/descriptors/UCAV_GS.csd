<?xml version="1.0"?> <!-- -*- SGML -*-  -->

<!-- If this file is moved to a different directory, make sure to change the
     path to the DTD below. Otherwise the examples won't work. -->

<!DOCTYPE softpkg SYSTEM "http://www.dre.vanderbilt.edu/CIAO/DTD/softpkg.dtd">

<!-- UCAV_GS's Software Package Descriptor  -->
<!-- This file describes various UCAV_GS executor -->
<!-- implementations. -->

<softpkg name="BBN-UAV-TEST" version="1.0">
  <pkgtype>CORBA Component</pkgtype>
  <title>UCAV_GS</title>
  <author>
    <company>ISIS at Vanderbilt University</company>
    <webpage href="http://www.dre.vanderbilt.edu/"/>
  </author>
  <description>A UCAV_GS component exec implementation.</description>
  <license href="http://www.cs.wustl.edu/~schmidt/ACE-copying.html"/>
  <idl id="IDL:BBN_UAV/UCAV_GS:1.0" homeid="IDL:BBN_UAV/UCAV_GSHome:1.0">
    <fileinarchive name="UCAV_GS.idl"/>
  </idl>
  <descriptor type="CORBA Component">
    <fileinarchive name="UCAV_GS.ccd"/>
  </descriptor>
  <implementation id="DCE:0988a2a0-1162-47f5-b7b0-f3e222488777">

    <!-- CIAO extension -->
  <dependency type="CIAODLL">
    <softpkgref>
    <!-- .ssd stands for Servant Software Descriptors which is a CIAO extension -->
    <fileinarchive name="UCAV_GS.ssd"/>
    <implref idref="DCE:14d41c19-0d79-4394-9a67-64947205d898"/>
    </softpkgref>
  </dependency>

  <code type="DLL">
  <!-- No need to specify extension below since ACE takes care of that, -->
  <fileinarchive name="UCAV_GS_exec"/>
  <entrypoint>createUCAV_GSHome_Impl</entrypoint>
  </code>

  </implementation>

</softpkg>
