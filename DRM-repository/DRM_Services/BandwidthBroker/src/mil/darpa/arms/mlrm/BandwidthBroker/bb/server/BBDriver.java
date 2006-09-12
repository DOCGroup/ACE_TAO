// $Id: BBDriver.java
// Contract No: NBCHC030132
// Telcordia Technologies, Inc.
// 1 Telcordia Drive
// Piscataway, NJ 08854-4157

// BBDriver.java

// Author : Kirthika Parmeswaran, Shrirang Gadgil
// This class drives the BB backend 

package mil.darpa.arms.mlrm.BandwidthBroker.bb.server;

import java.io.*;
import java.net.*;
import java.util.*;

import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import org.omg.CosNaming.*;
import org.omg.CosNaming.NamingContextPackage.*;
import mil.darpa.arms.mlrm.BandwidthBroker.*;
import mil.darpa.arms.mlrm.BandwidthBroker.AdmissionControlPackage.*;
import mil.darpa.arms.mlrm.BandwidthBroker.FaultMonitorPackage.*;
import mil.darpa.arms.mlrm.BandwidthBroker.CommonDef.*;
import mil.darpa.arms.mlrm.BandwidthBroker.L2Disc.*;
import mil.darpa.arms.mlrm.BandwidthBroker.ospf.*;

import mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.*;

/**
 * BBDriver class is instantiated by AdmissionControlServerImpl. While 
 * AdmissionControlClass provides a CORBA facade to bandwidth broker,
 * the main functionality of BB is embedded in this class.
 */
public class BBDriver
{
    public static int failAfterX = 0; // never fail
    public static int failWhen = 0; // never fail
    public static String flowProvisionerNameString = "FlowProvisioning"; //default
    public boolean multiDomain = false;

    // For reservation timeout, take it form props file 
    int flowReservationTimeout = 180000; // 3 mins
    int flowGarbageCollectionTimeout = 180000; // 3mins
    String bbConfigFile = new String ("./cfg/BandwidthBroker/bbconfig.prop");
    String dbServer = new String ("quasar.research.telcordia.com");
    String dbLogin = new String ("arms");
    String dbPassword = new String ("arms");
    String ospfRouter = "192.168.1.1";
    int numberOfDBConnections = 4;
    int configureNw = 1; // send configs to router/switches in network
    public NetworkMonitor networkMonitor;
    
   private void startEvictor () throws CDBException
   {
      // Start the Evictor Timer as a daemon thread to run every
      // flowReservationTimeout milliseconds. The daemon thread lets the
      // application exit even if the daemon threads havent*/

      Timer timer = new Timer (true);

      timer.schedule (new FlowEvictorTimerTask (this), 0, 
		       flowReservationTimeout);
      Timer timer2 = new Timer (true);
      timer2.schedule (new DelFlowsEvictor (), 0, 
			flowGarbageCollectionTimeout); 
   }

   /**
    * Constructors
    */
   // Added default constructor where config file name and sql trace file is determined 
   //   internally -  MRC
   // Sept 05 - as of right now this constructor is only used in the component
   //   implementation.  In the non-component the other constructor should
   //   be c
   public BBDriver () throws CDBException
   {

  
//	   this ( new String ("/deploy/cfg/BandwidthBroker/"+
//			             System.getProperty("file.seperator")+
//			             "cfg"+
//			             System.getProperty("file.seperator")+
//			             "BandwidthBroker"+
//			             System.getProperty("file.seperator")+
       this ( new String("/bbconfig.prop" ));
	   System.err.println("after googly");
	   
	  // Create standard properties for all components of this component type
      //   The component type being BandwidthBrokerComponent

	  String sqlTraceFile = new String ("data"+ 
                                 System.getProperty ("file.separator") + 
                                 "logs" + 
                                 System.getProperty ("file.separator") + 
                                 "bb.sqlTrace");

	  // DbUtil should be singleton
	  System.err.println( "Setting DbUtil trace");
      DbUtil.sqlTraceFile = sqlTraceFile;

   }
   // end added constructor - MRC

   public BBDriver (String bbConfigFileName) throws CDBException
   {
      setBBProperties (bbConfigFileName);
      NRDB.numConnections = numberOfDBConnections;
      NRDB.dbServerFQN = dbServer;
      NRDB.dbLoginName= dbLogin;
      NRDB.dbPasswordValue = dbPassword;
    
      //FindPaths.loadL3Paths (null, ospfRouter, true);
      L3PathManager.init (ospfRouter);
      // Fault Tolerance
      removeZombieFlows ();

      // Start the flow evictor timer.
      startEvictor ();
   }

    public void setBBProperties (String bbConfigFile)
    {
	PropertyResourceBundle properties=null;
	 try
	    {
		try {
		    properties = new PropertyResourceBundle(getClass().getResourceAsStream(bbConfigFile));
		} catch (Exception x) {
		    System.err.println("Exception trying to read bbconfig props: "+
				       "(this is expected in non-CCM mode");
		    // assume we are in non-component mode and got args
		    // from command line
		    properties = new PropertyResourceBundle(new FileInputStream(bbConfigFile));
		    BBServer.bblogger.info ("BB Configuration Properties from :"+bbConfigFile);
		}
		System.err.println("Properties is" + properties.toString());
		dbServer = properties.getString("DBServer");
		ospfRouter = properties.getString ("OspfRouter");
		dbLogin = properties.getString("DBLogin");
		dbPassword = properties.getString("DBPassword");
		BBServer.bblogger.info ("BB MySQL DB server:"+dbServer+" user:"+dbLogin+" passwd:"+dbPassword);
		numberOfDBConnections = Integer.parseInt( properties.getString("DBConnections") );
		BBServer.bblogger.info ("BB max DB connections ="+new Integer (numberOfDBConnections).toString());
		flowReservationTimeout = Integer.parseInt( properties.getString("FlowReservationTimeout") );
		BBServer.bblogger.info ("BB Uncommited Flow Request Eviction Timeout in milliseconds:"
					+new Integer (flowReservationTimeout).toString ());
		flowGarbageCollectionTimeout = Integer.parseInt( properties.getString("FlowGarbageCollectionTimeout") );
		BBServer.bblogger.info ("BB deleted Flow Record Eviction Timeout in milliseconds:"
					+new Integer (flowGarbageCollectionTimeout).toString ());
		configureNw = Integer.parseInt(properties.getString("ConfigureNetwork") );
		BBServer.bblogger.info ("Send configurations to network routers and switches ? "+
					new Integer (configureNw).toString());
		String fpn = properties.getString("FlowProvisionerName");
		if (fpn.compareTo ("") != 0)
		    flowProvisionerNameString = fpn;
		BBServer.bblogger.info ("Flow Provisioner to be used by this BB:"+flowProvisionerNameString);
	    }
	 catch( Exception e )
	     {
		    System.out.println( "Unable to read  file: "+bbConfigFile+ " hence using system defaults");
		    BBServer.bblogger.warn ( "Unable to read  file: "+bbConfigFile+ " hence using system defaults");
	     }
    }

   public class FlowReference
   {
      public String flowID;
      public int dscp;

      public FlowReference (String id, int dscp)
      {
         flowID = id;
	 this.dscp = dscp;
      }
   }
   
