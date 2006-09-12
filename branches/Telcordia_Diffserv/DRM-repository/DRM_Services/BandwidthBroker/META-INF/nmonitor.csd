<?xml version="1.0" encoding="UTF-8"?>

<!DOCTYPE softpkg PUBLIC "-//OMG//DTD CORBA Software Descriptor 3.0//EN" "http://openccm.objectweb.org/dtd/ccm/softpkg.dtd">
<softpkg version="0,8,0,0" name="Consumer">
  <title>OpenCCM Network Monitor Component</title>
  <pkgtype version="">CORBA Component</pkgtype>
  <author>
    <name>Murray Conarty</name>
    <company>PrismTech</company>
    <webpage href="http://openccm.objectweb.org/" xml:link="SIMPLE"></webpage>
  </author>
  <description>Package for Network Monitor Component</description>
  <license href="" xml:link="SIMPLE"></license>
  <idl homeid="IDL:/mil/darpa/arms/mlrm/BandwidthBroker/NetworkMonitorComponentHome:1.0" id="IDL:/mil/darpa/arms/mlrm/BandwidthBroker/NetworkMonitorComponent:1.0">
    <link href="BBComponents.idl3" xml:link="SIMPLE"></link>
  </idl>
  <dependency type="Java class" action="install">
    <fileinarchive name="extarchives/hsqldb.jar">
    </fileinarchive>
  </dependency>
  <dependency type="Java class" action="install">
    <fileinarchive name="extarchives/telcordia-imp.jar">
    </fileinarchive>
  </dependency>
  <dependency type="Java class" action="install">
    <fileinarchive name="extarchives/jaxb-api.jar">
    </fileinarchive>
  </dependency>
  <dependency type="Java class" action="install">
    <fileinarchive name="extarchives/jaxb-impl.jar">
    </fileinarchive>
  </dependency>
  <dependency type="Java class" action="install">
    <fileinarchive name="extarchives/jaxb-libs.jar">
    </fileinarchive>
  </dependency>
  <dependency type="Java class" action="install">
    <fileinarchive name="extarchives/jaxb-xjc.jar">
    </fileinarchive>
  </dependency>
  <dependency type="Java class" action="install">
    <fileinarchive name="extarchives/jax-qname.jar">
    </fileinarchive>
  </dependency>
  <dependency type="Java class" action="install">
    <fileinarchive name="extarchives/relaxngDatatype.jar">
    </fileinarchive>
  </dependency>
  <dependency type="Java class" action="install">
    <fileinarchive name="extarchives/namespace.jar">
    </fileinarchive>
  </dependency>
  <descriptor type="CORBA Component">
    <fileinarchive name="META-INF/nmonitor.ccd">
    </fileinarchive>
  </descriptor>
  <implementation variation="" id="NetworkMonitorComponentImpl">
    <code type="Java Class">
      <fileinarchive name="archives/nmcomponent.jar">
      </fileinarchive>
      <entrypoint>mil.darpa.arms.mlrm.BandwidthBroker.cif.NetworkMonitorComponentHomeImpl.create_home</entrypoint>
    </code>
    <compiler version="" name="JDK"/>
    <programminglanguage name="Java"/>
    <os version="" name="Windows XP"/>
    <os version="2,4,20,0" name="Linux"/>
    <processor name="x86"/>
    <runtime version="1,3,1,0" name="Java VM"/>
  </implementation>
</softpkg>
