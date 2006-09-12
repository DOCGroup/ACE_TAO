//$Id: CDBHome.java,
/*
 * CDBHome.java - Home class accessing CDB
 *
 * The DARPA ARMS PROGRAM
 * This software is governed by Government Purpose Rights
 * Contract No: NBCHC030132
 * Telcordia Technologies, Inc.
 * 1 Technology Drive
 * Piscataway, NJ 08854-4157
 *
 *
 */

package mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db;

import java.sql.*;

/**
 * This home class for CDB. It provides transactional interface for 
 * accessing the common data base
 *
 */

public class CDBHome 
{
   static boolean jdbcInit = false;
   Connection c = null;
   boolean inUse = false;

   /**
    * get a Database connection to CDBHome for a given user
    *
    */
   public CDBHome (String host, String user, String password) throws CDBException
   {
      try
      {
         c = connect (host, user, password);
         System.out.println ("Connected to " + user + "@" + host);
      }
      catch (SQLException se)

      {
         System.err.println ("Error connecting to CDB");
         throw new CDBException ();
      }
      
   }

   public static void setLogging (boolean logFlag)
   {
      DbUtil.log = logFlag;
   }

   public TopHome getHomebyName (String table) throws CDBException
   {
      
      if (table.equals ("Router"))
         return getRouterHome ();
      else if (table.equals ("Switch"))
         return getSwitchHome ();
      else if (table.equals ("L2Interface"))
         return getL2InterfaceHome ();
      else if (table.equals ("L3Interface"))
         return getL3InterfaceHome ();
      else if (table.equals ("PTP"))
	  return getPTPHome ();
      else if (table.equals ("ServiceClass"))
         return getServiceClassHome ();
      else if (table.equals ("BWPerClass"))
         return getBWPerClassHome ();
      else if (table.equals ("FlowRecord"))
         return getFlowRecordHome ();
      else if (table.equals ("HostSubnet"))
         return getHostSubnetHome ();
      else if (table.equals ("L3Path"))
         return getL3PathHome ();
      else if (table.equals ("L2Path"))
         return getL2PathHome ();
      else if (table.equals ("L3Hops"))
         return getL3HopsHome ();
      else if (table.equals ("L2Hops"))
         return getL2HopsHome ();
      else if (table.equals ("activeVLAN"))
         return getActiveVLANHome ();
      else if (table.equals ("Link"))
         return getLinkHome ();
      else if (table.equals ("FaultSubscriptions"))
         return getFaultSubscriptionsHome ();
      else if (table.equals ("NetworkMonitorEventSubscriptions"))
	  return getNetworkMonitorEventSubscriptionsHome ();
      else
      throw new CDBException ();
      
   }

    public RouterHome getRouterHome ()
    {
	return new RouterHome (c);
    }
    public SwitchHome getSwitchHome ()
    {
	return new SwitchHome (c);
    }
    public L2InterfaceHome getL2InterfaceHome ()
    {
	return new L2InterfaceHome (c);
    }
    public L3InterfaceHome getL3InterfaceHome ()
    {
	return new L3InterfaceHome (c);
    }
    public PTPHome getPTPHome ()
    {
	return new PTPHome (c);
    }
    public ServiceClassHome getServiceClassHome ()
    {
	return new ServiceClassHome(c);
    }
    public BWPerClassHome getBWPerClassHome ()
    {
	return new BWPerClassHome (c);
    }
    public FlowRecordHome getFlowRecordHome ()
    {
	return new FlowRecordHome (c);
    }
    public HostSubnetHome getHostSubnetHome ()
    {
	return new HostSubnetHome (c);
    }
    public L3PathHome getL3PathHome ()
    {
	return new L3PathHome (c);
    }
    public L2PathHome getL2PathHome ()
    {
	return new L2PathHome (c);
    }
    public L3HopsHome getL3HopsHome ()
    {
	return new L3HopsHome (c);
    }
    public L2HopsHome getL2HopsHome ()
    {
	return new L2HopsHome (c);
    }
    public activeVLANHome getActiveVLANHome ()
    {
	return new activeVLANHome (c);
    }
    public LinkHome getLinkHome ()
    {
	return new LinkHome (c);
    }  
    public FaultSubscriptionsHome getFaultSubscriptionsHome ()
    {
	return new FaultSubscriptionsHome (c);
    }  
     public NetworkMonitorEventSubscriptionsHome getNetworkMonitorEventSubscriptionsHome ()
    {
	return new NetworkMonitorEventSubscriptionsHome (c);
    }  

