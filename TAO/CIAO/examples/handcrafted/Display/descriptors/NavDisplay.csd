<?xml version="1.0"?> <!-- -*- SGML -*-  -->

<!-- XML doesn't really support the following $(CIAO_ROOT) syntax :) -->
<!DOCTYPE softpkg SYSTEM "$(CIAO_ROOT)/docs/XML/softpkg.dtd">

<!-- NavDisplay's Software Package Descriptor  -->
<!-- This file describes various NavDisplay executor -->
<!-- implementations. -->


<softpkg name="CIAO-NavDisplay" version="1.0">
  <pkgtype>CORBA Component</pkgtype>
  <title>Rate Generator</title>
  <author>
    <company>Washington University in St. Louis</company>
    <webpage href="http://www.cs.wustl.edu/~doc/"/>
  </author>
  <description>A NavDisplay executor implementation.</description>
  <license href="http://www.cs.wustl.edu/~schmidt/ACE-copying.html"/>
  <idl id="IDL:HUDisplay/NavDisplay:1.0" homeid="IDL:HUDisplay/NavDisplayHome:1.0">
    <fileinarchive name="NavDisplay.idl"/>
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
    <fileinarchive name="NavDisplay.ccd"/>
  </descriptor>

  <implementation id="DCE:8E6C468D-A39F-46b4-962B-265F1AA8D538">

    <!-- CIAO extension -->
    <dependency type="CIAODLL">
      <softpkgref>
        <!-- .ssd stands for Servant Software Descriptors which is a CIAO extension -->
        <fileinarchive name="NavDisplay.ssd"/>
        <implref idref="DCE:CDC06FCA-50FC-43ca-8ECC-BEFBD33FEE78"/>
      </softpkgref>
    </dependency>

    <!-- What happens when one define descriptors for both softpkg and -->
    <!-- implementations? -->

    <code type="DLL">
      <!-- No need to specify extension below since ACE takes care of that, -->
      <fileinarchive name="NavDisplay_exec"/>
      <entrypoint>createNavDisplayHome_Impl</entrypoint>
    </code>

  </implementation>

  <!-- Let's add more implementation description later when we try to -->
  <!-- compile this stuff on, say, Solaris and Linux.                 -->

</softpkg>