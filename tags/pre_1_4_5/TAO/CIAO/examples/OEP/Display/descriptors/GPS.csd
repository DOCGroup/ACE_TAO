<?xml version="1.0"?> <!-- -*- SGML -*-  -->

<!-- If this file is moved to a different directory, make sure to change the
     path to the DTD below. Otherwise the examples won't work. -->
<!DOCTYPE softpkg SYSTEM "../../../../docs/XML/softpkg.dtd">

<!-- GPS's Software Package Descriptor  -->
<!-- This file describes various GPS executor -->
<!-- implementations. -->


<softpkg name="CIAO-GPS" version="1.0">
  <pkgtype>CORBA Component</pkgtype>
  <title>Rate Generator</title>
  <author>
    <company>Washington University in St. Louis</company>
    <webpage href="http://www.cs.wustl.edu/~doc/"/>
  </author>
  <description>A GPS executor implementation.</description>
  <license href="http://www.cs.wustl.edu/~schmidt/ACE-copying.html"/>
  <idl id="IDL:HUDisplay/GPS:1.0" homeid="IDL:HUDisplay/GPSHome:1.0">
    <fileinarchive name="GPS.idl"/>
  </idl>

  <!-- We don't need a property file for this example.  However,         -->
  <!-- what to do with this element?  Cache it in ComponentInstallation? -->
  <!-- A better question maybe, when do we actually read the file and    -->
  <!-- build the attributes defined in the file?  By Assembly perhaps?   -->
  <!-- Notice that this property file applies to the implementation itself. -->
  <!-- Whatever that means. -->
<!--   <propertyfile> -->
<!--     <fileinarchive name="empty.cpf"/> -->
<!--   </propertyfile> -->

  <descriptor type="CORBA Component">
    <fileinarchive name="GPS.ccd"/>
  </descriptor>

  <implementation id="DCE:3148F760-F2ED-4204-A775-6B972C10E8CB">

    <!-- CIAO extension -->
    <dependency type="CIAODLL">
      <softpkgref>
        <!-- .ssd stands for Servant Software Descriptors which is a CIAO extension -->
        <fileinarchive name="GPS.ssd"/>
        <implref idref="DCE:75309233-0E0A-4cfb-B186-3E99F69B1D40"/>
      </softpkgref>
    </dependency>

    <!-- What happens when one define descriptors for both softpkg and -->
    <!-- implementations? -->

    <code type="DLL">
      <!-- No need to specify extension below since ACE takes care of that, -->
      <fileinarchive name="GPS_exec"/>
      <entrypoint>createGPSHome_Impl</entrypoint>
    </code>

  </implementation>

  <!-- Let's add more implementation description later when we try to -->
  <!-- compile this stuff on, say, Solaris and Linux.                 -->

</softpkg>
