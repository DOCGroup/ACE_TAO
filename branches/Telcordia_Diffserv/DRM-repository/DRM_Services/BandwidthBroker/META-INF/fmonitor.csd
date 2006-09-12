<?xml version="1.0" encoding="UTF-8"?>

<!DOCTYPE softpkg PUBLIC "-//OMG//DTD CORBA Software Descriptor 3.0//EN" "http://openccm.objectweb.org/dtd/ccm/softpkg.dtd">
<softpkg version="0,8,0,0" name="Consumer">
  <title>OpenCCM Fault Monitor Component</title>
  <pkgtype version="">CORBA Component</pkgtype>
  <author>
    <name>Murray Conarty</name>
    <company>PrismTech</company>
    <webpage href="http://openccm.objectweb.org/" xml:link="SIMPLE"></webpage>
  </author>
  <description>Component implementation of Fault Monitor component</description>
  <license href="" xml:link="SIMPLE"></license>
  <idl homeid="IDL:/mil/darpa/arms/mlrm/BandwidthBroker/FaultMonitorComponentHome:1.0" id="IDL:/mil/darpa/arms/mlrm/BandwidthBroker/FaultMonitorComponent:1.0">
    <link href="BBComponents.idl3" xml:link="SIMPLE"></link>
  </idl>
  <descriptor type="CORBA Component">
    <fileinarchive name="META-INF/fmonitor.ccd">
    </fileinarchive>
  </descriptor>
  <implementation variation="" id="FaultMonitorComponentImpl">
    <code type="Java Class">
      <fileinarchive name="archives/fmcomponent.jar">
      </fileinarchive>
      <entrypoint>mil.darpa.arms.mlrm.BandwidthBroker.cif.FaultMonitorComponentHomeImpl.create_home</entrypoint>
    </code>
    <compiler version="" name="JDK"/>
    <programminglanguage name="Java"/>
    <os version="" name="Windows XP"/>
    <os version="2,4,20,0" name="Linux"/>
    <processor name="x86"/>
    <runtime version="1,3,1,0" name="Java VM"/>
  </implementation>
</softpkg>
