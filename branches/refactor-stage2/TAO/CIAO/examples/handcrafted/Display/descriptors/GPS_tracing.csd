<?xml version="1.0"?> <!-- -*- SGML -*-  -->

<!-- XML doesn't really support the following $(CIAO_ROOT) syntax :) -->
<!DOCTYPE softpkg SYSTEM "$(CIAO_ROOT)/docs/XML/softpkg.dtd">

<!-- GPS's Software Package Descriptor  -->
<!-- This file describes various GPS executor -->
<!-- implementations. -->


<softpkg name="CIAO-GPS" version="1.0">
  <pkgtype>CORBA Component</pkgtype>
  <title>GPS tracing emulater</title>
  <author>
    <company>Washington University in St. Louis</company>
    <webpage href="http://www.cs.wustl.edu/~doc/"/>
  </author>
  <description>A GPS executor implementation that emulate a trace.</description>
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

  <implementation id="DCE:8E92655E-CA07-46C8-B127-0F0872A8CC29">

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
      <fileinarchive name="GPS_tracing_exec"/>
      <entrypoint>createGPSHome_Tracing_Impl</entrypoint>
    </code>

  </implementation>

  <!-- Let's add more implementation description later when we try to -->
  <!-- compile this stuff on, say, Solaris and Linux.                 -->

</softpkg>