   /** 
    *  Process a new flow setup request for UNIO-DIRECTINAL flows, it should be invoked twice 
    * for BI-DIRECTIONAL flows
    *  returns flowID on success
    *          null on error
    *
    *  src_port_high and dst_port_high can be null in case of single port
    *  src_ip_mask and dest_ip_mask can be 255.255.255.255 in case of single
    *  hosts
    *  reserveAndCommit = true means the flow is configured in the network
    */
   public FlowReference processFlowSetupRequest (String src_ip_address,
				     String src_ip_mask,
				     String dst_ip_address,
				     String dst_ip_mask,
				     String src_port,
				     String src_port_high,
				     String dst_port,
				     String dst_port_high,
				     String protocol,
				     String serviceClass,
				     long bandwidthMin,
				     long extraBudget,
				     boolean reserveAndCommit,
				     int priority,
				     String reference
				     )
	  throws AdmissionControlException
   {
    	// Fault tolerance code.
	// flowSetupRequest is not an idempotent operation and needs special 
	// handling

	// check if there exist a flow record that has the supplied 
	// reference


	// If there is one, then this request is treated as duplicate

	// If the status of the flow is statusCommitted, then the previous 
	// instance of the bandwidth broker was able to complete the 
	// transaction before failure. Return the flowID and success

	// If the status of the flow is statusCommitting then the flow 
	// provisioner has not yet configured the request. We could issue
	// a request to flow provisioner to redo the configuration. Call to 
	// flow provisioner is idempotent.

      NRDB nrdb = null;
      String flowID = null;
      int dscp = 0;
      PTP [] path = new PTP [0];
      String inInterfaceID, outInterfaceID;
      String l3PathID = null;
      try
      {
        
         // Create new instance of NRDB automatically gets a connection
         // from the connnection pool
         nrdb = new NRDB ();


         // Find a flow with the given reference number.
	 if (reference.equals ("") == false)
	 {
	    FlowRecord fr = nrdb.findFlowRecordByReference (reference);
	    if (fr != null)
	    {
	       AttributeValues frv = fr.getAttributes ();

               String flowStatus = frv.getValueForName (FlowRecord.attrStatus);
		
               if ((flowStatus.equals (FlowRecord.statusCommitted) == true)
                   || (flowStatus.equals (FlowRecord.statusReserved) == true))
	       {
	          flowID = fr.getID ();
	          dscp = Integer.parseInt (frv.getValueForName (FlowRecord.attrDSCP));
		  nrdb.releaseConnection ();
                  return new FlowReference (flowID, dscp);
	          // return the flow reference
	       }
	       else if (flowStatus.equals (FlowRecord.statusCommitting) == true)
	       { 
                  flowID = fr.getID ();
                  // invoke flow provisioner to provision the flow
                  boolean rc = configFlow (nrdb, flowID, true);
                  // return the flow reference
                  dscp = Integer.parseInt (frv.getValueForName (FlowRecord.attrDSCP));
		  nrdb.releaseConnection ();
                  return new FlowReference (flowID, dscp);

	       }
	    }
	 }

         String serviceClassID = nrdb.findServiceClassByName (serviceClass);

         NRDB.HostsInfo hinfo = nrdb.getHostsInfo (src_ip_address,
                                             dst_ip_address);
         if (hinfo.isSrcHostName == true)
         {
            // Convert from hostname to ip address
            src_ip_address = hinfo.srcIP;
            src_ip_mask = hinfo.srcMask;
         }
         if (hinfo.isDestHostName == true)
         {
            // Convert from hostname to ip address
            dst_ip_address = hinfo.destIP;
            dst_ip_mask = hinfo.destMask;
         }

         if (hinfo.isPureL2Path)
         {
            inInterfaceID = hinfo.inInterfaceID; 
            outInterfaceID = hinfo.outInterfaceID;
            path = nrdb.lookupL2Path (inInterfaceID, outInterfaceID, false);
         }
         else // consider only pure L3
         {
            // Find the edges of the network through which the traffic will flow
            inInterfaceID = nrdb.findProviderEdge (src_ip_address, false);
            outInterfaceID = nrdb.findProviderEdge (dst_ip_address, false);

	    String ingressID = nrdb.getRouterOfL3Interface (inInterfaceID);
	    String egressID = nrdb.getRouterOfL3Interface (outInterfaceID);
	    l3PathID = nrdb.getL3PathID (ingressID, egressID);
	    
	    
	    /*
	    BBServer.bblogger.info ("ingressID ="+inInterfaceID+
				    " , egressID="+outInterfaceID+
				    ", serviceClassID="+serviceClassID);
	 
	    */
            path = nrdb.lookupPath (inInterfaceID, outInterfaceID, false);
         }


         // Find available bandwidth along the path for the 
         // service class
         long availableBW = nrdb.findAvailableBW (path, serviceClassID);

         // decide the bandwidth that can be allocated to the flow
         // based on some criteria
         long allocatedBW = decideBandwidth (availableBW, bandwidthMin, extraBudget); 

         if (allocatedBW > 0)
         {

	    tryToFail (1);
            nrdb.reserveBW (path, allocatedBW, serviceClassID);

	    // may throw and exception if dscps are consumed
	    dscp = nrdb.allocateDSCP (src_ip_address, dst_ip_address); 

            // Insert flow record in database
            // status defaults to reserved
	    tryToFail (2);
            flowID = nrdb.insertFlowRecord (src_ip_address, src_ip_mask, 
                                  src_port, src_port_high,
                                  dst_ip_address, dst_ip_mask,
                                  dst_port, dst_port_high,
                                  protocol, dscp, serviceClassID,
                                  bandwidthMin, extraBudget, allocatedBW,
                                  inInterfaceID, outInterfaceID, 
				  hinfo.isPureL2Path,
                                  reserveAndCommit, priority, l3PathID, reference);
                                

            // commit the changaes to database
            nrdb.commit ();
	    tryToFail (3);

            // If the flow has to be commited ...
            if (reserveAndCommit)
	    {
		// configure the flow
	       configFlow (nrdb, flowID, true);
	    }
	    tryToFail (4);
         }
         else
         {
            if (nrdb != null)
               nrdb.rollback ();
	 }
	 if (nrdb != null)
         nrdb.releaseConnection ();
      }
       catch (org.omg.CORBA.COMM_FAILURE se)
	  {
	      throw new AdmissionControlException ("CORBA System Exception while configuring flow!" +
						   se.toString());
	  }
      catch (Exception e)
      {
         flowID = null;
         e.printStackTrace ();
         try
         {
         if (nrdb != null)
            nrdb.rollback ();
         }
         catch (CDBException ce)
         {
	     BBServer.bblogger.warn ("CDBException caught while nrdb tries to rollback");
         }
	 
	 // e.printStackTrace();
	 throw new AdmissionControlException ("Exception process flow request:"+
						       e.toString());
      }
    
      if (flowID == null)
         return null;
      else
         return new FlowReference (flowID, dscp);
   }

   /**
    * Process a request the update an existing flow for new rate
    * in the same service class
    **/
   boolean processFlowUpdateRequest (String flowID, long newRate)
   {
      NRDB nrdb = null;
      BBServer.bblogger.info ("updateFlow invoked for flowID:"+flowID);
      boolean rc = true;
      long rate;
      String svcClassID;
      FlowRecord fr = null;
      try
      {
         nrdb = new NRDB ();
         fr = nrdb.findFlowRecord (flowID);
         if (fr == null)
	    return false;
		
         AttributeValues frv = fr.getAttributes ();
		
         String flowStatus = frv.getValueForName (FlowRecord.attrStatus);
		
         if (flowStatus.equals (FlowRecord.statusDown) != true)
         {
            boolean l2 = frv.getValueForName (FlowRecord.attrL2Endpoints).equals ("1");
	    rate = Long.parseLong (
	                  frv.getValueForName (FlowRecord.attrBWAlloc));
	    svcClassID = frv.getValueForName (FlowRecord.attrSvcClassID);
	    // find the current path of the flow
	    PTP [] path = 
	       nrdb.lookupPath (
		  frv.getValueForName (FlowRecord.attrInInterfaceID),
		  frv.getValueForName (FlowRecord.attrOutInterfaceID), l2);

	    // get the difference
	    rate = newRate - rate;
            long availableBW = nrdb.findAvailableBW (path, svcClassID);

	    if (availableBW >= rate)
               nrdb.reserveBW (path, rate, svcClassID);
	    else
	    {
               BBServer.bblogger.info ("Additional bandwidth not available for flow " +
	                               flowID);
	       nrdb.rollback ();
	       nrdb.releaseConnection ();
	       return false;
	    }
	    
	    AttributeValues av = new AttributeValues ();
            av.addNameValue (FlowRecord.attrStatus, FlowRecord.statusCommitting);
	    fr.setAttributes (av);
	    nrdb.commit ();
// Consistency bug : Shree
// If subsequent config_flow_release fails or 
// BB fails, we have deducted the updated bandwidth for all the links
// but when this flow is deleted as zombie flow, we will replenish
// only the old bandwidth value.

// the fix is to record the bandwidth difference in a new field
// of flow record and use it for accounting while deleting the zombie
// flow. The fix will incorporated at later point.

// we could have committed the new rate to the db to fix this problem
// But, some flow provisioners like Linux flow provisioners would
// need the old bandwidth value to remove the old configuration

// other fix could be provide an updateFlow API in flowProvisioner

	    if (configureNw == 1)
	    {
	       try
	       {
                  NRDB.FlowConfigInfo fci = nrdb.getFlowConfigInfo (fr);
	          config_flow_release (fci);
	       }
	       catch (Exception e)
	       {
                  nrdb.releaseBW (path, rate, svcClassID);
		  nrdb.commit ();
		  nrdb.releaseConnection ();
		  return false;
	       }
	    }

	    av = new AttributeValues ();
	    av.addNameValue (FlowRecord.attrBWAlloc, ""+newRate);
	    fr.setAttributes (av);
	    nrdb.commit ();

	    if (configureNw == 1)
	    {
	       try
	       {
	          config_flow_setup (fr, nrdb);
	       }
	       catch (Exception e)
	       {
	          // this exception is difficult because we have removed
		  // the flow from the router and we have failed
		  // to setup flow with new parameters
		  // at present do nothing

		  nrdb.releaseConnection ();
		  return false;
	       }
	    }

	    av  = new AttributeValues ();
            av.addNameValue (FlowRecord.attrStatus, FlowRecord.statusCommitted);
	    fr.setAttributes (av);
	    nrdb.commit ();
	    nrdb.releaseConnection ();
	 }
      }
      catch (CDBException e)
      {
	 try
	 {
	 if (nrdb != null)
	    nrdb.releaseConnection ();
	 }
	 catch (Exception x)
	 {
	 }
         BBServer.bblogger.warn ("No flowID:"+flowID+" found!");
	 return false;
      }
      return true;
   }
  
   /** 
    * Send configuration to routers / swiches in the future or any 
    * other device
    * setupRelease: true for setup, false for release
    */
   public boolean configFlow (NRDB nrdb, String flowID, boolean setupRelease) 
       throws AdmissionControlException
   {
      try
      {
	  FlowRecord fr = null;
	  // find the flow record in the table and get the details, if no flowID, exception
	  try{
	      fr = nrdb.findFlowRecord (flowID);
	  }catch (CDBException e)
	      {
		  BBServer.bblogger.warn ("No flowID:"+flowID+" found!");
	      }

         if (fr == null)
             return false;


	 // assume success.... will deal failures later
	 if (setupRelease == true)
	 {
	    if (configureNw == 1)
	    {
	       config_flow_setup (fr, nrdb);
	    }
	    // update the status to active
	    updateFlowStatus (fr, FlowRecord.statusCommitted); 
            nrdb.commit ();
	 }
	 else
	 {
	    if (configureNw == 1)
	    {
               NRDB.FlowConfigInfo fci = nrdb.getFlowConfigInfo (fr);
	       config_flow_release (fci);
	       //config_flow_release (fr, nrdb);
	    }
	    // remove the flow from the database
	 
	    /* No more removal, simply garbage collect so we have the flowID around for backup BB 
	     nrdb.removeFlow (flowID);
	     nrdb.commit ();
	    */
	 }
	    
      }
      catch (org.omg.CORBA.COMM_FAILURE ce)
	  {
	       BBServer.bblogger.warn ("Communication failure Exception "+
				    "configFlow() for flowID:"+flowID+" ....let backup take over");
	       System.exit (1);
	  }
      catch (Exception e)
      {
	 e.printStackTrace ();
	 try
	 {
	    if (nrdb != null)
	       nrdb.rollback ();
	 }
         catch (CDBException ce)
	 {
	    BBServer.bblogger.warn ("Exception in nrdb rollback in "+
				    "configFlow() for flowID:"+flowID);
	    //  throw new CDBException ();
	 }
	 nrdb=null;
         throw new AdmissionControlException ("Error while configuring flow!" +
                                              e.toString());
      }
      return true;
   }

