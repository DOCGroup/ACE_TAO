<?xml version="1.0"?> <!-- -*- SGML -*-  -->

<!-- If this file is moved to a different directory, make sure to change the
     path to the DTD below. Otherwise the examples won't work. -->
<!DOCTYPE softpkg SYSTEM "http://www.dre.vanderbilt.edu/CIAO/DTD/softpkg.dtd">
<!-- RoundTripClient's Software Package Descriptor  -->
<!-- This file describes various RoundTripClient executor -->
<!-- implementations. -->


<softpkg name="CIAO-TEST" version="1.0">
  <pkgtype>CORBA Component</pkgtype>
  <title>RoundTripClient</title>
  <author>
    <company>ISIS at Vanderbilt University</company>
    <webpage href="http://www.dre.vanderbilt.edu/"/>
  </author>
  <description>A RoundTripClient component.</description>
  <license href="http://www.cs.wustl.edu/~schmidt/ACE-copying.html"/>
  <idl id="IDL:Benchmark/RoundTripClient:1.0" homeid="IDL:Benchmark/RoundTripClientHome:1.0">
    <fileinarchive name="RoundTripClient.idl"/>
  </idl>
  <descriptor type="CORBA Component">
    <fileinarchive name="RoundTripClient.ccd"/>
  </descriptor>
  <implementation id="DCE:568a3872-f7bb-4b3f-baba-44c6824d966d">

    <!-- CIAO extension -->
  <dependency type="CIAODLL">
    <softpkgref>
    <!-- .ssd stands for Servant Software Descriptors which is a CIAO extension -->
    <fileinarchive name="RoundTripClient.ssd"/>
    <implref idref="DEC:cf88cb97-b5a8-4522-9fa3-e626c75c5470"/>
    </softpkgref>
  </dependency>

  <code type="DLL">
  <!-- No need to specify extension below since ACE takes care of that, -->
  <fileinarchive name="RoundTripClient_exec"/>
  <entrypoint>createRoundTripClientHome_Impl</entrypoint>
  </code>
  </implementation>

</softpkg>
