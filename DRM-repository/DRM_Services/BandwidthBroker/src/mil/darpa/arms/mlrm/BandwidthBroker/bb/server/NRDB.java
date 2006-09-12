//$Id: NRDB.java
// This software is governed by Government Purpose Rights
// Contract No: NBCHC030132
// Telcordia Technologies, Inc.
// 1 Telcordia Drive
// Piscataway, NJ 08854-4157

// BBDriver.java

// This class provides Application friendly API for 
// accessing the database


// Revision history

// 03-11-04        Shree	Initial version
//                              
//

package mil.darpa.arms.mlrm.BandwidthBroker.bb.server;

import java.io.*;
import java.net.*;
import java.util.*;
import java.math.*;
import javax.sql.*;
import mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.*;
import mil.darpa.arms.mlrm.BandwidthBroker.L2Disc.*;

public class NRDB
{
   public static int numConnections = 4;
    // public static String dbServerFQN = "192.4.12.89";
   public static String dbServerFQN = "127.0.0.1";
   public static String dbLoginName ="arms";
   public static String dbPasswordValue = "arms";
   public static CDBConnectionPool cp = null;
   CDBHome cdbHome;

   /**
    * Constructor
    * Allocates a database connection for this class
    */
   public NRDB () throws CDBException
   {
       if (cp == null) {
	   System.err.println("numCon: " + numConnections +
			      "dbServer: " + dbServerFQN +
			      "login: " + dbLoginName +
			      "pwd: " + dbPasswordValue);
         cp = new CDBConnectionPool (numConnections, dbServerFQN, dbLoginName, dbPasswordValue);
       }

      // allocate a connection to database for this instance
      cdbHome = cp.allocateCDBHome ();
   }

   /*
    * Release the underlying database connection back to the pool
    * All further call will report a NullPointerException
    */
   public void releaseConnection () throws CDBException
   {
      try
      {
         cp.freeCDBHome (cdbHome);
         cdbHome = null;
      }
      catch (Exception rn)
      {
	  rn.printStackTrace ();
	  throw new CDBException ();
      }
   }

   /**
    * finalize method
    * release the connection to the pool is user has forgotten to do it
    */
   protected void finalize() throws CDBException
   {
      if (cdbHome != null)
         releaseConnection ();
   }

   public AttributeValues getHostSubnetValues (String ipAddress) 
                                               throws CDBException
   {
      AttributeValues rc = null;

      HostSubnetHome hsh = 
                   (HostSubnetHome) cdbHome.getHomebyName ("HostSubnet");

      ObjectList ol;

      // check if hostname is specified
      AttributeValues avhn = new AttributeValues ();
      avhn.addNameValue (HostSubnet.attrHostname, ipAddress);
      ol = hsh.findByAttributes (avhn);

      if (ol.size () > 0)
      {
         HostSubnet hs = (HostSubnet) ol.get (0);
         rc = hs.getAttributes ();
      }
      else
      {
         // get all the subnets
         ol = hsh.findByAttributes (null);
         //System.out.println ("Searching " + ipAddress);

         for (int i = 0; i < ol.size (); i++)
         {
            HostSubnet hs = (HostSubnet) ol.get (i);
            AttributeValues av = hs.getAttributes ();
   
            String hostAddr = av.getValueForName (HostSubnet.attrHostSubnetAddr);  

	    if (hostAddr.equals (ipAddress))
	    {
	       rc = av;
	       break;
	    }

	    try
	    {
               Inet4Subnet i4s = new Inet4Subnet (
                                 av.getValueForName (HostSubnet.attrHostSubnetAddr),
                                 av.getValueForName (HostSubnet.attrHostSubnetMask));
               if (i4s.isInSubnet (ipAddress))
               {
	          rc = av;
               }
            }
	    catch (Exception e)
	    {
	       // ignore the entry
	    }
         }
      }
      if (rc != null)
         return rc;

      // Otherwise we didn't find the host or its subnet
      throw new CDBException ();
   }

   /**
    * Finds the L3Interface to which this host IP address is connected
    */
   public String findProviderEdge (String ipAddress, boolean l2Edge) throws CDBException, 
                                                            Exception
   {
      AttributeValues av = getHostSubnetValues (ipAddress);

      if (l2Edge)
         return av.getValueForName (HostSubnet.attrL2InterfaceID);
      else
         return av.getValueForName (HostSubnet.attrL3InterfaceID);
   }

   /**
    * Find serviceClassID from the name
    * throw CDBException if name not found
    */
   String findServiceClassByName (String svcClass) throws CDBException
   {
      
     ServiceClassHome sch = (ServiceClassHome)
                                cdbHome.getHomebyName ("ServiceClass");

      AttributeValues av = new AttributeValues ();
      av.addNameValue (ServiceClass.attrName, svcClass);
      ObjectList ol = sch.findByAttributes (av);
      if (ol.size () > 0)
         return ol.get (0).getID ();

      throw new CDBException ();
   }

   public BWPerClass [] getBWPerClassFromPTP (PTP [] path, 
                                              String svcClassID) throws CDBException
   {     
      BWPerClass rc [] = new BWPerClass [path.length];

      BWPerClassHome bpch = (BWPerClassHome) cdbHome.getHomebyName ("BWPerClass");
      for (int i = 0; i < path.length; i++)
      {
         String ptpID = path [i].getID ();
	 String inoutValue = "1"; // inbound by default
         AttributeValues av = new AttributeValues ();
         av.addNameValue (BWPerClass.attrPtpID, ptpID);
         av.addNameValue (BWPerClass.attrSvcClassID, svcClassID);
	 // for the last hop get BWPerClass instance for outgoing traffic
         if (i == (path.length -1))
         {
	    inoutValue = "0";
         }
	 av.addNameValue (BWPerClass.attrInbound, inoutValue);

         ObjectList ol = bpch.findByAttributes (av);

	 // In case of L3 BWPerClass for outgoing traffic may not
	 // be defined! In that case ol.size = 0;
         if (ol.size () > 0)
         {
	    rc [i] = (BWPerClass) ol.get (0);
         }
      }
      return rc;
   }
   /**
    * Find available bandwidth between along the path for the 
    * service class
    */
   public long findAvailableBW (PTP [] path, String svcClassID)
                                     throws CDBException
   {
      long leastAvlBW = -1;

      BWPerClassHome bpch = (BWPerClassHome) cdbHome.getHomebyName ("BWPerClass");
      for (int i = 0; i < path.length; i++)
      {
         String ptpID = path [i].getID ();
	 String inoutValue = "1"; // inbound by default
         AttributeValues av = new AttributeValues ();
         av.addNameValue (BWPerClass.attrPtpID, ptpID);
         av.addNameValue (BWPerClass.attrSvcClassID, svcClassID);
	 // for the last hop get BWPerClass instance for outgoing traffic
         if (i == (path.length -1))
         {
	    inoutValue = "0";
         }
	 av.addNameValue (BWPerClass.attrInbound, inoutValue);

         ObjectList ol = bpch.findByAttributes (av);

	 // In case of L3 BWPerClass for outgoing traffic may not
	 // be defined! In that case ol.size = 0;
         if (ol.size () > 0)
         {
            av = ol.get (0).getAttributes ();
            long avlBW = Long.parseLong (av.getValueForName (BWPerClass.attrBWTotal));

            avlBW = avlBW - Long.parseLong (av.getValueForName (BWPerClass.attrBWAlloc));
            if (leastAvlBW == -1)
               leastAvlBW = avlBW;
            else if (leastAvlBW > avlBW)
               leastAvlBW = avlBW;
         }
      }
      return leastAvlBW;

   }

   public String getRouterOfL3Interface (String interfaceID)
					throws CDBException
   {
      L3InterfaceHome l3ih = (L3InterfaceHome) cdbHome.getHomebyName ("L3Interface");
      L3Interface inf = (L3Interface) l3ih.findByID (interfaceID);
      
      AttributeValues av = inf.getAttributes ();
      return av.getValueForName (L3Interface.attrRouterID);
   }


   /**
    * Find the path between ingress and egress Interfages
    */
   public PTP [] lookupL3Path (String inInterfaceID, 
                             String outInterfaceID,
			     boolean oldPath) 
                             throws CDBException
   {
      L3InterfaceHome l3ih = (L3InterfaceHome) cdbHome.getHomebyName ("L3Interface");
      PTPHome ptph = (PTPHome) cdbHome.getHomebyName ("PTP");

      L3Interface inInf = (L3Interface) l3ih.findByID (inInterfaceID);
      L3Interface outInf = (L3Interface) l3ih.findByID (outInterfaceID);
      
      AttributeValues av = inInf.getAttributes ();
      String ingress = av.getValueForName (L3Interface.attrRouterID);
      String inPTP =  av.getValueForName (L3Interface.attrPTPID);
      PTP in = (PTP) ptph.findByID (inPTP);

      av = outInf.getAttributes ();
      String egress = av.getValueForName (L3Interface.attrRouterID);
      String outPTP =  av.getValueForName (L3Interface.attrPTPID);
      PTP out = (PTP) ptph.findByID (outPTP);

      return getEdgeToEdgePath (in, out, ingress, egress, oldPath);

   }

