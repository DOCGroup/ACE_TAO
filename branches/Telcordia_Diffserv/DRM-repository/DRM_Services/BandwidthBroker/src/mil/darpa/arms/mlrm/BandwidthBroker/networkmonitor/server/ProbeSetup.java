//$Id$
/*
 * ProbeSetup.java - setting up the probe 
 *
 * The DARPA ARMS PROGRAM
 * This software is governed by Government Purpose Rights
 * Contract No: NBCHC030132
 * Telcordia Technologies, Inc.
 * One Telcordia Drive
 * Piscataway, NJ 08854-4157
 *
 *
 */

package mil.darpa.arms.mlrm.BandwidthBroker.networkmonitor.server;
import mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.*;
import mil.darpa.arms.mlrm.BandwidthBroker.networkmonitor.server.db.*;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.*;
import java.io.StringWriter;
import java.io.StringReader;

import javax.xml.transform.stream.StreamSource;
import javax.xml.bind.Marshaller;
import javax.xml.bind.Unmarshaller;
import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBException;

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
import imp.xml.*;
import java.sql.*;

/**
 * Class for setting up probe
 * Authors: Kirthika Parmeswaran , Allen Mcintosh
 */

public class ProbeSetup
{
    private Marshaller m;
    private Unmarshaller u;
    private Channel server;
    private ObjectFactory factory;
    private HSQLDBHome hsqldbHome = null;
    // Flag that continues/discontinues probe
    private boolean stopFlag = false;
    private JAXBContext jc =null;
    private ClientChannelizerManager ccm=null;
    private Channelizer cz=null;
    private TreeMap tm;
    private String serverName = null;
    
    ProbeSetup (String servername) 
	throws JAXBException
    {
	serverName = servername;
		
	String h = "localhost";
	try {
	    int n;
	    h = InetAddress.getLocalHost().getHostName();
	    if((n = h.indexOf('.')) >= 0)
		h = h.substring(0, n);
	} catch (UnknownHostException e) {
	    System.err.println("Weird exception getting hostname:");
	}
	
	// Kirthika: IS host needed?
	
	System.err.println("This host is " + h);
	imp.Global.host = h;

	// JAXB boilerplate
	factory = new ObjectFactory();
	
	// Changed to add class loader - MRC
	jc = JAXBContext.newInstance("imp.xml", this.getClass().getClassLoader() );

	u = jc.createUnmarshaller();
	m = jc.createMarshaller();
	m.setProperty(Marshaller.JAXB_FORMATTED_OUTPUT, Boolean.TRUE);

	// Set up the connection to the server.  We need the name of the
	// server and the port number.  The name field is arbitrary.
	tm = new TreeMap();
	tm.put("name", "To:" + serverName);
	tm.put("host", serverName);
	tm.put("port", Integer.toString(Ports.EXTERNAL_CONFIG_CONTROL_PORT));

	// Build a channelized socket to the server
	
	ccm = new ClientChannelizerManager();
	cz  = ccm.getChannelizer(tm);
	
	// Open a channel to the server, and start a thread to read
	// results.
	
	String className = "imp.cfg.external.ServerInstance";
	String channelName = "external:" + imp.Global.host + ":dlj";// impNode + ":dlj";
	server = new Channel(tm, className, channelName, cz);

	//NOTE:: If having more than one channel from same host, the
	//channel name should be different else there is
	// reconnection and it unleashes some other probes imp bug.
    }

