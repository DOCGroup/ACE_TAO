
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

package mil.darpa.arms.mlrm.BandwidthBroker.networkmonitor.server;
import mil.darpa.arms.mlrm.BandwidthBroker.networkmonitor.server.db.*;
import mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.*;
import mil.darpa.arms.mlrm.BandwidthBroker.bb.server.*;
import javax.xml.transform.stream.StreamSource;
import javax.xml.bind.Marshaller;
import javax.xml.bind.Unmarshaller;
import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBException;
import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import mil.darpa.arms.mlrm.BandwidthBroker.*;
import mil.darpa.arms.mlrm.BandwidthBroker.NetworkMonitorPackage.*;
import mil.darpa.arms.mlrm.BandwidthBroker.ProbeControlPackage.*;
import mil.darpa.arms.mlrm.BandwidthBroker.CommonDef.*;
import java.sql.*;
import org.omg.CosNaming.*;
import org.omg.CosNaming.NamingContextPackage.*;
import java.io.*;
import java.util.*;
import java.lang.*;
import java.net.*;
import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;

public class NetworkMonitorImpl extends NetworkMonitorPOA
   implements NetworkMonitorCallback
{
    NMDB nmdb = null;
    ProbeControl probeControl = null;
    // How often to perform garbage collection
    long dbGarbageCollectionTimeout = 900000; // in milliseconds
    // How long entries can stay before being purged
    long dbStayTimePeriod = 1800000; // in milliseconds
    boolean useProbes = true;
    String probeServer="impdb";
    String probeSink = "science"; // should be local host as sink
				  // accesses the incore db
    ProbeControlImpl pcImpl = null;
    String bbConfigFile = new String ("."+System.getProperty ("file.separator") + "cfg"+ 
				      System.getProperty ("file.separator") + 
				      "BandwidthBroker" + 
				      System.getProperty ("file.separator") + 
				      "bbconfig.prop");
	    
    String dbServer = new String ("quasar.research.telcordia.com");
    String dbLogin = new String ("arms");
    String dbPassword = new String ("arms");
    int numberOfDBConnections = 4;
    NRDB nrdb=null;
    public Map measurementHostsTable = null;
    public Map eventMonitorThreadTable = null;
    public Logger nmImplLogger=null;
    String [] _args;
    /**
       Create the Network Monitor servant with properties denoting the
       time for garbage collection for the in-core DB as well as
       whether probes should be used, the names of the probe server
       and sink.
       Args could be used later for PRobeControl initialization
   */
    public NetworkMonitorImpl (String nmConfigFile, String [] args)
	throws NetworkProbesException
    {
	_args=args;
	nmImplLogger = NetworkMonitorServer.nmlogger;
	measurementHostsTable = Collections.synchronizedMap(new HashMap ());
	eventMonitorThreadTable = Collections.synchronizedMap(new HashMap());
	try {
	    setNMProperties (nmConfigFile);
	    
	} catch (Exception ee)
	    {
		throw new NetworkProbesException (ee.toString (), false);
	    }
	NetworkMonitorServer.nmlogger.info ("Finished reading nmconfig.prop");
	try {
	    nmdb = new NMDB ();
	    // @todo:
	    // start the evictor of incore db here in a separet thread
	    startEvictor ();
	    
	}
	catch (CDBException e)
	    {
		NetworkMonitorServer.nmlogger.warn ("Error initiating incore network monitor DB");
		throw new NetworkProbesException (e.toString (), false);
	    }
	NetworkMonitorServer.nmlogger.info ("Initiated incore network monitor DB");
	// Get the collocated probe control constructed, the CORBA way
	// But for now, lets just call the probe control w.o. corba.
	// The incore DB is shared by Network Monitor
	// and PRobe Control

	if (useProbes)
	    {
		NetworkMonitorServer.nmlogger.info ("Instantiating Probes");
		try {
		    pcImpl = new ProbeControlImpl (probeServer, 
						   probeSink); 
		}
		catch (ProbeControlException pe)
		    {
			NetworkMonitorServer.nmlogger.warn ("Error instantiating probes");
			throw new NetworkProbesException (pe.toString (), true);

		    }
	    }
	// Get access to the BandwidthBroker DB
	setBBProperties (bbConfigFile);
	// Since NetMon needs more than one connection, setting it constant to 4 even if the BB doesnt need 4 @@ Kirthika
        if (numberOfDBConnections <= 4)	
  	  NRDB.numConnections = 4; //numberOfDBConnections;
	else
	  NRDB.numConnections = numberOfDBConnections;
	
	NRDB.dbServerFQN = dbServer;
	NRDB.dbLoginName= dbLogin;
	NRDB.dbPasswordValue = dbPassword;
	NetworkMonitorServer.nmlogger.info ("Got BB DB properties, num of connections="+
					    new Integer (NRDB.numConnections).toString ());
	// TODO:MAke sure the BB- NetworkMonitor is fault-tolerant by initating monitor for registered events in BB DB
		
	// Instantiate when needed
	nrdb=null;
	
    }

    private void startEvictor ()
	throws CDBException
    {
	// Start the Evictor Timer as a daemon thread to run every
	// dbGarbageCollectionTimeout milliseconds. The daemon thread lets the
	// application exit even if the daemon threads havent*/
	
	Timer timer = new Timer (true);
	
	timer.schedule (new NMDBEvictor (nmdb, dbStayTimePeriod), 
			0, 
			dbGarbageCollectionTimeout);
    }

    /** Get network performace measurement such as latency (in usecs), jitter,
     * loss etc. Currently it only returns latency */
    public void get_measurement(MeasurementEndpointsType type, 
				MeasurementEndpointsHolder latencyInfo) 
	throws NetworkProbesException
    {

	int meType = type.value ();
	if (meType == -1)
	    {
		NetworkMonitorServer.nmlogger.warn ("get_measurement type error");
		throw new NetworkProbesException ("measurement type not specified", true);
	    }

	switch (meType)
	    {
	    
	    case 0:  // latency betn flow endpoints
		{
		    // Get Flow ID and appropriate service class from
		    // BB DB? Best to just use the QoSRequired in
		    // LatencyQoS for this. But src and dest need to
		    // be obtained from the flowID 
		    NetworkMonitorServer.nmlogger.info ("get_measurement between flow endpoints");
		    getLatencyBetnFlowEndpoints (latencyInfo);
		    break;
		}
	    case 1: // latency betn host endpoints
		{
		    NetworkMonitorServer.nmlogger.info ("get_measurement between hosts");
		    getLatencyBetnHosts (latencyInfo);
		    break;
		}
	    case 2: // latency betn pool endpoints
		{
		    NetworkMonitorServer.nmlogger.info ("get_measurement between pools");
		    getLatencyBetnPoolHosts (latencyInfo);
		    break;
		}
	    }
	
    }


    public void subscribeForLatencyEvent(String consumerComponentName, 
					 MeasurementEndpointsType endpointType, 
					 MeasurementEndpointsHolder latencyInfo, 
					 NMEventConditionType conditionType,
					 NMEventCondition condition, 
					 String cookie) 
	throws NetworkMonitorEventSubscriptionException,NetworkProbesException
    {
	String probeId=null;
	int meType = endpointType.value ();
	if (meType == -1)
	    {
		NetworkMonitorServer.nmlogger.warn ("subscribe for latency event: type error");
		throw new NetworkMonitorEventSubscriptionException ("subscribe for latency event:type not specified");
	    }
	try{
	switch (meType)
	    {
	    
	    case 0:  // latency betn flow endpoints
		{
		    // Get Flow ID and appropriate service class from
		    // BB DB? Best to just use the QoSRequired in
		    // LatencyQoS for this. But src and dest need to
		    // be obtained from the flowID 
		    NetworkMonitorServer.nmlogger.info ("subscribe for measurement between flow endpoints");
		    subscribeForLatencyBetnFlowEndpoints (consumerComponentName,
							  latencyInfo,
							  conditionType,
							  condition,
							  cookie); 
		
		    break;
		}
	    case 1: // latency betn host endpoints
		{
		    NetworkMonitorServer.nmlogger.info ("subscribe for measurement between hosts");
		    subscribeForLatencyBetnHosts (consumerComponentName,
						  latencyInfo,
						  conditionType,
						  condition,
						  cookie); 
		    break;
		}
	    case 2: // latency betn pool endpoints
		{
		    NetworkMonitorServer.nmlogger.info ("subscribe for measurement between pools");
		    subscribeForLatencyBetnPoolHosts (consumerComponentName,
						      latencyInfo,
						      conditionType,
						      condition,
						      cookie);
		    break;
		}
	    }
	}catch (NetworkProbesException ne)
	    {
		NetworkMonitorServer.nmlogger.warn ("subscribe for measurement exception: "+ne.reason);
		ne.printStackTrace ();
		// unsubscribe
		try{
		unsubscribeForLatencyEvent(consumerComponentName, 
					   cookie);
		}catch (Exception e)
		    {
			NetworkMonitorServer.nmlogger.warn ("unsubscribe for measurement exception for consumer:"+
							    consumerComponentName+" with cookie:"+cookie);	
		    }
		throw new NetworkMonitorEventSubscriptionException (ne.reason );

	    }
	catch (Exception ee)
	    {
		NetworkMonitorServer.nmlogger.warn ("subscribe for measurement exception: "+ee.toString ());
		ee.printStackTrace ();
		try{
		    unsubscribeForLatencyEvent(consumerComponentName, 
					       cookie);
		}catch (Exception e)
		    {	NetworkMonitorServer.nmlogger.warn ("unsubscribe for measurement exception for consumer:"+
							    consumerComponentName+" with cookie:"+cookie);	
		    }
		throw new NetworkMonitorEventSubscriptionException (ee.toString () );

	    }
    }

    public void unsubscribeForLatencyEvent(String consumerComponentName, 
					   String cookie) 
	throws NetworkMonitorEventSubscriptionException
    {
	try{
	    // get connection to BB DB
	    nrdb = new NRDB ();
		} catch (CDBException ce)
		    {
			NetworkMonitorServer.nmlogger.warn ("unsubcribe for measurement event:"+
							    " error connecting to BB DB");
			throw new NetworkMonitorEventSubscriptionException ("UnSubscribeForLatency:Error connecting"+
									    " to BandwidthBroker Database!");
			
	    }

	// Stop monitoring thread by getting the appropriate threadname and setting its status to false in the 
	// <eventMonitorThreadTable> synchronized hashmap
	try {
	ObjectList ol = nrdb.findNetworkMonitorEventSubscriptions (consumerComponentName,
								   cookie);
	
	AttributeValues av=null;
	for (int i=0;i< ol.size (); i++)
	    {
		// Get the record
		av = ol.get (i).getAttributes ();
		String monitorName= av.getValueForName (NetworkMonitorEventSubscriptions.attrEventSupplierMonitor);

		//Thread m = (Thread)eventMonitorThreadTable.get ((String)monitorName);
		//m=null; 
		EventSupplierMonitor em = (EventSupplierMonitor)eventMonitorThreadTable.get ((String)monitorName);
		if (em !=null)
		    em.stop (); // safe way to stop thread

		NetworkMonitorServer.nmlogger.info ("unsubcribe for measurement event:"+
						    " Resetting monitor thread:"+monitorName+ " to stop running");
	    }

	// Remove probe-consumer info from BB DB
	nrdb.removeNetworkMonitorEventSubscriptions (consumerComponentName,
						     cookie);
	nrdb.commit ();
	NetworkMonitorServer.nmlogger.info ("unsubcribe for measurement event:"+
					    "Removing all records for consumer:"+consumerComponentName+
					    " with cookie:"+cookie+
					    " from BB DB");
       
	
	
	} catch (CDBException ee)
	    {
		ee.printStackTrace ();
		NetworkMonitorServer.nmlogger.warn ("unsubcribe for measurement event:"+
						    "Error in using BandwidthBroker Database connection");
		throw new NetworkMonitorEventSubscriptionException  ("Error in using BandwidthBroker"+
								     " Database connection");
	    }
	catch (Exception ex)
	    {
		ex.printStackTrace ();
		NetworkMonitorServer.nmlogger.warn ("unsubcribe for measurement event:"+
						    "Error in using BandwidthBroker Database connection:"+ex.toString ());
		throw new NetworkMonitorEventSubscriptionException  ("Error in using BandwidthBroker"+
								     " Database connection");
	    }
	finally {
	    try{
	    if (nrdb != null)
	    nrdb.releaseConnection ();
	    }catch (CDBException cee) 
	    {
		NetworkMonitorServer.nmlogger.warn ("unsubcribe for measurement event:"+
						    "Error in releasing BandwidthBroker Database connection");
		throw new NetworkMonitorEventSubscriptionException  ("unsubcribe for measurement event:"+
								     "Error in releasing BandwidthBroker"+
								     " Database connection");
	    }
	}
	// Stop probe if none else using it
    }

    public void subscribeForLatencyBetnFlowEndpoints (String consumerComponentName, 
						      MeasurementEndpointsHolder latencyInfo, 
						      NMEventConditionType conditionType,
						      NMEventCondition condition, 
						      String cookie)
	throws NetworkProbesException
    {
	LatencyBetnFlowEndpoints[] latencyBetnFlowEndpointsSeq = latencyInfo.value.flowLatencyInfo ();
	LatencyBetnFlowEndpoints latencyStruct = null;
	LatencyPerQoS latencyPerClass = null;
	Latency latency = null;
	String source = null;
	String destination = null;
	long measurementPeriod = 60000000;
	long averagingWindow = 1;
	QOSRequired serviceClass;
	String flowToken = null;
	long localThresholdLatency = 0;

	try{
	// get connection to BB DB
	nrdb = new NRDB ();
	} catch (CDBException ce)
	    {
		NetworkMonitorServer.nmlogger.warn ("subcribe for measurement between flow endpoints:"+
						    " error connecting to BB DB");
		throw new NetworkProbesException ("SubscribeForLatencyBetnFlowEndpoints ():Error connecting"+
						  " to BandwidthBroker Database!", false);
		
	    }
	NetworkMonitorServer.nmlogger.info ("BB DB access obtained");
	String ssubnet=null;
	String dsubnet=null;
	for (int i=0;i<latencyBetnFlowEndpointsSeq.length;i++)
	    {
		latencyStruct = latencyBetnFlowEndpointsSeq [i];
		flowToken = latencyStruct.flowToken;
		
		// Look up BB DB for flowrecord with flowToken and
		// obtain the src and dest IPs.
		try{
		FlowRecord fr= nrdb.findFlowRecord (flowToken);
		AttributeValues av = fr.getAttributes ();
	        ssubnet = getSubnetAddr (av.getValueForName (FlowRecord.attrSrcIP), 
					 av.getValueForName (FlowRecord.attrSrcMask));
		NetworkMonitorServer.nmlogger.info ("subscribe for measurement between flow endpoints:"+
						    " source subnet addr="+ssubnet);
		source = (String)measurementHostsTable.get((String) ssubnet);
		NetworkMonitorServer.nmlogger.info ("subscribe for measurement between flow endpoints:"+
						    " source ="+source);
		dsubnet = getSubnetAddr (av.getValueForName (FlowRecord.attrDestIP), 
					 av.getValueForName (FlowRecord.attrDestMask));
		NetworkMonitorServer.nmlogger.info ("subscribe for measurement between flow endpoints:"+
						    " destination subnet addr="+dsubnet);
		destination = (String)measurementHostsTable.get((String) dsubnet);
		NetworkMonitorServer.nmlogger.info ("subscribe for measurement between flow endpoints:"+
						    " destination ="+destination);
		if (source == null || destination == null)
		    throw new CDBException ();
		}catch (CDBException cee)
		    {
			try{
			if (nrdb != null)
			    nrdb.releaseConnection ();
			}catch (CDBException eec)
			{
			    NetworkMonitorServer.nmlogger.warn ("subscribeForLatencyBetnFlowEndpoints ():"+
								"Error in releasing BandwidthBroker Database connection");
			    eec.printStackTrace ();
			}
			NetworkMonitorServer.nmlogger.warn ("subscribeForLatencyBetnFlowEndpoints ():Error"+
							  " obtaining flow record:"+ flowToken+
							    " from BandwidthBroker Database!");
			throw new NetworkProbesException ("subscribeForLatencyBetnFlowEndpoints ():Error"+
							  " obtaining flow record:"+ flowToken+
							  " from BandwidthBroker Database!", false);
			
		    }
	
		for (int j=0;j < latencyStruct.latencyPerClassSeq.length; j++)
		    {
			latencyPerClass = latencyStruct.latencyPerClassSeq[j];
			latency = latencyPerClass.availableLatency;
			measurementPeriod = latency.measurementPeriod;
			averagingWindow = latency.averagingWindow;
			// threshold time only needed for events
			localThresholdLatency = latency.thresholdTime;

			// set default delay to be 0
			latency.delayTime = 0;

			// Now get the service class
			serviceClass = latencyPerClass.qos;

			ProbeParametersHolder defaults = new ProbeParametersHolder ();
		
			// Build the event to be sent by the EventSupplierMonitor to the consumer
			// It should be of the type subscribed for by the consumer
			MeasurementEndpointsType typeForEvent = MeasurementEndpointsType.latency_between_flow_endpoints;
			MeasurementEndpoints meForEvent = new MeasurementEndpoints ();
			LatencyBetnFlowEndpoints[] latencyBetnFlowEndpointsForEvent = new LatencyBetnFlowEndpoints [1];
			LatencyPerQoS[] latencyPerQoSSeqForEvent = new LatencyPerQoS[1];
			Latency latencyForEvent = new Latency (0, // delay will be populated in this field
							       averagingWindow,
							       measurementPeriod,
							       localThresholdLatency);
			latencyPerQoSSeqForEvent [0]= new LatencyPerQoS (latencyForEvent, serviceClass);
			latencyBetnFlowEndpointsForEvent [0] = new LatencyBetnFlowEndpoints (flowToken,
											     latencyPerQoSSeqForEvent);
			meForEvent.flowLatencyInfo (latencyBetnFlowEndpointsForEvent);
			MeasurementEndpointsHolder latencyInfoForEvent = new MeasurementEndpointsHolder (meForEvent);
			if (useProbes)
			    {
				try {
				    startEventSupplierMonitor (consumerComponentName, 
							       conditionType,
							       condition, 
							       cookie,
							       source,
							       destination,
							       serviceClass,
							       measurementPeriod,
							       averagingWindow,
							       defaults,
							       localThresholdLatency,
							       latencyInfoForEvent,
							       typeForEvent);
				    
				} catch (NetworkProbesException ne)
				    {
					//	ne.printStackTrace ();
					if (ne.probeConfigurationError=true)
					    {
						// return default so
						// end user knows why
						// the error was
						// potentially caused
						latency.measurementPeriod = defaults.value.measurementPeriod;
						NetworkMonitorServer.nmlogger.warn ("Default measurement period"+
										    " violated, hence returning it:"+
										    defaults.value.measurementPeriod);
					    }
					NetworkMonitorServer.nmlogger.warn (ne.reason);
					throw new NetworkProbesException (ne.reason, 
									  true);
				    }
			    }// end of if
			
		    }// end of for j
	    } // end of for i
	
       try{
	   if (nrdb != null)
	       nrdb.releaseConnection ();
       } catch (CDBException ee)
	   {
	       NetworkMonitorServer.nmlogger.warn ("subscribeForLatencyBetnFlowEndpoints ():"+
						   "Error in releasing BandwidthBroker Database connection");
	       throw new NetworkProbesException ("subscribeForLatencyBetnFlowEndpoints ():"+
						 "Error in releasing BandwidthBroker Database connection", 
						 false);
	   }
	

    }

    public void subscribeForLatencyBetnPoolHosts (String consumerComponentName, 
						  MeasurementEndpointsHolder latencyInfo, 
						  NMEventConditionType conditionType,
						  NMEventCondition condition, 
						  String cookie)
	throws NetworkProbesException
    {
	LatencyBetnPools[] latencyBetnPoolsSeq = latencyInfo.value.poolLatencyInfo ();
	LatencyBetnPools latencyStruct = null;
	LatencyPerQoS latencyPerClass = null;
	Latency latency = null;
	String source = null;
	String destination = null;
	long measurementPeriod = 60000000;
	long averagingWindow = 1;
	QOSRequired serviceClass;
	String flowToken = null;	
	String srcPoolName = null;
	String destPoolName = null;
	long localThresholdLatency = 0;

	try{
	// get connection to BB DB
	nrdb = new NRDB ();
	} catch (CDBException ce)
	    {
		NetworkMonitorServer.nmlogger.warn ("subscribeForLatencyBetnPools ():Error connecting"+
						    " to BandwidthBroker Database!");
		throw new NetworkProbesException ("subscribeForLatencyBetnPools ():Error connecting"+
						  " to BandwidthBroker Database!", false);
	    }
	NetworkMonitorServer.nmlogger.info ("BB DB access obtained");
	for (int i=0;i<latencyBetnPoolsSeq.length;i++)
	    {
		latencyStruct = latencyBetnPoolsSeq [i];
	
		srcPoolName = latencyStruct.srcPoolName;
		destPoolName = latencyStruct.destPoolName;
		// Look up BB DB for pool record 
		// obtain the src and dest hostnames.
		try {
		source = (String) measurementHostsTable.get ((String)nrdb.findSubnetInPool (srcPoolName));
		destination = (String) measurementHostsTable.get ((String)nrdb.findSubnetInPool (destPoolName));
		if (source == null || destination == null)
		    {
			throw new CDBException ();
		    }
		}catch (CDBException cee)
		    {
			NetworkMonitorServer.nmlogger.warn ("subscribeForLatencyBetnPoolHosts ():Error"+
							  " obtaining pool measurement hosts for pools "+srcPoolName+
							  " and "+destPoolName+" respectively "+
							    " from BandwidthBroker Database!");
			try{
			    if (nrdb != null)
				nrdb.releaseConnection ();
			} catch (CDBException ee)
			    {
				NetworkMonitorServer.nmlogger.warn ("subscribeForLatencyBetnPoolEndpoints ():"+
								    "Error in releasing BandwidthBroker Database connection");
				ee.printStackTrace ();
			    }
			throw new NetworkProbesException ("subscribeForLatencyBetnPoolHosts ():Error"+
							  " obtaining pool measurement hosts for pools "+srcPoolName+
							  " and "+destPoolName+" respectively "+
							  " from BandwidthBroker Database!", false);
		    }
	
		NetworkMonitorServer.nmlogger.info ("source and destination measurement hosts for pools "+
						    srcPoolName+" and "+destPoolName+" respectively are "+
						    source+" "+destination);

		
		for (int j=0;j < latencyStruct.latencyPerClassSeq.length; j++)
		    {
			latencyPerClass = latencyStruct.latencyPerClassSeq[j];
			latency = latencyPerClass.availableLatency;
			measurementPeriod = latency.measurementPeriod;
			averagingWindow = latency.averagingWindow;
			// threshold time only needed for events
			localThresholdLatency = latency.thresholdTime;
			// set default delay to be 0
			latency.delayTime = 0;

			// Now get the service class
			serviceClass = latencyPerClass.qos;

			ProbeParametersHolder defaults = new ProbeParametersHolder ();
		
			// Build the event to be sent by the EventSupplierMonitor to the consumer
			// It should be of the type subscribed for by the consumer
			MeasurementEndpointsType typeForEvent = MeasurementEndpointsType.latency_between_pools;
			MeasurementEndpoints meForEvent = new MeasurementEndpoints ();
			LatencyBetnPools[] latencyBetnPoolsForEvent = new LatencyBetnPools [1];
			LatencyPerQoS[] latencyPerQoSSeqForEvent = new LatencyPerQoS[1];
			Latency latencyForEvent = new Latency (0, // delay will be populated in this field
							       averagingWindow,
							       measurementPeriod,
							       localThresholdLatency);
			latencyPerQoSSeqForEvent [0]= new LatencyPerQoS (latencyForEvent, serviceClass);
			latencyBetnPoolsForEvent [0] = new LatencyBetnPools (srcPoolName,
									     destPoolName,
									     latencyPerQoSSeqForEvent);
			meForEvent.poolLatencyInfo (latencyBetnPoolsForEvent);
			MeasurementEndpointsHolder latencyInfoForEvent = new MeasurementEndpointsHolder (meForEvent);
			
			if (useProbes)
			    {
				try {
				     startEventSupplierMonitor (consumerComponentName, 
							       conditionType,
							       condition, 
							       cookie,
							       source,
							       destination,
							       serviceClass,
							       measurementPeriod,
							       averagingWindow,
							       defaults,
								localThresholdLatency,
							       latencyInfoForEvent,
							       typeForEvent);
				    
				} catch (NetworkProbesException ne)
				    {
					//ne.printStackTrace ();
					if (ne.probeConfigurationError=true)
					    {
						// return default so
						// end user knows why
						// the error was
						// potentially caused
						latency.measurementPeriod = defaults.value.measurementPeriod;
						NetworkMonitorServer.nmlogger.warn ("Default measurement period"+
										    " violated, hence returning it:"+
										    defaults.value.measurementPeriod);
					    }
					NetworkMonitorServer.nmlogger.warn (ne.reason);
					throw new NetworkProbesException (ne.reason, 
									  true);
				    }
			    }// end of if
			
		    }// end of for j
	    } // end of for i
	try{
	    if (nrdb != null)
		nrdb.releaseConnection ();
	} catch (CDBException ee)
	    {
		NetworkMonitorServer.nmlogger.warn ("subscribeForLatencyBetnPoolEndpoints ():"+
						    "Error in releasing BandwidthBroker Database connection");
		throw new NetworkProbesException ("subscribeForLatencyBetnPoolEndpoints ():"+
						  "Error in releasing BandwidthBroker Database connection", 
						  false);
	    }
	
    }
    
    public void subscribeForLatencyBetnHosts (String consumerComponentName, 
						  MeasurementEndpointsHolder latencyInfo, 
						  NMEventConditionType conditionType,
						  NMEventCondition condition, 
						  String cookie)
	throws NetworkProbesException
    {
	LatencyBetnHosts[] latencyBetnHostsSeq = latencyInfo.value.hostsLatencyInfo ();
	LatencyBetnHosts latencyStruct = null;
	LatencyPerQoS latencyPerClass = null;
	Latency latency = null;
	String source = null;
	String destination = null;
	long measurementPeriod = 60000000;
	long averagingWindow = 1;
	QOSRequired serviceClass;
	long localThresholdLatency=0;

	try{
	// get connection to BB DB
	nrdb = new NRDB ();
	} catch (CDBException ce)
	    {
		NetworkMonitorServer.nmlogger.warn ("subscribeForLatencyBetnHosts ():Error connecting"+
						    " to BandwidthBroker Database!");
		throw new NetworkProbesException ("subscribeForLatencyBetnHosts ():Error connecting"+
						  " to BandwidthBroker Database!", false);
	    }
	NetworkMonitorServer.nmlogger.info ("BB DB access obtained");
	for (int i=0;i<latencyBetnHostsSeq.length;i++)
	    {
		latencyStruct = latencyBetnHostsSeq [i];
		String ssubnet=null;
		String dsubnet=null;
		// Get the Subnet Addr from the HostSubnet Table, so
		// we can obtain the measurmentHost for this subnet
		try{
		    NetworkMonitorServer.nmlogger.info ("input sourceHostName=*"+latencyStruct.srcHostName+"*");
		    NetworkMonitorServer.nmlogger.info ("input destinationHostName=*"+latencyStruct.destHostName+"*");
		   
		    ssubnet = nrdb.findSubnetForHost (latencyStruct.srcHostName);
		    NetworkMonitorServer.nmlogger.info ("sourceSubnet=*"+ssubnet+"*");
		    source = (String)measurementHostsTable.get ((String)ssubnet);
		    NetworkMonitorServer.nmlogger.info ("measurement source=*"+source+"*");
		    
		    dsubnet = nrdb.findSubnetForHost (latencyStruct.destHostName);
		    NetworkMonitorServer.nmlogger.info ("destSubnet=*"+dsubnet+"*");
		    destination = (String)measurementHostsTable.get ((String)dsubnet);
		    NetworkMonitorServer.nmlogger.info ("measurement dest=*"+destination+"*");
		    if (source==null || destination ==null)
			throw new CDBException ();
		} catch (CDBException ce)
		    {
			NetworkMonitorServer.nmlogger.warn ("subscribeForLatencyBetnHostss ():Error obtaining"+
							    " measurement hosts from BandwidthBroker Database!");
			try{
			    if (nrdb != null)
				nrdb.releaseConnection ();
			} catch (CDBException ee) {
			    	NetworkMonitorServer.nmlogger.warn ("subscribeForLatencyBetnHostEndpoints ():"+
							      "Error in releasing BandwidthBroker Database connection");
				ee.printStackTrace ();
			}
		
			throw new NetworkProbesException ("subscribeForLatencyBetnHostss ():Error obtaining"+
							  " measurement hosts from BandwidthBroker Database!", false);
			
		    }
			
		NetworkMonitorServer.nmlogger.info ("measurement hosts for subscribeForLatencyBetnHostss () is "+
						    source+" "+destination);
	
		for (int j=0;j < latencyStruct.latencyPerClassSeq.length; j++)
		    {
			latencyPerClass = latencyStruct.latencyPerClassSeq[j];
			latency = latencyPerClass.availableLatency;
			measurementPeriod = latency.measurementPeriod;
			averagingWindow = latency.averagingWindow;
			// threshold time only needed for events
			localThresholdLatency = latency.thresholdTime;
			// set default delay to be 0
			latency.delayTime = 0;

			// Now get the service class
			serviceClass = latencyPerClass.qos;

			ProbeParametersHolder defaults = new ProbeParametersHolder ();

			// Build the event to be sent by the EventSupplierMonitor to the consumer
			// It should be of the type subscribed for by the consumer
			MeasurementEndpointsType typeForEvent = MeasurementEndpointsType.latency_between_hosts;
			MeasurementEndpoints meForEvent = new MeasurementEndpoints ();
			LatencyBetnHosts[] latencyBetnHostsForEvent = new LatencyBetnHosts [1];
			LatencyPerQoS[] latencyPerQoSSeqForEvent = new LatencyPerQoS[1];
			Latency latencyForEvent = new Latency (0, // delay will be populated in this field
							       averagingWindow,
							       measurementPeriod,
							       localThresholdLatency);
			latencyPerQoSSeqForEvent [0]= new LatencyPerQoS (latencyForEvent, serviceClass);
			latencyBetnHostsForEvent [0] = new LatencyBetnHosts (source,
									     destination,
									     latencyPerQoSSeqForEvent);
			meForEvent.hostsLatencyInfo (latencyBetnHostsForEvent);
			MeasurementEndpointsHolder latencyInfoForEvent = new MeasurementEndpointsHolder (meForEvent);
			
			if (useProbes)
			    {
				try {
				     startEventSupplierMonitor (consumerComponentName, 
							       conditionType,
							       condition, 
							       cookie,
							       source,
							       destination,
							       serviceClass,
							       measurementPeriod,
							       averagingWindow,
								defaults,
								localThresholdLatency,
							       latencyInfoForEvent,
							       typeForEvent);
				} catch (NetworkProbesException ne)
				    {
					//ne.printStackTrace ();
					if (ne.probeConfigurationError=true)
					    {
						// return default so
						// end user knows why
						// the error was
						// potentially caused
						latency.measurementPeriod = defaults.value.measurementPeriod;
						NetworkMonitorServer.nmlogger.warn ("Default measurement period"+
										    " violated, hence returning it:"+
										    defaults.value.measurementPeriod);
					    }
					NetworkMonitorServer.nmlogger.warn (ne.reason);
					throw new NetworkProbesException (ne.reason, 
									  true);
				    }
			    }// end of if probes
			
		    }// end of for j
	    }// end of for i
	try{
	    if (nrdb != null)
		nrdb.releaseConnection ();
	} catch (CDBException ee)
	    {
		NetworkMonitorServer.nmlogger.warn ("subscribeForLatencyBetnHostEndpoints ():"+
						    "Error in releasing BandwidthBroker Database connection");
		throw new NetworkProbesException ("subscribeForLatencyBetnHostEndpoints ():"+
						  "Error in releasing BandwidthBroker Database connection", 
						  false);
	    }

    }
    
    public void startEventSupplierMonitor (String consumerComponentName, 
					   NMEventConditionType conditionType,
					   NMEventCondition condition, 
					   String cookie,
					   String source,
					   String  destination,
					   QOSRequired serviceClass,
					   long measurementPeriod,
					   long averagingWindow,
					   ProbeParametersHolder defaults,
					   long localThresholdLatency,
					   MeasurementEndpointsHolder latencyInfoForEvent,
					   MeasurementEndpointsType typeForEvent)
	throws NetworkProbesException
    {
	long periodicLatency=0;
	long globalThresholdLatency=0;
	Thread monitor;
	String probeId=null;
	org.omg.CORBA.Object nmclient;

	try {
	// get the client object, if not throw error
	    nmclient = lookupClient (consumerComponentName, 
						     _args);
	}catch (NetworkProbesException e)
	    {
		throw new NetworkProbesException ("Error looking up "+consumerComponentName+
						  " name with CORBA Naming service:reason:"+e.reason, true);
	    }
	
	

	NetworkMonitorServer.nmlogger.info ("Launching EventSupplier monitor"+
					    " with"+
					    "source,destination,serviceClass (QoSRequired),"+
					    "measurementPeriod,averagingWindow as "+
					    source+" "+destination+" "+ 
					    new Integer (serviceClass.value ()).toString ()+
					    " "+measurementPeriod+" "+averagingWindow);
	
	probeId = getMeasurementUsingProbesEventSubscription (source,
							      destination,
							      serviceClass,
							      measurementPeriod,
							      averagingWindow,
							      defaults);
	
	NetworkMonitorServer.nmlogger.info ("PROBEID:"+probeId);

   
	try{
	    // get connection to BB DB if null
	    if (nrdb == null)
		nrdb = new NRDB ();
	} catch (CDBException ce)
	    {
		NetworkMonitorServer.nmlogger.warn ("start event supplier monitor:"+
						    " error connecting to BB DB");
		throw new NetworkProbesException ("start event supplier monitor:Error connecting"+
								    " to BandwidthBroker Database!", false);
		
	    }  	 
	// Check the condition 
	// Spawn event supplier thread that will function as per event condition type
	// store into BB DB along with  the monitor thread ref
	String monitorName=null;
	if (conditionType.value () == 0)// periodic latency request
	    {
		periodicLatency = condition.rp ();
	
		monitorName  = new String ("periodic"+new Integer (serviceClass.value ()).toString ()+
					   probeId+
					   consumerComponentName+cookie+
					   new Long (periodicLatency).toString ());
		try{
		    nrdb.addNetworkMonitorEventSubscription (consumerComponentName, 
							     cookie, 
							     probeId, 
							     monitorName);
		    // commit the changaes to database
		    nrdb.commit ();

		}catch (CDBException ce)
		    {
			NetworkMonitorServer.nmlogger.warn ("start event supplier monitor:"+
							    " error inserting records into BB DB, "+
							    "check for duplicate requests!"+ce.toString ());
			//ce.printStackTrace ();
			String exc=" ";
			try{
			    if (nrdb != null)
				nrdb.releaseConnection ();
			} catch (CDBException ee)
			    {
				NetworkMonitorServer.nmlogger.warn ("start event supplier monitor:"+
							    "Error in releasing BandwidthBroker"+
								    " Database connection");
				
				exc = "start event supplier monitor:"+
				    "Error in releasing BandwidthBroker Database connection";
			    }
			
			throw new NetworkProbesException ("start event supplier monitor:"+
							  " error inserting records into BB DB"+
							  ce.toString ()+ exc, false);
			
		    }
		//eventMonitorThreadTable.put ((String)monitorName,(Integer) new Integer (1)); 
	
		PeriodicEventSupplierMonitor pem = 
		    new PeriodicEventSupplierMonitor (this,
						      monitorName,
						      NetworkMonitorServer.nmlogger,
						      periodicLatency,
						      measurementPeriod,
						      averagingWindow,
						      consumerComponentName,
						      nmclient,
						      cookie,
						      probeId,
						      latencyInfoForEvent,
						      typeForEvent);

	
		//monitor = new Thread (pem, monitorName);
		//	eventMonitorThreadTable.put ((String)monitorName,(Thread) monitor); 
		eventMonitorThreadTable.put ((String)monitorName,(EventSupplierMonitor) pem); 
		//monitor.start ();
		pem.start ();
		NetworkMonitorServer.nmlogger.info ("PeriodicEventSupplierMonitor launched with period:"+
						    new Long (periodicLatency).toString ()+ 
						    " for probeid:"+probeId+ " for consumer:"+
						    consumerComponentName+ " with cookie:"+cookie);
	    }
	    long threshold=0;
	    if (conditionType.value () == 1)// threshold latency request
		{
		    // global threashold
		    globalThresholdLatency = condition.tc();
		    // verify if any of local latency has been specified
		    if (localThresholdLatency <=0)
			threshold = globalThresholdLatency;
		    else
			threshold = localThresholdLatency;
		    
		    monitorName = new String ("threshold"+new Integer (serviceClass.value ()).toString ()+
					      probeId+consumerComponentName+cookie+
					      new Long (threshold).toString ());
		    
		    try{
		    nrdb.addNetworkMonitorEventSubscription (consumerComponentName, 
							     cookie, 
							     probeId, 
							     monitorName);
		     // commit the changaes to database
		    nrdb.commit ();

		    }catch (CDBException ce)
		    {
			NetworkMonitorServer.nmlogger.warn ("start event supplier monitor:"+
							    " error obtaining records from BB DB"+ce.toString ());
			String exc=" ";
			try{
			    if (nrdb != null)
				nrdb.releaseConnection ();
			} catch (CDBException ee)
			    {
				NetworkMonitorServer.nmlogger.warn ("start event supplier monitor:"+
							    "Error in releasing BandwidthBroker"+
								    " Database connection");
				exc = "start event supplier monitor:"+
				    "Error in releasing BandwidthBroker Database connection";	
			
			    }
			throw new NetworkProbesException ("start event supplier monitor:"+
							  " error obtaining records from BB DB"+ce.toString ()+ exc, 
							  false);
			
		    }
		    ThresholdEventSupplierMonitor tem = 
			new ThresholdEventSupplierMonitor (this,
							   monitorName,
							   NetworkMonitorServer.nmlogger,
							   threshold,
							   measurementPeriod,
							   averagingWindow,
							   consumerComponentName,
							   nmclient,
							   cookie,
							   probeId,
							   latencyInfoForEvent,
							   typeForEvent);
		    eventMonitorThreadTable.put ((String)monitorName,(EventSupplierMonitor) tem); 
		    tem.start ();
		    NetworkMonitorServer.nmlogger.info ("ThresholdEventSupplierMonitor launched with threshold:"+
							new Long (threshold).toString ()+ 
							" for probeid:"+probeId+ " for consumer:"+
							consumerComponentName+ " with cookie:"+cookie);
		}
    }
    
     public String getLatencyBetnFlowEndpoints (MeasurementEndpointsHolder latencyInfo)
	throws NetworkProbesException
    {
	LatencyBetnFlowEndpoints[] latencyBetnFlowEndpointsSeq = latencyInfo.value.flowLatencyInfo ();
	LatencyBetnFlowEndpoints latencyStruct = null;
	LatencyPerQoS latencyPerClass = null;
	Latency latency = null;
	String source = null;
	String destination = null;
	long measurementPeriod = 60000000;
	long averagingWindow = 1;
	QOSRequired serviceClass;
	String flowToken = null;
	
	try{
	// get connection to BB DB
	nrdb = new NRDB ();
	} catch (CDBException ce)
	    {
		NetworkMonitorServer.nmlogger.warn ("measurement between flow endpoints: error connecting to BB DB");
		throw new NetworkProbesException ("getLatencyBetnFlowEndpoints ():Error connecting"+
						  " to BandwidthBroker Database!", false);
		
	    }
	NetworkMonitorServer.nmlogger.info ("BB DB access obtained");
	String ssubnet=null;
	String dsubnet=null;
	for (int i=0;i<latencyBetnFlowEndpointsSeq.length;i++)
	    {
		latencyStruct = latencyBetnFlowEndpointsSeq [i];
		flowToken = latencyStruct.flowToken;
		
		// Look up BB DB for flowrecord with flowToken and
		// obtain the src and dest IPs.
		try{
		FlowRecord fr= nrdb.findFlowRecord (flowToken);
		AttributeValues av = fr.getAttributes ();
	        ssubnet = getSubnetAddr (av.getValueForName (FlowRecord.attrSrcIP), 
					 av.getValueForName (FlowRecord.attrSrcMask));
		NetworkMonitorServer.nmlogger.info ("measurement between flow endpoints:"+
						    " source subnet addr="+ssubnet);
		source = (String)measurementHostsTable.get((String) ssubnet);
		NetworkMonitorServer.nmlogger.info ("measurement between flow endpoints:"+
						    " source ="+source);
		dsubnet = getSubnetAddr (av.getValueForName (FlowRecord.attrDestIP), 
					 av.getValueForName (FlowRecord.attrDestMask));
		NetworkMonitorServer.nmlogger.info ("measurement between flow endpoints:"+
						    " destination subnet addr="+dsubnet);
		destination = (String)measurementHostsTable.get((String) dsubnet);
		NetworkMonitorServer.nmlogger.info ("measurement between flow endpoints:"+
						    " destination ="+destination);
	
		}catch (CDBException cee)
		    {
			NetworkMonitorServer.nmlogger.warn ("getLatencyBetnFlowEndpoints ():Error"+
							  " obtaining flow record:"+ flowToken+
							    " from BandwidthBroker Database!");
			String exc = " ";
			try{
			    if (nrdb != null)
				nrdb.releaseConnection ();
			} catch (CDBException ee)
			    {
				NetworkMonitorServer.nmlogger.warn ("Error in releasing BandwidthBroker"+
								    " Database connection");
				exc = "Error in releasing BandwidthBroker Database connection";	
			    }
			
			throw new NetworkProbesException ("getLatencyBetnFlowEndpoints ():Error"+
							  " obtaining flow record:"+ flowToken+
							  " from BandwidthBroker Database!"+exc, false);
			
		    }
		if (source == null || destination == null)
		    {
			NetworkMonitorServer.nmlogger.warn ("Error getting measurementHosts for flowToken:"+flowToken);
			throw new NetworkProbesException ("Error getting measurementHosts for flowToken:"+flowToken, 
							  false);
		    }
		for (int j=0;j < latencyStruct.latencyPerClassSeq.length; j++)
		    {
			latencyPerClass = latencyStruct.latencyPerClassSeq[j];
			latency = latencyPerClass.availableLatency;
			measurementPeriod = latency.measurementPeriod;
			averagingWindow = latency.averagingWindow;
			// threshold time only needed for events
			
			// set default delay to be 0
			latency.delayTime = 0;

			// Now get the service class
			serviceClass = latencyPerClass.qos;

			ProbeParametersHolder defaults = new ProbeParametersHolder ();
			// call ProbeControl to do the needful
			if (useProbes)
			    {
				try {
				    
				    NetworkMonitorServer.nmlogger.info (" Launching getMeasurementUsingProbes ()"+
									"with"+
									"source,destination,"+
									"serviceClass (QoSRequired),"+
									"measurementPeriod,averagingWindow as "+
									source+" "+destination+" "+ 
									new Integer (
										     serviceClass.value ()).toString ()+
									" "+measurementPeriod+" "+averagingWindow);
				    
				    latency.delayTime = getMeasurementUsingProbes (source,
										   destination,
										   serviceClass,
										   measurementPeriod,
										   averagingWindow,
										   defaults);
				    
				    NetworkMonitorServer.nmlogger.info ("DELAY="+
									new Long (latency.delayTime).toString ());
				    
				} catch (NetworkProbesException ne)
				    {
					//	ne.printStackTrace ();
					if (ne.probeConfigurationError=true)
					    {
						// return default so
						// end user knows why
						// the error was
						// potentially caused
						latency.measurementPeriod = defaults.value.measurementPeriod;
						NetworkMonitorServer.nmlogger.warn ("Default measurement period"+
										    " violated, hence returning it:"+
										    defaults.value.measurementPeriod);
					    }
					NetworkMonitorServer.nmlogger.warn (ne.reason);
					throw new NetworkProbesException (ne.reason, 
									  true);
				    }
				
			    }// end of if
			
		    }// end of for j
	    } // end of for i
	try{
	    if (nrdb != null)
		nrdb.releaseConnection ();
	} catch (CDBException ee)
	    {
		NetworkMonitorServer.nmlogger.warn ("Error in releasing BandwidthBroker Database connection");
		throw new NetworkProbesException ("Error in releasing BandwidthBroker Database connection", 
						  false);
	    }
	return null;
    }

    public String getLatencyBetnPoolHosts (MeasurementEndpointsHolder latencyInfo)
	throws NetworkProbesException
    {
	LatencyBetnPools[] latencyBetnPoolsSeq = latencyInfo.value.poolLatencyInfo ();
	LatencyBetnPools latencyStruct = null;
	LatencyPerQoS latencyPerClass = null;
	Latency latency = null;
	String source = null;
	String destination = null;
	long measurementPeriod = 60000000;
	long averagingWindow = 1;
	QOSRequired serviceClass;
	String flowToken = null;	
	String srcPoolName = null;
	String destPoolName = null;
	try{
	// get connection to BB DB
	nrdb = new NRDB ();
	} catch (CDBException ce)
	    {
		NetworkMonitorServer.nmlogger.warn ("getLatencyBetnPools ():Error connecting"+
						    " to BandwidthBroker Database!");
		throw new NetworkProbesException ("getLatencyBetnPools ():Error connecting"+
						  " to BandwidthBroker Database!", false);
	    }
	NetworkMonitorServer.nmlogger.info ("BB DB access obtained");
	for (int i=0;i<latencyBetnPoolsSeq.length;i++)
	    {
		latencyStruct = latencyBetnPoolsSeq [i];
	
		srcPoolName = latencyStruct.srcPoolName;
		destPoolName = latencyStruct.destPoolName;
		// Look up BB DB for pool record 
		// obtain the src and dest hostnames.
		try {
		source = (String) measurementHostsTable.get ((String)nrdb.findSubnetInPool (srcPoolName));
		destination = (String) measurementHostsTable.get ((String)nrdb.findSubnetInPool (destPoolName));
		if (source == null || destination == null)
		    {
			throw new CDBException ();
		    }
		}catch (CDBException cee)
		    {
			NetworkMonitorServer.nmlogger.warn ("getLatencyBetnPoolHosts ():Error"+
							  " obtaining pool measurement hosts for pools "+srcPoolName+
							  " and "+destPoolName+" respectively "+
							    " from BandwidthBroker Database!");
			String exc = " ";
			try{
			    if (nrdb != null)
				nrdb.releaseConnection ();
			} catch (CDBException ee)
			    {
				NetworkMonitorServer.nmlogger.warn ("Error in releasing BandwidthBroker"+
								    " Database connection");
				exc = "Error in releasing BandwidthBroker Database connection";	
			    }
			
			throw new NetworkProbesException ("getLatencyBetnPoolHosts ():Error"+
							  " obtaining pool measurement hosts for pools "+srcPoolName+
							  " and "+destPoolName+" respectively "+
							  " from BandwidthBroker Database!", false);
		    }
		NetworkMonitorServer.nmlogger.info ("source and destination measurement hosts for pools "+
						    srcPoolName+" and "+destPoolName+" respectively are "+
						    source+" "+destination);
		for (int j=0;j < latencyStruct.latencyPerClassSeq.length; j++)
		    {
			latencyPerClass = latencyStruct.latencyPerClassSeq[j];
			latency = latencyPerClass.availableLatency;
			measurementPeriod = latency.measurementPeriod;
			averagingWindow = latency.averagingWindow;
			// threshold time only needed for events
			
			// set default delay to be 0
			latency.delayTime = 0;

			// Now get the service class
			serviceClass = latencyPerClass.qos;

			ProbeParametersHolder defaults = new ProbeParametersHolder ();
			// call ProbeControl to do the needful
			if (useProbes)
			    {
				try {
				    NetworkMonitorServer.nmlogger.info (" Launching getMeasurementUsingProbes () with"+
									"source,destination,serviceClass (QoSRequired),"+
									"measurementPeriod,averagingWindow as "+
									source+" "+destination+" "+ 
									new Integer (serviceClass.value ()).toString ()+
									" "+measurementPeriod+" "+averagingWindow);
				    
				    latency.delayTime = getMeasurementUsingProbes (source,
										   destination,
										   serviceClass,
										   measurementPeriod,
										   averagingWindow,
										   defaults);
				    NetworkMonitorServer.nmlogger.info ("DELAY="+
									new Long (latency.delayTime).toString ());
				} catch (NetworkProbesException ne)
				    {
					//ne.printStackTrace ();
					if (ne.probeConfigurationError=true)
					    {
						// return default so
						// end user knows why
						// the error was
						// potentially caused
						latency.measurementPeriod = defaults.value.measurementPeriod;
						NetworkMonitorServer.nmlogger.warn ("Default measurement period"+
										    " violated, hence returning it:"+
										    defaults.value.measurementPeriod);
					    }
					NetworkMonitorServer.nmlogger.warn (ne.reason);
					throw new NetworkProbesException (ne.reason, 
									  true);
				    }
			    }// end of if
			
		    }// end of for j
	    } // end of for i
	try{
	    if (nrdb != null)
		nrdb.releaseConnection ();
	} catch (CDBException ee)
	    {
		NetworkMonitorServer.nmlogger.warn ("Error in releasing BandwidthBroker Database connection");
		throw new NetworkProbesException ("Error in releasing BandwidthBroker Database connection", 
						  false);
	    }
	return null;
    }

    public String getLatencyBetnHosts (MeasurementEndpointsHolder latencyInfo)
	throws NetworkProbesException
    {
	LatencyBetnHosts[] latencyBetnHostsSeq = latencyInfo.value.hostsLatencyInfo ();
	LatencyBetnHosts latencyStruct = null;
	LatencyPerQoS latencyPerClass = null;
	Latency latency = null;
	String source = null;
	String destination = null;
	long measurementPeriod = 60000000;
	long averagingWindow = 1;
	QOSRequired serviceClass;
	
	try{
	// get connection to BB DB
	nrdb = new NRDB ();
	} catch (CDBException ce)
	    {
		NetworkMonitorServer.nmlogger.warn ("getLatencyBetnHosts ():Error connecting"+
						    " to BandwidthBroker Database!");
		throw new NetworkProbesException ("getLatencyBetnHosts ():Error connecting"+
						  " to BandwidthBroker Database!", false);
	    }
	NetworkMonitorServer.nmlogger.info ("BB DB access obtained");
	for (int i=0;i<latencyBetnHostsSeq.length;i++)
	    {
		latencyStruct = latencyBetnHostsSeq [i];
		String ssubnet=null;
		String dsubnet=null;
		// Get the Subnet Addr from the HostSubnet Table, so
		// we can obtain the measurmentHost for this subnet
		try{
		    NetworkMonitorServer.nmlogger.info ("input sourceHostName=*"+latencyStruct.srcHostName+"*");
		    NetworkMonitorServer.nmlogger.info ("input destinationHostName=*"+latencyStruct.destHostName+"*");
		   
		    ssubnet = nrdb.findSubnetForHost (latencyStruct.srcHostName);
		    NetworkMonitorServer.nmlogger.info ("sourceSubnet=*"+ssubnet+"*");
		    source = (String)measurementHostsTable.get ((String)ssubnet);
		    NetworkMonitorServer.nmlogger.info ("measurement source=*"+source+"*");
		    
		    dsubnet = nrdb.findSubnetForHost (latencyStruct.destHostName);
		    NetworkMonitorServer.nmlogger.info ("destSubnet=*"+dsubnet+"*");
		    destination = (String)measurementHostsTable.get ((String)dsubnet);
		    NetworkMonitorServer.nmlogger.info ("measurement dest=*"+destination+"*");
		    if (source==null || destination ==null)
			throw new CDBException ();
		} catch (CDBException ce)
		    {
			NetworkMonitorServer.nmlogger.warn ("getLatencyBetnHostss ():Error obtaining"+
							    " measurement hosts from BandwidthBroker Database!");
			String exc = " ";
			try{
			    if (nrdb != null)
				nrdb.releaseConnection ();
			} catch (CDBException ee)
			    {
				NetworkMonitorServer.nmlogger.warn ("Error in releasing BandwidthBroker"+
								    " Database connection");
				exc = "Error in releasing BandwidthBroker Database connection";	
			    }
			
			throw new NetworkProbesException ("getLatencyBetnHostss ():Error obtaining"+
							  " measurement hosts from BandwidthBroker Database!", true);
		    }
		NetworkMonitorServer.nmlogger.info ("measurement hosts for getLatencyBetnHostss () is "+
						    source+" "+destination);
		for (int j=0;j < latencyStruct.latencyPerClassSeq.length; j++)
		    {
			latencyPerClass = latencyStruct.latencyPerClassSeq[j];
			latency = latencyPerClass.availableLatency;
			measurementPeriod = latency.measurementPeriod;
			averagingWindow = latency.averagingWindow;
			// threshold time only needed for events
			
			// set default delay to be 0
			latency.delayTime = 0;

			// Now get the service class
			serviceClass = latencyPerClass.qos;

			ProbeParametersHolder defaults = new ProbeParametersHolder ();
			// call ProbeControl to do the needful
			if (useProbes)
			    {
				try {
				    NetworkMonitorServer.nmlogger.info (" Launching getMeasurementUsingProbes () with"+
									"source,destination,serviceClass (QoSRequired),"+
									"measurementPeriod,averagingWindow as "+
									source+" "+destination+" "+ 
									new Integer (serviceClass.value ()).toString ()+
									" "+measurementPeriod+" "+averagingWindow);
				    
				    latency.delayTime = getMeasurementUsingProbes (source,
										   destination,
										   serviceClass,
										   measurementPeriod,
										   averagingWindow,
										   defaults);

				NetworkMonitorServer.nmlogger.info  ("DELAY="+new Long (latency.delayTime).toString ());
				} catch (NetworkProbesException ne)
				    {
					// ne.printStackTrace ();
					if (ne.probeConfigurationError=true)
					    {
						// return default so
						// end user knows why
						// the error was
						// potentially caused
						latency.measurementPeriod = defaults.value.measurementPeriod;
						NetworkMonitorServer.nmlogger.warn ("Default measurement period"+
										    " violated, hence returning it:"+
										    defaults.value.measurementPeriod);
					    }
					NetworkMonitorServer.nmlogger.warn (ne.reason);
					throw new NetworkProbesException (ne.reason, 
									  true);
				    }
			    }
			
		    }
	    }
	try{
	    if (nrdb != null)
		nrdb.releaseConnection ();
	} catch (CDBException ee)
	    {
		NetworkMonitorServer.nmlogger.warn ("Error in releasing BandwidthBroker Database connection");
		throw new NetworkProbesException ("Error in releasing BandwidthBroker Database connection", 
						  false);
	    }
	return null;
    }

  /** Find the measurement using probes, first start them and then query
    the incore dB for the values. If exception, query defaults and
    return them. If event subscription, return probeId */
  public long getMeasurementUsingProbes (String source,
					 String destination,
					 QOSRequired svcClass,
					 long measurementPeriod,
					 long averagingWindow,
					 ProbeParametersHolder pph )// stores defaults on return
	throws NetworkProbesException
    {

	// get probe defaults, only superimpose the measurement period
	// and use these probe params for measurement
	ProbeEndpoints pe = new ProbeEndpoints (source, 
						destination);
	//	 ProbeParametersHolder pph = new ProbeParametersHolder ();


	try {
	     pcImpl.query_configuration(pe, 
					pph); 
	 } catch (ProbeControlException pcc)
	     {
		 // pcc.printStackTrace ();
		 NetworkMonitorServer.nmlogger.warn (" Failure in getting probe defaults");
		 throw new NetworkProbesException (" Failure in getting probe defaults", false);
	     }
	// start the probe between the specified endpoints with
	// specified periodicity
	 ProbeParameters pp = new ProbeParameters (pph.value.measurementPeriod,
						   pph.value.packetSize,
						   pph.value.packetTrainLength, 
						   pph.value.packetGap);

	 if (measurementPeriod < pp.measurementPeriod)
	     {
		 NetworkMonitorServer.nmlogger.warn (" Failure in setting up probe, "+
						     "measurementPeriod is smaller than the"+
						     " allowed measurementPeriod of "+ 
						     pp.measurementPeriod);
		 throw new NetworkProbesException (" Failure in setting up probe, "+
					       "measurementPeriod is smaller than the"+
					       " allowed measurementPeriod of "+ 
					       pp.measurementPeriod, true);
	     }
	 pp.measurementPeriod = measurementPeriod;

	     
	 /*ProbeParameters pp = new ProbeParameters (measurementPeriod, 
						  500,
						  10,
						  100);*/
	 ProbeControlSpec pcs = new ProbeControlSpec (svcClass,
						      pp);
	 StringHolder probeID = new StringHolder ();
	 try {
	     pcImpl.start_measurement (pe,
				       pcs,
				       probeID);
	 }
	 catch (ProbeControlException pc)
	     {
		 // pc.printStackTrace ();
		 NetworkMonitorServer.nmlogger.warn (" Failure in starting probe");
		 throw new NetworkProbesException (" Failure in starting probe", true);
	     }
	 NetworkMonitorServer.nmlogger.info ("Probe started");

	// Access the measurement DB and return the latency as per the
	// avergae measurement window period
	 long instant = System.currentTimeMillis ()*1000;
	 // As a tolerance for clock skew betn the imp nodes and the
	 // sink nodes.
	 instant =instant - 300000000; // 5 min

	 return getDelay (probeID.value, 
			  averagingWindow, 
			  measurementPeriod, 
			  instant);	 
    }

 /** Find the measurement using probes, first start them and then query
    the incore dB for the values. If exception, query defaults and
    return them. If event subscription, return probeId */
  public String getMeasurementUsingProbesEventSubscription (String source,
							  String destination,
							  QOSRequired svcClass,
							  long measurementPeriod,
							  long averagingWindow,
							  ProbeParametersHolder pph)// stores defaults on return
	throws NetworkProbesException
    {

	// get probe defaults, only superimpose the measurement period
	// and use these probe params for measurement
	ProbeEndpoints pe = new ProbeEndpoints (source, 
						destination);
	//	 ProbeParametersHolder pph = new ProbeParametersHolder ();


	try {
	     pcImpl.query_configuration(pe, 
					pph); 
	 } catch (ProbeControlException pcc)
	     {
		 // pcc.printStackTrace ();
		 NetworkMonitorServer.nmlogger.warn (" Failure in getting probe defaults");
		 throw new NetworkProbesException (" Failure in getting probe defaults", false);
	     }
	// start the probe between the specified endpoints with
	// specified periodicity
	 ProbeParameters pp = new ProbeParameters (pph.value.measurementPeriod,
						   pph.value.packetSize,
						   pph.value.packetTrainLength, 
						   pph.value.packetGap);
	 long defaultMeasurementPeriod = pp.measurementPeriod;
	 if (measurementPeriod < pp.measurementPeriod)
	     {
		 NetworkMonitorServer.nmlogger.warn (" Failure in setting up probe, "+
						     "measurementPeriod is smaller than the"+
						     " allowed measurementPeriod of "+ 
						     pp.measurementPeriod);
		 throw new NetworkProbesException (" Failure in setting up probe, "+
					       "measurementPeriod is smaller than the"+
					       " allowed measurementPeriod of "+ 
					       pp.measurementPeriod, true);
	     }
	 pp.measurementPeriod = measurementPeriod;

	     
	 /*ProbeParameters pp = new ProbeParameters (measurementPeriod, 
						  500,
						  10,
						  100);*/
	 ProbeControlSpec pcs = new ProbeControlSpec (svcClass,
						      pp);
	 StringHolder probeID = new StringHolder ();
	 try {
	     pcImpl.start_measurement (pe,
				       pcs,
				       probeID);
	     // To make sure things have time to start, sleep a bit
	     NetworkMonitorServer.nmlogger.info (" Probe Setup in progress ...");
	    Thread.sleep (defaultMeasurementPeriod/1000);
		 
	 }
	 catch (ProbeControlException pc)
	     {
		 // pc.printStackTrace ();
		 NetworkMonitorServer.nmlogger.warn (" Failure in starting probe");
		 throw new NetworkProbesException (" Failure in starting probe", true);
	     }
	 catch (InterruptedException ie)
	     { 
		 NetworkMonitorServer.nmlogger.warn (" Interrupted while starting probe");
		 throw new NetworkProbesException (" Interrupted while starting probe", true);
	     }
	 NetworkMonitorServer.nmlogger.info ("Probe started");

	 return probeID.value;

    }

  public long getDelay (String probeID, 
			long averagingWindow, 
			long measurementPeriod,
			long timeNow)
      throws NetworkProbesException
  {
      NetworkMonitorServer.nmlogger.info ("getDelay (): averagingWindow="+new Long (averagingWindow).toString ()
					  + "measurementPeriod="+new Long (measurementPeriod).toString ()+
					  "timestamp="+new Long (timeNow).toString ());
      ObjectList ol=null;

      // now grep for it

      if (averagingWindow > 1)
	  timeNow = timeNow - averagingWindow;
      
      try
	  {
	      int trial=0;
	      while (trial<3)
		  {
		      try {
		      // Find all probeID entries in NMDB with probeTime >= timeNow
		      ol = nmdb.findMeasurementByProbeIDAndTimestamp (probeID, 
								      timeNow,
								      true);
		      NetworkMonitorServer.nmlogger.info ("Looking up probe measurement entries for probeID="+
							  probeID+" with timestamp greater than equal to: "+
							  new Long (timeNow).toString ());
		      if (ol.size () == 0)
			  throw new Exception ();
		      AttributeValues av=null;
		      long delaySum = 0;
		      NetworkMonitorServer.nmlogger.info (" Number of entries found in INCORE DB="+ 
							  new Integer (ol.size()).toString ());
		      if (ol.size () > 0)
			  {
			      for (int i = 0; i < ol.size (); i++)
				  {
				      // Get the measurement
				      av = ol.get (i).getAttributes ();
				      long delay = Long.parseLong (av.getValueForName (Measurement.attrDelay)); 
				      NetworkMonitorServer.nmlogger.info ("FROM INCORE DB: delay="+
									  new Long (delay).toString ()+
									  "for probeID="+
									  av.getValueForName (Measurement.attrProbeID)+
									  " with probeTime="+
									  av.getValueForName (Measurement.attrProbeTime)+
									  " with DSCP="+
									  av.getValueForName (Measurement.attrDSCP));
				      // this is for the case when measurementperiod (say 60sec) == averaging window)
				      //if ( averagingWindow <= 1)
				      if ( averagingWindow <= measurementPeriod) 
					  return delay;
				      delaySum += delay;
				      NetworkMonitorServer.nmlogger.info ("delaySum="+
									  new Long (delaySum).toString());
				  }
			      return (delaySum/ol.size ());
			  }
		      }catch (Exception ec)
			  {
			     // Wait for measurement period so that the measurement actually
			      // gets to the incore DB. We didvide by 100 as measurementPeriod
			      // is in usecs.
			      NetworkMonitorServer.nmlogger.info ("No results as yet in incore DB, "+
								  "so wait for measurementEriod time (usecs)"+
								  measurementPeriod);
			      Thread.sleep (measurementPeriod/1000);
			      
			  }
		      trial++;
		  }
	  }
      catch (Exception e)
	  {
	      // e.printStackTrace ();
	      NetworkMonitorServer.nmlogger.warn ("Error in finding and obtaining delay measurement "+
						  "in NMDB with probeID "+probeID);
	      throw new NetworkProbesException ("Error in finding and obtaining delay measurement "+
						"in NMDB with probeID "+probeID, false);
	  }
      return -1;
  }

    public void initProbeControl (String [] args) throws Exception
    {
      ORB orb = ORB.init(args, null);

      // get the root naming context
      org.omg.CORBA.Object objRef =
          orb.resolve_initial_references("NameService");

      // Use NamingContextExt which is part of the Interoperable
      // Naming Service (INS) specification.
      NamingContextExt ncRef = NamingContextExtHelper.narrow(objRef);

      // resolve the Object Reference in Naming
      String name = "ProbeControl";

      probeControl = ProbeControlHelper.narrow(ncRef.resolve_str(name));
       
    }

    // Get the properties
    public void setNMProperties (String nmConfigFile)
    {
	PropertyResourceBundle properties=null;
	 try
	    {
		properties = new PropertyResourceBundle(new FileInputStream(nmConfigFile));
		NetworkMonitorServer.nmlogger.info ("Network Monitor Configuration Properties from :"+nmConfigFile);
	
		dbGarbageCollectionTimeout = Long.parseLong( properties.getString("DBGarbageCollectionTimeout") );
		NetworkMonitorServer.nmlogger.info ("Network Monitor In core DB garbage collection Timeout"+
						    " in milliseconds:"
						    +new Long (dbGarbageCollectionTimeout).toString ());
		dbStayTimePeriod = Long.parseLong( properties.getString("DBStayTimePeriod") );
		NetworkMonitorServer.nmlogger.info ("Network Monitor In core DB time period for retaining records"+
						    " in milliseconds:"
						    +new Long (dbStayTimePeriod).toString ());
		
		int use_probes = Integer.parseInt(properties.getString("UseProbes") );
		if (use_probes==1)
		    useProbes=true;
		else
		    useProbes=false;
	
		
		if (useProbes)
		{
		    probeServer=properties.getString ("ProbeServer");
		    NetworkMonitorServer.nmlogger.info (" Probe Server is "+
							probeServer);
		    probeSink=properties.getString ("ProbeSink");
		    NetworkMonitorServer.nmlogger.info (" Probe Sink is "+
							probeSink);
		    NetworkMonitorServer.nmlogger.info ("Use probes ? true");
		    
		    // get the hosts that have the probe hardware
		    int numMHosts =  Integer.parseInt (properties.getString ("NumOfMeasurementHosts"));
		    for (int n=1;n <= numMHosts; n++)
			{
			    String mhost = properties.getString ("MeasurementHost"+new Integer (n).toString ());
			    String sAddr = properties.getString ("MeasurementHostSubnet"+new Integer (n).toString ());
			    measurementHostsTable.put ((String)sAddr, (String)mhost);
			}
		}
		else
		    {
			  NetworkMonitorServer.nmlogger.info ("Use probes ? false");
		    }
		    
	    }
	 catch( Exception e )
	     {
		    System.out.println( "Unable to read  file: "+nmConfigFile+ "hence using system defaults");
		    NetworkMonitorServer.nmlogger.warn ( "Unable to read  file: "+nmConfigFile+ 
							 "hence using system defaults");
	     }	
    }

  public void setBBProperties (String bbConfigFile)
    {
	PropertyResourceBundle properties=null;
	 try
	    {
		properties = new PropertyResourceBundle(new FileInputStream(bbConfigFile));
		NetworkMonitorServer.nmlogger.info ("BB Configuration Properties from :"+bbConfigFile);
		dbServer = properties.getString("DBServer");
		dbLogin = properties.getString("DBLogin");
		dbPassword = properties.getString("DBPassword");
		NetworkMonitorServer.nmlogger.info ("BB MySQL DB server:"+dbServer+" user:"+
						    dbLogin+" passwd:"+dbPassword);
		numberOfDBConnections = Integer.parseInt( properties.getString("DBConnections") );
	    }
	 catch( Exception e )
	     {
		    System.out.println( "Unable to read  file: "+bbConfigFile+ "hence using system defaults");
		    NetworkMonitorServer.nmlogger.warn ( "Unable to read  file: "+
							 bbConfigFile+ "hence using system defaults");
	     }
    }

    /**
        This method returns bitwise AND of the given ip address and
        subnet mask. It returns null on error.
    */
    public String getSubnetAddr (String ipAddress, String subnetmask)
    {
	 try
        {
            // Get the InetAddress objects from dotted notation strings
            byte ip[] = InetAddress.getByName(ipAddress).getAddress();
            byte subnetMask[] = InetAddress.getByName( subnetmask ).getAddress();

            byte networkAddress[] = new byte[4];
            String address = new String();
            for( int i = 0; i < 4; i++ )
            {
                // compute the netork id of each octed by bitwise AND of corresponding octed of
                // ip address and subnet mask. Note: In java, bitwise operations is available
                // only for integer(int) and not for other data types so we need to cast the octets
                // to integers and revert the result back to byte.
                networkAddress[i] = (byte)((int) ip[i] & (int)subnetMask[i]);

                // removing negative sign
                int temp = networkAddress[i];

                // java does not support unsigned i.e. numbers greater than 128 is
                // taken as negative numbers in byte data type...thats why we are adding 256
                // to make them positive again.
                if (  temp < 0 )
                    temp += 256;
                // converting the network-id to dotted notation format
                if ( i == 0 )
                    address += temp;
                else
                    address += "." + temp;
            }
	    NetworkMonitorServer.nmlogger.info ("subnet addr:"+address+
						" from ipaddr and mask of"+
						ipAddress+" "+subnetmask); 
            return new String(address); // return network-id
        }
        // failure: return null.
	 catch ( UnknownHostException e ) { }//e.printStackTrace(); 
        return null;
    }

    public org.omg.CORBA.Object lookupClient (String cname, 
					      String [] args)
	throws NetworkProbesException
    {
	
	ORB orb = ORB.init( args, null );
	org.omg.CORBA.Object obj =null;
	
	NamingContextExt rootContext = null;
	NamingContextExt root;
	try{
	// Resolve NameService
	obj =
	    orb.resolve_initial_references ("NameService");
	rootContext = NamingContextExtHelper.narrow (obj);
	}catch (Exception e)
	    {
		throw new NetworkProbesException ("Unable to find Naming Service to lookup NMEvent client!",true);
	    }
	if (rootContext==null)
	    throw new NetworkProbesException ("Unable to find Naming Service to lookup NMEvent client!",true);
	
	try{
	// Lookup the client object)
	obj = rootContext.resolve_str (cname);
	}
	catch (NotFound nf)
	    {
		throw new NetworkProbesException ("Unable to find name:"+cname+" in Naming!",true);
	    }
	catch (InvalidName inn)
	    {
		throw new NetworkProbesException ("Invalid name:"+cname+" error while lookup in Naming!",true);
	    }
	catch (CannotProceed cp)
	    {
		throw new NetworkProbesException ("Cannot proceed for name:"+cname
						  +" error while lookup in Naming!",true);
	    }
	return obj;
    }

    public static void main (String args[])
    {

	String nmConfigFile = new String ("cfg"+ 
                                 System.getProperty ("file.separator") + 
                                 "BandwidthBroker" + 
                                 System.getProperty ("file.separator") + 
                                 "nmconfig.prop");
	try{
	NetworkMonitorImpl nm = new NetworkMonitorImpl (nmConfigFile, args);
	}catch (Exception eee)
	    {
		eee.printStackTrace ();
	    }
	ProbeSetup ps=null;
	try{
	    // gap=100 size=500 tos=0 #packets=10
	    // all time values in usecs
	    ps = new ProbeSetup ("impdb");
	    String pid =  ps.startProbe ("172.27.4.6", 
					 "172.26.4.6", 
					 "science", 
					 "100", 
					 "500", 
					 "0", 
					 "10",
					 "30000000");
	} catch ( JAXBException ee) {
		    System.err.println("probesetup:JAXBException: " + ee);
		}
	
	
	// Sleep 10 minutes, then exit.
	System.err.println("Writer done, sleeping.");
	try {
	    Thread.currentThread().sleep(10*60*1000);
	} catch(InterruptedException e) {
	    // break;
	}
	System.err.println("Probe shutdown");
	ps.probeShutdown ();
    }
}