   /*
    * commit changes to database;
    *
    */
   public void commit () throws CDBException
   {
      try
      {
         c.commit ();
      }
      catch (SQLException se)
      {
         throw new CDBException ();
      }
   }

   /**
    * rollback changes
    *
    */
   public void rollback () throws CDBException
   {
      try
      {
         c.rollback ();
      }
      catch (SQLException se)
      {
         se.printStackTrace ();
         throw new CDBException ();
      }
   }

    public void resetDB () throws CDBException
    {
	AttributeValues av = new AttributeValues();
	av.addNameValue (Top.ID, new String ("> 0"));
	RouterHome routerHome = getRouterHome ();
	SwitchHome switchHome = getSwitchHome ();
	L3InterfaceHome l3InterfaceHome = getL3InterfaceHome ();
	L2InterfaceHome l2InterfaceHome = getL2InterfaceHome ();
	PTPHome ptpHome = getPTPHome ();
	BWPerClassHome bwPerClassHome = getBWPerClassHome ();
	ServiceClassHome serviceClassHome = getServiceClassHome ();
	FlowRecordHome flowRecordHome = getFlowRecordHome ();
	HostSubnetHome hostSubnetHome = getHostSubnetHome ();
	L3HopsHome l3HopsHome = getL3HopsHome ();
	L2HopsHome l2HopsHome = getL2HopsHome ();
	L3PathHome l3PathHome = getL3PathHome ();
	L2PathHome l2PathHome = getL2PathHome ();

	bwPerClassHome.remove (av);
	serviceClassHome.remove (av);
	flowRecordHome.remove (av);
	hostSubnetHome.remove (av);
	l3HopsHome.remove (av);
	l2HopsHome.remove (av);
	l3PathHome.remove (av);
	l2PathHome.remove (av);
	l2InterfaceHome.remove (av);
	l3InterfaceHome.remove (av);
	ptpHome.remove (av);
	switchHome.remove (av);
	routerHome.remove (av);
    }

   private Connection connect (String host, String user, String password) throws SQLException
   {
      Connection c;

      if (jdbcInit == false)
      {
      
	  /**
	   * Load the JDBC Driver
	   */
	  try{
	  //Class.forName ("org.gjt.mm.mysql.Driver").newInstance();
	  Class.forName ("com.mysql.jdbc.Driver").newInstance();
	  }
	  catch (Exception ex) {
	      ex.printStackTrace();
	      throw new SQLException ("Unable to load driver.");
	  }
	  jdbcInit = true;
      }

      /**
       * make a JDBC connection
       */
      try{
	  
	  /* c = DriverManager.getConnection ("jdbc:mysql://"+host+"/ARMSBB" + 
					   "?user=" + user +
					   "&password=" + password);*/
	   c = DriverManager.getConnection ("jdbc:mysql://"+host+"/ARMSBB", 
					    user,
					   password);
      }
      catch(Exception ex){
    	  ex.printStackTrace();
          throw new SQLException("Unable to connect");
      }
      c.setAutoCommit(false);      

      return c;
      
   }
    