    public String startProbe (String impNode,  
			      String remoteImpNode, 
			      String sink, 
			      String gap, 
			      String size,
			      String tos,
			      String pkts,
			      String period) // should be >= 30sec for proper functioning of probe 
	throws JAXBException
    {
	System.err.println("server is " + serverName);
	System.err.println("imp is " + impNode);
	System.err.println("remoteImp is " + remoteImpNode);
	System.err.println("sink is " + sink);
	System.err.println("gap " + gap);
	System.err.println("size " + size);
        System.err.println("tos" + tos);
	System.err.println("# pkts " + pkts);
	System.err.println("period " + period);

	// Now build a DLJ request and send it to the server.
	// Sent the output to a NMSink (which populates the results into incore HSQLDB)
 	// This program hardwires the emitter and collector classes.
	
	DljRequest dreq = factory.createDljRequest();
	
	// long period = 1*60*1000000L;
	dreq.setPeriod(period);
	dreq.setTarget(remoteImpNode);
	dreq.setImp(impNode);
	long periodValue = Long.parseLong (period); 
	long startMicros = periodValue * (1000 * System.currentTimeMillis() / periodValue);
	dreq.setEndTime("0");
	dreq.setStartTime(""+startMicros);
	dreq.setCollectorClass("imp.active.LinuxDljCollector");
	
	// This emitter works for nodes with IRIGB signal boards
	//dreq.setEmitterClass("imp.active.RtcEmitter");
	//This emitter does synchronization using the on host clock
	dreq.setEmitterClass("imp.active.MicroEmitter");
	System.err.println("Setting emitter class: "+dreq.getEmitterClass ());
	RequestClient rcl = factory.createRequestClient();
	rcl.setForeignTag("hello-nm");
	rcl.setClient(sink + ":" +
		      imp.Ports.SINK_PORT + ":" +
		      "mil.darpa.arms.mlrm.BandwidthBroker.networkmonitor.server.NMSink");
	dreq.getRequestClient().add(rcl);
	
	dreq.setGap(gap);
	dreq.setTos(tos);
	dreq.setSize(size);
	dreq.setNumPackets(pkts);
	
	Message msg;
	StringWriter w;
	
	System.err.println("SENDING:");
	m.marshal(dreq, System.err);
	
	// marshal the XML to a string, and send the string to the server
	
	w = new StringWriter();
	m.marshal(dreq, w);
	msg = new Message("xml", w.toString());
	server.send(msg);

	/* Formula: If the measurement request duplicates an existing
	  measurement (means same period, gap, packet size, tos etc.)
	  then
	  if the foreign tag  is different
	  then
	  clone the measurement
	  else
	  if the client is the same (string comparison)
	  then
	  just ack the request
	  else
	  its an error (same measurement, same cookie, different client) 
	*/
    
	// Get ACK
	String probeID = readAck(server);
	return probeID; 

	//while (getStopFlag () != false)
	//  System.out.println ("ProbeSetup done");
	

	//@todo: Work on better shutdown of sink and sevrer threads too
	
    }

    public ProbeDefaults queryProbeDefaults (String probeNode)
	throws JAXBException
    {
	String results=null;
		
	// Build a default query request
	DefaultQuery q = factory.createDefaultQuery();
	
	q.setImp(probeNode);
	q.setForeignTag("hello-nm-world");
	
	Message msg;
	StringWriter w;
	
	System.err.println("SENDING PROBE DEFAULTS QUERY:");
	m.marshal(q, System.err);
	w = new StringWriter();
	m.marshal(q, w);
	msg = new Message("xml", w.toString());
	server.send(msg);
	
	// Now wait for the reply
	String gap=null;
	String packetSize=null;
	String tos=null;
	String packetsNum=null;
	String measurementPeriod =null;
	boolean nowBreak=false;
	for(int i=0;i<3;i++) {
	    // try {
		System.err.println("Waiting for an DefaultsReply message");
		msg = server.recv();
		String xml = msg.get("xml");
		if(xml == null) {
		    System.err.println("Null message!");
		    continue;
   		}
		
                // Build a StreamSource to read the XML, then use JAXB to
                // parse it.
		
		StreamSource xmlSrc = new StreamSource(new StringReader(xml));
		Object o = u.unmarshal(xmlSrc);
			
		if ((o instanceof DefaultReply))
		    {
			DefaultReply data = (DefaultReply)o;
			ListIterator li;
			li = data.getProperty().listIterator();
			while(li.hasNext()) 
			    {
				imp.xml.Property prop = (imp.xml.Property)li.next();
				String name = prop.getName();
				if(name.equals("Dlj.Gap")) 
				    gap = prop.getValue();
				if(name.equals("Dlj.Size")) 
				    packetSize = prop.getValue();
				if(name.equals("Dlj.Tos")) 
				    tos = prop.getValue();
				if(name.equals("Dlj.NumPackets")) 
				    packetsNum = prop.getValue();
				if(name.equals("Dlj.Period")) 
				    measurementPeriod = prop.getValue();
			    }
			nowBreak=true;
		    }
		if (nowBreak)
		    break;
	    }// end of for
	NetworkMonitorServer.nmlogger.info ("Got DefaultReply, the defaults: gap="+gap+
					    " packetSize="+packetSize+" tos="+tos+
					    " packetsNum="+packetsNum+ " measurementPeriod="+measurementPeriod);
	return new ProbeDefaults (gap, 
				  packetSize, 
				  tos, 
				  packetsNum, 
				  measurementPeriod);
    }