   /** Commit flow that has been reserved already*/
   public boolean processFlowRequestCommit (String flowID)
	               throws AdmissionControlException
   {
	
      boolean rc = false;

      try
      {
         NRDB nrdb = new NRDB ();
         FlowRecord fr = nrdb.findFlowRecord (flowID);
         String flowStatus = fr.getAttributes().getValueForName (FlowRecord.attrStatus); 
	 if (flowStatus.equals (FlowRecord.statusReserved)== true)
	     {
		 updateFlowStatus (fr, FlowRecord.statusCommitting); 
		 nrdb.commit ();

		 rc = configFlow (nrdb, flowID, true);
		
	     }
	 else // status of flow record has changed potentially to "raincheck" due to network change
	     rc=false;
	  
	 if (nrdb != null)
	     nrdb.releaseConnection ();
		     
      }
      catch (Exception ecx)
	  {
	      BBServer.bblogger.warn ("Error committing flow ID="+flowID);
	      throw new AdmissionControlException ("Error committing flow ID="+flowID);
	  }
      return rc;
   }

   /** 
    * Delete flow request 
    */
   public boolean processFlowDeleteRequest (String flowID)
       throws AdmissionControlException
   {
       boolean rc = false;
      try
      {

         NRDB nrdb = new NRDB ();
	 
	 rc = deleteFlow (flowID,nrdb);
	 if (nrdb != null)
	     nrdb.releaseConnection ();
	 if (rc==false)
	     throw new Exception (); // just so it gets caught and carried to the application
      }
      catch (Exception e)
      {
         BBServer.bblogger.warn ("Delete flowID="+flowID+" Failed!");
	 throw new AdmissionControlException (
	         "Exception deleting flow, ID= "+flowID+ ":" +e.toString());
      }
      return rc;
   }


   private class BulkFlowDeleteThread extends Thread
   {
      Vector fciList;
      Vector successList = new Vector ();
      NRDB nrdb;

      BulkFlowDeleteThread (Vector fciList, NRDB nrdb)
      {
         this.fciList = fciList;
	 this.nrdb = nrdb;

      }

      public void run ()
      {
         for (int j = 0; j < fciList.size (); j++)
	 {
	    try
	    {
	       NRDB.FlowConfigInfo fci = (NRDB.FlowConfigInfo) fciList.get(j);
	       //rc = deleteFlow (fci.fr, fci.av, nrdb);
	       if (configureNw == 1)
		   config_flow_release (fci);
	       
	       successList.add (fci);
	    }
	    catch (Exception e)
	    {
	       System.out.println ("Error removing flow configuration");
	    }
	 }
         for (int j = 0; j < successList.size (); j++)
	 {
	    NRDB.FlowConfigInfo fci = (NRDB.FlowConfigInfo) successList.get(j);
	    synchronized (nrdb)
	    {
	       try
	       {
		   updateFlowStatus (fci.fr, FlowRecord.statusDown);

		  // don't remove
	          // nrdb.removeFlow (fci.flowID);
	          nrdb.commit ();
	       }
	       catch (CDBException e)
	       {
	          // can't do much!
		  // Zombie flow will remain in the database
	       }
	    }
	 }
      }
   }
   
   /** 
    * Delete all flows
    */
   public boolean processReleaseAllFlows ()
       throws AdmissionControlException
   {
      // This method is called when the Bandwidth Broker is 
      // switched into a standby mode.

      // Attempt is made here to quickly release all the flowss
      // admitted so far. All the flow records are obtained
      // grouped by ingress. A thread is spawned per ingress 
      // and flow deletion is performed.

      String flowID = "";
      boolean rc = false;
      try
      {

         NRDB nrdb = new NRDB ();
	 Vector flows = nrdb.getAllFlowRecords ();
	 Vector listOfLists = new Vector ();

	 Hashtable hashOfLists = new Hashtable ();

	 while (flows.size () > 0)
	 {
	    FlowRecord fr = (FlowRecord) flows.remove (0);
	    NRDB.FlowConfigInfo fci = nrdb.getFlowConfigInfo (fr);

	    // release Bandwidth occupied by the flow
	    String flowStatus = fci.av.getValueForName (FlowRecord.attrStatus);
		
	    if (flowStatus.equals (FlowRecord.statusDeleted) != true)
	    {
	       releaseBandwidth (fci.fr, fci.av, nrdb);
               updateFlowStatus (fr, FlowRecord.statusDeleted);
	    }

	    Vector fciList = (Vector) hashOfLists.get (fci.managedIP);
	    if (fciList == null)
	    {
	       fciList = new Vector ();
	       hashOfLists.put (fci.managedIP, fciList);
	       listOfLists.add (fciList);
	    }
	    fciList.add (fci);
	 }
	 // TO DO: Loop over keys of hashOfList
	 Thread [] threads = new Thread [listOfLists.size ()];
	 for (int i = 0; i < listOfLists.size (); i++)
	 {
	    Vector fciList = (Vector) listOfLists.get (i);
            BulkFlowDeleteThread  bfdt = 
	       new BulkFlowDeleteThread (fciList, nrdb);
	    bfdt.start ();
	    threads [i] = bfdt;
         }


	 // Wait for all the flows to be deleted.
	 for (int i = 0; i < threads.length; i++)
	 {
	    try 
	    {
	       if (threads [i].isAlive ())
	          threads [i].join ();
	    }
	    catch (InterruptedException ie)
	    {
	       // Ignore the exception
	    }
	 }

	 /*
	 for (int i = 0; i < flows.size (); i++)
	    rc = deleteFlow (flowID = (String) flows.get (i), nrdb);
	 */
	 if (nrdb != null)
	     nrdb.releaseConnection ();
      }
      catch (Exception e)
      {
	 e.printStackTrace ();
         BBServer.bblogger.warn ("Delete All flows failed.");
	 throw new AdmissionControlException ("Exception deleting all flows ");
      }
      return rc;
   }
    
   public boolean deleteFlow (String flowID, NRDB nrdb)
       throws CDBException
   {
      BBServer.bblogger.info ("deleteFlow invoked for flowID:"+flowID);
      boolean rc = true;
      FlowRecord fr = null;
      // find the flow record in the table and get the details, 
      // if no flowID, exception
      try
      {
	 fr = nrdb.findFlowRecord (flowID);
      }
      catch (CDBException e)
      {
         BBServer.bblogger.warn ("No flowID:"+flowID+" found!");
      }
	      
      if (fr == null)
      {
         return false;
      }
      else
      {
	 AttributeValues frv = fr.getAttributes ();
         return deleteFlow (fr, frv, nrdb);
      }
    }

    // This method is used to reacquire bandwidth in the current
    // path after the the path has changed due to fault or repair
    // reasons
    public boolean acquireBandwidth (NRDB nrdb, 
                                     NRDB.FlowConfigInfo fci) 
                                     throws CDBException

    {
       boolean l2 = fci.av.getValueForName (FlowRecord.attrL2Endpoints).equals ("1");
       // find the current path of the flow
       PTP [] path = nrdb.lookupPath (
		       fci.av.getValueForName (FlowRecord.attrInInterfaceID),
		       fci.av.getValueForName (FlowRecord.attrOutInterfaceID), 
		       l2, false);

			
       System.out.print ("New path is ");
       for (int i = 0; i < path.length; i++)
       {
          System.out.print (path [i].getID () +":");
       }
       System.out.println ("");
       // acquire the bandwidth from all the PTPs
       long availableBW = nrdb.findAvailableBW (path, fci.svcClassID);

       System.out.println ("Looking for " + fci.rate + "; Available = " + availableBW);
       if (availableBW < fci.rate)
       {
          return false;
       }

       nrdb.reserveBW (path, fci.rate, fci.svcClassID);
       // commit to the database
       nrdb.commit ();
       return true;

    }

    private void releaseBandwidth (FlowRecord fr, 
                                   AttributeValues frv, 
				   NRDB nrdb)
                                  throws CDBException
    {
       releaseBandwidth (fr, frv, nrdb, false);
    }

    private void releaseBandwidth (FlowRecord fr, 
                                   AttributeValues frv, 
				   NRDB nrdb,
				   boolean oldPath)
                                  throws CDBException

    {
                     
       boolean l2 = frv.getValueForName (FlowRecord.attrL2Endpoints).equals ("1");
       // find the current path of the flow
       PTP [] path = nrdb.lookupPath (
		       frv.getValueForName (FlowRecord.attrInInterfaceID),
		       frv.getValueForName (FlowRecord.attrOutInterfaceID), 
		       l2, oldPath);

			
       // release the bandwidth from all the PTPs
       nrdb.releaseBW (path, 
		Long.parseLong (frv.getValueForName (FlowRecord.attrBWAlloc)),
		frv.getValueForName (FlowRecord.attrSvcClassID));
       // mark the record for delete
       // updateFlowStatus (fr, FlowRecord.statusDeleted);
       // commit to the database
       nrdb.commit ();
    }

