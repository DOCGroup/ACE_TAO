//$Id$
/*
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

/** Performs periodic monitoring of performance statistics and sends out events*/
public class PeriodicEventSupplierMonitor implements EventSupplierMonitor
{
    long _heartbeatPeriod =0;
    //NetworkMonitorImpl _nm = null;
    NetworkMonitorCallback _nm = null;
    long _measurementPeriod=0;
    long _averagingWindow=0;
    String _eventConsumer=null;
    String _cookie=null;
    String _probeID=null;
    String _threadName=null;
    boolean _status=true;
    Logger _mylogger=null;
    MeasurementEndpointsHolder _latencyInfoForEvent;
    MeasurementEndpointsType _meType;
    volatile Thread _monitor;
    NMEventConsumer _client;
    String _monitorName=null;

    public PeriodicEventSupplierMonitor (NetworkMonitorCallback nmImpl, 
					 String monitorName,
					 Logger mylogger,
					 long period,
					 long measurementPeriod,
					 long averagingWindow,
					 String eventConsumer,
					 org.omg.CORBA.Object nmclient,
					 String cookie,
					 String probeID,
					 MeasurementEndpointsHolder latencyInfoForEvent,
					 MeasurementEndpointsType meType)
    {
	_heartbeatPeriod = period;
	_nm = nmImpl;
	_mylogger=mylogger;
	_measurementPeriod= measurementPeriod;
	_averagingWindow= averagingWindow;
	_eventConsumer= eventConsumer;
	_cookie= cookie;
	_probeID= probeID;
	_status=true;
	_meType = meType;
	_latencyInfoForEvent = latencyInfoForEvent;
	_client = NMEventConsumerHelper.narrow (nmclient);
	_monitorName=monitorName;
    }
    
    public void start ()
    {
	_monitor = new Thread (this);
	_monitor.start();
    }
    
    public void stop ()
    {
	//	_status=false;
	_monitor=null;
	if (_mylogger != null)
	    _mylogger.info (_threadName+": monitor status changed to STOP..."); 
    }

    public boolean getStatus ()
    {
	//return _status;
	if (_monitor == Thread.currentThread())
	    return true;
	else
	    return false;
    }
    
    public void run ()
    {
	Thread thisThread = Thread.currentThread();
	_threadName = thisThread.getName ();

	System.out.println (_threadName+":PeriodicEventSupplierMonitor:STARTED to monitor ...");
	long delay=0;
        while (getStatus ()) {
	    if ( _mylogger !=null) 
		_mylogger.info (_threadName+":Continuing to monitor ..."); 
	    System.out.println (_threadName+":Continuing to monitor ..."); 
            try {
                thisThread.sleep(_heartbeatPeriod/1000);
            } catch (InterruptedException e){
            }
            // get the latency and send event
	    // Access the measurement DB and return the latency as per the
	    // avergae measurement window period. Its OK to have a tolerance in the time as the getDelay()
	    // function returns only the latest and greatest result sorted by time in the descending order
	    long instant = System.currentTimeMillis ()*1000 - 300000000;
	    try {
		delay = _nm.getDelay (_probeID, 
					   _averagingWindow, 
					   _measurementPeriod, 
					   instant);
		
		if ( _mylogger !=null) 
		    _mylogger.info ( _threadName+":PERIODIC DELAY EVENT ="+new Long (delay).toString () +
					    " at instant:"+new Long (instant).toString ()); 
		System.out.println (_threadName+":PERIODIC DELAY EVENT ="+new Long (delay).toString () +
				    " at instant:"+new Long (instant).toString ()); 
		 
		
	    }
	    catch (Exception e)
		{
		    if (_mylogger !=null) 
		   _mylogger.warn (_threadName+":"+e.toString ()+
						       "  Exception while looking up delay at:"+
						       new Long (instant).toString ()); 
		    System.out.println (_threadName+":"+e.toString ()+
					"  Exception while looking up delay at:"+
					new Long (instant).toString ()); 
		    // e.printStackTrace ();
		    break;
		}

	    try{
		// fill delay, send event
		send_event (delay);
	    }catch (Exception e)
		{
		    if (_mylogger !=null) 
		   _mylogger.warn (_threadName+":"+e.toString ()+
						       "  Exception while sending event to:"+
						       _eventConsumer); 
		    System.out.println (_threadName+":"+e.toString ()+
						       "  Exception while sending event to:"+
						       _eventConsumer); 
		    // e.printStackTrace ();

		    // Unsubscribe from BB DB
		    try {
		    cleanup ();
		    } catch (Exception ee)
			{
			    	NetworkMonitorServer.nmlogger.warn ("PeriodicEventSupplierMonitor:Error cleaning up"+
								    " from BandwidthBroker Database!");
				ee.printStackTrace ();
			}
		    break;
		}
	}
	if ( _mylogger !=null) 
	    _mylogger.info (_threadName+":Stopped to monitor ..."); 
	
	System.out.println (_threadName+":PeriodicEventSupplierMonitor:STOPPED to monitor ..."); 
    }
    
    public void cleanup ()
	throws Exception
    {
	_mylogger.info (_threadName+":Cleanup monitor entry in BB DB ..."); 
	// get connection to BB DB
	NRDB nrdb = new NRDB ();
	if (nrdb!=null)
	    {
		nrdb.removeNetworkMonitorEventSubscriptions (_monitorName);
		nrdb.commit ();
		nrdb.releaseConnection ();
	    }
	
    }

    public void send_event (long delay)
	throws Exception
    {
	if (getStatus ())
	    {
		switch (_meType.value ())
		    {
			
		    case 0:  // latency betn flow endpoints
			{
		    
			    LatencyBetnFlowEndpoints[] latencyBetnFlowEndpointsSeq = 
				_latencyInfoForEvent.value.flowLatencyInfo ();
			    LatencyBetnFlowEndpoints latencyStruct = latencyBetnFlowEndpointsSeq [0];
			    LatencyPerQoS latencyPerClass = latencyStruct.latencyPerClassSeq[0];
			    Latency latency = latencyPerClass.availableLatency;
			    latency.delayTime = delay;	
			    
			    _mylogger.info ("periodic event between flow endpoints built");
			    
			    break;
			}
		    case 1: // latency betn host endpoints
			{
			    LatencyBetnHosts[] latencyBetnHostsSeq =
				_latencyInfoForEvent.value.hostsLatencyInfo ();
			    LatencyBetnHosts latencyStruct = latencyBetnHostsSeq [0];
			    LatencyPerQoS latencyPerClass = latencyStruct.latencyPerClassSeq[0];
			    Latency latency = latencyPerClass.availableLatency;
			    latency.delayTime = delay;	
			    
			    _mylogger.info ("periodic event between hosts built");
			    
			    break;
			}
		    case 2: // latency betn pool endpoints
			{
			    LatencyBetnPools[] latencyBetnPoolsSeq = 
				_latencyInfoForEvent.value.poolLatencyInfo ();
			    LatencyBetnPools latencyStruct = latencyBetnPoolsSeq [0];
			    LatencyPerQoS latencyPerClass = latencyStruct.latencyPerClassSeq[0];
			    Latency latency = latencyPerClass.availableLatency;
			    latency.delayTime = delay;	
			    
			    _mylogger.info ("periodic event between pools is built");
			    
			    break;
			}
		    }
		
	    }
	if (getStatus ())
	    {
		// now sending the event
		
		NMEvent event = new NMEvent (_cookie, 
					     _meType, 
					     _latencyInfoForEvent.value);
		_client.push_NMEvent (event);
		_mylogger.info ("EVENT SENT to "+_eventConsumer);
	    }
	
    }


}
