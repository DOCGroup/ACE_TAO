//$Id$
/*
 * NetworkMonitorImpl.java - the probe monitor
 *
 * The DARPA ARMS PROGRAM
 * This software is governed by Government Purpose Rights
 * Contract No: NBCHC030132
 * Telcordia Technologies, Inc.
 * One Telcordia Drive
 * Piscataway, NJ 08854-4157
 *
 * Author: Kirthika Parmeswaran
 */

package mil.darpa.arms.mlrm.BandwidthBroker.networkmonitor.client;
import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import mil.darpa.arms.mlrm.BandwidthBroker.*;
import mil.darpa.arms.mlrm.BandwidthBroker.NetworkMonitorPackage.*;
import mil.darpa.arms.mlrm.BandwidthBroker.CommonDef.*;
import org.omg.CosNaming.*;
import org.omg.CosNaming.NamingContextPackage.*;

/** This class tests the NetworkMonitor server portion of the
 * BandwidthBroker framework. 

 * The test_latency_betn_hosts () and test_latency_betn_pools () use the same
 * source-destination measurement hosts + TOS and hence have the same
 * probeID. The test_latency_betn_flowendpoints () uses flowtoken to find
 * latency. The TOS is different in this case, hence the probeID is different.
 * Since, the first time around, the probes are started and the test
 * has to wait for resuts, the test may take 3*measurementPeriod
 * (i.e. 3min in this case) to complete. Keep the server running, and
 * retry the client, it will be faster the second time.

 * It is required that before running this
 * test, the probe server and Imp software be running on the probe
 * hosts.  The scripts bin/runProbeServer.sh and bin/RunProbe.sh show how to
 * do this respectively, first start the probe server,then the probes.  
 * The nmconfig.prop file mentions the names of the probe
 * hosts. These hosts must also be populated into the BB Mysql DB as
 * part of the HostSubnet table.  The test also uses a <flowToken>
 * which is the flow record ID in the BB Mysql DB.  To populate the
 * BBMySQL DB for running this test, it is necessary to populate
 * db/nm-data.sql in addition to populating the other sql inputs. */


public class NetworkMonitorClient
{

    public static void main (String [] args)
    {
	NetworkMonitorClient nmClient = new NetworkMonitorClient ();

	if( args.length < 1 ) 
	    {
		System.out.println( "Usage: java "+
				    " com.telcordia.research.BandwidthBroker.networkmonitor.client.NetworkMonitorClient"+
				    " -ORBInitialHost <name service host> "+
				    "-ORBInitialPort <name service port>" );
		System.exit( 1 );
	    }
      // initialize the ORB.
	ORB orb = ORB.init( args, null );
	org.omg.CORBA.Object obj =null;
	try 
	    {
		NamingContextExt rootContext = null;
		NamingContextExt root;
		
		try
		    {
			// Resolve NameService
			obj =
			    orb.resolve_initial_references ("NameService");
			rootContext = NamingContextExtHelper.narrow (obj);
		    }
		catch (org.omg.CORBA.ORBPackage.InvalidName ex)
		    {
			System.err.println ("Unknown Service");
		    }
		if (rootContext == null)
		    {
			System.err.println ("Failed to resolve NameService");
			System.exit (1);
		    }
		else
		    {
			System.out.println ("Resolved NameService");
		    }
		// Lookup the NetworkMonitor object)
		NameComponent theName [] = {
              new NameComponent("mil", ""),
              new NameComponent("darpa", ""),
              new NameComponent("arms", ""),
              new NameComponent("mlrm", ""),
              new NameComponent("BandwidthBroker", ""),
              new NameComponent("NetworkMonitor", "")

            };
		obj = rootContext.resolve_str (rootContext.to_string (theName));
		
		// and narrow it to NetworkMonitor
		NetworkMonitor netMon = NetworkMonitorHelper.narrow( obj );

		System.out.println ("Testing PROBE SETUP begins ...");
		System.out.println (nmClient.test_probe_setup_error (netMon));
		System.out.println ("Testing PROBE SETUP ends.");

		System.out.println ("Testing DEFAULT PROBE SETTINGS begins ...");
		System.out.println (nmClient.test_probe_defaults (netMon));
		System.out.println ("Testing DEFAULT PROBE SETTINGS ends.");

		System.out.println ("Testing DELAY BETWEEN HOSTS begins ...");
		System.out.println (nmClient.test_delay_betn_hosts (netMon));
		System.out.println ("Testing DELAY BETWEEN HOSTS ends.");

		System.out.println ("Testing DELAY BETWEEN FLOW ENDPOINTS begins ...");
		System.out.println ("DELAY="+new Long (nmClient.test_delay_betn_flowendpoints (netMon)).toString ()+ 
				    " microseconds");
		System.out.println ("Testing DELAY BETWEEN FLOW ENDPOINTS ends.");

		System.out.println ("Testing DELAY BETWEEN POOLS begins ...");
		System.out.println ("DELAY="+new Long (nmClient.test_delay_betn_pools (netMon)).toString ()+
				    " microseconds");
		System.out.println ("Testing DELAY BETWEEN POOLS ends.");
		
	    }
	catch (Exception e)
	    {
		System.err.println ("Exception in network monitor client execution!"+e.toString ()); 
		e.printStackTrace ();
	    }
    }

