<?xml version="1.0"?> <!-- -*- SGML -*-  -->

<!-- If this file is moved to a different directory, make sure to change the
     path to the DTD below. Otherwise the examples won't work. -->
<!DOCTYPE softpkg SYSTEM "../../../docs/XML/softpkg.dtd">

<!-- CB_Worker's Software Package Descriptor  -->
<!-- This file describes various CB_Worker executor -->
<!-- implementations. -->


<softpkg name="CIAO-CB-Worker" version="1.0">
  <pkgtype>CORBA Component</pkgtype>
  <title>Compute-bound Worker Component</title>
  <author>
    <company>Washington University in St. Louis</company>
    <webpage href="http://www.cs.wustl.edu/~doc/"/>
  </author>
  <description>A compute-bound Worker executor implementation.</description>
  <license href="http://www.cs.wustl.edu/~schmidt/ACE-copying.html"/>
  <idl id="IDL:Priority_Test/Worker:1.0" homeid="IDL:Priority_Test/WorkerHome:1.0">
    <fileinarchive name="Worker.idl"/>
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
    <fileinarchive name="Worker.ccd"/>
  </descriptor>

  <implementation id="DCE:1EB5F4C6-7F5D-43F2-9A44-E89B6E96377F">

    <!-- CIAO extension -->
    <dependency type="CIAODLL">
      <softpkgref>
        <!-- .ssd stands for Servant Software Descriptors which is a CIAO extension -->
        <fileinarchive name="Worker.ssd"/>
        <implref idref="DCE:53E7624B-96D9-4E2B-BD73-66A56082AA1A"/>
      </softpkgref>
    </dependency>

    <!-- What happens when one define descriptors for both softpkg and -->
    <!-- implementations? -->

    <code type="DLL">
      <!-- No need to specify extension below since ACE takes care of that, -->
      <fileinarchive name="CB_Worker_exec"/>
      <entrypoint>createWorkerHome_Impl</entrypoint>
    </code>

  </implementation>

  <!-- Let's add more implementation description later when we try to -->
  <!-- compile this stuff on, say, Solaris and Linux.                 -->

</softpkg>