    // For testing mainly : Kirthika
    public static void main (String[] args)
    {
	
	try{
	    
	    CDBHome cdbHome = new CDBHome ("quasar.research.telcordia.com", "arms", "arms");

	    System.out.println ("reset DB first");

	    //Insert records and retrieve them for testing

	    // Router test
	    RouterHome routerHome = cdbHome.getRouterHome ();
	    System.out.println ("Table " + routerHome.getTable ());
	    String routerID = cdbHome.testRouterAdd (routerHome);
	    
	    // Find test
	    Top t = routerHome.findByID (routerID);
	    System.out.println ("Found by ID: "+routerID+
				" the following router table entry:");
	    Router router = routerHome.narrow (t);
	    (router.getAttributes ()).print();
	   
	    // Switch test
	    SwitchHome switchHome = cdbHome.getSwitchHome ();
	    System.out.println ("Table " + switchHome.getTable ());
	    String switchID = cdbHome.testSwitchAdd (switchHome);
	    
	    // L3 Interface test
	    L3InterfaceHome l3InterfaceHome = cdbHome.getL3InterfaceHome ();
	    System.out.println ("Table " + l3InterfaceHome.getTable ());
	    String l3InterfaceID = cdbHome.testL3InterfaceAdd(l3InterfaceHome);

	    // L2 Interface test
	    L2InterfaceHome l2InterfaceHome = cdbHome.getL2InterfaceHome ();
	    System.out.println ("Table " + l2InterfaceHome.getTable ());
	    String l2InterfaceID = cdbHome.testL2InterfaceAdd (l2InterfaceHome);
	    
	    // PTP test
	    PTPHome ptpHome = cdbHome.getPTPHome ();
	    System.out.println ("Table " + ptpHome.getTable ());
	    String ptpID = cdbHome.testPTPAdd (ptpHome);
	    
	    // BWPerClass test
	    BWPerClassHome bwPerClassHome = cdbHome.getBWPerClassHome ();
	    System.out.println ("Table " + bwPerClassHome.getTable ());
	    String bwPerClassID = cdbHome.testBWPerClassAdd (bwPerClassHome);
	    
	    // Service Class Test
	    ServiceClassHome serviceClassHome = cdbHome.getServiceClassHome ();
	    System.out.println ("Table " + serviceClassHome.getTable ());
	    String serviceClassID = cdbHome.testServiceClassAdd (serviceClassHome);
	    
	    // Flow Record Test
	    FlowRecordHome flowRecordHome = cdbHome.getFlowRecordHome ();
	    System.out.println ("Table " + flowRecordHome.getTable ());
	    String flowRecordID = cdbHome.testFlowRecordAdd (flowRecordHome);
	    
	    // HostSubnet test
	     HostSubnetHome hostSubnetHome = cdbHome.getHostSubnetHome ();
	    System.out.println ("Table " + hostSubnetHome.getTable ());
	    String hostSubnetID = cdbHome.testHostSubnetAdd (hostSubnetHome);
	    
	    // L3Hops test
	    L3HopsHome l3HopsHome = cdbHome.getL3HopsHome ();
	    System.out.println ("Table " + l3HopsHome.getTable ());
	    String l3HopsID = cdbHome.testL3HopsAdd (l3HopsHome);
	    
	     // L2Hops test
	    L2HopsHome l2HopsHome = cdbHome.getL2HopsHome ();
	    System.out.println ("Table " + l2HopsHome.getTable ());
	    String l2HopsID = cdbHome.testL2HopsAdd (l2HopsHome);

	    // L3Path test
	    L3PathHome l3PathHome = cdbHome.getL3PathHome ();
	    System.out.println ("Table " + l3PathHome.getTable ());
	    String l3PathID = cdbHome.testL3PathAdd (l3PathHome);

	    // L2Path test
	    L2PathHome l2PathHome = cdbHome.getL2PathHome ();
	    System.out.println ("Table " + l2PathHome.getTable ());
	    String l2PathID = cdbHome.testL2PathAdd (l2PathHome);
	    
	    /** Removal of tuples from DB tests */

	    // test BWPerClass remove
	    bwPerClassHome.remove (bwPerClassID);
	    System.out.println ("BWPerClass name: firstBWPerClass ID:"+
				bwPerClassID+ " has been removed!");
	    
	    // test ServiceClass remove
	    serviceClassHome.remove (serviceClassID);
	    System.out.println ("ServiceClass name: firstServiceClass ID:"+
				serviceClassID+ " has been removed!");

	     // test FlowRecord remove
	    flowRecordHome.remove (flowRecordID);
	    System.out.println ("FlowRecord name: firstFlowRecord ID:"+
				flowRecordID+ " has been removed!");

	     // test HostSubnet remove
	    hostSubnetHome.remove (hostSubnetID);
	    System.out.println ("HostSubnet name: firstHostSubnet ID:"+
				hostSubnetID+ " has been removed!");
	    
	    // test L3Hops remove
	    l3HopsHome.remove (l3HopsID);
	    System.out.println ("L3Hops name: firstL3Hops ID:"+
				l3HopsID+ " has been removed!");
	    // test L2Hops remove
	    l2HopsHome.remove (l2HopsID);
	    System.out.println ("L2Hops name: firstL2Hops ID:"+
				l2HopsID+ " has been removed!");
	    // test L3Path remove
	    l3PathHome.remove (l3PathID);
	    System.out.println ("L3Path name: firstL3Path ID:"+
				l3PathID+ " has been removed!");
	     // test L2Path remove
	    l2PathHome.remove (l2PathID);
	    System.out.println ("L2Path name: firstL2Path ID:"+
				l2PathID+ " has been removed!");
	    
	    // test L2Interface Delete 
	    l2InterfaceHome.remove (l2InterfaceID);
	    System.out.println ("L2Interface name: firstL2Interface ID:"+
				l2InterfaceID+ " has been removed!");
	    // test L3Interface Delete 
	    l3InterfaceHome.remove (l3InterfaceID);
	    System.out.println ("L3Interface name: firstL3Interface ID:"+
				l3InterfaceID+ " has been removed!");
	    // test PTP remove
	    ptpHome.remove (ptpID);
	    System.out.println ("ptp name: firstPTP ID:"+
				ptpID+ " has been removed!");
	    // test Switch Delete 
	    switchHome.remove (switchID);
	    System.out.println ("Switch name: firstSwitch ID:"+
				switchID+ " has been removed!");
	    // test Router Delete 
	    routerHome.remove (routerID);
	    System.out.println ("Router name: firstRouter ID:"+
	    routerID+ " has been removed!");
	    cdbHome.commit();
	}
	catch (Exception e)
	    {
		System.out.println ("Exception!");
		e.printStackTrace ();
	    }
    }
      
