<?xml version="1.0"?> <!-- -*- SGML -*-  -->

<!-- If this file is moved to a different directory, make sure to change the
     path to the DTD below. Otherwise the examples won't work. -->

<!DOCTYPE softpkg SYSTEM "http://www.dre.vanderbilt.edu/CIAO/DTD/softpkg.dtd">

<!-- Battle's Software Package Descriptor  -->
<!-- This file describes various Battle executor -->
<!-- implementations. -->

<softpkg name="BBN-UAV-TEST" version="1.0">
  <pkgtype>CORBA Component</pkgtype>
  <title>Battle</title>
  <author>
    <company>ISIS at Vanderbilt University</company>
    <webpage href="http://www.dre.vanderbilt.edu/"/>
  </author>
  <description>A Battle component exec implementation.</description>
  <license href="http://www.cs.wustl.edu/~schmidt/ACE-copying.html"/>
  <idl id="IDL:BBN_UAV/Battle:1.0" homeid="IDL:BBN_UAV/BattleHome:1.0">
    <fileinarchive name="Battle.idl"/>
  </idl>
  <descriptor type="CORBA Component">
    <fileinarchive name="Battle.ccd"/>
  </descriptor>
  <implementation id="DCE:915cc6c8-8651-4d01-a7a5-e8f0fb30deb8">

    <!-- CIAO extension -->
  <dependency type="CIAODLL">
    <softpkgref>
    <!-- .ssd stands for Servant Software Descriptors which is a CIAO extension -->
    <fileinarchive name="Battle.ssd"/>
    <implref idref="DCE:9dcbd41b-02f7-4150-a958-423a6ce740c1"/>
    </softpkgref>
  </dependency>

  <code type="DLL">
  <!-- No need to specify extension below since ACE takes care of that, -->
  <fileinarchive name="Battle_exec"/>
  <entrypoint>createBattleHome_Impl</entrypoint>
  </code>

  </implementation>

</softpkg>