    public String test_probe_setup_error (NetworkMonitor netMon)
    {

	// Build request for latency betn hosts
	MeasurementEndpointsType type = MeasurementEndpointsType.latency_between_hosts;
	MeasurementEndpoints me = new MeasurementEndpoints ();
	LatencyBetnHosts[] latencyBetnHosts = new LatencyBetnHosts [1];
	LatencyPerQoS[] latencyPerQoSSeq = new LatencyPerQoS[1];
	Latency latency = new Latency (0, // delay will be populated in this field
				       190000000,//averaging window >=1, in microseconds
				       60000000, // measurementPeriod , in microseconds
				       0); // threshold
	latencyPerQoSSeq [0]= new LatencyPerQoS (latency, QOSRequired.highPriority);

	// ERROR: misspelt destination!
	latencyBetnHosts[0] = new LatencyBetnHosts ("rondo-mon08", 
						    "rondo-monnnn09", 
						    latencyPerQoSSeq);
	
	me.hostsLatencyInfo (latencyBetnHosts);
	MeasurementEndpointsHolder latencyInfo = new MeasurementEndpointsHolder (me);
	System.out.println ("Delay measurement request between hosts rondo-mon08 and rondo-monnnn09"+
			    " launched with measurement period of 60 seconds and averaging window of 190 seconds."+
			    " A nonexistent hostname: rondo-monnnn09 has been provided for this test.");
	long defaultMeasurementPeriod=0;
	try{
	    netMon.get_measurement (type,latencyInfo );
	} catch (NetworkProbesException ne)
	    {
		//System.out.println (ne.reason);
		//ne.printStackTrace ();
		//System.exit (-1);
		if (ne.probeConfigurationError)
		    {
			// Now retrieve and display the defaults for both the probes
			LatencyBetnHosts[] latencyBetnHostsSeq = latencyInfo.value.hostsLatencyInfo ();
			LatencyBetnHosts latencyStruct1 = latencyBetnHostsSeq[0];
			LatencyPerQoS latencyPerClass1 = latencyStruct1.latencyPerClassSeq[0];
			Latency latencyVal1 = latencyPerClass1.availableLatency;
			defaultMeasurementPeriod = latencyVal1.measurementPeriod;
		       	System.out.println ("Exception during PROBE SETUP!");
			return ne.reason;
			
		    }
		
	    }

	return null;
    }

