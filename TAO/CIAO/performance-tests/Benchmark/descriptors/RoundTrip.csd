<?xml version="1.0"?> <!-- -*- SGML -*-  -->

<!-- If this file is moved to a different directory, make sure to change the
     path to the DTD below. Otherwise the examples won't work. -->

<!DOCTYPE softpkg SYSTEM "http://www.dre.vanderbilt.edu/CIAO/DTD/softpkg.dtd">

<!-- RoundTrip's Software Package Descriptor  -->
<!-- This file describes various RoundTrip executor -->
<!-- implementations. -->

<softpkg name="CIAO-TEST" version="1.0">
  <pkgtype>CORBA Component</pkgtype>
  <title>RoundTrip</title>
  <author>
    <company>ISIS at Vanderbilt University</company>
    <webpage href="http://www.dre.vanderbilt.edu/"/>
  </author>
  <description>A RoundTrip component.</description>
  <license href="http://www.cs.wustl.edu/~schmidt/ACE-copying.html"/>
  <idl id="IDL:Benchmark/RoundTrip:1.0" homeid="IDL:Benchmark/RoundTripHome:1.0">
    <fileinarchive name="RoundTrip.idl"/>
  </idl>
  <descriptor type="CORBA Component">
    <fileinarchive name="RoundTrip.ccd"/>
  </descriptor>
  <implementation id="DCE:2498aa68-09aa-478c-8ae1-30ed67924f82">

    <!-- CIAO extension -->
  <dependency type="CIAODLL">
    <softpkgref>
    <!-- .ssd stands for Servant Software Descriptors which is a CIAO extension -->
    <fileinarchive name="RoundTrip.ssd"/>
    <implref idref="DCE:88c14bff-5158-4f48-9b22-8a1120e87f76"/>
    </softpkgref>
  </dependency>

  <code type="DLL">
  <!-- No need to specify extension below since ACE takes care of that, -->
  <fileinarchive name="RoundTrip_exec"/>
  <entrypoint>createRoundTripHome_Impl</entrypoint>
  </code>

  </implementation>

</softpkg>
