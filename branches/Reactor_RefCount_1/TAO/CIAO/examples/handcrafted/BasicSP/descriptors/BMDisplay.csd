<?xml version="1.0"?> <!-- -*- SGML -*-  -->

<!-- XML doesn't really support the following $(CIAO_ROOT) syntax :) -->
<!DOCTYPE softpkg SYSTEM "$(CIAO_ROOT)/docs/XML/softpkg.dtd">

<!-- GPS's Software Package Descriptor  -->
<!-- This file describes various GPS executor -->
<!-- implementations. -->


<softpkg name="CIAO-BMDisplay" version="1.0">
  <pkgtype>CORBA Component</pkgtype>
  <title>BMDisplay</title>
  <author>
    <company>ISIS at Vanderbilt University</company>
    <webpage href="http://www.cs.wustl.edu/~doc/"/>
  </author>
  <description>A BMDisplay executor implementation.</description>
  <license href="http://www.cs.wustl.edu/~schmidt/ACE-copying.html"/>
  <idl id="IDL:BasicSP/BMDisplay:1.0" homeid="IDL:BasicSP/BMDisplayHome:1.0">
  <fileinarchive name="BMDisplay.idl"/>
  </idl>

  <descriptor type="CORBA Component">
    <fileinarchive name="BMDisplay.ccd"/>
  </descriptor>

  <implementation id="DCE:8E92655E-CA07-46C8-B127-0F0872A8CC29">

    <!-- CIAO extension -->
    <dependency type="CIAODLL">
      <softpkgref>
        <fileinarchive name="BMDisplay.ssd"/>
        <implref idref="DCE:D7984625-8561-431d-9927-4E498B317C02"/>
      </softpkgref>
    </dependency>

    <code type="DLL">
      <fileinarchive name="BMDisplay_exec"/>
      <entrypoint>createBMDisplayHome_Impl</entrypoint>
    </code>

  </implementation>

</softpkg>