     public String test_probe_defaults (NetworkMonitor netMon)
    {

	// Build request for latency betn hosts
	MeasurementEndpointsType type = MeasurementEndpointsType.latency_between_hosts;
	MeasurementEndpoints me = new MeasurementEndpoints ();
	LatencyBetnHosts[] latencyBetnHosts = new LatencyBetnHosts [1];
	LatencyPerQoS[] latencyPerQoSSeq = new LatencyPerQoS[1];
	Latency latency = new Latency (0, // delay will be populated in this field
				       190000000,//averaging window >=1, in microseconds
				       30000000, // measurementPeriod (in microseconds) < less than default, 
				                 //should give error
				       0); // threshold
	latencyPerQoSSeq [0]= new LatencyPerQoS (latency, QOSRequired.highPriority);
	latencyBetnHosts[0] = new LatencyBetnHosts ("rondo-mon08", 
						    "rondo-mon09", 
						    latencyPerQoSSeq);
	
	me.hostsLatencyInfo (latencyBetnHosts);
	MeasurementEndpointsHolder latencyInfo = new MeasurementEndpointsHolder (me);
	System.out.println ("Delay measurement request between hosts rondo-mon08 and rondo-mon09"+
			    " launched with measurement period of 30 seconds and averaging window of 190 seconds."+
			    " A lower-than-default measurement period has been provided for this test.");

	long defaultMeasurementPeriod=0;
	try{
	    netMon.get_measurement (type,latencyInfo );
	} catch (NetworkProbesException ne)
	    {
		//System.out.println (ne.reason);
		//ne.printStackTrace ();
		//System.exit (-1);
		if (ne.probeConfigurationError)
		    {
			// Now retrieve and display the defaults for both the probes
			LatencyBetnHosts[] latencyBetnHostsSeq = latencyInfo.value.hostsLatencyInfo ();
			LatencyBetnHosts latencyStruct1 = latencyBetnHostsSeq[0];
			LatencyPerQoS latencyPerClass1 = latencyStruct1.latencyPerClassSeq[0];
			Latency latencyVal1 = latencyPerClass1.availableLatency;
			defaultMeasurementPeriod = latencyVal1.measurementPeriod;
			System.out.println ("Exception in DEFAULT PROBE CONFIGURATION!");	
			return ne.reason + "microseconds";
			
		    }
		
	    }

	return null;
    }

    

    public String test_delay_betn_hosts (NetworkMonitor netMon)
    {

	// Build request for latency betn hosts
	MeasurementEndpointsType type = MeasurementEndpointsType.latency_between_hosts;
	MeasurementEndpoints me = new MeasurementEndpoints ();
	LatencyBetnHosts[] latencyBetnHosts = new LatencyBetnHosts [2];
	LatencyPerQoS[] latencyPerQoSSeq = new LatencyPerQoS[2];
	Latency latency = new Latency (0, // delay will be populated in this field
				       190000000,//averaging window >=1, in microseconds
				       60000000, // measurementPeriod, in microseconds
				       0); // threshold
	latencyPerQoSSeq [0]= new LatencyPerQoS (latency, QOSRequired.highPriority);
	latencyBetnHosts[0] = new LatencyBetnHosts ("rondo-mon08", 
						    "rondo-mon09", 
						    latencyPerQoSSeq);
	latencyPerQoSSeq [1]= new LatencyPerQoS (latency, QOSRequired.highReliability);
	latencyBetnHosts[1] = new LatencyBetnHosts ("rondo-mon08", 
						    "rondo-mon09", 
						    latencyPerQoSSeq);
	me.hostsLatencyInfo (latencyBetnHosts);
	MeasurementEndpointsHolder latencyInfo = new MeasurementEndpointsHolder (me);
	System.out.println ("Delay measurement request between hosts rondo-mon08 and rondo-mon09"+
			    " for highPriority and highReliability QoS flows"+
			    " launched with measurement period of 60 seconds and averaging window of 190 seconds.");
	
	try{
	    netMon.get_measurement (type,latencyInfo );
	} catch (NetworkProbesException ne)
	    {
		System.out.println (ne.reason);
		ne.printStackTrace ();
		//System.exit (-1);
	
		return null;
	    }
	
	// Now retrieve and display the delay for both the probes
	LatencyBetnHosts[] latencyBetnHostsSeq = latencyInfo.value.hostsLatencyInfo ();
	LatencyBetnHosts latencyStruct1 = latencyBetnHostsSeq[0];
	LatencyPerQoS latencyPerClass1 = latencyStruct1.latencyPerClassSeq[0];
	Latency latencyVal1 = latencyPerClass1.availableLatency;
	long delay1 = latencyVal1.delayTime;//  in microseconds
	LatencyBetnHosts latencyStruct2 = latencyBetnHostsSeq[1];
	LatencyPerQoS latencyPerClass2 = latencyStruct1.latencyPerClassSeq[1];
	Latency latencyVal2 = latencyPerClass2.availableLatency;
	long delay2 = latencyVal2.delayTime;//  in microseconds
	
	String ret = " DELAY for highPriority flow ="+new Long (delay1).toString () +
	    " microseconds and for highReliability flow ="+new Long (delay2).toString ()+ " microseconds";

	return ret;
	
    }
    public long test_delay_betn_pools (NetworkMonitor netMon)
    {
	// Build request for latency betn pools
	MeasurementEndpointsType type = MeasurementEndpointsType.latency_between_pools;
	MeasurementEndpoints me = new MeasurementEndpoints ();
	LatencyBetnPools[] latencyBetnPools = new LatencyBetnPools [1];
	LatencyPerQoS[] latencyPerQoSSeq = new LatencyPerQoS[1];
	Latency latency = new Latency (0, // delay will be populated in this field
				       60000000,//averaging window >=1,  in microseconds
				       60000000, // measurementPeriod,  in microseconds
				       0); // threshold
	latencyPerQoSSeq [0]= new LatencyPerQoS (latency, QOSRequired.highPriority);
	latencyBetnPools[0] = new LatencyBetnPools ("Pool-1C", 
						    "Pool-1D", 
						    latencyPerQoSSeq);
	me.poolLatencyInfo (latencyBetnPools);
	MeasurementEndpointsHolder latencyInfo = new MeasurementEndpointsHolder (me);
	System.out.println ("Delay measurement request between pools Pool-1C and Pool-1D"+
			    " for highPriority QoS flow"+
			    " launched with measurement period of 60 seconds and averaging window of 60 seconds.");
	
	try{
	    netMon.get_measurement (type,latencyInfo );
	} catch (NetworkProbesException ne)
	    {
		System.out.println (ne.reason);
		ne.printStackTrace ();
		//	System.exit (-1);
		return 0;
	    }
	
	// Now retrieve and display the delay
	LatencyBetnPools[] latencyBetnPoolsSeq = latencyInfo.value.poolLatencyInfo ();
	LatencyBetnPools latencyStruct = latencyBetnPoolsSeq[0];
	LatencyPerQoS latencyPerClass = latencyStruct.latencyPerClassSeq[0];
	Latency latencyVal = latencyPerClass.availableLatency;
	return latencyVal.delayTime;//  in microseconds
	
    }