   /**
    * Find the path between ingress and egress Interfages
    */
   public PTP [] getEdgeToEdgePath (PTP in, PTP out,
                                    String ingress, 
                                    String egress)
                                    throws CDBException
   {
      return getEdgeToEdgePath (in, out, ingress, egress, false);
   }
   /**
    * Find the path between ingress and egress Interfages
    */
   public PTP [] getEdgeToEdgePath (PTP in, PTP out,
                                    String ingress, 
                                    String egress,
				    boolean oldPath)
                                    throws CDBException
   {
      //BBServer.bblogger.info ("ingress ="+ingress+", egress="+egress);

      if (ingress.equals (egress))
      {
         if (in == null && out == null)
            return new PTP [0];

         PTP [] rc =  new PTP [2];
         rc [0] = in;
         rc [1] = out;
         return rc;
      }

      AttributeValues av = new AttributeValues ();
      av.addNameValue (L3Path.attrIngressID, ingress); 
      av.addNameValue (L3Path.attrEgressID, egress);
      L3PathHome l3ph = (L3PathHome) cdbHome.getHomebyName ("L3Path");
      ObjectList ol = l3ph.findByAttributes (av);

      String l3PathID = null;
      if (ol.size () > 0)
      {
         l3PathID = ol.get (0).getID ();
      }
      else
	  throw new CDBException ();

      L3HopsHome lhh = (L3HopsHome) cdbHome.getHomebyName ("L3Hops");
      L3InterfaceHome l3ih = (L3InterfaceHome) cdbHome.getHomebyName ("L3Interface");
      L3Interface inInf = null;

      PTPHome ptph = (PTPHome) cdbHome.getHomebyName ("PTP");

      av = new AttributeValues ();
      av.addNameValue (L3Hops.attrL3PathID, l3PathID);

      if (oldPath == true)
         av.addNameValue (L3Hops.attrIsOldPath, "1");
      else
         av.addNameValue (L3Hops.attrIsOldPath, "0");

      ol = lhh.findByAttributes (av);

      String l3hops [] = new String [ol.size ()];

      // If the l3hops is of zero length (and we know ingress != egress) something is amiss
      // This case will arise when the traceroute fails betn two endpoints
      if (l3hops.length == 0)
	  return new PTP[0];

      int offset = 0;
      PTP [] rc;

      if (in == null && out == null) // if in out not specified don't include
      {
         rc = new PTP [l3hops.length];  
         offset = -1;
      }
      else
      {
         rc = new PTP [l3hops.length + 2];  
         rc [0] = in;
         rc [rc.length -1] = out;
      }
      if (ol.size () > 0)
      {
         for (int i = 0; i < ol.size (); i++)
         {
	     // Get the L3Interface hop
            av = ol.get (i).getAttributes ();
            int hopNum = Integer.parseInt (av.getValueForName (L3Hops.attrHopNumber));
            if (hopNum < l3hops.length +1)
               {
		   l3hops [hopNum -1] = av.getValueForName (L3Hops.attrL3InterfaceID);
	    	    // Using the L3InterfaceID, get the PTPID and finally the PTP itself
		   inInf = (L3Interface) l3ih.findByID (l3hops[hopNum -1]);
		   av = inInf.getAttributes ();

		   String ptpID =  av.getValueForName (L3Interface.attrPTPID);
		   rc[hopNum + offset] = (PTP) ptph.findByID (ptpID);
	       }
	 }
      }
      return rc;
   }


   private class L3IDInfo
   {
      String id;
      String linkID;
      
   }
   private L3IDInfo [] ipHopsToL3InterfaceIDs (String ipAddress []) 
                                           throws CDBException
   {
      L3IDInfo [] rc = new L3IDInfo [ipAddress.length];

      L3InterfaceHome l3ih = (L3InterfaceHome) cdbHome.getHomebyName ("L3Interface");

      for (int i = 0; i < ipAddress.length; i++)
      {
         AttributeValues av = new AttributeValues ();
         av.addNameValue (L3Interface.attrIPAddress, ipAddress [i]);
	 Attribute fa [] = {L3Interface.ID, L3Interface.attrLinkID};
	 Vector l3Attrs = l3ih.findAttributesByAttributes (av, fa);
	 rc [i] = new L3IDInfo ();

	 rc [i].id = ((String[])l3Attrs.elementAt (0)) [0];
	 rc [i].linkID = ((String[])l3Attrs.elementAt(1)) [0];
      }
      return rc;
   }

  
   public void setL3Path (String ingressIP, 
                          String egressIP, 
			  String [] ipHops)
			  throws CDBException
   {
      setEdgeToEdgePath (routerIPtoID (ingressIP),
                         routerIPtoID (egressIP),
			 ipHops);
   }

   public void removeOldL3Paths () throws CDBException
   {
      L3HopsHome l3hh = (L3HopsHome) cdbHome.getHomebyName ("L3Hops");
      AttributeValues av = new AttributeValues ();
      av.addNameValue (L3Hops.attrIsOldPath, "1");
      l3hh.remove (av);
   }

   public void makeL3PathsOld () throws CDBException
   {
      L3HopsHome l3hh = (L3HopsHome) cdbHome.getHomebyName ("L3Hops");
      AttributeValues av = new AttributeValues ();
      av.addNameValue (L3Hops.attrIsOldPath, "1");
      l3hh.remove (av);
      av = new AttributeValues ();
      av.addNameValue (L3Hops.attrIsOldPath, "1");
      l3hh.bulkUpdate (av, null);
   }

   public String getL3PathID (String ingressID, 
                              String egressID) throws CDBException
   {
      L3PathHome l3ph = (L3PathHome) cdbHome.getHomebyName ("L3Path");
      AttributeValues av = new AttributeValues ();
      av.addNameValue (L3Path.attrIngressID, ingressID); 
      av.addNameValue (L3Path.attrEgressID, egressID);

      ObjectList ol = l3ph.findByAttributes (av);
      if (ol.size () > 0)
      {
         return ol.get(0).getID();
      }
      else 
         return null;
   }



   /**
    * Set the path between ingress and egress routers
    */
   public void setEdgeToEdgePath (String ingress, 
                                    String egress,
                                    String [] ipHops)
                                    throws CDBException
   {
      //BBServer.bblogger.info ("ingress ="+ingress+", egress="+egress);

      AttributeValues where;
      L3IDInfo l3HopIDs [] = ipHopsToL3InterfaceIDs (ipHops);

      L3PathHome l3ph = (L3PathHome) cdbHome.getHomebyName ("L3Path");
      L3HopsHome l3hh = (L3HopsHome) cdbHome.getHomebyName ("L3Hops");

      AttributeValues av = new AttributeValues ();
      av.addNameValue (L3Path.attrIngressID, ingress); 
      av.addNameValue (L3Path.attrEgressID, egress);

      String l3pID;
      // remove the old path
      ObjectList ol = l3ph.findByAttributes (av);
      if (ol.size () > 0)
      {
         l3pID = ol.get (0).getID ();
	 // remove if there is any old path (sanity check)
         //av = new AttributeValues ();
	 //av.addNameValue (L3Hops.attrIsOldPath, "1");
	 //av.addNameValue (L3Hops.attrL3PathID, l3pID);
	 //l3hh.remove (av);
	 
	 // make the current path old....
         //av = new AttributeValues ();
	 //av.addNameValue (L3Hops.attrIsOldPath, "1");
         //where = new AttributeValues ();
	 //where.addNameValue (L3Hops.attrL3PathID, l3pID);
	 //l3hh.bulkUpdate (av, where);
      }
      else
      {
         av = new AttributeValues ();
         av.addNameValue (L3Path.attrIngressID, ingress); 
         av.addNameValue (L3Path.attrEgressID, egress);
	 l3pID = l3ph.create (av).getID ();
      }
      

      for (int i = 0; i < l3HopIDs.length; i++)
      {
	 // insert current path
         av = new AttributeValues ();
         av.addNameValue (L3Hops.attrL3PathID, l3pID); 
	 av.addNameValue (L3Hops.attrL3InterfaceID, l3HopIDs[i].id);
	 av.addNameValue (L3Hops.attrLinkID, l3HopIDs[i].linkID);
	 av.addNameValue (L3Hops.attrHopNumber, ""+ (i+1));
	 l3hh.create (av);
      }
   }

   public long findAvailBW (String ptpID, String svcClassID) throws CDBException
   {
      BWPerClassHome bpch = (BWPerClassHome) cdbHome.getHomebyName ("BWPerClass");

      AttributeValues av = new AttributeValues ();

      av.addNameValue (BWPerClass.attrPtpID, ptpID);
      av.addNameValue (BWPerClass.attrSvcClassID, svcClassID);
      ObjectList ol = bpch.findByAttributes (av);

      if (ol.size () > 0)
      {
         Top rec = ol.get (0);
         av = rec.getAttributes ();

         long allocBW = Long.parseLong (av.getValueForName (BWPerClass.attrBWAlloc));
         long totalBW = Long.parseLong (av.getValueForName (BWPerClass.attrBWTotal));
         return (totalBW - allocBW);
      }
      else
         return 0;
   }

   public long findTotalBW (String ptpID, String svcClassID) throws CDBException
   {
      BWPerClassHome bpch = (BWPerClassHome) cdbHome.getHomebyName ("BWPerClass");

      AttributeValues av = new AttributeValues ();

      av.addNameValue (BWPerClass.attrPtpID, ptpID);
      av.addNameValue (BWPerClass.attrSvcClassID, svcClassID);
      ObjectList ol = bpch.findByAttributes (av);

      if (ol.size () > 0)
      {
         Top rec = ol.get (0);
         av = rec.getAttributes ();

         long totalBW = Long.parseLong (av.getValueForName (BWPerClass.attrBWTotal));
         return (totalBW);
      }
      else
         return 0;
   }

   /**
    * Reserve bandwidth along the path for the specified serviceClass
    */
   public boolean reserveBW (PTP [] path, long bandwidth, 
                             String svcClassID) throws CDBException
   {
      BWPerClassHome bpch = (BWPerClassHome) cdbHome.getHomebyName ("BWPerClass");
      for (int i = 0; i < path.length; i++)
      {
	 String inoutValue = "1"; // inbound by default
         String ptpID = path [i].getID ();
         AttributeValues av = new AttributeValues ();
         av.addNameValue (BWPerClass.attrPtpID, ptpID);
         av.addNameValue (BWPerClass.attrSvcClassID, svcClassID);
         if (i == (path.length -1))
         {
	    inoutValue = "0";
         }
	 av.addNameValue (BWPerClass.attrInbound, inoutValue);

         ObjectList ol = bpch.findByAttributes (av);
         if (ol.size () > 0)
         {
            Top rec = ol.get (0);
            av = rec.getAttributes ();
            long allocBW = Long.parseLong (av.getValueForName (BWPerClass.attrBWAlloc));
            allocBW = allocBW + bandwidth;

            av = new AttributeValues ();
            av.addNameValue (BWPerClass.attrBWAlloc, ""+allocBW);
            rec.setAttributes (av);

         }
         else
            throw new CDBException ();
      }
      return true;
   }

