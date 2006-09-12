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
 * BandwidthBroker framework. The focus is on latency events from the
 * NetworkMonitor server. The events are subscribed to and resutls
 * awaited for some minutes and then the events are unsubscribed.

 * The test_latency_betn_hosts* () and test_latency_betn_pools* () use the same
 * source-destination measurement hosts + TOS and hence have the same
 * probeID. The test_latency_betn_flowendpoints* () uses flowtoken to find
 * latency. The TOS is different in this case, hence the probeID is different.
 * Since, the first time around, the probes are started and the test
 * has to wait for resuts, the subscription may take a minute to complete. 

 * IMPORTANT: It is required that before running this
 * test, the probe server and Imp software be running on the probe
 * hosts.  The scripts bin/runProbeServer.sh and bin/RunProbe.sh show how to
 * do this respectively, first start the probe server,then the probes.  
 * The nmconfig.prop file mentions the names of the probe
 * hosts. These hosts must also be populated into the BB Mysql DB as
 * part of the HostSubnet table.  The test also uses a <flowToken>
 * which is the flow record ID in the BB Mysql DB.  To populate the
 * BBMySQL DB for running this test, it is necessary to populate
 * db/nm-data.sql in addition to populating the other sql inputs such as 
 * arms-intidb.sql. */


public class NetworkMonitorEventClient extends NMEventConsumerPOA
{
    public String nmEventClient=null;
    private static ORB orb;
    private static POA poa;

    public static void main (String [] args)
    {
	NetworkMonitorEventClient nmClient = new NetworkMonitorEventClient ();
	NetworkMonitor netMon=null;
	org.omg.CORBA.Object obj =null;
	if( args.length < 3 ) 
	    {
		System.out.println( "Usage: java "+
				    " com.telcordia.research.BandwidthBroker.networkmonitor.client.NetworkMonitorEventClient "+
				    " <clientname> -ORBInitialHost <name service host> "+
				    "-ORBInitialPort <name service port>" );
		System.exit( 1 );
	    }
      
	String clientName = args[0];//"NetworkMonitorEventClient"; //default
      
	try 
	    {

		// initialize the ORB.
		orb = ORB.init( args, null );
	
		//initialize POA
		poa = 
		    POAHelper.narrow( orb.resolve_initial_references( "RootPOA" ));
		
		poa.the_POAManager().activate();

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
			System.err.println ("ERROR:"+
					    "Unknown Service");
		    }
		if (rootContext == null)
		    {
			System.err.println ("ERROR:"+
					    "Failed to resolve NameService");
			System.exit (1);
		    }
		else
		    {
			System.out.println ("Resolved NameService");
		    }

		// register as the push_condition servant
		// create the object reference
		obj =
		    poa.servant_to_reference(nmClient);
		 NameComponent myName [] = {
		     new NameComponent("mil", ""),
		     new NameComponent("darpa", ""),
		     new NameComponent("arms", ""),
		     new NameComponent("mlrm", ""),
		     new NameComponent("BandwidthBroker", ""),
		     new NameComponent(clientName, "")
		 };
		 
		 nsBind (myName, obj);
		
		 nmClient.nmEventClient = rootContext.to_string (myName);
		 System.out.println ("Registered"+nmClient.nmEventClient+" client IOR with Naming");
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
		netMon = NetworkMonitorHelper.narrow( obj );
	
	    }
	catch (Exception e)
	    {
		System.err.println ("ERROR:"+
				    "Exception in network monitor client setup execution!"+e.toString ()); 
		e.printStackTrace ();
		System.exit (-1);
	    }

//	System.out.println ("Testing EVENT SUBSCRIPTION for latency updates for multiple QoS requirements ...");
	
/*	nmClient.test_delay_betn_hosts_event_bundle (netMon);
	
	try{
	Thread.sleep (300000);
	} catch (Exception e)
	    {
		System.out.println ("Thread sleep interrupted in client execution");
		e.printStackTrace ();
		System.exit (-1);
	    }
	System.out.println ("Testing UNSUBSCIBE EVENT for latency updates for multiple QoS requirements ...");
	nmClient.test_delay_betn_hosts_event_bundle_unsubscribe (netMon);
*/	
	System.out.println ("\n\nTesting SUBSCRIBE EVENT for DELAY BETWEEN HOSTS with periodic updates ...");
	nmClient.test_delay_betn_hosts_periodic_event (netMon);
        System.out.println ("\n\nTesting SUBSCRIBE EVENT for DELAY BETWEEN FLOW ENDPOINTS with local threshold"); 
	nmClient.test_delay_betn_flowendpoints_local_threshold_event (netMon);
	System.out.println ("\n\nTesting SUBSCRIBE EVENT for DELAY BETWEEN POOLS with global threshold"); 
	nmClient.test_delay_betn_pools_global_threshold_event (netMon);
	
