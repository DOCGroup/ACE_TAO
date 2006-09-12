//$Id$
/*
 * ProbeControlImpl.java - the probe monitor
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
import mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.*;
import mil.darpa.arms.mlrm.BandwidthBroker.networkmonitor.server.db.*;
import javax.xml.transform.stream.StreamSource;
import javax.xml.bind.Marshaller;
import javax.xml.bind.Unmarshaller;
import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBException;
import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import mil.darpa.arms.mlrm.BandwidthBroker.*;
import mil.darpa.arms.mlrm.BandwidthBroker.ProbeControlPackage.*;
import mil.darpa.arms.mlrm.BandwidthBroker.CommonDef.*;
import java.net.*;
import java.io.*;
import imp.ipc.ClientChannelizerManager;
import imp.ipc.ServerChannelizerManager;
import imp.ipc.Channelizer;
import imp.ipc.Channel;
import imp.ipc.Message;
import imp.Ports;
import imp.xml.BootReply;
import imp.xml.ObjectFactory;
import imp.xml.DljRequest;
import imp.xml.RequestClient;


public class ProbeControlImpl extends ProbeControlPOA
{
    public String impServerHost="impdb";
    public String impSinkServerHost="science";
    public ProbeSetup psetup = null;

    public ProbeControlImpl (String server,
			     String sinkServer)
	throws ProbeControlException
    {
	impServerHost=server;
	impSinkServerHost=sinkServer;
	
	// Run the sink server
	// Start Sink Server
	Thread ss = new Thread() {
                public void run() 
		    //   throws ProbeControlException 
		{
		    try {
			startSinkServer();
		    }catch (ProbeControlException pxe)
			{
			    System.err.println ("Probes Sink Server starting error!"); 
			    pxe.printStackTrace ();
			    
			}
		}
            };
	ss.start();
	try{
	// Get the probe manager ready
	    psetup = new ProbeSetup (server);
	}catch (JAXBException je)
	    {
	
		NetworkMonitorServer.nmlogger.warn ("Error setting up probes! probeserver="+impServerHost+ 
					    " and the probe sinkserver="+impSinkServerHost);
		throw new ProbeControlException ("Error setting up probes!", false);
	    }
	NetworkMonitorServer.nmlogger.info ("ProbeControlImpl:probeserver="+impServerHost+ 
					    " and the probe sinkserver="+impSinkServerHost+ 
					    " have started");
					    
    }

    private void startSinkServer () 
	throws ProbeControlException 
    {
	try{
	    String h = null;

	    int n;
	    h = InetAddress.getLocalHost().getHostName();
	    if((n = h.indexOf('.')) >= 0)
		h = h.substring(0, n);
	    System.err.println("This host is " + h);
	    imp.Global.host = h;

	    // make sure the path is in classpath
	    imp.Global.xmlContext = JAXBContext.newInstance ("imp.xml");
	    imp.Global.clientManager = null; // new ClientChannelizerManager();
	    new ServerChannelizerManager(imp.Ports.SINK_PORT);
	}catch (Exception j)
	    {
		//j.printStackTrace ();
		//probeShutdown ();
		NetworkMonitorServer.nmlogger.warn ("Error starting sink server"+impSinkServerHost);
		throw new ProbeControlException ("Error starting sink server", false);
	    }
    }

    // query probe defaults
    public void query_configuration(ProbeEndpoints pe, 
				    ProbeParametersHolder pp) 
	throws ProbeControlException
    {
	String src = pe.sourceHostName;
	String dst = pe.destinationHostName;
	ProbeDefaults pd=null;
	try{
	    pd = psetup.queryProbeDefaults(src);
	} catch ( JAXBException ee) {
	    NetworkMonitorServer.nmlogger.warn ("probequerydefaults:JAXBException: " + ee);
	    throw new ProbeControlException ("probe defaults query error", true);
	}
	ProbeParameters pparams = new ProbeParameters (Long.parseLong (pd.getMeasurementPeriod ()), 
						       Integer.parseInt (pd.getPacketSize ()),
						       Integer.parseInt (pd.getPacketsNum ()),
						       Integer.parseInt (pd.getGap ()));
	pp.value=pparams;
    }

    public String getTOSFromQoS (QOSRequired q)
    {
	// get approperiate mapping to QOSRequired enum values
	String tos = new String ("0");
	switch (q.value())
	  {
	  case QOSRequired._highReliability :
	    {
	      // With higher WFQ size
	      tos = "32";
	      break;
	    }
	  case QOSRequired._highPriority :
	    {
	      // low latency, low priority queue size
	      tos = "40";
	      break;
	    }
	  case QOSRequired._highThroughput :
	    {
	      // raw physical bw class
	      tos = "24";
	      break;
	    }
	  default:
	    tos = new String ("0");
	  }
	NetworkMonitorServer.nmlogger.info ("DSCP = "+tos+" from QoSRequired value="+
					    new Long (q.value ()).toString ());
	return tos;
    }

    // Start a probe
    public void start_measurement(ProbeEndpoints p, 
				  ProbeControlSpec ps, 
				  org.omg.CORBA.StringHolder probeID) 
	throws ProbeControlException
    {
	String src = p.sourceHostName;
	String dst = p.destinationHostName;
	
	// Get the actual TOS digit given the QoSRequired enum value
	String tos  = getTOSFromQoS (ps.qos);
		
	String period = new Long (ps.pp.measurementPeriod).toString ();
	String packetSize = new Integer (ps.pp.packetSize).toString ();
	String packetTrainLength = new Integer (ps.pp.packetTrainLength).toString ();
	String packetGap = new Integer (ps.pp.packetGap).toString ();
	
	String probe=null;
	try{
	    // period value in usecs
	    NetworkMonitorServer.nmlogger.info (" Sending DLJRequest to the probe server for src="+src+
						" dst="+dst+ " sink="+impSinkServerHost+" gap="+packetGap+
						" size="+packetSize+" DSCP="+tos+" numPAckets="+packetTrainLength+
						" periodicity="+period);
	    probe = psetup.startProbe(src, 
				      dst, 
				      impSinkServerHost, 
				      packetGap, 
				      packetSize, 
				      tos, 
				      packetTrainLength,
				      period);
	} catch ( JAXBException ee) {
	    NetworkMonitorServer.nmlogger.warn ("probesetup:JAXBException: " + ee);
	    throw new ProbeControlException ("probe setup error", true);
	}
	
	// make sure probeID is filled
	NetworkMonitorServer.nmlogger.info ("PROBE ID="+probe);
	probeID.value=probe;
    }

    public void stop_measurement(java.lang.String probeID) 
	throws ProbeControlException
    {
	// for later
    }
    
 //   public static void main (String args[])
 //   {
//	
//    }
}
