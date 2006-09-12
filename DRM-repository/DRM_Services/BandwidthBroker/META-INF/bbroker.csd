<?xml version="1.0" encoding="UTF-8"?>

<!DOCTYPE softpkg PUBLIC "-//OMG//DTD CORBA Software Descriptor 3.0//EN" "http://openccm.objectweb.org/dtd/ccm/softpkg.dtd">
<softpkg version="0,8,0,0" name="BandwidthBroker">
  <title>OpenCCM Bandwidth Broker Component</title>
  <pkgtype version="">CORBA Component</pkgtype>
  <author>
    <name>Murray Conarty</name>
    <company>PrismTech</company>
    <webpage href="http://openccm.objectweb.org/" xml:link="SIMPLE"></webpage>
  </author>
  <description>This is the package for the component implementation of the Admission Control</description>
  <license href="http://openccm.objectweb.org/license.html" xml:link="SIMPLE"></license>
  <idl homeid="IDL:/mil/darpa/arms/mlrm/BandwidthBroker/BandwidthBrokerComponentHome:1.0" id="IDL:/mil/darpa/arms/mlrm/BandwidthBroker/BandwidthBrokerComponent:1.0">
    <link href="idl/BBComponents.idl3" xml:link="SIMPLE"></link>
  </idl>
  <dependency type="Java class" action="install">
    <fileinarchive name="archives/cosproperty.jar">
    </fileinarchive>
  </dependency>
  <dependency type="Java class" action="install">
    <fileinarchive name="extarchives/mysql-connector-java-3.1.12-bin.jar">
    </fileinarchive>
  </dependency>
  <dependency type="Java class" action="install">
    <fileinarchive name="archives/l2disc.jar">
    </fileinarchive>
  </dependency>
  <descriptor type="CORBA Component">
    <fileinarchive name="META-INF/bbroker.ccd">
    </fileinarchive>
  </descriptor>
  <implementation variation="" id="BandwidthBrokerComponentImpl">
    <code type="Java Class">
      <fileinarchive name="archives/bbcomponent.jar">
      </fileinarchive>
      <entrypoint>mil.darpa.arms.mlrm.BandwidthBroker.cif.BandwidthBrokerComponentHomeImpl.create_home</entrypoint>
    </code>
    <compiler version="" name="JDK"/>
    <programminglanguage version="" name="Java"/>
    <os version="" name="Windows XP"/>
    <os version="2,4,20,0" name="Linux"/>
    <processor name="x86"/>
    <runtime version="1,3,1,0" name="Java VM"/>
  </implementation>
</softpkg>