	try{
	Thread.sleep (300000);
	} catch (Exception ee)
	    {
		System.out.println ("Thread sleep interrupted in client execution");
		ee.printStackTrace ();
		System.exit (-1);
	    }
	
	
	System.out.println ("\n\nTesting UNSUBSCRIBE EVENT for DELAY BETWEEN HOSTS with periodic updates ...");
	nmClient.test_delay_betn_hosts_periodic_event_unsubscribe (netMon);
	System.out.println ("\n\nTesting UNSUBSCRIBE EVENT for DELAY BETWEEN FLOW ENDPOINTS with local threshold .."); 
	nmClient.test_delay_betn_flowendpoints_local_threshold_event_unsubscribe (netMon);
	System.out.println ("\n\nTesting UNSUBSCRIBE EVENT for DELAY BETWEEN POOLS with global threshold ..."); 
	nmClient.test_delay_betn_pools_global_threshold_event_unsubscribe (netMon);
	
	
    }

    private static NamingContext findOrMakeContext(NamingContext parent,
                                                   NameComponent cname)
        throws NotFound,
               CannotProceed,
               InvalidName
    {
        NameComponent[] name = { cname };
        try {
            parent.bind_new_context(name);
        } catch (AlreadyBound ab) {
            // ignore this error
        }
        org.omg.CORBA.Object ref = parent.resolve(name);
        return NamingContextHelper.narrow(ref);
    }

     private static void bind(NamingContext root,
                             NameComponent[] name,
                             org.omg.CORBA.Object ref)
        throws NotFound,
               CannotProceed,
               InvalidName,
               AlreadyBound
    {
        int context_length = name.length-1;
        NamingContext context = root;
        for (int i=0; i<context_length; i++) {
            context = findOrMakeContext(context, name[i]);
        }
        NameComponent[] tname = { name[context_length] };
        context.rebind(tname, ref);
    }

    public static void nsBind (NameComponent[] name,
			org.omg.CORBA.Object ref)
    {
        try {
            // Depends on -ORBInitRef having been set properly.
            org.omg.CORBA.Object o =
                orb.resolve_initial_references("NameService");
            NamingContextExt root = NamingContextExtHelper.narrow(o);
            bind(root, name, ref);
        } catch (Exception ex) 
        {
        }
    }

    public void test_delay_betn_hosts_periodic_event (NetworkMonitor netMon)
    {
	// Build request for latency betn hosts
	MeasurementEndpointsType type = MeasurementEndpointsType.latency_between_hosts;
	MeasurementEndpoints me = new MeasurementEndpoints ();
	//LatencyBetnHosts[] latencyBetnHosts = new LatencyBetnHosts [2];
	//LatencyPerQoS[] latencyPerQoSSeq = new LatencyPerQoS[2];
	LatencyBetnHosts[] latencyBetnHosts = new LatencyBetnHosts [1];
	LatencyPerQoS[] latencyPerQoSSeq = new LatencyPerQoS[1];
	Latency latency = new Latency (0, // delay will be populated in this field
				       190000000,//averaging window >=1, in microseconds
				       60000000, // measurementPeriod, in microseconds
				       0); // threshold
	latencyPerQoSSeq [0]= new LatencyPerQoS (latency, QOSRequired.highPriority);
	latencyBetnHosts[0] = new LatencyBetnHosts ("rondo-mon08", 
						    "rondo-mon09", 
						    latencyPerQoSSeq);
	//latencyPerQoSSeq [1]= new LatencyPerQoS (latency, QOSRequired.highReliability);
	//latencyBetnHosts[1] = new LatencyBetnHosts ("rondo-mon09", 
	//				    "rondo-mon08", 
	//					    latencyPerQoSSeq);
	me.hostsLatencyInfo (latencyBetnHosts);
	MeasurementEndpointsHolder latencyInfo = new MeasurementEndpointsHolder (me);
	// Name used to register with Naming
	String consumer = nmEventClient;
	String cookie = new String ("rondohosts12345");
	// periodic updates event
	NMEventConditionType nmconditiontype= NMEventConditionType.periodicLatency;
	NMEventCondition nmcondition = new NMEventCondition ();
	nmcondition.rp (60000000);// 1 min updates, in microseconds
	System.out.println ("Subscribing for delay measurement event between hosts rondo-mon08 and rondo-mon09"+
			    " with cookie:"+cookie+" for highPriority QoS flow"+
			    " launched with periodic updates per 60 seconds, measurement period of "+
			    "60 seconds and averaging window of 190 seconds.");
	
	try{
	    netMon.subscribeForLatencyEvent (consumer,
					     type,
					     latencyInfo,
					     nmconditiontype,
					     nmcondition,
					     cookie);
	} catch (NetworkMonitorEventSubscriptionException ne)
	    {
		System.out.println ("Subscription ERROR:"+ne.reason);
		ne.printStackTrace ();
		System.exit (-1);
	    }
	catch (NetworkProbesException ne2)
	    {
		System.out.println ("Subscription ERROR:"+ne2.reason);
		ne2.printStackTrace ();
		System.exit (-1);
	    }
	catch (Exception e)
	    {
		System.out.println ("Subscription ERROR:"+e.toString ());
		e.printStackTrace ();
		System.exit (-1);
	    }
	
	//System.out.println ("Delay measurement request event between hosts subscribed!");		
    }

    
    public void test_delay_betn_hosts_periodic_event_unsubscribe (NetworkMonitor netMon)
    {
	// Name used to register with Naming
	String consumer = nmEventClient;
	String cookie = new String ("rondohosts12345");
	System.out.println ("Unsubscribing  for events with cookie"+cookie+"...");
	try{
	    netMon.unsubscribeForLatencyEvent (consumer,
					     cookie);
	} catch (NetworkMonitorEventSubscriptionException ne)
	    {
		System.out.println ("Unsubscribe ERROR:"+ne.reason);
		ne.printStackTrace ();
		System.exit (-1);
	    }
	System.out.println ("Unsubscribed successfully!");		
    }
    
    public void test_delay_betn_pools_global_threshold_event (NetworkMonitor netMon)
    {
	// Build request for latency betn pools
	MeasurementEndpointsType type = MeasurementEndpointsType.latency_between_pools;
	MeasurementEndpoints me = new MeasurementEndpoints ();
	LatencyBetnPools[] latencyBetnPools = new LatencyBetnPools [1];
	LatencyPerQoS[] latencyPerQoSSeq = new LatencyPerQoS[1];
	Latency latency = new Latency (0, // delay will be populated in this field
				       60000000,//averaging window >=1, in microseconds
				       60000000, // measurementPeriod, in microseconds
				       0); // local threshold: 
	latencyPerQoSSeq [0]= new LatencyPerQoS (latency, QOSRequired.highPriority);
	latencyBetnPools[0] = new LatencyBetnPools ("Pool-1C", 
						    "Pool-1D", 
						    latencyPerQoSSeq);
	me.poolLatencyInfo (latencyBetnPools);
	MeasurementEndpointsHolder latencyInfo = new MeasurementEndpointsHolder (me);
	// Name used to register with Naming
	String consumer = nmEventClient;
	String cookie = new String ("mlrmpools6789");
	//  updates event if delay bound is passed
	NMEventConditionType nmconditiontype= NMEventConditionType.thresholdLatency;
	NMEventCondition nmcondition = new NMEventCondition ();
	nmcondition.tc (2000);// 2000 usecs delay bound (this is the
				   // global one)which will be
				   // overrided by the local one if
				   // local threshold >0
	System.out.println ("Susbcribing for delay measurement event between pools Pool-1C and Pool-1D"+
			    " with cookie:"+cookie+" for highPriority QoS flow"+
			    " launched with global threshold of 2000 microseconds, measurement period of "+
			    "60 seconds and averaging window of 60 seconds...");
	
	try{
	    netMon.subscribeForLatencyEvent (consumer,
					     type,
					     latencyInfo,
					     nmconditiontype,
					     nmcondition,
					     cookie);
	} catch (NetworkMonitorEventSubscriptionException ne)
	    {
		System.out.println ("Subscription ERROR:"+ne.reason);
		ne.printStackTrace ();
		System.exit (-1);
		
	    }
	catch (NetworkProbesException ne2)
	    {
		System.out.println ("Subscription ERROR:"+ne2.reason);
		ne2.printStackTrace ();
		System.exit (-1);
	    }
	catch (Exception e)
	    {
		System.out.println ("Subscription ERROR:"+e.toString ());
		e.printStackTrace ();
		System.exit (-1);
	    }
	//System.out.println ("Delay measurement request between pools global threshold event subscribed!");
	
    }
    public void test_delay_betn_pools_global_threshold_event_unsubscribe (NetworkMonitor netMon)
    {
	// Name used to register with Naming
	String consumer = nmEventClient;
	String cookie = new String ("mlrmpools6789");
	System.out.println ("Unsubscribing  for events with cookie"+cookie+"...");
	try{
	    netMon.unsubscribeForLatencyEvent (consumer,
					     cookie);
	} catch (NetworkMonitorEventSubscriptionException ne)
	    {
		System.out.println ("Unsubscribe ERROR:"+ne.reason);
		ne.printStackTrace ();
		System.exit (-1);
	    }
	System.out.println ("Unsubscribed successfully!");		
    }
    
    public void test_delay_betn_flowendpoints_local_threshold_event (NetworkMonitor netMon)
    {
	// Now for delay betn flow endpoints
	MeasurementEndpointsType type = MeasurementEndpointsType.latency_between_flow_endpoints;
	MeasurementEndpoints me = new MeasurementEndpoints ();
	LatencyBetnFlowEndpoints[] latencyBetnFlowEndpoints = new LatencyBetnFlowEndpoints [1];
	LatencyPerQoS[] latencyPerQoSSeq = new LatencyPerQoS[1];
	Latency latency = new Latency (0, // delay will be populated in this field
				       190000000,//averaging window >=1, in microseconds
				       60000000, // measurementPeriod, in microseconds
				       2000); // local threshold: 2000 , in microseconds
	latencyPerQoSSeq [0]= new LatencyPerQoS (latency, QOSRequired.highReliability);

	// Note: this is something that the user may have to change before running this test.
	String flowToken = "1";
	latencyBetnFlowEndpoints[0] = new LatencyBetnFlowEndpoints (flowToken,
								    latencyPerQoSSeq);
	me.flowLatencyInfo (latencyBetnFlowEndpoints);
	MeasurementEndpointsHolder latencyInfo = new MeasurementEndpointsHolder (me);
	// Name used to register with Naming
	String consumer = nmEventClient;
	String cookie = new String ("flowendpts11");
	//  updates event if delay bound is passed
	NMEventConditionType nmconditiontype= NMEventConditionType.thresholdLatency;
	NMEventCondition nmcondition = new NMEventCondition ();
	nmcondition.tc (120000000);// 2 min delay bound (this is the
				   // global one)which will be
				   // overrided by the local one if
				   // local threshold >0
	
	System.out.println ("Subscribing for delay measurement event between flow endpoints with flowToken of 1"+
			    " with cookie:"+cookie+" for highReliability QoS flow"+
			    " launched with local threshold of 2000 microseconds, global threshold of 120 seconds"+
			    " (local threshold overrides global threshold), measurement period of 60 seconds "+
			    "and averaging window of 190 seconds.");
	

	try{
	    netMon.subscribeForLatencyEvent (consumer,
					     type,
					     latencyInfo,
					     nmconditiontype,
					     nmcondition,
					     cookie);
	} catch (NetworkMonitorEventSubscriptionException ne)
	    {
		System.out.println ("Subscription ERROR:"+ne.reason);
		ne.printStackTrace ();
		//System.exit (-1);
		System.exit (-1) ;
	    }
	catch (NetworkProbesException ne2)
	    {
		System.out.println ("Subscription ERROR:"+ne2.reason);
		ne2.printStackTrace ();
		System.exit (-1);
	    }
	catch (Exception e)
	    {
		System.out.println ("Subscription ERROR:"+e.toString ());
		e.printStackTrace ();
		System.exit (-1);
	    }
	//	System.out.println ("Delay measurement request event between flowendpts subscribed!");		
    }
    
    public void test_delay_betn_flowendpoints_local_threshold_event_unsubscribe (NetworkMonitor netMon)
    {
	// Name used to register with Naming
	String consumer = nmEventClient;
	String cookie = new String ("flowendpts11");
	System.out.println ("Unsubscribing  for events with cookie"+cookie+"...");
	try{
	    netMon.unsubscribeForLatencyEvent (consumer,
					       cookie);
	} catch (NetworkMonitorEventSubscriptionException ne)
	    {
		System.out.println ("Unsubscribe ERROR:"+ne.reason);
		ne.printStackTrace ();
		System.exit (-1);
	    }
	System.out.println ("Unsubscribed successfully!");		
    }

    /* Test events registered as a bundle */
    public void test_delay_betn_hosts_event_bundle (NetworkMonitor netMon)
    {
	// Build request for latency betn hosts
	MeasurementEndpointsType type = MeasurementEndpointsType.latency_between_hosts;
	MeasurementEndpoints me = new MeasurementEndpoints ();
	LatencyBetnHosts[] latencyBetnHosts = new LatencyBetnHosts [1];
	LatencyPerQoS[] latencyPerQoSSeq = new LatencyPerQoS[4];
	Latency latency = new Latency (0, // delay will be populated in this field
				       190000000,//averaging window >=1, in microseconds
				       60000000, // measurementPeriod, in microseconds
				       0); // threshold
	latencyPerQoSSeq [0]= new LatencyPerQoS (latency, QOSRequired.highPriority);
	latencyPerQoSSeq [1]= new LatencyPerQoS (latency, QOSRequired.highReliability);
	latencyPerQoSSeq [2]= new LatencyPerQoS (latency, QOSRequired.highThroughput);
       	latencyPerQoSSeq [3]= new LatencyPerQoS (latency, QOSRequired.bestEffort);
	latencyBetnHosts[0] = new LatencyBetnHosts ("rondo-mon09", 
						    "rondo-mon08", 
						    latencyPerQoSSeq);
	me.hostsLatencyInfo (latencyBetnHosts);
	MeasurementEndpointsHolder latencyInfo = new MeasurementEndpointsHolder (me);
	// Name used to register with Naming
	String consumer = nmEventClient;
	String cookie = new String ("rondohostsABCD");
	// periodic updates event
	NMEventConditionType nmconditiontype= NMEventConditionType.periodicLatency;
	NMEventCondition nmcondition = new NMEventCondition ();
	nmcondition.rp (60000000);// 1 min updates, in microseconds

	System.out.println (" Subscribing for latency event between hosts rondo-mon09 "+
			    "and rondo-mon08 with cookie:"+cookie+
			    " for HighPriority, HighReliability, HighThroughput, "+
			    "BestEffort QoS with 60 seconds periodic updates, measurement period"+
			    " of 60 seconds and averagiung window of 190 seconds.");
	try{
	    netMon.subscribeForLatencyEvent (consumer,
					     type,
					     latencyInfo,
					     nmconditiontype,
					     nmcondition,
					     cookie);
	} catch (NetworkMonitorEventSubscriptionException ne)
	    {
		System.out.println ("Susbcription ERROR:"+ne.reason);
		ne.printStackTrace ();
		System.exit (-1);
	    }
	catch (NetworkProbesException ne2)
	    {
		System.out.println ("Susbcription ERROR:"+ne2.reason);
		ne2.printStackTrace ();
		System.exit (-1);
	    }
	catch (Exception e)
	    {
		System.out.println ("Susbcription ERROR:"+e.toString ());
		e.printStackTrace ();
		System.exit (-1);
	    }
	
	//System.out.println ("Delay measurement (Qos: HighPriority, HighReliability, HighThroughput, BestEffort)"+
	//		    " request event bundle between hosts subscribed!");		
    }

    public void test_delay_betn_hosts_event_bundle_unsubscribe (NetworkMonitor netMon)
    {
	// Name used to register with Naming
	String consumer = nmEventClient;
	String cookie = new String ("rondohostsABCD");
	System.out.println ("Unsubscribe for events with cookie:"+cookie+
			    " for delay measurements with Qos: HighPriority, HighReliability, "+
			    "HighThroughput, BestEffort");

	try{
	    netMon.unsubscribeForLatencyEvent (consumer,
					       cookie);
	} catch (NetworkMonitorEventSubscriptionException ne)
	    {
		System.out.println ("Unsubscription ERROR:"+ne.reason);
		ne.printStackTrace ();
		System.exit (-1);
	    }
	System.out.println ("Unsubscribed successfully!");
    }
    
    public String getQoSStr (QOSRequired q)
    {
	// get approperiate mapping to QOSRequired enum values
	String tos = new String ("BestEffort");
	switch (q.value())
	  {
	  case QOSRequired._highReliability :
	    {
	      // With higher WFQ size
	      tos = "HighReliability";
	      break;
	    }
	  case QOSRequired._highPriority :
	    {
	      // low latency, low priority queue size
	      tos = "HighPriority";
	      break;
	    }
	  case QOSRequired._highThroughput :
	    {
	      // raw physical bw class
	      tos = "HighThroughput";
	      break;
	    }
	  default:
	    tos = new String ("BestEffort");
	  }
	return tos;
    }

    public void push_NMEvent (NMEvent the_nmevent)
    {
	String cookie = the_nmevent.cookie;
	MeasurementEndpointsType metype = the_nmevent.type;
	MeasurementEndpoints latencyInfo = the_nmevent.latencyInfo;
	long delay = 0;
	String qos = new String ("qos");
	switch (metype.value ())
	    {
	    
	    case 0:  // latency betn flow endpoints
		{
		    
		    LatencyBetnFlowEndpoints[] latencyBetnFlowEndpointsSeq = 
			latencyInfo.flowLatencyInfo ();
		    LatencyBetnFlowEndpoints latencyStruct = latencyBetnFlowEndpointsSeq [0];
		    LatencyPerQoS latencyPerClass = latencyStruct.latencyPerClassSeq[0];
		    Latency latency = latencyPerClass.availableLatency;
		    QOSRequired q = latencyPerClass.qos;
		    qos = getQoSStr (q);
		    delay = latency.delayTime;
		    break;
		}
	    case 1: // latency betn host endpoints
		{
		    LatencyBetnHosts[] latencyBetnHostsSeq =
			latencyInfo.hostsLatencyInfo ();
		    LatencyBetnHosts latencyStruct = latencyBetnHostsSeq [0];
		    LatencyPerQoS latencyPerClass = latencyStruct.latencyPerClassSeq[0];
		    Latency latency = latencyPerClass.availableLatency;
		    QOSRequired q = latencyPerClass.qos;
		    qos = getQoSStr (q);
		    delay = latency.delayTime;
		    break;
		}
	    case 2: // latency betn pool endpoints
		{
		    LatencyBetnPools[] latencyBetnPoolsSeq = 
			latencyInfo.poolLatencyInfo ();
		    LatencyBetnPools latencyStruct = latencyBetnPoolsSeq [0];
		    LatencyPerQoS latencyPerClass = latencyStruct.latencyPerClassSeq[0];
		    Latency latency = latencyPerClass.availableLatency;
		    QOSRequired q = latencyPerClass.qos;
		    qos = getQoSStr (q);
		    delay = latency.delayTime;
		    break;
		}
	    }
	System.out.println ("\nReceived latency event with cookie="+cookie+" , QOS="+qos+
			    " with delay="+new Long (delay).toString ()+ " microseconds");
	
    }
}