    private boolean deleteFlow (FlowRecord fr, 
                                AttributeValues frv, 
                                NRDB nrdb) throws CDBException, org.omg.CORBA.COMM_FAILURE
    {
      boolean rc = true;
      String flowID = fr.getID ();

      try
      {
	 String flowStatus = frv.getValueForName (FlowRecord.attrStatus);
		
	 System.out.println ("Delete Flow:"+flowID+" with status:"+flowStatus);

	 if (flowStatus.equals (FlowRecord.statusDeleted) != true)
	 {
	     if (flowStatus.equals (FlowRecord.statusDown) == true ||
		 flowStatus.equals (FlowRecord.statusCancelled) == true)
		 {
		     updateFlowStatus (fr, FlowRecord.statusDeleted);
		     nrdb.commit ();
		 }
	     else if (flowStatus.equals (FlowRecord.statusCommitted) == true ||
		      flowStatus.equals (FlowRecord.statusFailing) != true)
	     {
		 // Instead of configFlow (nrdb, flowID, false) use config_flow_Release
		  if (configureNw == 1)
		      {
			  NRDB.FlowConfigInfo fci = nrdb.getFlowConfigInfo (fr);
			  config_flow_release (fci);
		      }
		     //releaseBandwidth (fr, frv, nrdb);
		  if (flowStatus.equals (FlowRecord.statusCommitted) == true)
		      updateFlowStatus (fr, FlowRecord.statusDeleting);
		  else if (flowStatus.equals (FlowRecord.statusFailing) == true)
		      updateFlowStatus (fr, FlowRecord.statusDeleted);
		  nrdb.commit ();
		  System.out.println ("Delete Flow:"+flowID+" successful (stage: router)");
	     }
	 
	     tryToFailDuringDel(5);
	     // refresh fr
	     frv = fr.getAttributes ();
	     flowStatus = frv.getValueForName (FlowRecord.attrStatus);
	     if (flowStatus.equals (FlowRecord.statusDeleting) == true)
		 {
		     releaseBandwidth (fr, frv, nrdb);
		     updateFlowStatus (fr, FlowRecord.statusDeleted);
		     nrdb.commit ();
		     System.out.println ("Delete Flow:"+flowID+" successful (stage: DB)");
		 }
	     rc = true;
	     System.out.println ("Delete Flow:"+flowID+" successful (all stages including DB and router)");
	 }
      }
      catch (Exception e)
      {
	  rc = false;
         // e.printStackTrace ();
	 BBServer.bblogger.warn ("Exception in deleteFlow for flowID:"+flowID);
	 try
	 {
	    if (nrdb != null)
	       nrdb.rollback ();
	 }
         catch (CDBException ce)
	 {
	    BBServer.bblogger.warn ("Exception in nrdb rollback in "+
				    "deleteFlow() for flowID:"+flowID);
	    throw new CDBException ();
	 }
      }
	
      if (rc)
      {
         BBServer.bblogger.info ("Successful deleteFlow() for flowID:"+flowID);
      }
      else
      {
	 BBServer.bblogger.info (" deleteFlow() for flowID:"+flowID+" failed!");
      }
      return rc;
   }

   private void updateFlowStatus (FlowRecord fr, 
				  String status) 
                                  throws CDBException
   {
      AttributeValues av = new AttributeValues ();
      av.addNameValue (FlowRecord.attrStatus, status);
      fr.setAttributes (av);
   }

   /** Send commands to routers for deploying the DSCP
    *
    */
   void config_flow_setup (FlowRecord fr, NRDB nrdb) throws Exception
   {
      NRDB.FlowConfigInfo fci = nrdb.getFlowConfigInfo (fr);

      /*
      AttributeValues av = fr.getAttributes ();

      String inInfID = av.getValueForName (FlowRecord.attrInInterfaceID);
      String flowID = fr.getID ();
      String managedIP, infName;
      String svcClassID = av.getValueForName (FlowRecord.attrSvcClassID);
      String dscp;
      String l2Endpoints = av.getValueForName (FlowRecord.attrL2Endpoints); 
      
      if (l2Endpoints.equals ("0"))
      {
         L3Interface l3i = nrdb.findL3Interface (inInfID);
         AttributeValues l3iav = l3i.getAttributes ();
         infName = l3iav.getValueForName (L3Interface.attrName);
         Router r = nrdb.findRouter (l3iav.getValueForName (L3Interface.attrRouterID));
         managedIP = r.getAttributes ().getValueForName (Router.attrManIP);
      }
      else
      {
         L2Interface l2i = nrdb.findL2Interface (inInfID);
         AttributeValues l2iav = l2i.getAttributes ();
         infName = l2iav.getValueForName (L2Interface.attrName);
         Switch s = nrdb.findSwitch (l2iav.getValueForName (L2Interface.attrSwitchID));
         managedIP = s.getAttributes ().getValueForName (Switch.attrManIP);
      }

      ServiceClass sc = nrdb.findServiceClass (svcClassID);
      dscp = sc.getAttributes ().getValueForName (ServiceClass.attrDSCP);
      */

      AttributeValues av = fci.av;

      mil.darpa.arms.mlrm.BandwidthBroker.FlowProvisionerPackage.FlowInfo fi = 
                 new mil.darpa.arms.mlrm.BandwidthBroker.FlowProvisionerPackage.FlowInfo ();

      String protocol = av.getValueForName (FlowRecord.attrProtocol);
      fi.proto = getProtocol (protocol);
      fi.dscp = (short)Integer.parseInt (av.getValueForName (FlowRecord.attrDSCP));
      fi.srcIP = new IPAddress (av.getValueForName (FlowRecord.attrSrcIP),
                                av.getValueForName (FlowRecord.attrSrcMask));
      fi.destIP = new IPAddress (av.getValueForName (FlowRecord.attrDestIP),
                                av.getValueForName (FlowRecord.attrDestMask));


      fi.srcPorts = 
         new PortInfo (
         (short) Integer.parseInt (av.getValueForName (FlowRecord.attrSrcPort)),
         (short) Integer.parseInt (av.getValueForName (FlowRecord.attrSrcPortHigh)));
      fi.destPorts = 
         new PortInfo (
         (short) Integer.parseInt (av.getValueForName (FlowRecord.attrDestPort)),
         (short) Integer.parseInt (av.getValueForName (FlowRecord.attrDestPortHigh)));
      int rate, burst, excessRate;

      rate = Integer.parseInt (av.getValueForName (FlowRecord.attrBWAlloc)) *
                1000;

      if (protocol.equals ("tcp"))
      {
         burst = (rate * 3)/2;
         excessRate = (rate * 2);
      }
      else
      {
         burst = 3000; // 2 ethernet packets
         excessRate = 4500; // 3 ethernet packets
      }


      // make flow request
      flowProvisioner.addFlow (fci.managedIP, fci.infName, fci.flowID, 
                mil.darpa.arms.mlrm.BandwidthBroker.FlowProvisionerPackage.TrustState.none,
                (short) Integer.parseInt (fci.dscp),
                fi, rate, burst, excessRate,
                mil.darpa.arms.mlrm.BandwidthBroker.FlowProvisionerPackage.ExceedAction.MARKDOWN, 
                mil.darpa.arms.mlrm.BandwidthBroker.FlowProvisionerPackage.ExceedAction.DROP);

       System.out.println(" Configuring routers");
       BBServer.bblogger.info (" Configuring routers");
   }
     
   /** Send commands to routers for deploying the DSCP*/
   void config_flow_release (NRDB.FlowConfigInfo fci) throws Exception
   {

      /*
      AttributeValues av = fr.getAttributes ();
      String inInfID = av.getValueForName (FlowRecord.attrInInterfaceID);
      String flowID = fr.getID ();
      String managedIP, infName;
      String svcClassID = av.getValueForName (FlowRecord.attrSvcClassID);

      String l2Endpoints = av.getValueForName (FlowRecord.attrL2Endpoints);

      if (l2Endpoints.equals ("0"))
      {
         L3Interface l3i = nrdb.findL3Interface (inInfID);
         AttributeValues l3iav = l3i.getAttributes ();
         infName = l3iav.getValueForName (L3Interface.attrName);
         Router r = nrdb.findRouter (l3iav.getValueForName (L3Interface.attrRouterID));
         managedIP = r.getAttributes ().getValueForName (Router.attrManIP);
      }
      else
      {
         L2Interface l2i = nrdb.findL2Interface (inInfID);
         AttributeValues l2iav = l2i.getAttributes ();
         infName = l2iav.getValueForName (L2Interface.attrName);
         Switch s = nrdb.findSwitch (l2iav.getValueForName (L2Interface.attrSwitchID));
         managedIP = s.getAttributes ().getValueForName (Switch.attrManIP);
      }

      ServiceClass sc = nrdb.findServiceClass (svcClassID);
      String dscp = sc.getAttributes ().getValueForName (ServiceClass.attrDSCP);
      */

      //NRDB.FlowConfigInfo fci = nrdb.getFlowConfigInfo (fr);

      
      AttributeValues av = fci.av;

      mil.darpa.arms.mlrm.BandwidthBroker.FlowProvisionerPackage.FlowInfo fi = 
                 new mil.darpa.arms.mlrm.BandwidthBroker.FlowProvisionerPackage.FlowInfo ();

      String protocol = av.getValueForName (FlowRecord.attrProtocol);
      fi.proto = getProtocol (protocol);
      fi.dscp = (short)Integer.parseInt (av.getValueForName (FlowRecord.attrDSCP));
      fi.srcIP = new IPAddress (av.getValueForName (FlowRecord.attrSrcIP),
                                av.getValueForName (FlowRecord.attrSrcMask));
      fi.destIP = new IPAddress (av.getValueForName (FlowRecord.attrDestIP),
                                av.getValueForName (FlowRecord.attrDestMask));


      fi.srcPorts = 
         new PortInfo (
         (short) Integer.parseInt (av.getValueForName (FlowRecord.attrSrcPort)),
         (short) Integer.parseInt (av.getValueForName (FlowRecord.attrSrcPortHigh)));
      fi.destPorts = 
         new PortInfo (
         (short) Integer.parseInt (av.getValueForName (FlowRecord.attrDestPort)),
         (short) Integer.parseInt (av.getValueForName (FlowRecord.attrDestPortHigh)));
      int rate, burst, excessRate;

      rate = Integer.parseInt (av.getValueForName (FlowRecord.attrBWAlloc)) *
                1000;

      if (protocol.equals ("tcp"))
      {
         burst = (rate * 3)/2;
         excessRate = (rate * 2);
      }
      else
      {
         burst = 3000; // 2 ethernet packets
         excessRate = 4500; // 3 ethernet packets
      }


      // make flow release request

      //flowProvisioner.delFlow (managedIP, infName, flowID); 
      flowProvisioner.delFlowSL (fci.managedIP, fci.infName, fci.flowID,
                mil.darpa.arms.mlrm.BandwidthBroker.FlowProvisionerPackage.TrustState.none,
                (short) Integer.parseInt (fci.dscp),
                fi, rate, burst, excessRate,
                mil.darpa.arms.mlrm.BandwidthBroker.FlowProvisionerPackage.ExceedAction.MARKDOWN, 
                mil.darpa.arms.mlrm.BandwidthBroker.FlowProvisionerPackage.ExceedAction.DROP);
      System.out.println(" Configuring routers");
      BBServer.bblogger.info (" Configuring routers");
   }