    private synchronized void setStopFlag (boolean value)
    {
	stopFlag=value;
    }
    
    private synchronized boolean getStopFlag ()
    {
	return stopFlag;
    }
    
    public void probeShutdown ()
    {
	setStopFlag (true);
	/*try {
	    Thread.join();
	}catch (InterruptedException e)
	    {
		System.out.println ("Probe shutdown interrupted");
		}*/
    }

    private String readAck(Channel c) 
	throws JAXBException
    {
	String probeID=null;
	// Make sure an ack is received else measurement not started 
	for(;;) {
	    try {
		System.err.println("Waiting for an ACK message");
		Message msg = server.recv();
		String xml = msg.get("xml");
		if(xml == null) {
			System.err.println("Null message!");
			continue;
   		}

                // Build a StreamSource to read the XML, then use JAXB to
                // parse it.

		StreamSource xmlSrc = new StreamSource(new StringReader(xml));
		Object o = u.unmarshal(xmlSrc);
                
		//String client=null;
                if ((o instanceof RequestAck))
		    {
			RequestAck ack = (RequestAck)o;
			probeID = ack.getTag ();
			//	client=ack.getAckClient ();
			//
			break;
		    }

		if ((o instanceof imp.xml.Error))
		    {
			imp.xml.Error er = (imp.xml.Error)o;
			probeID = er.getTag ();
			//	client=ack.getAckClient ();
			throw new JAXBException ("Error in starting probe");
		    }
	    } catch (Exception e) {
		System.err.println("ProbeSetup::startProbe::Exception: " + e);
		throw new JAXBException (e.toString ());
	
	    }
	}
	return probeID;
	
    }
    
    
    public static void main (String args[]) 
    {

	// args: ProbeSetup <dbHost> <imp1Host> <imp2Host> <sinkHost> <gap> <packetSize> <TOS> <#packets> <period>
	String h = null;
	try {
	    int n;
	    h = InetAddress.getLocalHost().getHostName();
	    if((n = h.indexOf('.')) >= 0)
		h = h.substring(0, n);
	} catch (UnknownHostException e) {
	    System.err.println("Weird exception getting hostname:");
	    System.err.println(e);
	    System.exit(1);
	}
	
	// Kirthika: IS host needed?
	
	System.err.println("This host is " + h);
	imp.Global.host = h;
	ProbeSetup ps = null;
	try{
	// all time values in usecs
	    ps = new ProbeSetup ("impdb");
	    ps.startProbe("172.27.4.6", 
			  "172.26.4.6", 
			  "science.research", 
			  "100", 
			  "500", 
			  "0", 
			  "10",
			  "30000000");
	} catch ( JAXBException ee) {
	    System.err.println("probesetup:JAXBException: " + ee);
	}


	// Sleep 10 minutes, then exit.
	System.err.println("Writer done, sleeping.....");
	try {
		Thread.currentThread().sleep(10*60*1000);
	} catch(InterruptedException e) 
	    {
	    //		break;
	    }
	ps.probeShutdown ();
	
    }
    
}
