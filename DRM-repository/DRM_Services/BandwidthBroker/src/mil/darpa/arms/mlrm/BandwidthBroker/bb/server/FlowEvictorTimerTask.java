//$Id: FlowEvictorTimerTask.java
// This software is governed by Government Purpose Rights
// Contract No: NBCHC030132
// Telcordia Technologies, Inc.
// 1 Telcordia Drive
// Piscataway, NJ 08854-4157

// FlowEvictorTimerTask.java

// Author: Kirthika Parmeswaran
// This class implements the Evictor pattern to removed those flows
// that have been reserved but not committed

package mil.darpa.arms.mlrm.BandwidthBroker.bb.server;

import java.io.*;
import java.net.*;
import java.util.*;
import java.util.TimerTask;

import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import mil.darpa.arms.mlrm.BandwidthBroker.*;
import mil.darpa.arms.mlrm.BandwidthBroker.AdmissionControlPackage.*;
import mil.darpa.arms.mlrm.BandwidthBroker.CommonDef.*;
import mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.*;

public class FlowEvictorTimerTask extends TimerTask
{
    NRDB nrdb = null;
    BBDriver bd = null;

    public FlowEvictorTimerTask (BBDriver bbdriver) throws CDBException
    {
	bd = bbdriver;
	
    }

    /** At every timeout, a check will be made on the "purgeStatus"
	of only the flow record entries in "reserved" status. 

	If purgeStatus is 0, it is set to 1, if it is already 1, it is
	deleted. The flow is deleted since it signifies that the flow has
	been reserved but not yet commited and the timeout period has
	passed.
    */
    public void run ()
    {
	BBServer.bblogger.info ("FlowEvictorTimerTask invoked!");
	NRDB nrdb = null;
	AttributeValues av =null;
	FlowRecord [] frEntries = null;
	try{
	    // Since nrdb represents a connection, hold the lock 
	    // for as less time as possible, but it is also necessary that 
	    // the transaction be atomic
	    nrdb =  new NRDB ();
	    
	    // Delete those entries whose purge flag is set
	    frEntries = nrdb.findAllReservedFlowRecordsToPurge ();
	    if (frEntries != null)
	    {
		for (int i = 0 ; i < frEntries.length ; i++)
		    {
			av = frEntries[i].getAttributes ();
			String flowID = av.getValueForName (FlowRecord.ID);

			if (flowID == null)
			    throw new CDBException ();
 
			//getflowToken for bi flows			
			if (bd.deleteFlow(flowID,nrdb))
			    BBServer.bblogger.info ("Evicted flow: "+
						    flowID+
						    " as the reservation"+
						    " timeout has passed");
			else
			    BBServer.bblogger.warn ("Cound not evict flow: "+
						    flowID+
						    " eventhough the reservation"+
						    " timeout has passed");
			
		    }
	    }
	    // This time only those with PurgeStatus = 0 will be collected and set to 1
	    nrdb.purgeActivateReservedFlowRecords ();

	    nrdb.commit ();
	    
	    BBServer.bblogger.info ("Activated flow records for purging ");

	    if (nrdb != null)
	    nrdb.releaseConnection ();
	}catch (CDBException e)
	    {
		BBServer.bblogger.error ("Database exception while evicting "+
					 "reserved but not yet committed flow records!");
	    }
	
	
	BBServer.bblogger.info ("FlowTImerTask finished ");
    }
}




