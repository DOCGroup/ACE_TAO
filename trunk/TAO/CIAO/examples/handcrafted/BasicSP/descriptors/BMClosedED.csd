<?xml version="1.0"?> <!-- -*- SGML -*-  -->

<!-- If this file is moved to a different directory, make sure to change the
     path to the DTD below. Otherwise the examples won't work. -->
<!DOCTYPE softpkg SYSTEM "../../../../docs/XML/softpkg.dtd">

<!-- GPS's Software Package Descriptor  -->
<!-- This file describes various GPS executor -->
<!-- implementations. -->


<softpkg name="CIAO-BMClosedED" version="1.0">
  <pkgtype>CORBA Component</pkgtype>
  <title>BMClosedED</title>
  <author>
    <company>ISIS at Vanderbilt University</company>
    <webpage href="http://www.cs.wustl.edu/~doc/"/>
  </author>
  <description>A BMClosedED executor implementation.</description>
  <license href="http://www.cs.wustl.edu/~schmidt/ACE-copying.html"/>
  <idl id="IDL:BasicSP/BMClosedED:1.0" homeid="IDL:BasicSP/BMClosedEDHome:1.0">
    <fileinarchive name="BMClosedED.idl"/>
  </idl>

  <descriptor type="CORBA Component">
    <fileinarchive name="BMClosedED.ccd"/>
  </descriptor>

  <implementation id="DCE:8E6C468D-A39F-46b4-962B-265F1AA8D538">

    <dependency type="CIAODLL">
      <softpkgref>
        <fileinarchive name="BMClosedED.ssd"/>
        <implref idref="DCE:CDC06FCA-50FC-43ca-8ECC-BEFBD33FEE78"/>
      </softpkgref>
    </dependency>

    <code type="DLL">
      <fileinarchive name="BMClosedED_exec"/>
      <entrypoint>createBMClosedEDHome_Impl</entrypoint>
    </code>

  </implementation>

</softpkg>