   /** Send commands to routers for deploying the DSCP*/
   public void configure_routers () 
   {
       /** Do check for "notSpecified" protocol type */

      
   }

    /** How much bandwidth can be allocated? */
    public long decideBandwidth (long availableBW, 
				 long bandwidthMin, 
				 long bandwidthMax)
    {
	/** Right now we simply allocated the minimum required bandwidth, 
	    in the future more adaptive ways can be incorporated */
	if (availableBW >= bandwidthMin)
	    return bandwidthMin;
	else
	    return 0;
    }

    public ProtocolType getProtocol (String ps)
    {
        ProtocolType p;

	if (ps.equals ("tcp"))
	   p = ProtocolType.tcp;
	else if (ps.equals ("udp"))
	   p = ProtocolType.udp;
	else if (ps.equals ("stcp"))
	   p = ProtocolType.tcp;
	else //if (ps.equals ("notSpecified"))
	   p = ProtocolType.notSpecified;

	return p;
    }

    
    // Currently the policy has hardcoded service fractions, this
    // method needs to be made more generic in future.
    boolean changeBandwidthAllocationPolicyRequest (int mode)  
	throws AdmissionControlException
    {
	// Bandwidth per service class (high_priority,
	// high_reliability, high_throughput) for modes such as "standard"
	// and "battle" are 10,20,40,30 and 20,30,40,10 respectively.
	
	boolean retval = false;
	
	NRDB nrdb = null;

	AttributeValues av = new AttributeValues ();

	try {
	    nrdb = new NRDB ();

	    ObjectList o = nrdb.findAllBWPerClass ();
	
	// Get the PTP per BWPerClass and then its
	// rate, divide the rate by the new service
	// fraction and set it as the total bandwidth
	// in BWPErClass
	 


        Hashtable bcList = new Hashtable ();

	if (o.size () != 0)
	    {
		for (int i=0; i < o.size(); i++)
		    {
			BWPerClass bc = (BWPerClass)o.get(i);
			av = bc.getAttributes ();
			String ptpID = av.getValueForName (BWPerClass.attrPtpID);
			long rate = nrdb.getRate (ptpID);
		        String svcClassID = av.getValueForName (BWPerClass.attrSvcClassID);
			String svcName = nrdb.getServiceClassName (svcClassID);
	                AttributeValues av1 = new AttributeValues ();

			if (mode == 1) // battle
			    {
				
				if (svcName.compareTo ("highPriority") ==0)
				    av1.addNameValue (BWPerClass.attrBWTotal, 
						      new Long(rate * 20/100).toString());
				if (svcName.compareTo ("highReliability")==0)
				    av1.addNameValue (BWPerClass.attrBWTotal, 
						      new Long(rate * 30/100).toString());
				if (svcName.compareTo ("mmVideo")==0)
				    av1.addNameValue (BWPerClass.attrBWTotal, 
						      new Long(rate * 20/100).toString());
				if (svcName.compareTo ("mmVoice")==0)
				    av1.addNameValue (BWPerClass.attrBWTotal, 
						      new Long(rate * 20/100).toString());
				if (svcName.compareTo ("bestEffort")==0)
				    av1.addNameValue (BWPerClass.attrBWTotal, 
						      new Long(rate * 10/100).toString());
			    }
			else // mode == 0 == standard
			    {
				if (svcName.compareTo ("highPriority")==0)
				    av1.addNameValue (BWPerClass.attrBWTotal, 
						      new Long(rate * 10/100).toString());
				if (svcName.compareTo ("highReliability")==0)
				    av1.addNameValue (BWPerClass.attrBWTotal, 
						      new Long(rate * 20/100).toString());
				if (svcName.compareTo ("mmVideo")==0)
				    av1.addNameValue (BWPerClass.attrBWTotal, 
						      new Long(rate * 20/100).toString());
				if (svcName.compareTo ("mmVoice")==0)
				    av1.addNameValue (BWPerClass.attrBWTotal, 
						      new Long(rate * 20/100).toString());
				if (svcName.compareTo ("bestEffort")==0)
				    av1.addNameValue (BWPerClass.attrBWTotal, 
						      new Long(rate * 30/100).toString());
				
			    }
			
			bc.setAttributes (av1);
			

			// Find if the bandwidth allocated on this PTP for this class
			// is more than available Total bandwidth. If so add the PTP 
			// to the list
			av1 = bc.getAttributes ();
			long totalBW = Long.parseLong 
			        (av1.getValueForName(BWPerClass.attrBWTotal));
			long allocBW = Long.parseLong (
			av1.getValueForName(BWPerClass.attrBWTotal));

			if (allocBW > totalBW)
			{
			   // the link is overbooked in this class
			   bcList.put (bc, new Long (allocBW - totalBW));
			}


		    } // end of for loop
		retval=true;
	    }


            if (flowProvisioner  != null)
	    {

                String policy = "STANDARD_OPS";

	        if (mode == 1) // battle
		   policy = "BATTLE_MODE";
   
                // For every switch in the network
		// 1. Find the interface names
		// 2. add the QoS policy
		ObjectList switches = nrdb.getAllSwitches ();

		for (int i = 0; i < switches.size (); i++)
		{
		   Switch aSwitch = (Switch) switches.get (i);

	           av = aSwitch.getAttributes ();
	           String swIPAddr = av.getValueForName (aSwitch.attrManIP);
		   //System.out.println ("Finding interfaces for " + swIPAddr);

		   PTP [] ptps = nrdb.getPtpsOnSwitch (aSwitch.getID ());
		   {
		      String intfs [] = new String [ptps.length];
		      for (int j = 0; j < ptps.length; j++)
		      {
			 av = ptps [j].getAttributes ();
		         intfs [j] = av.getValueForName (PTP.attrName);
		         //System.out.print (intfs [j] + ",");
		      }
		      //System.out.println (".");
		      if (ptps.length > 0)
		      {
                         flowProvisioner.addQoSPolicy(swIPAddr, intfs, policy);
		      }
		   }
		}

		/*
     		System.out.println("Adding policy " + policy + " to LA");
                String [] intfs = {"Ethernet1/1", "Ethernet1/2"};
                //flowProvisioner.addQoSPolicy("192.4.20.67", intfs, policy);
                flowProvisioner.addQoSPolicy("LA", intfs, policy);
     		System.out.println("Adding policy " + policy + " to BOS");
                String [] intfs2 = {"Ethernet1/3", "Ethernet1/2"};
                //flowProvisioner.addQoSPolicy("192.4.20.78", intfs2, policy);
                flowProvisioner.addQoSPolicy("BOS", intfs2, policy);
                String [] intfs3 = {"FastEthernet3/0", "FastEthernet2/0"};
                //flowProvisioner.addQoSPolicy("192.4.20.78", intfs2, policy);
                flowProvisioner.addQoSPolicy("DC", intfs3, policy);
		*/
	    }
	    nrdb.commit ();

	    // If we fail at this point we could certainly do the next
	    // part when we come back again
	    // process the flows that use bandwidth partitions on bcList
	    // processAffectedFlows (nrdb, bcList);

            nrdb.releaseConnection ();
	}
	catch (Exception e)
	    {
	       e.printStackTrace ();
		BBServer.bblogger.warn ("Error changing bandwidth allocation policy:"+mode);
		throw new AdmissionControlException ("Error changing bandwidth allocation policy:"+mode);
	    }
	return retval;
	    
    }

    FlowProvisioner flowProvisioner = null;

