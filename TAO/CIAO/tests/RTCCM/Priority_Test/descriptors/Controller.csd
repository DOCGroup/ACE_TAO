<?xml version="1.0"?> <!-- -*- SGML -*-  -->

<!-- If this file is moved to a different directory, make sure to change the
     path to the DTD below. Otherwise the examples won't work. -->
<!DOCTYPE softpkg SYSTEM "../../../docs/XML/softpkg.dtd">

<!-- Controller Software Package Descriptor  -->
<!-- This file describes various Controller executors -->
<!-- implementations. -->


<softpkg name="CIAO-Controller" version="1.0">
  <pkgtype>CORBA Component</pkgtype>
  <title>Controller Component</title>
  <author>
    <company>Washington University in St. Louis</company>
    <webpage href="http://www.cs.wustl.edu/~doc/"/>
  </author>
  <description>A controller executor implementation.</description>
  <license href="http://www.cs.wustl.edu/~schmidt/ACE-copying.html"/>
  <idl id="IDL:Priority_Test/Controller:1.0" homeid="IDL:Priority_Test/ControllerHome:1.0">
    <fileinarchive name="Controller.idl"/>
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
    <fileinarchive name="Controller.ccd"/>
  </descriptor>

  <implementation id="DCE:9663E039-96F8-41BE-BDA9-3BF4654DE996">

    <!-- CIAO extension -->
    <dependency type="CIAODLL">
      <softpkgref>
        <!-- .ssd stands for Servant Software Descriptors which is a CIAO extension -->
        <fileinarchive name="Controller.ssd"/>
        <implref idref="DCE:A4981A0E-DF91-46B1-A77C-58B6052CC799"/>
      </softpkgref>
    </dependency>

    <!-- What happens when one define descriptors for both softpkg and -->
    <!-- implementations? -->

    <code type="DLL">
      <!-- No need to specify extension below since ACE takes care of that, -->
      <fileinarchive name="Controller_exec"/>
      <entrypoint>createControllerHome_Impl</entrypoint>
    </code>

  </implementation>

  <!-- Let's add more implementation description later when we try to -->
  <!-- compile this stuff on, say, Solaris and Linux.                 -->

</softpkg>
