//$Id$
/*
 * NMSink.java - the probe measurement sink
 *
 * The DARPA ARMS PROGRAM
 * This software is governed by Government Purpose Rights
 * Contract No: NBCHC030132
 * Telcordia Technologies, Inc.
 * One Telcordia Drive
 * Piscataway, NJ 08854-4157
 *
 * Author: Kirthika Parmeswaran, Allen Mcintosh
 */

package mil.darpa.arms.mlrm.BandwidthBroker.networkmonitor.server;
import mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.*;
import mil.darpa.arms.mlrm.BandwidthBroker.networkmonitor.server.db.*;
import imp.sink.*;
import imp.*;

import java.util.ListIterator;
import java.util.TreeMap;
import java.util.Map;

import javax.xml.bind.JAXBException;
import javax.xml.bind.Unmarshaller;
import javax.xml.bind.Marshaller;

import imp.ipc.Channel;
import imp.gui.PanelManager;
import imp.gui.StripChartPanel;
import imp.gui.StripChart;
import imp.xml.ImpData;
import imp.xml.DataAttr;
import imp.xml.DataItem;
import imp.gui.PanelManager;
import imp.gui.StatusPanel;
import imp.db.XmlUtils;

/** Sink class for the probe */

// Notes: to stop probe, best to grab record and drop it rather than stopping thread.

public class NMSink extends Thread {

    private String tos = "";
    private String tag= "";
    private String mTime ="";
   /** The status panel.
     */    
    private StatusPanel sp;
    /** Converts XML to objects.
     */    
    protected Unmarshaller u;
    /** Converts objects to XML.
     */    
    protected Marshaller m;
    /** The channel to listen to.
     */    
    private Channel c;
    
    /** Create a new DLJ probe sink that listens on a channel.
     * @param c The channel.
     */    
    public NMSink(Channel c) {
	this.c = c;
	if(imp.sink.Global.guiActive && imp.sink.Global.pm != null) {
	    sp = new imp.gui.StatusPanel("NM Sink");
	    imp.sink.Global.pm.register(sp, "NM Sink", imp.Global.host, null, null, null);
	   	}
	this.start();
    }

    /** Process a single XML message.  It should be an ImpData
     * message.
     * @param o The message.
     * @throws JAXBException JAXB was unhappy about something.
     */
    protected void processMessage(Object o) throws JAXBException
    {
	//tos=null;
	System.out.println ("NMSink:processMEssage:");
	if(!(o instanceof ImpData)) {
	    status("Bad data class: " + o.getClass().getName());
	    return;
	}
	ImpData data = (ImpData) o;
	tag = data.getTag (); 
	if (tag == null)
	    {
		status("No tag in ImpData");
	    }
	mTime = data.getTime ();
	if (mTime == null)
	    {
		status("No time in ImpData");
	    }
	if(data.getDataItem () == null) {
	    status("No data item list in ImpData");
	    return;
	}

	ListIterator li;
	li = data.getDataAttr().listIterator();
	while(li.hasNext()) 
	    {
		DataAttr attr = (DataAttr)li.next();
		String name = attr.getName();
		if (name != null)
		    {
			
			//if((tos == null || tos.length() == 0) && name.equals("tos")) 
			if(name.compareTo("tos")==0) 
			    {
				tos = attr.getValue();
				System.out.println ("TOS="+tos);
				NetworkMonitorServer.nmlogger.info ("Dlj response Impdata: tos DataAttr="+tos);
			    }

		    }
		
	    }

	double delay[] = new double[3];
	double jitter[] = new double[3];
	double loss[] = new double[1];
	delay[0] = delay[1] = delay[2] = 0.0;
	jitter[0] = jitter[1] = jitter[2] = 0.0;
	loss[0] = 0.0;

	li = data.getDataItem().listIterator();
	while(li.hasNext()) {
	    DataItem item = (DataItem)li.next();
	    String label = item.getLabel();
	    if(label.equals("mindelay"))
		delay[0] = extract(item);
	    else if(label.equals("maxdelay"))
		delay[1] = extract(item);
	    else if(label.equals("avgdelay"))
		delay[2] = extract(item);
	    else if(label.equals("mindv"))
		jitter[0] = extract(item);
	    else if(label.equals("maxdv"))
		jitter[1] = extract(item);
	    else if(label.equals("avgdv"))
		jitter[2] = extract(item);
	    else if(label.equals("loss"))
		loss[0] = extract(item);
	    else status("Unknown label: " + label);
	}
	
	String sDelay = new Double (delay[2]).toString ();
	String sJitter = new Double (jitter[2]).toString ();
	String sLoss = new Double (loss[0]).toString ();
	
	System.out.println ("Latency="+sDelay);

	try {
	// write to DB
	    NetworkMonitorServer.nmlogger.info ("NMDB.populateMeasurementInDB: tag="+tag+
						"time="+mTime+" delay="+sDelay+" jitter="+sJitter+
						" loss="+sLoss+" TOS="+tos);
	String id = NMDB.populateMeasurementInDB (tag,
						  mTime,
						  sDelay,
						  sJitter,
						  sLoss,
						  tos);
	
	System.err.println ("populated HSQLDB, id="+id);
	NMDB.printMeasurementRecord (id);
	}catch (CDBException ce)
	    {
		//	ce.printStackTrace ();
		  NetworkMonitorServer.nmlogger.warn("Exception populating probe measurement into DB: " + ce);
		//	throw new JAXBException ();
	    }
    }
 
   
    /** Extract a numeric value from a DataItem.
     * @param item The DataItem.
     * @return The value
     */
    private double extract(DataItem item) {
	double v = -1.0;
	try {
	    v = Double.parseDouble(item.getValue());
	} catch (NumberFormatException e) {
	    status("Bad number: " + item.getValue());
	}
	return v;
    }

    /** Display a message on the status panel, or on standard
     * error if there is no GUI.
     * @param s The message.
     */    
    protected void status(String s) {
        if(sp == null)
            System.err.println(s);
        else 
            sp.status(s);
    }

    /** Wait for messages and call the message processor.
     */    
    public void run() {
	status("Running");

	try {

	    u = imp.Global.xmlContext.createUnmarshaller();
	    m = imp.Global.xmlContext.createMarshaller();
	    m.setProperty(Marshaller.JAXB_FORMATTED_OUTPUT, Boolean.TRUE);

	} catch(JAXBException e) {
	    status("JAXBException starting NM sink instance: " + e);
	    return;
	}

	for(;;) {
	    status("Waiting for a message");
	    try {
		Object o = XmlUtils.readXmlObject(c, u);
		if(o == null) {
		    status("Null message");
		    continue;
		}
		processMessage(o);
	    } catch(JAXBException e) {
		status("JAXBException: " + e);
	    }
	}
    }
    

}
