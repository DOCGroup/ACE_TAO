<?xml version="1.0"?> <!-- -*- SGML -*-  -->

<!-- XML doesn't really support the following $(CIAO_ROOT) syntax :) -->
<!DOCTYPE softpkg SYSTEM "$(CIAO_ROOT)/docs/XML/softpkg.dtd">

<!-- RateGen's Software Package Descriptor  -->
<!-- This file describes various RateGen executor -->
<!-- implementations. -->


<softpkg name="CIAO-EC" version="1.0">
  <pkgtype>CORBA Component</pkgtype>
  <title>Event Generator</title>
  <author>
    <company>ISIS at Vanderbilt University</company>
    <webpage href="http://www.dre.vanderbilt.edu/"/>
  </author>
  <description>A Event executor implementation.</description>
  <license href="http://www.cs.wustl.edu/~schmidt/ACE-copying.html"/>
  <idl id="IDL:BasicSP/EC:1.0" homeid="IDL:BasicSP/ECHome:1.0">
    <fileinarchive name="EC.idl"/>
  </idl>
  <descriptor type="CORBA Component">
    <fileinarchive name="EC.ccd"/>
  </descriptor>
  <implementation id="DCE:3148F760-F2ED-4204-A775-6B972C10E8CB">

    <!-- CIAO extension -->
  <dependency type="CIAODLL">
    <softpkgref>
    <!-- .ssd stands for Servant Software Descriptors which is a CIAO extension -->
    <fileinarchive name="EC.ssd"/>
    <implref idref="DCE:75309233-0E0A-4cfb-B186-3E99F69B1D40"/>
    </softpkgref>
  </dependency>

  <code type="DLL">
  <!-- No need to specify extension below since ACE takes care of that, -->
  <fileinarchive name="EC_exec"/>
  <entrypoint>createECHome_Impl</entrypoint>
  </code>

  </implementation>

</softpkg>