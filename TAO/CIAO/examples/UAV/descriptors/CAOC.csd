<?xml version="1.0"?> <!-- -*- SGML -*-  -->

<!-- If this file is moved to a different directory, make sure to change the
     path to the DTD below. Otherwise the examples won't work. -->

<!DOCTYPE softpkg SYSTEM "http://www.dre.vanderbilt.edu/CIAO/DTD/softpkg.dtd">

<!-- CAOC's Software Package Descriptor  -->
<!-- This file describes various CAOC executor -->
<!-- implementations. -->

<softpkg name="BBN-UAV-TEST" version="1.0">
  <pkgtype>CORBA Component</pkgtype>
  <title>CAOC</title>
  <author>
    <company>ISIS at Vanderbilt University</company>
    <webpage href="http://www.dre.vanderbilt.edu/"/>
  </author>
  <description>A CAOC component exec implementation.</description>
  <license href="http://www.cs.wustl.edu/~schmidt/ACE-copying.html"/>
  <idl id="IDL:BBN_UAV/CAOC:1.0" homeid="IDL:BBN_UAV/CAOCHome:1.0">
    <fileinarchive name="CAOC.idl"/>
  </idl>
  <descriptor type="CORBA Component">
    <fileinarchive name="CAOC.ccd"/>
  </descriptor>
  <implementation id="DCE:d516ad39-043f-4fd5-a780-c1f2bc864bdb">

    <!-- CIAO extension -->
  <dependency type="CIAODLL">
    <softpkgref>
    <!-- .ssd stands for Servant Software Descriptors which is a CIAO extension -->
    <fileinarchive name="CAOC.ssd"/>
    <implref idref="DCE:68820cab-e051-4692-a986-b141f78ded76"/>
    </softpkgref>
  </dependency>

  <code type="DLL">
  <!-- No need to specify extension below since ACE takes care of that, -->
  <fileinarchive name="CAOC_exec"/>
  <entrypoint>createCAOCHome_Impl</entrypoint>
  </code>

  </implementation>

</softpkg>