    public void initFlowProvisioner (String [] args) throws Exception
    {
      ORB orb = ORB.init(args, null);

      // get the root naming context
      org.omg.CORBA.Object objRef =
          orb.resolve_initial_references("NameService");

      // Use NamingContextExt which is part of the Interoperable
      // Naming Service (INS) specification.
      NamingContextExt ncRef = NamingContextExtHelper.narrow(objRef);

      // resolve the Object Reference in Naming
      String name = flowProvisionerNameString ;//"FlowProvisioning";

      flowProvisioner = FlowProvisionerHelper.narrow(ncRef.resolve_str(name));
       
    }
    // Added Murray - 08/30/2005
    public void connectToFlowProvisioner (FlowProvisioner _impl )
    {
   	 	BBServer.bblogger.info ("Connected to FlowProvisioner");
	    flowProvisioner = _impl;       
    }
    // end Added - MRC

    // Added Rick/Kirthika
    public void connectToNetworkMonitor (NetworkMonitor _impl )
    {
   	 	BBServer.bblogger.info ("Connected to NetworkMonitor");
	    networkMonitor = _impl;       
    }
    // end Added - FDP

   
   public void processBWQueriesByPools (BWBetnPools bbs [], boolean provisioned) throws Exception
   {
      NRDB nrdb = new NRDB ();

      nrdb.commit ();
      for (int i = 0; i < bbs.length; i++)
      {
         String srcDottedDecimal, destDottedDecimal;

         srcDottedDecimal = nrdb.findSubnetInPool (bbs [i].srcPoolName);
         destDottedDecimal = nrdb.findSubnetInPool (bbs [i].destPoolName);

         for (int j = 0; j < bbs [i].bwPerClassSeq.length; j++)
         {

            bbs [i].bwPerClassSeq [j].availableBW.requiredBW = (int)
            processBWQuery (srcDottedDecimal,
                            destDottedDecimal,
                            getServiceClass (bbs [i].bwPerClassSeq [j].qos),
                            (long)bbs [i].bwPerClassSeq [j].availableBW.requiredBW,
                            nrdb, provisioned);

         }
      }

      nrdb.rollback ();
      nrdb.releaseConnection ();
   }
   public void processBWQueries (BWBetnSubnets bbs [], boolean provisioned) throws Exception
   {
      NRDB nrdb = new NRDB ();

      for (int i = 0; i < bbs.length; i++)
      {
         for (int j = 0; j < bbs [i].bwPerClassSeq.length; j++)
         {

            bbs [i].bwPerClassSeq [j].availableBW.requiredBW = (int)
            processBWQuery (bbs [i].srcSubnet.dottedDecimal,
                            bbs [i].destSubnet.dottedDecimal,
                            getServiceClass (bbs [i].bwPerClassSeq [j].qos),
                            (long)bbs [i].bwPerClassSeq [j].availableBW.requiredBW,
                            nrdb, provisioned);

         }
      }

      nrdb.rollback ();
   }

   private long processBWQuery (String src_ip_address,
				String dst_ip_address,
				String svcClass,
				long bandwidth,
				NRDB nrdb,
                                boolean provisioned)
	  throws Exception
   {

         if (bandwidth <= 0)
            return 0L;
         // Create new instance of NRDB automatically gets a connection
         // from the connnection pool

         // Find the edges of the network through which the traffic will flow
         String inInterfaceID = nrdb.findProviderEdge (src_ip_address, false);
         String outInterfaceID = nrdb.findProviderEdge (dst_ip_address, false);
	 
	 /*
	 BBServer.bblogger.info ("ingressID ="+inInterfaceID+
				 " , egressID="+outInterfaceID);
				 */
	 

	 // Bug: Bug: Pure L2 paths not handled
         PTP [] path = nrdb.lookupPath (inInterfaceID, outInterfaceID, false);


         if (provisioned == true)
         {
	    long bw = 0;
            if (svcClass.equals ("raw"))
	    {

	       // Find physical bandwidth
	       for (int i = 0; i < path.length; i++)
	       {
	          AttributeValues a = path [i].getAttributes ();
		  long b = Long.parseLong (a.getValueForName (PTP.attrRate));
		  if (bw == 0)
		  {
		     bw = b;
		  }
	          else if (b < bw)
		     bw = b;
	       }
	       // return bw: the physical rate of the slowest link in the path
	    }
	    else
	    {
	       // find bandiwdth for the class;
               String svcClassID = 
	                  nrdb.findServiceClassByName (svcClass);

	       // Find physical bandwidth
	       for (int i = 0; i < path.length; i++)
	       {
		  long b = nrdb.findTotalBW (path[i].getID (), svcClassID);
		  if (bw == 0)
		  {
		     bw = b;
		  }
	          else if (b < bw)
		     bw = b;
	       }
	       // return bw: the provisioned bw for the class of the slowest 
	       // link in the path
	    }
	    return bw;
    
         }
	 else
	 {
	    // may change to invalid parameter excepyion
	    if (svcClass.equals ("raw") || svcClass.equals ("bestEffort"))
	       throw new Exception ();
	 }
         // Find available bandwidth along the path for the 
         // service class
         String svcClassID = nrdb.findServiceClassByName (svcClass);
         long availableBW = nrdb.findAvailableBW (path, svcClassID);

         long allocatedBW = 0;

         if (availableBW >= bandwidth)
            allocatedBW = bandwidth;
         else
            allocatedBW = availableBW;


         if (allocatedBW > 0)
         {
            nrdb.reserveBW (path, allocatedBW, svcClassID);
         }
         return allocatedBW;
      
    }

    public String getServiceClass (QOSRequired q)
    {
	String svcClass;

	switch (q.value())
	  {
	  case QOSRequired._highReliability :
	    {
	      // With higher WFQ size
	      svcClass = "highReliability";
	      break;
	    }
	  case QOSRequired._mmVoice :
	    {
	      // class for voice traffic
	      svcClass = "mmVoice";
	      break;
	    }
	  case QOSRequired._mmVideo :
	    {
	      // class for video traffic
	      svcClass = "mmVideo";
	      break;
	    }
	  case QOSRequired._highPriority :
	    {
	      // low latency, low priority queue size
	      svcClass = "highPriority";
	      break;
	    }
	  case QOSRequired._raw :
	    {
	      // raw physical bw class
	      svcClass = "raw";
	      break;
	    }
	  default:
	    svcClass = new String ("bestEffort");
	  }
	return svcClass;
    } 

    void removeZombieFlows () throws CDBException
    {
       NRDB nrdb = new NRDB ();

       String [] fids = nrdb.findZombieFlows ();
       nrdb.commit ();
       nrdb.releaseConnection ();

       for (int i = 0; i < fids.length; i++)
       {
	  try
	  {
             processFlowDeleteRequest (fids [i]);
          }
	  catch (AdmissionControlException e)
	  {
	     // could not do anuthing more
	  }
       }
    }
    private class FCIComparator implements Comparator
    {
       public int compare (java.lang.Object o1, java.lang.Object o2)
       {
	  NRDB.FlowConfigInfo fci1 = (NRDB.FlowConfigInfo) o1;
	  NRDB.FlowConfigInfo fci2 = (NRDB.FlowConfigInfo) o2;

	  if (fci1.priority != fci2.priority)
	    return fci1.priority - fci2.priority;
	  else
	  {
	    long rc  = fci1.rate - fci2.rate;
	    if (rc > 0)
	       return 1;
	    else if (rc == 0)
	       return 0;
	    else
	       return -1;
	  }
       }
    }
    /*
     
    private class FlowRecordComparator implements Comparator
    {
       public int compare (Object o1, Object o2)
       {
	  AttributeValues av1 = (AttributeValues) o1;

          long bwAlloc1 = Long.parseLong (av1.getValueForName (FlowRecord.attrBWAlloc));
          int priority1 = Integer.parseInt (av1.getValueForName (FlowRecord.attrPriority));

	  AttributeValues av2 = (AttributeValues) o2;

          long bwAlloc2 = Long.parseLong (av2.getValueForName (FlowRecord.attrBWAlloc));
          int priority2 = Integer.parseInt (av2.getValueForName (FlowRecord.attrPriority));
	  if (priority1 !=priority2)
	    return priority1 - priority2;
	  else
	  {
	    return bwAlloc1 - bwAlloc2;
	  }
       }
    }
    FlowRecordComparator frComp = new FlowRecordComparator ();

    void processAffectedFlows (NRDB nrdb, Hashtable bcList) throws CDBException
    {
       AttributeValues [] aFlows = identifyAffectedFlows (nrdb, bcList);
       Arrays.sort (aFlows, frComp);
    }
    */

    AttributeValues [] identifyAffectedFlows (NRDB nrdb, 
                                              Hashtable bcList) 
					      throws CDBException
    {
       Vector flows = nrdb.getAllFlowRecords ();
       Vector affectedFlows = new Vector ();

       for (int i = 0; i < flows.size (); i++)
       {
	  FlowRecord fr = (FlowRecord) flows.get (i);
          AttributeValues frv = fr.getAttributes ();
          String flowStatus = frv.getValueForName (FlowRecord.attrStatus);

          if (flowStatus.equals (FlowRecord.statusCommitted) != true)
	  {
	     // Bug: Other flow states are important but not considered.
	     continue;
	  }

	  String svcClassID = frv.getValueForName (FlowRecord.attrSvcClassID);

          boolean l2 = frv.getValueForName (FlowRecord.attrL2Endpoints).equals ("1");
          PTP [] path = 
	       nrdb.lookupPath (frv.getValueForName (FlowRecord.attrInInterfaceID),
	                        frv.getValueForName (FlowRecord.attrOutInterfaceID), l2);
          BWPerClass [] bpc = nrdb.getBWPerClassFromPTP (path, svcClassID);

	  // Compare with bpc List

	  for (int j = 0; j < bpc.length; j++)
	  {
	     // if bcList contains the link traversed by the flow then identify the flow
	     // to be affected.
	     if (bcList.get (bpc [i]) != null)
	     {
	        ;// put the flow in the list of affected flows
		affectedFlows.add (frv);  
	     }
	  }
       }
       AttributeValues [] rc = new AttributeValues [affectedFlows.size ()];
       affectedFlows.toArray (rc);
       return rc;
    }