   /**
    * Release bandwidth along the path for the specified serviceClass
    */
   public void releaseBW (PTP [] path, long bandwidth, 
                          String svcClassID) throws CDBException
   {
      BWPerClassHome bpch = (BWPerClassHome) cdbHome.getHomebyName ("BWPerClass");

      for (int i = 0; i < path.length; i++)
      {
	 String inoutValue = "1"; // inbound by default
         String ptpID = path [i].getID ();
         AttributeValues av = new AttributeValues ();
         av.addNameValue (BWPerClass.attrPtpID, ptpID);
         av.addNameValue (BWPerClass.attrSvcClassID, svcClassID);
         if (i == (path.length -1))
         {
	    inoutValue = "0";
         }
	 av.addNameValue (BWPerClass.attrInbound, inoutValue);
         ObjectList ol = bpch.findByAttributes (av);
         if (ol.size () > 0)
         {
            Top rec = ol.get (0);
            av = rec.getAttributes ();
            long allocBW = Long.parseLong (av.getValueForName (BWPerClass.attrBWAlloc));
            allocBW = allocBW - bandwidth;

            av = new AttributeValues ();
            av.addNameValue (BWPerClass.attrBWAlloc, ""+allocBW);
            rec.setAttributes (av);

         }
         else
            throw new CDBException ();
      }
      return;
   }

   int allocateDSCP (String srcIP, String destIP) throws Exception
   {
      // allocated DSCP value for the flow between two endpoints

      // assumed that flows are always between two hosts
       FlowRecordHome frh = (FlowRecordHome) cdbHome.getHomebyName ("FlowRecord");
       AttributeValues avp = new AttributeValues ();
       avp.addNameValue (FlowRecord.attrSrcIP, srcIP);
       avp.addNameValue (FlowRecord.attrDestIP, destIP);
       ObjectList ol = frh.findByAttributes (avp);

       BigInteger dscpFlags = new BigInteger ("1");
       // bit 0 is always set and never allocated

       for (int i = 0; i < ol.size (); i++)
       {
          avp = ol.get (i).getAttributes ();
          int dscpVal = Integer.parseInt (
	                           avp.getValueForName (FlowRecord.attrDSCP));
	  dscpFlags = dscpFlags.setBit (dscpVal);
       }


       for (int i = 1; i < 64; i++)
       {
          if (dscpFlags.testBit (i))
	     continue;
          else
	     return i;
       }
       throw new Exception ();
      
   }

   /**
    * inserts a new flow record in the data base
    * The status of the record is FlowRecord.statusReserved
    */
   public String insertFlowRecord (String src_ip_Address, 
                                   String src_ip_Mask, 
                                   String srcPort, 
                                   String srcPortHigh,
                                   String dst_ip_Address, 
                                   String dst_ip_Mask,
                                   String dstPort, 
                                   String dstPortHigh,
				   String protocol,
				   int dscp,
				   String serviceClassID,
                                   long bandwidthMin, 
                                   long extraBudget, 
                                   long allocatedBW,
                                   String inInterfaceID, 
                                   String outInterfaceID,
				   boolean isPureL2,
				   boolean reserveAndCommit,
				   int priority,
				   String l3PathID,
				   String reference)
       throws CDBException
   {
       FlowRecordHome frh = (FlowRecordHome) cdbHome.getHomebyName ("FlowRecord");
       AttributeValues av = new AttributeValues ();
       av.addNameValue (FlowRecord.attrSrcIP, src_ip_Address);
       av.addNameValue (FlowRecord.attrSrcMask, src_ip_Mask);
       av.addNameValue (FlowRecord.attrSrcPort, srcPort);
       av.addNameValue (FlowRecord.attrSrcPortHigh, srcPortHigh);
       av.addNameValue (FlowRecord.attrDestIP, dst_ip_Address);
       av.addNameValue (FlowRecord.attrDestMask,dst_ip_Mask );
       av.addNameValue (FlowRecord.attrDestPort, dstPort);
       av.addNameValue (FlowRecord.attrDestPortHigh, dstPortHigh);
       av.addNameValue (FlowRecord.attrProtocol, protocol);
       av.addNameValue (FlowRecord.attrDSCP, "" + dscp);
       av.addNameValue (FlowRecord.attrBWMin, new Long (bandwidthMin).toString());
       av.addNameValue (FlowRecord.attrExtraBudget,new Long (extraBudget).toString());
       av.addNameValue (FlowRecord.attrBWAlloc, new Long (allocatedBW).toString());
       av.addNameValue (FlowRecord.attrSvcClassID, serviceClassID );
       av.addNameValue (FlowRecord.attrInInterfaceID, inInterfaceID);
       av.addNameValue (FlowRecord.attrOutInterfaceID, outInterfaceID);
       av.addNameValue (FlowRecord.attrPriority, "" + priority);
       av.addNameValue (FlowRecord.attrL3PathID, l3PathID);
       av.addNameValue (FlowRecord.attrReference, reference);

       if (reserveAndCommit == true)
          av.addNameValue (FlowRecord.attrStatus, FlowRecord.statusCommitting);
       else
          av.addNameValue (FlowRecord.attrStatus, FlowRecord.statusReserved);

       if (isPureL2)
          av.addNameValue (FlowRecord.attrL2Endpoints, "1");
       else
          av.addNameValue (FlowRecord.attrL2Endpoints, "0");

       Top t= frh.create (av);
       FlowRecord  flowRecord = frh.narrow (t);
       return flowRecord.getID();
   }

   public void commit () throws CDBException
   {
      cdbHome.commit ();
   }

   public void rollback () throws CDBException
   {
      cdbHome.rollback ();
   }

   public void removeFlow (String flowID) throws CDBException
   {
       FlowRecordHome frh = (FlowRecordHome) cdbHome.getHomebyName ("FlowRecord");
       frh.remove (flowID);
   }

   public FlowRecord findFlowRecord (String flowID) throws CDBException
   {
      FlowRecordHome frh = (FlowRecordHome) cdbHome.getHomebyName ("FlowRecord");

      return (FlowRecord) frh.findByID (flowID);
   }

   public FlowRecord findFlowRecordByReference (String reference) 
                                               throws CDBException
   {
      FlowRecordHome frh = (FlowRecordHome) cdbHome.getHomebyName ("FlowRecord");
      AttributeValues av = new AttributeValues ();
      av.addNameValue (FlowRecord.attrReference, reference);
      ObjectList olc = frh.findByAttributes (av);
      if (olc.size () > 0)
         return (FlowRecord)olc.get(0);
      else
         return null;
   }

   // gets all ids of all the flows in the database
   Vector getAllFlows () throws CDBException
   {
      FlowRecordHome frh = (FlowRecordHome) cdbHome.getHomebyName ("FlowRecord");
      AttributeValues av = new AttributeValues ();
      ObjectList olc = frh.findByAttributes (av);
      Vector rc = new Vector ();

      for (int i = 0; i < olc.size (); i++)
      {
         rc.add (olc.get (i).getID ());
      }

      return rc;
   }

   // gets all the flows in the database
   public Vector getAllFlowRecords () throws CDBException
   {
      FlowRecordHome frh = (FlowRecordHome) cdbHome.getHomebyName ("FlowRecord");
      AttributeValues av = new AttributeValues ();
      ObjectList olc = frh.findByAttributes (av);
      Vector rc = new Vector ();

      for (int i = 0; i < olc.size (); i++)
      {
         rc.add (olc.get (i));
      }

      return rc;
   }
 
     
   public void removeDeletedFlowRecords () throws CDBException
  {
	
      FlowRecordHome frh = (FlowRecordHome) cdbHome.getHomebyName ("FlowRecord");
      AttributeValues av = new AttributeValues ();
      av.addNameValue (FlowRecord.attrStatus, FlowRecord.statusDeleted);
      ObjectList old = frh.findByAttributes (av);
 
      for (int i = 0; i < old.size (); i++)
      {
         frh.remove (old.get (i).getID ());
       }
          
  }

   public String [] findZombieFlows () throws CDBException
   {
      String rc [];

      FlowRecordHome frh = (FlowRecordHome) cdbHome.getHomebyName ("FlowRecord");
      AttributeValues av = new AttributeValues ();
      av.addNameValue (FlowRecord.attrStatus, FlowRecord.statusDeleted);
      ObjectList old = frh.findByAttributes (av);
      av = new AttributeValues ();
      av.addNameValue (FlowRecord.attrStatus, FlowRecord.statusCommitting);
      ObjectList olc = frh.findByAttributes (av);

      rc = new String [old.size () + olc.size ()];

      for (int i = 0; i < old.size (); i++)
      {
         rc [i] = old.get (i).getID ();
      }
      int j = old.size ();
      for (int i = 0; i < olc.size (); i++)
      {
         rc [j+i] = olc.get (i).getID ();
      }
      return rc;
   }

   public PTP [] getPtpsOnSwitch (String switchID) throws CDBException
   {
      PTPHome ph = (PTPHome) cdbHome.getHomebyName ("PTP");
      AttributeValues av = new AttributeValues ();
      av.addNameValue (PTP.attrSwitchID, switchID);
      ObjectList ol = ph.findByAttributes (av);

      PTP [] rc = new PTP [ol.size ()];
      for ( int i = 0; i < ol.size (); i++ )
      {
         rc [i] = (PTP)ol.get (i);
      }
      return rc;
   }

   public L2Interface findL2Interface (String infID) throws CDBException
   {
      L2InterfaceHome l2h = (L2InterfaceHome) cdbHome.getHomebyName ("L2Interface");

      return (L2Interface) l2h.findByID (infID);
   }

   public L3Interface findL3Interface (String infID) throws CDBException
   {
      L3InterfaceHome l3h = (L3InterfaceHome) cdbHome.getHomebyName ("L3Interface");

      return (L3Interface) l3h.findByID (infID);
   }

   public String routerIPtoID (String routerIP) throws CDBException
   {
      RouterHome rh = (RouterHome) cdbHome.getHomebyName ("Router");

      AttributeValues av = new AttributeValues ();
      av.addNameValue (Router.attrManIP, routerIP);

      ObjectList ol = rh.findByAttributes (av);

      if (ol.size () > 0)
      {
         return ol.get (0).getID ();
      }
      throw new CDBException ();
   }


