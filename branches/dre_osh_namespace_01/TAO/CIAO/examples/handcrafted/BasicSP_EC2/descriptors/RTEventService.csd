<?xml version="1.0"?> <!-- -*- SGML -*-  -->

<!-- If this file is moved to a different directory, make sure to change the
     path to the DTD below. Otherwise the examples won't work. -->
<!DOCTYPE softpkg SYSTEM "../../../../docs/XML/softpkg.dtd">

<!-- RTEventService's Software Package Descriptor  -->
<!-- This file describes various RTEventService executor -->
<!-- implementations. -->


<softpkg name="CIAO-RTEventService" version="1.0">
  <pkgtype>CORBA Component</pkgtype>
  <title>RTEventService</title>
  <author>
    <company>ISIS at Vanderbilt University</company>
    <webpage href="http://www.dre.vanderbilt.edu/"/>
  </author>
  <description>An Event Service executor implementation.</description>
  <license href="http://www.cs.wustl.edu/~schmidt/ACE-copying.html"/>
  <idl id="IDL:BasicSP/RTEventService:1.0" homeid="IDL:BasicSP/RTEventServiceHome:1.0">
    <fileinarchive name="RTEventService.idl"/>
  </idl>
  <descriptor type="CORBA Component">
    <fileinarchive name="RTEventService.ccd"/>
  </descriptor>

  <implementation id="DCE:CC400D95-BF6B-4BBD-8D36-BDDCBB42A3EF">

    <!-- CIAO extension -->
  <dependency type="CIAODLL">
    <softpkgref>
    <!-- .ssd stands for Servant Software Descriptors which is a CIAO extension -->
    <fileinarchive name="RTEventService.ssd"/>
    <implref idref="DCE:515351D2-45F7-4BA3-8128-48283B8EEE5F"/>
    </softpkgref>
  </dependency>

  <code type="DLL">
  <!-- No need to specify extension below since ACE takes care of that, -->
  <fileinarchive name="RTEventService_exec"/>
  <entrypoint>createRTEventServiceHome_Impl</entrypoint>
  </code>

  </implementation>

</softpkg>