    public String testRouterAdd (RouterHome routerHome) 
	throws Exception
    {
	try{
	    AttributeValues av = new AttributeValues ();
	    av.addNameValue (Router.attrName, new String("firstRouter"));
	    av.addNameValue (Router.attrAdminState, new String ("up"));
	    av.addNameValue (Router.attrOpnState, new String ("up"));
	    av.addNameValue (Router.attrEquipCode, new String ("3200"));
	    av.addNameValue (Router.attrVendorName, new String("cisco"));
	    av.addNameValue (Router.attrOSVersion, new String ("12.2"));
	    av.addNameValue (Router.attrPW, new String ("cisco"));
	    av.addNameValue (Router.attrEnablePW, new String ("cisco"));
	    av.addNameValue (Router.attrManIP, new String ("128.3.4.5"));
	    av.addNameValue (Router.attrIsPE, new String ("1"));
	    av.addNameValue (Router.attrLastACL,new String ("1"));
	    Top t= routerHome.create (av);

	    Router router = routerHome.narrow (t);
	    av = router.getAttributes ();
	    av.print ();
	    return router.getID();
	 
	    
	}
	catch (Exception e)
	    {
		throw e;
	    }
    }
  

    public String testSwitchAdd (SwitchHome switchHome) 
	throws Exception
    {
	try{
	    AttributeValues av = new AttributeValues ();
	    av.addNameValue (Switch.attrName, new String("firstSwitch"));
	    av.addNameValue (Switch.attrAdminState, new String ("up"));
	    av.addNameValue (Switch.attrOpnState, new String ("up"));
	    av.addNameValue (Switch.attrEquipCode, new String ("6500"));
	    av.addNameValue (Switch.attrVendorName, new String("cisco"));
	    av.addNameValue (Switch.attrOSVersion, new String ("5.2"));
	    av.addNameValue (Switch.attrPW, new String ("cisco"));
	    av.addNameValue (Switch.attrEnablePW, new String ("cisco"));
	    av.addNameValue (Switch.attrManIP, new String ("128.3.4.6"));
	    Top t= switchHome.create (av);

	    Switch switchElement = switchHome.narrow (t);
	    av = switchElement.getAttributes ();
	    av.print ();

	    return switchElement.getID();

	
	}
	catch (Exception e)
	    {
		throw e;
	    }
    }
    
   public String testL3InterfaceAdd (L3InterfaceHome l3InterfaceHome) 
	throws Exception
    {
	try{
	    AttributeValues av = new AttributeValues ();
	    av.addNameValue (L3Interface.attrName, new String("firstL3Interface"));
	    av.addNameValue (L3Interface.attrAdminState, new String ("up"));
	    av.addNameValue (L3Interface.attrOpnState, new String ("up"));
	    av.addNameValue (L3Interface.attrVLanID, new String ("0"));
	    av.addNameValue (L3Interface.attrIPAddress, new String("128.4.5.6"));
	    av.addNameValue (L3Interface.attrIPMask, new String ("255.255.255.0"));
	    av.addNameValue (L3Interface.attrRouterID, new String ("1"));
	    Top t= l3InterfaceHome.create (av);

	    L3Interface l3Interface = l3InterfaceHome.narrow (t);
	    av = l3Interface.getAttributes ();
	    av.print ();
	    return l3Interface.getID();
	   
	}
	catch (Exception e)
	    {
		throw e;
	    }
    }