   public Router findRouter (String routerID) throws CDBException
   {
      RouterHome rh = (RouterHome) cdbHome.getHomebyName ("Router");

      return (Router) rh.findByID (routerID);
   }

   public Switch findSwitch (String switchID) throws CDBException
   {
      SwitchHome sh = (SwitchHome) cdbHome.getHomebyName ("Switch");

      return (Switch) sh.findByID (switchID);
   }

   public ObjectList getAllSwitches () throws CDBException
   {
      SwitchHome sh = (SwitchHome) cdbHome.getHomebyName ("Switch");

      return sh.findByAttributes (null);
   }

   public String [] getAllRoutersIPs () throws CDBException
   {
      RouterHome rh = (RouterHome) cdbHome.getHomebyName ("Router");

      Attribute fa [] = {Router.attrManIP}; 
      return (String[])rh.findAttributesByAttributes (null, fa).elementAt(0) ;
   }

   public ObjectList getAllRouters () throws CDBException
   {
      RouterHome rh = (RouterHome) cdbHome.getHomebyName ("Router");

      return rh.findByAttributes (null);
   }


   public ServiceClass findServiceClass (String svcClassID) throws CDBException
   {
      ServiceClassHome sch = (ServiceClassHome) cdbHome.getHomebyName ("ServiceClass");

      return (ServiceClass) sch.findByID (svcClassID);
   }

    
   public String findSubnetInPool (String poolName) throws CDBException
   {
      HostSubnetHome hsh = 
                   (HostSubnetHome) cdbHome.getHomebyName ("HostSubnet");

      AttributeValues av = new AttributeValues ();

      av.addNameValue (HostSubnet.attrPoolName, poolName);

      ObjectList ol = hsh.findByAttributes (av);

      if (ol.size () > 0)
      {
         av = ol.get (0).getAttributes ();
         return av.getValueForName (HostSubnet.attrHostSubnetAddr);
      }

      throw new CDBException ();
   }
      
   public String findSubnetForHost (String hostname) throws CDBException
   {
      HostSubnetHome hsh = 
	  (HostSubnetHome) cdbHome.getHomebyName ("HostSubnet");

      AttributeValues av = new AttributeValues ();
      av.addNameValue (HostSubnet.attrHostname, hostname);

      ObjectList ol = hsh.findByAttributes (av);

      if (ol.size () > 0)
      {
         av = ol.get (0).getAttributes ();
         return av.getValueForName (HostSubnet.attrHostSubnetAddr);
      }

      throw new CDBException ();
   }

   public String findHostInPool (String poolName) throws CDBException
   {
      HostSubnetHome hsh = 
                   (HostSubnetHome) cdbHome.getHomebyName ("HostSubnet");

      AttributeValues av = new AttributeValues ();

      av.addNameValue (HostSubnet.attrPoolName, poolName);

      ObjectList ol = hsh.findByAttributes (av);

      if (ol.size () > 0)
      {
         av = ol.get (0).getAttributes ();
         return av.getValueForName (HostSubnet.attrHostname);
      }

      throw new CDBException ();
   }
   
    public String getServiceClassName (String svcClassID) throws CDBException
   {
      ServiceClassHome sch = (ServiceClassHome) cdbHome.getHomebyName ("ServiceClass");
      ServiceClass sc =  (ServiceClass) sch.findByID (svcClassID);
      AttributeValues av = sc.getAttributes ();
      return av.getValueForName (ServiceClass.attrName);
   }
   
    
    public ObjectList findAllBWPerClass () throws CDBException
    {
	 BWPerClassHome bh = (BWPerClassHome) cdbHome.getHomebyName ("BWPerClass");
	 return bh.findByAttributes (null);
    }
    
    public long getRate (String ptpID) throws CDBException
    {
	PTPHome ph = (PTPHome) cdbHome.getHomebyName ("PTP");
	PTP p = (PTP) ph.findByID (ptpID);
	AttributeValues av = p.getAttributes ();	
	long rate = Long.parseLong (av.getValueForName (PTP.attrRate));
	return rate;
    }

    /** Obtains the collected of flowRecords in reserved status */
    public FlowRecord [] findAllReservedFlowRecordsToPurge () throws CDBException
    {
	FlowRecordHome frh = (FlowRecordHome) cdbHome.getHomebyName ("FlowRecord");
	AttributeValues av = new AttributeValues ();
	av.addNameValue (FlowRecord.attrStatus, FlowRecord.statusReserved);
	av.addNameValue (FlowRecord.purgeStatus, new String ("1"));
	ObjectList ol = frh.findByAttributes (av);

	FlowRecord [] fr = new FlowRecord [ol.size()];
	for (int i=0; i< ol.size(); i++)
	    fr[i] = (FlowRecord)ol.get(i);
	    
	return fr;

    }

    /** Activate flowRecords for purging */
    public void purgeActivateReservedFlowRecords () throws CDBException
    {
	FlowRecordHome frh = (FlowRecordHome) cdbHome.getHomebyName ("FlowRecord");
	AttributeValues av = new AttributeValues ();
	av.addNameValue (FlowRecord.attrStatus, FlowRecord.statusReserved);
	av.addNameValue (FlowRecord.purgeStatus, new String ("0"));
	ObjectList ol = frh.findByAttributes (av);
	
	if (ol != null)
	    {
		for (int i = 0 ; i < ol.size () ; i++)
		    {
			av = new AttributeValues ();
			av.addNameValue (FlowRecord.purgeStatus, 
					 new Integer ("1").toString());
			FlowRecord fr = (FlowRecord)(ol.get(i));
			fr.setAttributes (av);
		    }
	    }    
    }


    /** Get a list of Switch Managed IP addresses from the Switch table */
    public String[] getSwitchMIPs () throws CDBException
    {
        SwitchHome sh = (SwitchHome) cdbHome.getHomebyName("Switch");

        ArrayList sw = new ArrayList();
			
	// get all the switches
	ObjectList ol = sh.findByAttributes(null);
	for (int i= 0; i<ol.size(); i++) {
		Switch s = (Switch) ol.get(i);

		AttributeValues av = s.getAttributes();
		sw.add(av.getValueForName(Switch.attrManIP));
	} 

	return (String[]) sw.toArray(new String [] {});


    }


    /** Find PTP Id from the PTP Name for a specific switch*/
    public String getPtpIDFromName (String swID, String name) throws CDBException 
    {

	PTPHome ph = (PTPHome) cdbHome.getHomebyName("PTP"); 
	AttributeValues av = new AttributeValues ();

	av.addNameValue (PTP.attrName, name);
	av.addNameValue (PTP.attrSwitchID, swID);
	ObjectList ol = ph.findByAttributes (av);

	if (ol.size () > 0)
		return ol.get(0).getID ();

	return null;

	// throw new CDBException ();
    }

    /** Find Switch Id from the L2 Interface ID */
    public String getSwitchIDFromL2IntfID (String intf) throws CDBException 
    {
	L2InterfaceHome lh = (L2InterfaceHome) cdbHome.getHomebyName("L2Interface");

	L2Interface l2 = (L2Interface) lh.findByID(intf);
	AttributeValues av = l2.getAttributes();
	return (av.getValueForName(L2Interface.attrSwitchID));
	  
    }

    /** Find VLAN number from the L2 Interface ID */
    public String getVlanFromL2IntfID (String intf) throws CDBException 
    {
	L2InterfaceHome lh = (L2InterfaceHome) cdbHome.getHomebyName("L2Interface");
	L2Interface l2 = (L2Interface) lh.findByID(intf);

        AttributeValues av = l2.getAttributes();
	String ptpID = av.getValueForName(L2Interface.attrPTPID);

        activeVLANHome avh = (activeVLANHome) cdbHome.getHomebyName("activeVLAN");
	AttributeValues av2 = new AttributeValues(); 
	av2.addNameValue(activeVLAN.attrPtpID, ptpID);
	ObjectList ol = avh.findByAttributes (av2);

	if (ol.size () == 0) throw new CDBException();

	activeVLAN v = (activeVLAN) ol.get(0);
	AttributeValues av3 = v.getAttributes();
	return(av3.getValueForName(activeVLAN.attrVlanID));
	
    }

    /** Find VLAN number from the PTP ID */
    public String getVlanFromPtpID (String ptpID) throws CDBException 
    {

        activeVLANHome avh = (activeVLANHome) cdbHome.getHomebyName("activeVLAN");
	AttributeValues av = new AttributeValues(); 
	av.addNameValue(activeVLAN.attrPtpID, ptpID);
	ObjectList ol = avh.findByAttributes (av);

	if (ol.size () == 0) throw new CDBException();

	activeVLAN v = (activeVLAN) ol.get(0);
	AttributeValues av2 = v.getAttributes();
	return(av2.getValueForName(activeVLAN.attrVlanID));
    }

    public String [] getActiveVlansOnPTP (String ptpID) throws CDBException
    {

       Vector result = new Vector ();
       activeVLANHome avh = (activeVLANHome) cdbHome.getHomebyName("activeVLAN"); 
       AttributeValues av1 = new AttributeValues ();
       av1.addNameValue (activeVLAN.attrPtpID, ptpID);
	
       ObjectList ol = avh.findByAttributes (av1);

       for (int i = 0; i < ol.size (); i++)
       {
          Top t = ol.get (i);
	  av1 = t.getAttributes ();
	  String vlan = av1.getValueForName (activeVLAN.attrVlanID);
	  result.add (vlan);
       }
       String rc [] = new String [result.size ()];
       result.toArray (rc);
       return rc;

    }

    public String [] getActiveVlansOnLink (String linkID) throws CDBException
    {
	LinkHome lh = (LinkHome) cdbHome.getHomebyName("Link");
	Link l = (Link) lh.findByID (linkID);
	AttributeValues av = l.getAttributes ();
	String ptpA =  av.getValueForName(Link.attrAPtpID);
	String ptpZ =  av.getValueForName(Link.attrAPtpID);

	String [] vlansOnA = getActiveVlansOnPTP (ptpA);
	String [] vlansOnZ = getActiveVlansOnPTP (ptpZ);

	Vector result = new Vector ();
	Arrays.sort (vlansOnZ);
	for (int i = 0; i < vlansOnA.length; i++)
	{
	   if (Arrays.binarySearch (vlansOnZ, vlansOnA [i]) >= 0)
	      result.add (vlansOnA [i]);
	}
	String [] rc = new String [result.size ()];
	result.toArray (rc); 
	return rc;
    }