    private class AffectedEdgePairs
    {
       Hashtable affectedEdgePairs = new Hashtable ();
       
       private String getKey (String switch1, String switch2, String vlan)
       {
          return switch1 +"/" + switch2 + "/" + vlan;
       }
       void addPair (String switch1, String switch2, String vlan)
       {
          affectedEdgePairs.put (getKey (switch1, switch2, vlan), "");
          affectedEdgePairs.put (getKey (switch2, switch1, vlan), "");
       }
       boolean isAffected (String switch1, String switch2, String vlan)
       {
          return (affectedEdgePairs.get (getKey (switch1, switch2, vlan)) != null); 
       }

    }
    // This method processes a network event that reports 
    // a fault or a repair of the link
    // This method computes a list of affected flows and
    // find which of them need to be preempted in order to
    // guarantee bandwidth to the rest of the admitted flows

    public void processL3NetworkEvent (String [] l3i )
    {
       NRDB nrdb=null;
       String flowStatus=null;
       Vector affectedFlows = new Vector ();

       try
       {
	  //L3PathManager.processFaultBegin ();
	  System.out.println ("Begin fault processing...");
          long t = System.currentTimeMillis ();
          nrdb = new NRDB ();
	  FindPaths.loadL3Paths (nrdb, ospfRouter, false);
          System.out.println ("Path discovery time = " + 
	     (System.currentTimeMillis () - t));
          String [] flows =  nrdb.findFlowsOnIP (l3i);

	  System.out.print ("Affected flows: ");
	  for (int i = 0; i < flows.length; i++)
	  {
	     System.out.print (flows [i] + " ");
	  }
	  System.out.println ();
          for (int i = 0; i < flows.length; i++)
	  {
	     FlowRecord fr = nrdb.findFlowRecord (flows [i]);
	     NRDB.FlowConfigInfo fci = nrdb.getFlowConfigInfo (fr);
	     flowStatus = fci.status;
// Flows that are on the way to be set to "deleted" status, 
// must be looked at and moved out of the way. This case will occur when 
// the n/w fault occurs before the s/w fault during processing of a s/w fault
	     if (flowStatus.equals (FlowRecord.statusDeleting) == true)
	     {
	        releaseBandwidth (fr, fci.av, nrdb, true);
	        flowStatus = FlowRecord.statusDeleted;
             }
	     else if (flowStatus.equals (FlowRecord.statusCommitted) == true ||
		      flowStatus.equals (FlowRecord.statusCommitting) == true)
             {
// release bandwidth on the old path, the config instructions on ingress
// would be the same in case a new path is found,so we dont need to 
// tackle that here
	        releaseBandwidth (fr, fci.av, nrdb, true);
		if (flowStatus.equals (FlowRecord.statusCommitted) == true)
		{
		   flowStatus = FlowRecord.statusFailing;
		}
	        else 
		{
		   if (flowStatus.equals (FlowRecord.statusCommitting) == true)
	              flowStatus = FlowRecord.statusCancelled;
		}
             }
// Add the "failing" and "failed" flows to this list too since with change 
// in network, maybe "failed" flows can be readmitted, irrespective whether 
// the paths have changed 
	     if (flowStatus.equals (FlowRecord.statusDown)== true || 
	         flowStatus.equals (FlowRecord.statusFailing)== true || 
		 flowStatus.equals (FlowRecord.statusCancelled)== true )
	     {
		affectedFlows.add (fci);
		//System.out.println ("Affected flow (currently failing or down): id = " + fr.getID ());
	     }
	     if (fci.status.equals (flowStatus) != true)
	     {
	        fci.status = flowStatus;
	        updateFlowStatus (fci.fr, flowStatus);
	     }
	  }

	  NRDB.FlowConfigInfo fcis [] = 
	               new NRDB.FlowConfigInfo [affectedFlows.size ()];
	  affectedFlows.toArray (fcis);
	  //sort by priority descending/bandwidth ascending
	  Arrays.sort (fcis, new FCIComparator ());
	  
	  // reuse affected Flows for the final list of preempted flows
	  Hashtable hashOfLists = new Hashtable ();
	  Vector listOfLists = new Vector ();

	  for (int i = 0; i < fcis.length; i++)
	  {
	     NRDB.FlowConfigInfo fci = fcis [i];
	     flowStatus = fci.status;
	     //System.out.println ("FlowID: " + fci.flowID + ":" + flowStatus);
	     if (acquireBandwidth (nrdb, fci) == false)
	     {
	         //System.out.println ("\t no bw");
	         
		 // "Failing" flows are those that were previously in the "committed" state while
		 // "cancelled" flows are those that were previously in  "committing" state.
		 // Those flows that are "down" or "committing" dont need deconfiguration, 
		 // only "committed" ones do. But "committing" flows could have finsihed
		 // configuring routers before fault occured, de-configure to be in the safer side 	
		 if (flowStatus.equals (FlowRecord.statusFailing)==true ||
		     flowStatus.equals (FlowRecord.statusCancelled)==true)
		 {
		    // get a list of flows starting from the same router
		    Vector fciList = (Vector) hashOfLists.get (fci.managedIP);
		    if (fciList == null)
		    {
		        fciList = new Vector ();
		        hashOfLists.put (fci.managedIP, fciList);
		        listOfLists.add (fciList);
		    }
		    fciList.add (fci);
		 }
		 // for those flows that are "down" we do nothing
	     }
	     else 
// bandwidth has been re-reserved,but what if they were currently "down" or 
// "cancelled"? 
// Once bandwidth has been acquired, the flow moves from "down"/"cancelled" 
// to "committing"
	     {
	        //System.out.println ("\t got bw");
		if (flowStatus.equals (FlowRecord.statusDown)==true || 
		    flowStatus.equals (FlowRecord.statusCancelled)==true)
	        {
		   updateFlowStatus (fci.fr, FlowRecord.statusCommitting);
		   //nrdb.commit ();

	           if (configureNw == 1)
		   {
		      // reconfigure routers
		      try 
		      {
			config_flow_setup (fci.fr, nrdb);
		      }
		      catch (Exception e)
		      {
		         throw new CDBException ();
		      }
		   }
		   //set the status to committed
		   updateFlowStatus (fci.fr, FlowRecord.statusCommitted);
		   //nrdb.commit ();
			 
	           //System.out.println ("Readdmitting preempted flow, flowID="+fci.fr.getID ());
	        }
		else if (flowStatus.equals (FlowRecord.statusFailing)==true)
		{
		   updateFlowStatus (fci.fr, FlowRecord.statusCommitted);
		   //nrdb.commit ();
	           //System.out.println ("\t committed back.");
	        }
	     }
	  }
	
	  // deconfigure all the preempted flows and set them to "down"
	  Thread [] threads = new Thread [listOfLists.size ()];
	  for (int i = 0; i < listOfLists.size (); i++)
	  {
	     Vector fciList = (Vector) listOfLists.get (i);
             BulkFlowDeleteThread  bfdt = 
	        new BulkFlowDeleteThread (fciList, nrdb);
	     bfdt.start ();
	     threads [i] = bfdt;
          }
	  
	  // Wait for all the flows to be released
	  for (int i = 0; i < threads.length; i++)
	  {
	    try 
	     {
	        if (threads [i].isAlive ())
	           threads [i].join ();
	     }
	     catch (InterruptedException ie)
	     {
	        // Ignore the exception
	     }
	  }
	  nrdb.removeOldL3Paths ();
	  nrdb.commit ();
	  nrdb.releaseConnection ();
          t = System.currentTimeMillis () - t;
          System.out.println ("Processing network event time = " + t);

	  //nrdb.releaseConnection ();
       }
       catch (CDBException e)
       {
          System.out.println ("Error processing network event");
	  try
	      {
		  if (nrdb != null)
		      nrdb.releaseConnection ();
	      }
	  catch (Exception x)
	      {
	      }
       }
       catch (Exception ex)
       {
          System.out.println ("Error processing network event");
	  try
	      {
		  if (nrdb != null)
		      nrdb.releaseConnection ();
	      }
	  catch (Exception x)
	      {
	      }
       }
    }

    // This method processes a network event that reports 
    // a fault or a repair of the link
    // This method computes a list of affected flows and
    // find which of them need to be preempted in order to
    // guarantee bandwidth to the rest of the admitted flows