     public String testL2InterfaceAdd (L2InterfaceHome l2InterfaceHome) 
	throws Exception
    {
	try{
	    AttributeValues av = new AttributeValues ();
	    av.addNameValue (L2Interface.attrName, new String("firstL2Interface"));
	    av.addNameValue (L2Interface.attrAdminState, new String ("up"));
	    av.addNameValue (L2Interface.attrOpnState, new String ("up"));
	    av.addNameValue (L2Interface.attrVLanID, new String ("0"));
	    av.addNameValue (L2Interface.attrSwitchID, new String ("1"));
	    Top t= l2InterfaceHome.create (av);

	    L2Interface l2Interface = l2InterfaceHome.narrow (t);
	    av = l2Interface.getAttributes ();
	    av.print ();
	    return l2Interface.getID();
	   
	}
	catch (Exception e)
	    {
		throw e;
	    }
    }
    
     public String testPTPAdd (PTPHome ptpHome) 
	throws Exception
    {
	try{
	    AttributeValues av = new AttributeValues ();
	    av.addNameValue (PTP.attrName, new String("firstPTP"));
	    av.addNameValue (PTP.attrAdminState, new String ("up"));
	    av.addNameValue (PTP.attrOpnState, new String ("up"));
	    av.addNameValue (PTP.attrRate, new String ("1000"));
	    av.addNameValue (PTP.attrPhyAddress, new String ("FastEthernet0/0"));
	    av.addNameValue (PTP.attrRouterID, new String ("1"));
	    Top t= ptpHome.create (av);

	    PTP ptp = ptpHome.narrow (t);
	    av = ptp.getAttributes ();
	    av.print ();
	    return ptp.getID();
	   
	}
	catch (Exception e)
	    {
		throw e;
	    }
    }
    
   public String testBWPerClassAdd (BWPerClassHome bwPerClassHome) 
	throws Exception
    {
	try{
	    AttributeValues av = new AttributeValues ();
	    av.addNameValue (BWPerClass.attrBWAlloc, new String ("200"));
	    av.addNameValue (BWPerClass.attrBWTotal, new String ("1000"));
	    av.addNameValue (BWPerClass.attrSvcClassID, new String ("1"));
	    av.addNameValue (BWPerClass.attrPtpID, new String ("1"));
	    Top t= bwPerClassHome.create (av);

	    BWPerClass bwPerClass = bwPerClassHome.narrow (t);
	    av = bwPerClass.getAttributes ();
	    av.print ();
	    return bwPerClass.getID();
	   
	}
	catch (Exception e)
	    {
		throw e;
	    }
    }
    
    public String testServiceClassAdd (ServiceClassHome serviceClassHome) 
	throws Exception
    {
	try{
	    AttributeValues av = new AttributeValues ();
	    av.addNameValue (ServiceClass.attrDSCP, new String ("16"));
	    av.addNameValue (ServiceClass.attrDropPrecedence,new String ("1"));
	    Top t= serviceClassHome.create (av);

	    ServiceClass serviceClass = serviceClassHome.narrow (t);
	    av = serviceClass.getAttributes ();
	    av.print ();
	    return serviceClass.getID();
	   
	}
	catch (Exception e)
	    {
		throw e;
	    }
    }