    /** Find Switch Id from the Switch Managed IP Address*/
    public String getSwitchIDFromMIP (String mip) throws CDBException 
    {

	SwitchHome sh = (SwitchHome) cdbHome.getHomebyName("Switch"); 
	AttributeValues av = new AttributeValues ();

	av.addNameValue (Switch.attrManIP, mip);
	ObjectList ol = sh.findByAttributes (av);

	if (ol.size () > 0) {
		return ol.get(0).getID ();
		
	} 
	throw new CDBException ();
    }

    /** Find Switch Name from the Switch ID*/
    public String getSwitchNameFromID (String swID) throws CDBException 
    {

	SwitchHome sh = (SwitchHome) cdbHome.getHomebyName("Switch"); 
	AttributeValues av = new AttributeValues ();

	av.addNameValue (Switch.ID, swID);
	ObjectList ol = sh.findByAttributes (av);

	if (ol.size () > 0) {
		Switch s = (Switch) ol.get(0);

		AttributeValues av2 = s.getAttributes();
		return (av2.getValueForName(Switch.attrName));
	}
	throw new CDBException ();
    } 

    /** Find Read Community String from the Switch ID*/
    public String getCommunityFromSwitchID (String swID) throws CDBException 
    {

	SwitchHome sh = (SwitchHome) cdbHome.getHomebyName("Switch"); 
	AttributeValues av = new AttributeValues ();

	av.addNameValue (Switch.ID, swID);
	ObjectList ol = sh.findByAttributes (av);

	if (ol.size () > 0) {
		Switch s = (Switch) ol.get(0);

		AttributeValues av2 = s.getAttributes();
		return (av2.getValueForName(Switch.attrReadCommunity));
	}
	throw new CDBException ();
    }


    /** Find OS Version from the Switch ID*/
    public String getOSFromSwitchID (String swID) throws CDBException 
    {

	SwitchHome sh = (SwitchHome) cdbHome.getHomebyName("Switch"); 
	AttributeValues av = new AttributeValues ();

	av.addNameValue (Switch.ID, swID);
	ObjectList ol = sh.findByAttributes (av);

	if (ol.size () > 0) {
		Switch s = (Switch) ol.get(0);

		AttributeValues av2 = s.getAttributes();
		return (av2.getValueForName(Switch.attrOSVersion));
	}
	throw new CDBException ();
    }

    /** Find the isMonitored status from the Switch name*/
    public String isSwitchMonitored (String swName) throws CDBException 
    {

	SwitchHome sh = (SwitchHome) cdbHome.getHomebyName("Switch"); 
	AttributeValues av = new AttributeValues ();

	av.addNameValue (Switch.attrName, swName);
	ObjectList ol = sh.findByAttributes (av);

	if (ol.size () > 0) {
		Switch s = (Switch) ol.get(0);

		AttributeValues av2 = s.getAttributes();
		return (av2.getValueForName(Switch.attrIsMonitored));
	}
	throw new CDBException ();
    }

    /** update the isMonitored status from the Switch name*/
    public void updateSwitchMonitoringStatus (String sw, String status) throws CDBException 
    {

	SwitchHome sh = (SwitchHome) cdbHome.getHomebyName("Switch"); 
	AttributeValues av = new AttributeValues ();

	av.addNameValue (Switch.attrName, sw);
	ObjectList ol = sh.findByAttributes (av);

	if (ol.size () > 0) {
		Top s =  ol.get(0);
		AttributeValues av2 = new AttributeValues();
		av2.addNameValue(Switch.attrIsMonitored, status);
		s.setAttributes(av2);
		return;
	}
	throw new CDBException ();
    }


    /** get the port Name given a Switch MIP and ifIndex */
    public String getPortName (String swID, String ifIndex) throws CDBException
    {

	int index = Integer.parseInt(ifIndex);

	PTPHome ptph = (PTPHome) cdbHome.getHomebyName("PTP"); 
	AttributeValues av = new AttributeValues ();


	av.addNameValue (PTP.attrIfIndex, String.valueOf(index));
	av.addNameValue (PTP.attrSwitchID, swID);
	ObjectList ol = ptph.findByAttributes (av);

	if (ol.size () > 0) {
		PTP p = (PTP) ol.get(0);

		AttributeValues av2 = p.getAttributes();
		return (av2.getValueForName(PTP.attrName));
	}

	throw new CDBException ();
    }

    /** update the ifIndex for a Switch, PTP */
    public void updateIfIndex (String swID, String ptpName, String ifIndex) throws CDBException 
    {

	PTPHome ptph = (PTPHome) cdbHome.getHomebyName("PTP"); 
	AttributeValues av = new AttributeValues ();

	av.addNameValue (PTP.attrName, ptpName);
	av.addNameValue (PTP.attrSwitchID, swID);
	ObjectList ol = ptph.findByAttributes (av);

	if (ol.size () > 0) {
		Top p = ol.get(0);
		AttributeValues av2 = new AttributeValues();
		av2.addNameValue(PTP.attrIfIndex, ifIndex);
		p.setAttributes(av2);
		return;
	}
	throw new CDBException ();
    }


    /** get the admin and operational status for a switch, ifIndex */
    public String getIfIndexStatus (String swID, String ifIndex) throws CDBException 
    {

	PTPHome ptph = (PTPHome) cdbHome.getHomebyName("PTP"); 
	AttributeValues av = new AttributeValues ();

	av.addNameValue (PTP.attrIfIndex, ifIndex);
	av.addNameValue (PTP.attrSwitchID, swID);
	ObjectList ol = ptph.findByAttributes (av);

	if (ol.size () > 0) {
		PTP p = (PTP) ol.get(0);
		AttributeValues av2 = p.getAttributes();
		return (av2.getValueForName(PTP.attrOpnState));
        }

        throw new CDBException();

    }

    public String getLinkStatus (String linkID) throws CDBException
    {
       LinkHome lh = (LinkHome) cdbHome.getHomebyName ("Link");
       Link l  = (Link) lh.findByID (linkID);
       AttributeValues av  = l.getAttributes ();
       String ptpIDA = av.getValueForName (Link.attrAPtpID);
       String ptpIDZ = av.getValueForName (Link.attrZPtpID);

       PTPHome ptph = (PTPHome) cdbHome.getHomebyName("PTP"); 
       PTP p = (PTP) ptph.findByID (ptpIDA);
       av = p. getAttributes ();
       if (av.getValueForName(PTP.attrOpnState).equals ("up")) 
       {
          p = (PTP) ptph.findByID (ptpIDZ);
          av = p. getAttributes ();
          if (av.getValueForName(PTP.attrOpnState).equals ("up")) 
	     return "up";
       }

       return "down";
    }

    /** This function returns the switch ID from the PTP ID */
    public String getLinkFromPtp (String ptpID) throws CDBException
    {
        LinkHome lh = (LinkHome) cdbHome.getHomebyName("Link");
        AttributeValues av = new AttributeValues ();

        av.addNameValue (Link.attrAPtpID, ptpID);
        ObjectList ol = lh.findByAttributes (av);
        Link l;

        if (ol.size () > 0) {
            l = (Link) ol.get(0);
            return l.getID ();
        }
        av = new AttributeValues ();
        av.addNameValue (Link.attrZPtpID, ptpID);
        ol = lh.findByAttributes (av);

        if (ol.size () > 0) {
            l = (Link) ol.get(0);
            return l.getID ();
        }
        return null;
    }


    /** update the ifIndex Admin and Op status for a Switch, PTP */
    public void updateIfIndexStatus (String swID, String ifIndex, String opStatus, String adStatus) throws CDBException 
    {

	PTPHome ptph = (PTPHome) cdbHome.getHomebyName("PTP"); 
	AttributeValues av = new AttributeValues ();

	av.addNameValue (PTP.attrIfIndex, ifIndex);
	av.addNameValue (PTP.attrSwitchID, swID);
	ObjectList ol = ptph.findByAttributes (av);

	if (ol.size () > 0) {
		Top p = ol.get(0);
		AttributeValues av2 = new AttributeValues();
		av2.addNameValue(PTP.attrAdminState, adStatus);
		av2.addNameValue(PTP.attrOpnState, opStatus);
		p.setAttributes(av2);
		return;
	}
	throw new CDBException ();
    }
    /** Find the operational status from the Switch name*/
    public String getSwitchStatus (String swName) throws CDBException 
    {

	SwitchHome sh = (SwitchHome) cdbHome.getHomebyName("Switch"); 
	AttributeValues av = new AttributeValues ();

	av.addNameValue (Switch.attrName, swName);
	ObjectList ol = sh.findByAttributes (av);

	if (ol.size () > 0) {
		Switch s = (Switch) ol.get(0);

		AttributeValues av2 = s.getAttributes();
		return (av2.getValueForName(Switch.attrOpnState));
	}
	throw new CDBException ();
    }

    /** Utility function to get the switch ID from the name */
    public String getSwitchIDFromName(String sw) throws CDBException
    {
	// get the switch ID from the name
	SwitchHome sh = (SwitchHome) cdbHome.getHomebyName("Switch"); 
	AttributeValues av = new AttributeValues ();

	av.addNameValue (Switch.attrName, sw);
	ObjectList ol = sh.findByAttributes (av);

	if (ol.size () == 0)
          throw new CDBException();

	return(ol.get(0).getID ());      
     }

