<?xml version="1.0"?> <!-- -*- SGML -*-  -->

<!-- If this file is moved to a different directory, make sure to change the
     path to the DTD below. Otherwise the examples won't work. -->
<!DOCTYPE softpkg SYSTEM "../../../docs/XML/softpkg.dtd">

<!-- Hello World's Software Package Descriptor  -->
<!-- This file describes various HelloWorld executor -->
<!-- implementations. -->


<softpkg name="CIAO-HelloWorld" version="1.0">
  <pkgtype>CORBA Component</pkgtype>
  <title>Hello World</title>
  <author>
    <company>Washington University in St. Louis</company>
    <webpage href="http://www.cs.wustl.edu/~doc/"/>
  </author>
  <description>A Hello World executor implementation.</description>
  <license href="http://www.cs.wustl.edu/~schmidt/ACE-copying.html"/>
  <idl id="IDL:HelloWorld:1.0" homeid="IDL:HelloHome:1.0">
    <fileinarchive name="hello.idl"/>
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
    <fileinarchive name="hello.ccd"/>
  </descriptor>

  <implementation id="DCE:530a6305-8181-47ca-bd82-0b834016db97">
    <!-- Perhaps we can list more OS here as ACE knows what to look for? -->
    <!-- Maybe not... But then selecting the right configuration becomes hard. -->
    <os name="WinNT" version="4.0"/>
    <os name="WinXP" version="5.0"/>
    <processor name="x86"/>
    <!-- What about configuration?  Such as Debug/Release? -->
    <compile name="MSVC" version="6.0"/>
    <programminglanguage name="C++"/>
    <dependency type="ORB">
      <name>TAO</name>
    </dependency>

    <!-- CIAO extension -->
    <dependency type="CIAODLL">
      <softpkgref>
        <!-- .ssd stands for Servant Software Descriptors which is a CIAO extension -->
        <fileinarchive name="hello.ssd"/>
        <implref idref="DCE:05833d92-4783-4b85-ac14-e2575dac26f7"/>
      </softpkgref>
    </dependency>

    <!-- What happens when one define descriptors for both softpkg and -->
    <!-- implementations? -->

    <code type="DLL">
      <!-- No need to specify extension below since ACE takes care of that, -->
      <fileinarchive name="hello_executors"/>
      <entrypoint>createHelloHome_Impl</entrypoint>
    </code>

  </implementation>

  <!-- Let's add more implementation description later when we try to -->
  <!-- compile this stuff on, say, Solaris and Linux.                 -->

</softpkg>