    public void processNetworkEvent (switchInfo f)
    {
       // Get the Switch Id from name
       // Get the ptpID from the interface name


       NRDB nrdb=null;
       try
       {
	  //L3PathManager.processFaultBegin ();
	  System.out.println ("Begin fault processing...");
          long t = System.currentTimeMillis ();
          nrdb = new NRDB ();

	  FindPaths.loadL3Paths (nrdb, ospfRouter, false);
          System.out.println ("Path discovery time = " + 
	     (System.currentTimeMillis () - t));

          AffectedEdgePairs aep = new AffectedEdgePairs ();

          ObjectList ol = nrdb.getAllRouters ();
          String routers [] = new String [ol.size ()];
	  String routersIP [] = new String [ol.size ()];

          for (int i = 0; i < ol.size (); i++)
          {
             routers [i] = ol.get(i).getID ();
	     routersIP [i] = ol.get(i).getAttributes().getValueForName (ME.attrManIP);
          }
   
          for (int j = 0; j < routers.length; j++)
	  {
             for (int k = j+1; k < routers.length; k++)
	     {
	        if (L3PathManager.comparePaths (nrdb, routersIP [j], 
		                  routersIP [k]) == false)
	       // if (L3PathManager.comparePathsInMemory (routersIP [j], 
	       //	                  routersIP [k]) == false)
	        {
		      //System.out.println ("Affected: " +
		      //routers [j] + ":" + routers [k]);
		      aep.addPair (routers [j], routers [k], "noVlan");
		      aep.addPair (routers [k], routers [j], "noVlan");
	              System.out.println ("Affected pair = " + routers [j] + ":" + routers [k]);
		}
	     }
          }

	  String flowStatus=null;
          Vector flows = nrdb.getAllFlowRecords ();
          Vector affectedFlows = new Vector ();
	  int numFlows = flows.size ();
          for (int i = 0; i < numFlows; i++)
	      {
		  FlowRecord fr = (FlowRecord) flows.remove (0);
		  NRDB.FlowConfigInfo fci = nrdb.getFlowConfigInfo (fr);
		  //System.out.println ("FCI = " + 
				//      fci.ingress + ":" + fci.egress + ":" + fci.vlan);
		  //flowStatus = fci.fr.getAttributes().getValueForName (FlowRecord.attrStatus);
		  flowStatus = fci.status;
		  
		  if (aep.isAffected (fci.ingress, fci.egress, "noVlan"))
		  {
		      // Flows that are on the way to be set to "deleted" status, 
		      // must be looked at and moved out of the way. This case will occur when 
		      // the n/w fault occurs before the s/w fault during processing of a s/w fault
		      if (flowStatus.equals (FlowRecord.statusDeleting) == true)
			  {
			      releaseBandwidth (fr, fci.av, nrdb, true);
			      //updateFlowStatus (fci.fr, FlowRecord.statusDeleted);
			      flowStatus = FlowRecord.statusDeleted;
			      nrdb.commit ();
			  }
		      else if (flowStatus.equals (FlowRecord.statusCommitted) == true ||
			       flowStatus.equals (FlowRecord.statusCommitting) == true)
			  {
			      // release bandwidth on the old path, the config instructions on ingress
			      // would be the same in case a new path is found,so we dont need to tackle that here
			      releaseBandwidth (fr, fci.av, nrdb, true);
			      if (flowStatus.equals (FlowRecord.statusCommitted) == true)
			          flowStatus = FlowRecord.statusFailing;
				  // updateFlowStatus (fci.fr, FlowRecord.statusFailing);
			      else if (flowStatus.equals (FlowRecord.statusCommitting) == true)
			          flowStatus = FlowRecord.statusCancelled;
				  //updateFlowStatus (fci.fr, FlowRecord.statusCancelled);
			      nrdb.commit ();
			  }
		      // Add the "failing" and "failed" flows to this list too since with change in network,
		      // maybe "failed" flows can be readmitted, irrespective whether the paths have changed
		      //flowStatus = fci.fr.getAttributes().getValueForName (FlowRecord.attrStatus);
		      if (flowStatus.equals (FlowRecord.statusDown)== true || 
			  flowStatus.equals (FlowRecord.statusFailing)== true || 
			  flowStatus.equals (FlowRecord.statusCancelled)== true )
			  {
			      affectedFlows.add (fci);
			      //System.out.println ("Affected flow (currently failing or down): id = " + fr.getID ());
			  }
		     fci.status = flowStatus;
	             updateFlowStatus (fci.fr, flowStatus);
		  }// end of if
	      }// end of for
              nrdb.commit ();
	  
	 
	  NRDB.FlowConfigInfo fcis [] = 
	               new NRDB.FlowConfigInfo [affectedFlows.size ()];
	  affectedFlows.toArray (fcis);
	  //sort by priority descending/bandwidth ascending
	  Arrays.sort (fcis, new FCIComparator ());
	  
	  // reuse affected Flows for the final list of preempted flows
	  Hashtable hashOfLists = new Hashtable ();
	  Vector listOfLists = new Vector ();

	  for (int i = 0; i < fcis.length; i++)
	  {
	     NRDB.FlowConfigInfo fci = fcis [i];
	     //flowStatus = fci.fr.getAttributes().getValueForName (FlowRecord.attrStatus);
	     flowStatus = fci.status;
	     //System.out.println ("FlowID: " + fci.flowID + ":" + flowStatus);
	     if (acquireBandwidth (nrdb, fci) == false)
	     {
	         //System.out.println ("\t no bw");
	         
		 // "Failing" flows are those that were previously in the "committed" state while
		 // "cancelled" flows are those that were previously in  "committing" state.
		 // Those flows that are "down" or "committing" dont need deconfiguration, 
		 // only "committed" ones do. But "committing" flows could have finsihed
		 // configuring routers before fault occured, de-configure to be in the safer side 	
		 if (flowStatus.equals (FlowRecord.statusFailing)==true ||
		     flowStatus.equals (FlowRecord.statusCancelled)==true)
		    {
			// get a list of flows starting from the same router
			Vector fciList = (Vector) hashOfLists.get (fci.managedIP);
			if (fciList == null)
			    {
				fciList = new Vector ();
				hashOfLists.put (fci.managedIP, fciList);
				listOfLists.add (fciList);
			    }
			fciList.add (fci);
		    }
		 // for those flows that are "down" we do nothing
		 
	     }
	     else // bandwidth has been re-reserved,but what if they were currently "down" or "cancelled"? 
		 // Once bandwidth has been acquired, the flow moves from "down"/"cancelled" to "committing"
		 {
	             //System.out.println ("\t got bw");
		     if (flowStatus.equals (FlowRecord.statusDown)==true || 
			 flowStatus.equals (FlowRecord.statusCancelled)==true)
			{
			    updateFlowStatus (fci.fr, FlowRecord.statusCommitting);
			    //nrdb.commit ();

			     if (configureNw == 1)
				 {
				     // reconfigure routers
				     try {
					 config_flow_setup (fci.fr, nrdb);
				     }catch (Exception e)
					 {
					     throw new CDBException ();
					 }
				 }
			     //set the status to committed
			     updateFlowStatus (fci.fr, FlowRecord.statusCommitted);
			     //nrdb.commit ();
			 
			     //System.out.println ("Readdmitting preempted flow, flowID="+fci.fr.getID ());
			}
		     else if (flowStatus.equals (FlowRecord.statusFailing)==true)
			{
			    updateFlowStatus (fci.fr, FlowRecord.statusCommitted);
			    //nrdb.commit ();
	                    //System.out.println ("\t committed back.");
			}
		 }
	  }
	
	  // deconfigure all the preempted flows and set them to "down"
	  Thread [] threads = new Thread [listOfLists.size ()];
	  for (int i = 0; i < listOfLists.size (); i++)
	  {
	      Vector fciList = (Vector) listOfLists.get (i);
             BulkFlowDeleteThread  bfdt = 
	        new BulkFlowDeleteThread (fciList, nrdb);
	     bfdt.start ();
	     threads [i] = bfdt;
          }
	  
	  // Wait for all the flows to be released
	  for (int i = 0; i < threads.length; i++)
	  {
	     try 
	     {
	        if (threads [i].isAlive ())
	           threads [i].join ();
	     }
	     catch (InterruptedException ie)
	     {
	        // Ignore the exception
	     }
	  }
	  nrdb.removeOldL3Paths ();
	  nrdb.releaseConnection ();
          t = System.currentTimeMillis () - t;
          System.out.println ("Processing network event time = " + t);
	  //L3PathManager.processFaultEnd ();
   
       }
       catch (CDBException e)
       {
          System.out.println ("Error processing network event");
	  try
	      {
		  if (nrdb != null)
		      nrdb.releaseConnection ();
	      }
	  catch (Exception x)
	      {
	      }
       }
       catch (Exception ex)
       {
          System.out.println ("Error processing network event");
	  try
	      {
		  if (nrdb != null)
		      nrdb.releaseConnection ();
	      }
	  catch (Exception x)
	      {
	      }
       }
    }
    
    public int getConfigureNw()
    {
    	return configureNw;
    }

    static int inv = 0;
    public void tryToFail (int when)
    {
       if (failAfterX == 0)
          return;
       if (when == 1)
          inv ++;
       //System.out.println ("when="+when+" failWhen="+failWhen+" inv="+inv);
       if (inv <= failAfterX)
          return;
       if (failWhen == when)
       {
          System.out.println ("Failure simulated on invocation #" + inv + "\n"
                            + "Failure type = " + when + "\n");
	  try
	  {

	     FileOutputStream  f = new FileOutputStream ("bbevent");
	     f.write ('\n');
	     f.close ();
             Thread.sleep (15000);
	  }

	  catch (Exception e)
	  {
	     e.printStackTrace ();
	  }
          //System.exit (1);
       }
    }

    public void tryToFailDuringDel (int when)
    {
       if (failAfterX == 0)
          return;
       if (when == 5) // This is the one change different from tryToFail (): Kirthika
          inv ++;
       //System.out.println ("when="+when+" failWhen="+failWhen+" inv="+inv);
       if (inv <= failAfterX)
          return;
       if (failWhen == when)
       {
          System.out.println ("Failure simulated on invocation #" + inv + "\n"
                            + "Failure type = " + when + "\n");
	  Runtime r = Runtime.getRuntime ();
          try
	  {
             Process p = r.exec ("bin/failpool1.sh");
             System.out.println ("Pool failure invoked.");
	     p.waitFor ();
             System.exit (1);
	  }
	  catch (Exception e)
	  {
	  }
          //System.exit (1);
       }
    }


}