    /** Find all the PTPs on a switch, and report on the operational and admin status
     *  Return a Hashtable of the PTPs and the status as (key, value) 
     *  The status is "adminState:operationalState:lastEvent"
     */
    public Hashtable getSwitchPortStatus (String switchName) throws CDBException
    {

	Hashtable portStatus = new Hashtable();

        String switchID = this.getSwitchIDFromName(switchName);  

	// first find all the PTPIDs that belong to a switch
	PTPHome ptph = (PTPHome) cdbHome.getHomebyName("PTP"); 
	AttributeValues av1 = new AttributeValues ();
	av1.addNameValue(PTP.attrSwitchID, switchID);

	ObjectList ol1 = ptph.findByAttributes(av1);

	for (int i=0; i<ol1.size(); i++) {

	    PTP p = (PTP) ol1.get(i);
	    AttributeValues av2 = p.getAttributes();
            String ptpName = av2.getValueForName(PTP.attrName);
            String admin = av2.getValueForName(PTP.attrAdminState);
            String opn = av2.getValueForName(PTP.attrOpnState);
            String event = av2.getValueForName(PTP.attrLastEvent);
            portStatus.put(ptpName, admin + ":" + opn + ":" + event);
	}

	return portStatus;

    }

    /** Find all the PTPs on a switch
     */
    public String[] getSwitchPtps (String switchID) throws CDBException
    {

	ArrayList ports = new ArrayList();

	// find all the PTPIDs that belong to a switch
	PTPHome ptph = (PTPHome) cdbHome.getHomebyName("PTP"); 
	AttributeValues av1 = new AttributeValues ();
	av1.addNameValue(PTP.attrSwitchID, switchID);

	ObjectList ol1 = ptph.findByAttributes(av1);

	for (int i=0; i<ol1.size(); i++) {

	    PTP p = (PTP) ol1.get(i);
	    AttributeValues av2 = p.getAttributes();
            ports.add(av2.getValueForName(PTP.attrName));
	}

	return (String[]) ports.toArray(new String[] {});

    }

    /** Find all the ifIndices on a switch
     */
    public String[] getSwitchIfIndices (String switchID) throws CDBException
    {

	ArrayList indices = new ArrayList();

	// find all the PTPIDs that belong to a switch
	PTPHome ptph = (PTPHome) cdbHome.getHomebyName("PTP"); 
	AttributeValues av1 = new AttributeValues ();
	av1.addNameValue(PTP.attrSwitchID, switchID);

	ObjectList ol1 = ptph.findByAttributes(av1);

	for (int i=0; i<ol1.size(); i++) {

	    PTP p = (PTP) ol1.get(i);
	    AttributeValues av2 = p.getAttributes();
            indices.add(av2.getValueForName(PTP.attrIfIndex));
	}

	return (String[]) indices.toArray(new String[] {});

    }

    /** update the Fault Monitor subscriptions in the DB */
    public String updateFaultSubscriptions(String eventSinkName, 
			String cookie, String sw, int eventType) throws CDBException  
    {

       String swID = this.getSwitchIDFromName(sw);

       // write the subscription into the DB, if it violates unique key constraint
       // an exception will be thrown - we're not too concerned
       FaultSubscriptionsHome fsh = (FaultSubscriptionsHome) cdbHome.getHomebyName("FaultSubscriptions"); 
       AttributeValues av = new AttributeValues ();

       av.addNameValue (FaultSubscriptions.attrEventSinkName, eventSinkName);
       av.addNameValue (FaultSubscriptions.attrCookie, cookie);
       av.addNameValue (FaultSubscriptions.attrSwitchID, swID);
       av.addNameValue (FaultSubscriptions.attrEventType, String.valueOf(eventType));

       Top t = fsh.create(av);
       FaultSubscriptions fs = fsh.narrow(t);
       return fs.getID();

    }


    /** remove Fault Monitor subscriptions from the DB - return the switches associated with this 
        eventSinkName, cookie pair */
    public String[] removeFaultSubscriptions(String eventSinkName, 
			String cookie) throws CDBException  
    {

       FaultSubscriptionsHome fsh = (FaultSubscriptionsHome) cdbHome.getHomebyName("FaultSubscriptions"); 
       AttributeValues av = new AttributeValues ();

       av.addNameValue (FaultSubscriptions.attrEventSinkName, eventSinkName);
       av.addNameValue (FaultSubscriptions.attrCookie, cookie);

       ObjectList ol = fsh.findByAttributes(av);

       ArrayList switches = new ArrayList();
       
       for (int i=0; i<ol.size(); i++) {

         FaultSubscriptions fs = (FaultSubscriptions) ol.get(i);
         String sw = fs.getAttributes().getValueForName(FaultSubscriptions.attrSwitchID); 
	 switches.add(sw);

       }

       fsh.remove(av);
       return (String[]) switches.toArray(new String [] {});

    }


    /** check to see if there are any other subscriptions on this switch */
    public boolean isSwitchSubscribed(String swID) throws CDBException
    {

      FaultSubscriptionsHome fsh = (FaultSubscriptionsHome) cdbHome.getHomebyName("FaultSubscriptions"); 
      AttributeValues av = new AttributeValues ();

      av.addNameValue (FaultSubscriptions.attrSwitchID, swID);
       
      ObjectList ol = fsh.findByAttributes(av);

      if (ol.size() > 0)
        return true;

      return false;
    }

    /** get the switch access in terms of pwd:enPwd:userName */
    public String getSwitchPwds(String swID) throws CDBException
    {
      SwitchHome sh = (SwitchHome) cdbHome.getHomebyName("Switch");

      AttributeValues av = new AttributeValues();

      av.addNameValue (Switch.ID, swID);

      ObjectList ol = sh.findByAttributes(av);

      if (ol.size() > 0) {

        Switch s = (Switch) ol.get(0);
        AttributeValues av2 = s.getAttributes();

        String pwd = av2.getValueForName(Switch.attrPW);
        String enPwd = av2.getValueForName(Switch.attrEnablePW);

        // need to add the username stuff here

        return (pwd + ":" + enPwd);
      }
      
      return null;
    }


    /** get a list of eventSinks:cookies that are interested in events on this switch */
    public String[] getFaultSinks(String swName, String event) throws CDBException
    {

      FaultSubscriptionsHome fsh = (FaultSubscriptionsHome) cdbHome.getHomebyName("FaultSubscriptions"); 
      AttributeValues av = new AttributeValues ();

      av.addNameValue (FaultSubscriptions.attrSwitchID, this.getSwitchIDFromName(swName));
       
      ObjectList ol = fsh.findByAttributes(av);

      ArrayList sinks = new ArrayList();
      for (int i=0; i < ol.size(); i++) {

        FaultSubscriptions fs = (FaultSubscriptions) ol.get(i);
        AttributeValues av2 = fs.getAttributes(); 
        String sink = av2.getValueForName(FaultSubscriptions.attrEventSinkName);
        String cookie = av2.getValueForName(FaultSubscriptions.attrCookie);

        sinks.add(sink + ":" + cookie);
      }

      return (String []) sinks.toArray(new String[] {}) ;
    }


    /** Add a VLAN to the table */
    public String addVLAN (String switchID, String ptpID, String vlan) throws CDBException 
    {

	activeVLANHome avh = (activeVLANHome) cdbHome.getHomebyName("activeVLAN"); 
	AttributeValues av = new AttributeValues ();

	av.addNameValue (activeVLAN.attrVlanID, vlan);
	av.addNameValue (activeVLAN.attrPtpID, ptpID);
	av.addNameValue (activeVLAN.attrSwitchID, switchID);

	Top t= avh.create (av);
	activeVLAN actVlan = avh.narrow (t);
	return actVlan.getID();


    }

    /** Get a list of unique VLANS from the active VLAN table */
    public String[] getAllVlans () throws CDBException
    {
        activeVLANHome avh = (activeVLANHome) cdbHome.getHomebyName("activeVLAN");

        Hashtable vlans = new Hashtable();
			
	// get all the VLANs
	ObjectList ol = avh.findByAttributes(null);
	for (int i= 0; i<ol.size(); i++) {
		activeVLAN v = (activeVLAN) ol.get(i);

		AttributeValues av = v.getAttributes();
		String vlan = av.getValueForName(activeVLAN.attrVlanID);
		if (!vlans.containsKey(vlan))
		    vlans.put(vlan, vlan);
	} 
	return (String[]) vlans.values().toArray(new String [] {});

    }

    /** Get a list of switches that are members of the specified VLAN, 
	i.e. have one or more ports in the vlan */
    public String[] getAllMemberSwitches(int vlanID) throws CDBException
    {
        Hashtable switches = new Hashtable();

        activeVLANHome avh = (activeVLANHome) cdbHome.getHomebyName("activeVLAN");
	AttributeValues av = new AttributeValues ();
	av.addNameValue (activeVLAN.attrVlanID, String.valueOf(vlanID));
	
	// get all the switches for this VLAN
	ObjectList ol = avh.findByAttributes (av);
			
	for (int i= 0; i<ol.size(); i++) {
		activeVLAN v = (activeVLAN) ol.get(i);

		AttributeValues av2 = v.getAttributes();
		String sw = av2.getValueForName(activeVLAN.attrSwitchID);
		if (!switches.containsKey(sw))
		    switches.put(sw, sw);
	} 
	return (String[]) switches.values().toArray(new String [] {});

    }

    /** Gets the first switchID that corresponds to the passed VLAN id */
    public String getRootSwitch (String vlanID) throws CDBException
    { 

	activeVLANHome avh = (activeVLANHome) cdbHome.getHomebyName("activeVLAN"); 
	AttributeValues av = new AttributeValues ();
	av.addNameValue (activeVLAN.attrVlanID, vlanID);
	
	ObjectList ol = avh.findByAttributes (av);

	if (ol.size() > 0) {

	    activeVLAN v = (activeVLAN) ol.get(0);
	    AttributeValues av2 = v.getAttributes();
	    return(av2.getValueForName(activeVLAN.attrSwitchID));
	}

	throw new CDBException();
	
    }

    /** Cleans out the activeVLAN table */
    public void purgeActiveVLAN() throws CDBException
    {

      activeVLANHome avh = (activeVLANHome) cdbHome.getHomebyName("activeVLAN");

      // get all the VLAN entries
      ObjectList ol = avh.findByAttributes(null);
      for (int i= 0; i<ol.size(); i++) {
	activeVLAN v = (activeVLAN) ol.get(i);
        avh.remove(v.getID());
      }
    }


