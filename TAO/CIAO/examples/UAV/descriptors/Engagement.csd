<?xml version="1.0"?> <!-- -*- SGML -*-  -->

<!-- If this file is moved to a different directory, make sure to change the
     path to the DTD below. Otherwise the examples won't work. -->

<!DOCTYPE softpkg SYSTEM "http://www.dre.vanderbilt.edu/CIAO/DTD/softpkg.dtd">

<!-- Engagement's Software Package Descriptor  -->
<!-- This file describes various Engagement executor -->
<!-- implementations. -->

<softpkg name="BBN-UAV-TEST" version="1.0">
  <pkgtype>CORBA Component</pkgtype>
  <title>Engagement</title>
  <author>
    <company>ISIS at Vanderbilt University</company>
    <webpage href="http://www.dre.vanderbilt.edu/"/>
  </author>
  <description>A Engagement component exec implementation.</description>
  <license href="http://www.cs.wustl.edu/~schmidt/ACE-copying.html"/>
  <idl id="IDL:BBN_UAV/Engagement:1.0" homeid="IDL:BBN_UAV/EngagementHome:1.0">
    <fileinarchive name="Engagement.idl"/>
  </idl>
  <descriptor type="CORBA Component">
    <fileinarchive name="Engagement.ccd"/>
  </descriptor>
  <implementation id="DCE:814c6d66-26c7-48a5-8f37-7f18f0c2c02c">

    <!-- CIAO extension -->
  <dependency type="CIAODLL">
    <softpkgref>
    <!-- .ssd stands for Servant Software Descriptors which is a CIAO extension -->
    <fileinarchive name="Engagement.ssd"/>
    <implref idref="DCE:3ae83f07-9e59-4cb3-a08b-b0ea26eb167e"/>
    </softpkgref>
  </dependency>

  <code type="DLL">
  <!-- No need to specify extension below since ACE takes care of that, -->
  <fileinarchive name="Engagement_exec"/>
  <entrypoint>createEngagementHome_Impl</entrypoint>
  </code>

  </implementation>

</softpkg>