    public String testFlowRecordAdd (FlowRecordHome flowRecordHome) 
	throws Exception
    {
	try{
	    AttributeValues av = new AttributeValues ();
	    av.addNameValue (FlowRecord.attrSrcIP, new String ("192.2.3.5"));
	    av.addNameValue (FlowRecord.attrSrcMask, new String ("255.255.255.0"));
	    av.addNameValue (FlowRecord.attrSrcPort, new String ("5000"));
	    av.addNameValue (FlowRecord.attrSrcPortHigh, new String ("7000"));
	    av.addNameValue (FlowRecord.attrDestIP, new String ("128.2.3.5"));
	    av.addNameValue (FlowRecord.attrDestMask, new String ("255.255.255.0"));
	    av.addNameValue (FlowRecord.attrDestPort, new String ("2000"));
	    av.addNameValue (FlowRecord.attrDestPortHigh, new String ("5000"));
	    av.addNameValue (FlowRecord.attrBWMin, new String ("100"));
	    av.addNameValue (FlowRecord.attrExtraBudget, new String ("10000"));
	    av.addNameValue (FlowRecord.attrBWAlloc, new String ("500"));
	    av.addNameValue (FlowRecord.attrACL, new String ("20"));
	    av.addNameValue (FlowRecord.attrSvcClassID, new String ("1"));
	    Top t= flowRecordHome.create (av);

	    FlowRecord  flowRecord = flowRecordHome.narrow (t);
	    av = flowRecord.getAttributes ();
	    av.print ();
	    return flowRecord.getID();
	   
	}
	catch (Exception e)
	    {
		throw e;
	    }
    }

    public String testHostSubnetAdd (HostSubnetHome hostSubnetHome) 
	throws Exception
    {
	try{
	    AttributeValues av = new AttributeValues ();
	    av.addNameValue (HostSubnet.attrHostSubnetAddr, new String ("128.3.4.0"));
	    av.addNameValue (HostSubnet.attrHostSubnetMask, new String ("255.255.255.0"));
	    av.addNameValue (HostSubnet.attrL3InterfaceID, new String ("1"));
	    Top t= hostSubnetHome.create (av);

	    HostSubnet  hostSubnet = hostSubnetHome.narrow (t);
	    av = hostSubnet.getAttributes ();
	    av.print ();
	    return hostSubnet.getID();
	   
	}
	catch (Exception e)
	    {
		throw e;
	    }
    }

   public String testL3HopsAdd (L3HopsHome l3HopsHome) 
	throws Exception
    {
	try{
	    AttributeValues av = new AttributeValues ();
	    av.addNameValue (L3Hops.attrL3InterfaceID, new String ("1"));
	    av.addNameValue (L3Hops.attrL3PathID, new String ("1"));
	    av.addNameValue (L3Hops.attrHopNumber, new String ("1"));
	    Top t= l3HopsHome.create (av);

	    L3Hops  l3Hops = l3HopsHome.narrow (t);
	    av = l3Hops.getAttributes ();
	    av.print ();
	    return l3Hops.getID();
	   
	}
	catch (Exception e)
	    {
		throw e;
	    }
    }

    public String testL2HopsAdd (L2HopsHome l2HopsHome) 
	throws Exception
    {
	try{
	    AttributeValues av = new AttributeValues ();
	    av.addNameValue (L2Hops.attrL2InterfaceID, new String ("1"));
	    av.addNameValue (L2Hops.attrL2PathID, new String ("1"));
	    av.addNameValue (L2Hops.attrHopNumber, new String ("1"));
	    Top t= l2HopsHome.create (av);

	    L2Hops  l2Hops = l2HopsHome.narrow (t);
	    av = l2Hops.getAttributes ();
	    av.print ();
	    return l2Hops.getID();
	   
	}
	catch (Exception e)
	    {
		throw e;
	    }
    }

    public String testL3PathAdd (L3PathHome l3PathHome) 
	throws Exception
    {
	try{
	    AttributeValues av = new AttributeValues ();
	    av.addNameValue (L3Path.attrIngressID, new String("1"));
	    av.addNameValue (L3Path.attrEgressID, new String ("5"));
	    Top t= l3PathHome.create (av);

	    L3Path  l3Path = l3PathHome.narrow (t);
	    av = l3Path.getAttributes ();
	    av.print ();
	    return l3Path.getID();
	   
	}
	catch (Exception e)
	    {
		throw e;
	    }
    }
   
      public String testL2PathAdd (L2PathHome l2PathHome) 
	throws Exception
    {
	try{
	    AttributeValues av = new AttributeValues ();
	    av.addNameValue (L2Path.attrIngressHop, new String("128.4.5.6"));
	    av.addNameValue (L2Path.attrEgressHop, new String ("192.3.4.5"));
	    Top t= l2PathHome.create (av);

	    L2Path  l2Path = l2PathHome.narrow (t);
	    av = l2Path.getAttributes ();
	    av.print ();
	    return l2Path.getID();
	   
	}
	catch (Exception e)
	    {
		throw e;
	    }
    }
   
  
}