    /** Find all the PTPs that are connected to a particular switch 
     *  Return a Hashtable with the local switch and the remote switch PTP
     *  IDs as (key, value). If parentID is not null, then links connected to
     *  parent switch are not considered */
    public Hashtable getConnectedLinks (String switchID, String parentID) throws CDBException
    {

	// first find all the PTPIDs that belong to a switch
	PTPHome ptph = (PTPHome) cdbHome.getHomebyName("PTP"); 
	AttributeValues av = new AttributeValues ();
	av.addNameValue(PTP.attrSwitchID, switchID);

	ObjectList ol = ptph.findByAttributes(av);

	String[] ptps = new String [ol.size()];

	for (int i=0; i<ol.size(); i++) {

	    PTP p = (PTP) ol.get(i);
	    ptps[i] = p.getID();
	}

	LinkHome lh = (LinkHome) cdbHome.getHomebyName("Link");
	AttributeValues av1 = new AttributeValues ();
	AttributeValues av2 = new AttributeValues ();

	Hashtable allLinks = new Hashtable();

	for (int i=0; i < ptps.length; i++) {
	    av1.addNameValue(Link.attrAPtpID, ptps[i]);
	    av2.addNameValue(Link.attrZPtpID, ptps[i]);

	    ObjectList ol1 = lh.findByAttributes(av1);
	    for (int j=0; j < ol1.size(); j++) { 
		Link l = (Link) ol1.get(j);
		AttributeValues av3 = l.getAttributes();

	        String remotePtp = av3.getValueForName(Link.attrZPtpID);
		if (parentID != null) {
		    if (parentID.equals(getSwitchFromPtp(remotePtp)))
			continue;
		}

		allLinks.put(av3.getValueForName(Link.attrAPtpID), remotePtp); 

	    }
	    ObjectList ol2 = lh.findByAttributes(av2);
	    for (int j=0; j < ol2.size(); j++) { 
		Link l = (Link) ol2.get(j);
		AttributeValues av3 = l.getAttributes();

		String remotePtp = av3.getValueForName(Link.attrAPtpID);
		if (parentID != null) {
		    if (parentID.equals(getSwitchFromPtp(remotePtp)))
			continue;
		}

		allLinks.put(av3.getValueForName(Link.attrZPtpID), remotePtp); 

	    }

	}

//	System.out.println(allLinks.toString());
	return allLinks;

    }

    /** Find all the PTPs that are connected to a particular switch 
     *  Return a Hashtable with the local switch and the remote switch PTP
     *  IDs as (key, value). If parentID is not null, then links connected to
     *  parent switch are not considered 
     *  THIS function additionally also checks that there is a common
     *  VLAN between the two links - otherwise it does not return them */
    public Hashtable getConnectedVlanLinks (String switchID, String parentID, int vlanID) throws CDBException
    {

	// first find all the PTPIDs that belong to a switch
	PTPHome ptph = (PTPHome) cdbHome.getHomebyName("PTP"); 
	AttributeValues av = new AttributeValues ();
	av.addNameValue(PTP.attrSwitchID, switchID);

	ObjectList ol = ptph.findByAttributes(av);

	String[] ptps = new String [ol.size()];

	for (int i=0; i<ol.size(); i++) {

	    PTP p = (PTP) ol.get(i);
	    ptps[i] = p.getID();
	}

	LinkHome lh = (LinkHome) cdbHome.getHomebyName("Link");
	AttributeValues av1 = new AttributeValues ();
	AttributeValues av2 = new AttributeValues ();

	Hashtable allLinks = new Hashtable();

	for (int i=0; i < ptps.length; i++) {
	    av1.addNameValue(Link.attrAPtpID, ptps[i]);
	    av2.addNameValue(Link.attrZPtpID, ptps[i]);

	    ObjectList ol1 = lh.findByAttributes(av1);
	    for (int j=0; j < ol1.size(); j++) { 
		Link l = (Link) ol1.get(j);
		AttributeValues av3 = l.getAttributes();

	        String remotePtp = av3.getValueForName(Link.attrZPtpID);
		if (parentID != null) {
		    if (parentID.equals(getSwitchFromPtp(remotePtp)))
			continue;
		}

		if (commonVlanExists(av3.getValueForName(Link.attrAPtpID), remotePtp, vlanID)) 
		  allLinks.put(av3.getValueForName(Link.attrAPtpID), remotePtp); 

	    }
	    ObjectList ol2 = lh.findByAttributes(av2);
	    for (int j=0; j < ol2.size(); j++) { 
		Link l = (Link) ol2.get(j);
		AttributeValues av3 = l.getAttributes();

		String remotePtp = av3.getValueForName(Link.attrAPtpID);
		if (parentID != null) {
		    if (parentID.equals(getSwitchFromPtp(remotePtp)))
			continue;
		}

		if (commonVlanExists(av3.getValueForName(Link.attrZPtpID), remotePtp, vlanID)) 
		  allLinks.put(av3.getValueForName(Link.attrZPtpID), remotePtp); 

	    }

	}

	//System.out.println(allLinks.toString());
	return allLinks;

    }

    /** This function returns the switch ID from the PTP ID */
    public String getSwitchFromPtp (String PtpID) throws CDBException
    {
	PTPHome ph = (PTPHome) cdbHome.getHomebyName("PTP"); 
	AttributeValues av = new AttributeValues ();

	av.addNameValue (PTP.ID, PtpID);
	ObjectList ol = ph.findByAttributes (av);
	
	if (ol.size () > 0) {
	    PTP p = (PTP) ol.get(0);
	    AttributeValues av2 = p.getAttributes();
	    return (av2.getValueForName(PTP.attrSwitchID));
	}

	throw new CDBException ();
    }

    /** This function returns true if both ptps are enabled for the passed vlanID */
    public boolean commonVlanExists (String ptp1, String ptp2, int vlanID) throws CDBException
    {	

	activeVLANHome avh = (activeVLANHome) cdbHome.getHomebyName("activeVLAN"); 
	AttributeValues av1 = new AttributeValues ();
	av1.addNameValue (activeVLAN.attrPtpID, ptp1);
	av1.addNameValue (activeVLAN.attrVlanID, String.valueOf(vlanID));
	
	ObjectList ol1 = avh.findByAttributes (av1);

	if (ol1.size() == 0)
	  return false;

	AttributeValues av2 = new AttributeValues ();
	av2.addNameValue (activeVLAN.attrPtpID, ptp2);
	av2.addNameValue (activeVLAN.attrVlanID, String.valueOf(vlanID));
	
	ObjectList ol2 = avh.findByAttributes (av2);

	if (ol2.size() == 0)
	  return false;


	return true;

	/*
	for (int i=0; i<ol1.size(); i++) {

	    activeVLAN v1 = (activeVLAN) ol1.get(i);
            av1 = v1.getAttributes();
	    String vlan1 = av1.getValueForName(activeVLAN.attrVlanID);

	    for (int j=0; j<ol2.size(); j++) {
		activeVLAN v2 = (activeVLAN) ol2.get(j);
	  	av2 = v2.getAttributes();		
	 	String vlan2 = av2.getValueForName(activeVLAN.attrVlanID);	

		if (vlan1.equals(vlan2))
		    return true;
	    }

	}
	return false;
	*/
    }


    public HostsInfo getHostsInfo (String srcIPAddress, String destIPAddress) 
                                                  throws CDBException
    {
       // This function reads the subnet mask from the HostSubnet table
       // and finds if the source and destination belong to same subnet.
       // Also it identifies l2 edge interfaces to which the hosts are 
       // connected. The informations is returned in L2Info structure.

       HostsInfo rc = new HostsInfo ();

       AttributeValues avs = getHostSubnetValues (srcIPAddress);
       rc.inInterfaceID = avs.getValueForName (HostSubnet.attrL2InterfaceID);
       AttributeValues avd = getHostSubnetValues (destIPAddress);
       rc.outInterfaceID = avd.getValueForName (HostSubnet.attrL2InterfaceID);

       // if the srcIPAddress is specified as a host name, populate the
       // actual IP address
       if (srcIPAddress.equals (avs.getValueForName (HostSubnet.attrHostname)))
       {
          rc.isSrcHostName = true;
          rc.srcIP = avs.getValueForName (HostSubnet.attrHostSubnetAddr);
	  srcIPAddress = rc.srcIP;
       }
       // if the destIPAddress is specified as a host name, populate the
       // actual IP address
       if (destIPAddress.equals (avd.getValueForName (HostSubnet.attrHostname)))
       {
          rc.isDestHostName = true;
	  rc.destIP = avd.getValueForName (HostSubnet.attrHostSubnetAddr);
          destIPAddress = rc.destIP;
       }
/*
       System.out.println (srcIPAddress + " to " + destIPAddress +
			avs.getValueForName (HostSubnet.attrHostSubnetMask) +
			avd.getValueForName (HostSubnet.attrHostSubnetMask));
                       
*/

       rc.isPureL2Path = Inet4Subnet.areInSameSubnet (
                        srcIPAddress, 
			avs.getValueForName (HostSubnet.attrHostSubnetMask),
                        destIPAddress, 
			avd.getValueForName (HostSubnet.attrHostSubnetMask));
       if (rc.isPureL2Path)
          System.out.println ("Computing pure L2 path");

       return rc;
    }