    public long test_delay_betn_flowendpoints (NetworkMonitor netMon)
    {
	// Now for delay betn flow endpoints
	MeasurementEndpointsType type = MeasurementEndpointsType.latency_between_flow_endpoints;
	MeasurementEndpoints me = new MeasurementEndpoints ();
	LatencyBetnFlowEndpoints[] latencyBetnFlowEndpoints = new LatencyBetnFlowEndpoints [1];
	LatencyPerQoS[] latencyPerQoSSeq = new LatencyPerQoS[1];
	Latency latency = new Latency (0, // delay will be populated in this field
				       190000000,//averaging window >=1,  in microseconds
				       60000000, // measurementPeriod,  in microseconds
				       0); // threshold
	latencyPerQoSSeq [0]= new LatencyPerQoS (latency, QOSRequired.highReliability);

	// Note: this is something that the user may have to change before running this test.
	String flowToken = "1";
	latencyBetnFlowEndpoints[0] = new LatencyBetnFlowEndpoints (flowToken,
								    latencyPerQoSSeq);
	me.flowLatencyInfo (latencyBetnFlowEndpoints);
	MeasurementEndpointsHolder latencyInfo = new MeasurementEndpointsHolder (me);
	System.out.println ("Delay measurement request between flow endpoints with flowToken of 1"+
			    " for highReliability QoS flow"+
			    " launched with measurement period of 60 seconds and averaging window of 190 seconds.");
	
	try{
	    netMon.get_measurement (type,latencyInfo );
	} catch (NetworkProbesException ne)
	    {
		System.out.println (ne.reason);
		ne.printStackTrace ();
		//System.exit (-1);
		return 0;
	    }
	
	// Now retrieve and display the delay
	LatencyBetnFlowEndpoints[] latencyBetnFlowEndpointsSeq = latencyInfo.value.flowLatencyInfo ();
	LatencyBetnFlowEndpoints latencyStruct = latencyBetnFlowEndpointsSeq[0];
	LatencyPerQoS latencyPerClass = latencyStruct.latencyPerClassSeq[0];
	Latency latencyVal = latencyPerClass.availableLatency;
	return latencyVal.delayTime; //  in microseconds

    }
}