    public PTP [] lookupL2Path (String inInfID, String outInfID, boolean oldPath)
                                throws CDBException
    {
      L2InterfaceHome l2ih = (L2InterfaceHome) cdbHome.getHomebyName ("L2Interface");
      PTPHome ptph = (PTPHome) cdbHome.getHomebyName ("PTP");

      L2Interface inInf = (L2Interface) l2ih.findByID (inInfID);
      L2Interface outInf = (L2Interface) l2ih.findByID (outInfID);
      
      AttributeValues av = inInf.getAttributes ();
      String ingress = av.getValueForName (L2Interface.attrSwitchID);
      String inPTP =  av.getValueForName (L2Interface.attrPTPID);
      PTP in = (PTP) ptph.findByID (inPTP);

      av = outInf.getAttributes ();
      String egress = av.getValueForName (L2Interface.attrSwitchID);
      String outPTP =  av.getValueForName (L2Interface.attrPTPID);
      PTP out = (PTP) ptph.findByID (outPTP);

      // call L2Path discovery code to find l2Interfaces in the path;
      // Note: this will call NRDB again to do some translations:
      // to avoid this call with Switch ID A, Switch ID Z and a VLAN number

      // resolve VLAN numbers
      String vlanA = this.getVlanFromPtpID(inPTP);
      String vlanB = this.getVlanFromPtpID(outPTP);

	if (!vlanA.equals(vlanB)) {
	  System.out.println("Mismatched vlans");
	  throw new CDBException();
	}

      String [] l2ptps = null;

      if (oldPath == true)
      {
         l2ptps = L2PathManager.getOldPath(ingress, egress, vlanA);
      }
      else
      {
         l2ptps = L2PathManager.getPath(ingress, egress, vlanA);
      }

      //String [] l2ptps = new String [0];

      // if L2Path is null then return only two interfaces
      PTP rc [] = new PTP [2 + l2ptps.length];

      int i=0;
      rc [i] = in;
      for (; i < l2ptps.length; i++) 
      {
	rc[i+1] = (PTP) ptph.findByID(l2ptps[i]);
	
      }
      System.out.println ("Layer 2 path is ");
      rc [i+1] = out;
      for (i = 0; i < rc.length; i++)
      {
         System.out.print (rc [i].getID () + ", ");
      }
      System.out.println ();
      return rc;
  
   }

   public PTP [] lookupPath (String inInterfaceID, 
                             String outInterfaceID,
                             boolean l2) 
                             throws CDBException

   {
      return lookupPath (inInterfaceID, outInterfaceID, l2, false);
   }
   public PTP [] lookupPath (String inInterfaceID, 
                             String outInterfaceID,
                             boolean l2,
			     boolean oldPath) 
                             throws CDBException
   {
      PTP [] rc;

      if (l2 == true)
      {
         rc = lookupL2Path (inInterfaceID, outInterfaceID, oldPath);
      }
      else
      {
         rc = lookupL3Path (inInterfaceID, outInterfaceID, oldPath);
      }
      return rc;
   }

   public class HostsInfo
   {
      boolean isPureL2Path = false;
      boolean isSrcHostName = false;
      boolean isDestHostName = false;
      String inInterfaceID;
      String outInterfaceID;
      String srcIP;
      String srcMask = "255.255.255.255";
      String destIP;
      String destMask = "255.255.255.255";
   }

    /** add Network Monitor Eventsubscriptions to the DB */
    public String addNetworkMonitorEventSubscription (String consumerComponentName, 
						    String cookie, 
						    String probeId, 
						    String monitorName)
	throws CDBException  
    {
	 NetworkMonitorEventSubscriptionsHome nmh = 
	   (NetworkMonitorEventSubscriptionsHome) cdbHome.getHomebyName("NetworkMonitorEventSubscriptions"); 
       AttributeValues av = new AttributeValues ();

       av.addNameValue (NetworkMonitorEventSubscriptions.attrEventConsumer, consumerComponentName);
       av.addNameValue (NetworkMonitorEventSubscriptions.attrCookie, cookie);
       av.addNameValue (NetworkMonitorEventSubscriptions.attrProbeID, probeId);
       av.addNameValue (NetworkMonitorEventSubscriptions.attrEventSupplierMonitor, monitorName);
       Top t = nmh.create(av);
       NetworkMonitorEventSubscriptions ns = nmh.narrow(t);
       return ns.getID();
       
    }

    public ObjectList findNetworkMonitorEventSubscriptions (String eventConsumer,
							    String cookie)
	throws CDBException
    { 
	NetworkMonitorEventSubscriptionsHome nmh = 
	    (NetworkMonitorEventSubscriptionsHome) cdbHome.getHomebyName("NetworkMonitorEventSubscriptions"); 
	AttributeValues av = new AttributeValues ();
	av.addNameValue (NetworkMonitorEventSubscriptions.attrEventConsumer, eventConsumer);
	av.addNameValue (NetworkMonitorEventSubscriptions.attrCookie, cookie);

	ObjectList ol = nmh.findByAttributes (av);
	return ol;
    }
    
    /** remove Network Monitor Eventsubscriptions from the DB - return the switches associated with this   
    eventConsumer, cookie pair */ 
    public void  removeNetworkMonitorEventSubscriptions(String eventConsumer, 
							String cookie) 
	throws CDBException  
    {

       NetworkMonitorEventSubscriptionsHome nmh = 
	   (NetworkMonitorEventSubscriptionsHome) cdbHome.getHomebyName("NetworkMonitorEventSubscriptions"); 
       AttributeValues av = new AttributeValues ();

       av.addNameValue (NetworkMonitorEventSubscriptions.attrEventConsumer, eventConsumer);
       av.addNameValue (NetworkMonitorEventSubscriptions.attrCookie, cookie);

       nmh.remove(av);
    }

    public void  removeNetworkMonitorEventSubscriptions(String eventMonitor) 
	throws CDBException  
    {

       NetworkMonitorEventSubscriptionsHome nmh = 
	   (NetworkMonitorEventSubscriptionsHome) cdbHome.getHomebyName("NetworkMonitorEventSubscriptions"); 
       AttributeValues av = new AttributeValues ();

       av.addNameValue (NetworkMonitorEventSubscriptions.attrEventSupplierMonitor, eventMonitor);
       
       nmh.remove(av);
    }
    /** remove all Network Monitor Eventsubscriptions from the DB */
    public void  removeAllNetworkMonitorEventSubscriptions()
	throws CDBException  
    {

       NetworkMonitorEventSubscriptionsHome nmh = 
	   (NetworkMonitorEventSubscriptionsHome) cdbHome.getHomebyName("NetworkMonitorEventSubscriptions"); 
        nmh.removeAll();
    }

    public class FlowConfigInfo
    {
       public FlowRecord fr;
       // ingress interface name
       public String infName;
       // managedIP of ingress
       public String managedIP;
       // flowID
       public String flowID;
       public String dscp;
       // other attributes
       public AttributeValues av;
       public String ingress;
       public String egress;
       public String vlan;
       public long rate;
       public int priority;
       public String svcClassID;
       public String status;

    }

    FlowConfigInfo getFlowConfigInfo (FlowRecord fr) throws CDBException
    {
       FlowConfigInfo rc = new FlowConfigInfo ();
       rc.fr = fr;
       rc.av = fr.getAttributes ();
 
       String inInfID = rc.av.getValueForName (FlowRecord.attrInInterfaceID);
       String outInfID = rc.av.getValueForName (FlowRecord.attrOutInterfaceID);
       rc.svcClassID = rc.av.getValueForName (FlowRecord.attrSvcClassID);
       rc.status = rc.av.getValueForName (FlowRecord.attrStatus);
       rc.flowID = fr.getID ();
       rc.rate = Integer.parseInt (rc.av.getValueForName (FlowRecord.attrBWAlloc));
       rc.priority = Integer.parseInt (rc.av.getValueForName (FlowRecord.attrPriority));
       String l2Endpoints = rc.av.getValueForName (FlowRecord.attrL2Endpoints);

       if (l2Endpoints.equals ("0"))
       {
          L3Interface l3i = findL3Interface (inInfID);
          AttributeValues l3iav = l3i.getAttributes ();
          rc.infName = l3iav.getValueForName (L3Interface.attrName);
          Router r = findRouter (l3iav.getValueForName (L3Interface.attrRouterID));
          rc.managedIP = r.getAttributes ().getValueForName (Router.attrManIP);
	  rc.ingress = r.getID ();

          l3i = findL3Interface (outInfID);
          l3iav = l3i.getAttributes ();
          r = findRouter (l3iav.getValueForName (L3Interface.attrRouterID));
	  rc.egress = r.getID ();
       }
       else
       {
	  //find the ingress properties
          L2Interface l2i = findL2Interface (inInfID);
          AttributeValues l2iav = l2i.getAttributes ();
          rc.infName = l2iav.getValueForName (L2Interface.attrName);
          Switch s = findSwitch (l2iav.getValueForName (L2Interface.attrSwitchID));
          rc.managedIP = s.getAttributes ().getValueForName (Switch.attrManIP);
	  rc.ingress = s.getID ();

	  // find the egress Switch
	  l2i = findL2Interface (outInfID);
          l2iav = l2i.getAttributes ();
          s = findSwitch (l2iav.getValueForName (L2Interface.attrSwitchID));
          rc.vlan = l2iav.getValueForName (L2Interface.attrVLanID);
	  rc.egress =s.getID ();
       }
 
       String svcClassID = rc.av.getValueForName (FlowRecord.attrSvcClassID);
       ServiceClass sc = findServiceClass (svcClassID);
       rc.dscp = sc.getAttributes ().getValueForName (ServiceClass.attrDSCP);
       return rc;
    }

   public String []  findLinksForIP (String [] ipAddresses) 
	throws CDBException  
   {
      L3InterfaceHome l3ih = 
              (L3InterfaceHome) cdbHome.getHomebyName ("L3Interface");
    
      String [] rc = l3ih.selectAttrWhereIn (L3Interface.attrLinkID,
                                             L3Interface.attrIPAddress,
					     ipAddresses);
      return rc;
   }

   public String []  findL3PathsOnLinks (String [] links) 
	throws CDBException  
   {
      L3HopsHome l3hh = 
              (L3HopsHome) cdbHome.getHomebyName ("L3Hops");
    
      String [] rc = l3hh.selectAttrWhereIn (L3Hops.attrL3PathID,
                                             L3Hops.attrLinkID,
					     links);
      return rc;
   }
   
   public String []  findFlowsOnL3Paths (String [] paths) 
	throws CDBException  
   {
      FlowRecordHome fh = 
              (FlowRecordHome) cdbHome.getHomebyName ("FlowRecord");
    
      String [] rc = fh.selectAttrWhereIn (FlowRecord.ID,
                                           FlowRecord.attrL3PathID,
					   paths);
      return rc;
   }
   
   public String []  findFlowsOnIP (String [] ipAddresses) 
	throws CDBException  
   {
      String links [] = findLinksForIP (ipAddresses);
      String paths [] = findL3PathsOnLinks (links);
      String flows [] = findFlowsOnL3Paths (paths);
      return flows;
   }
}

