//$Id: TopologyPopulator.java
package mil.darpa.arms.mlrm.BandwidthBroker.topologyDB;

import java.io.File;
import java.io.FileInputStream;
import java.io.StringWriter;
import java.util.*;
import java.math.*;
import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBException;
import javax.xml.bind.Marshaller;
import javax.xml.bind.Unmarshaller;

import org.xml.sax.InputSource;
import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;
import mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.*;
import mil.darpa.arms.mlrm.BandwidthBroker.topologyDB.generated.*;

public class TopologyPopulator
{
    // Map of Input ID from XML to the ID of element in DB
    HashMap IDTable = new HashMap ();

    CDBHome cdbHome = null;
    RouterHome routerHome = null;
    SwitchHome switchHome = null;
    L2InterfaceHome l2InterfaceHome = null;
    L3InterfaceHome l3InterfaceHome = null;
    PTPHome ptpHome = null;
    HostSubnetHome hostSubnetHome = null;
    ServiceClassHome serviceClassHome = null;
    BWPerClassHome bwPerClassHome = null;
    LinkHome linkHome = null;
    Logger logger = null;

    String dbHost = new String ("quasar.research.telcordia.com");
    String dbUser = new String ("arms");
    String dbPwd = new String ("arms");


    public TopologyPopulator (String logPropFile, String topologyDBConfigFilename) 
	throws CDBException
    {
	PropertyConfigurator.configure(logPropFile); // use topolog.prop
	logger = Logger.getLogger(TopologyPopulator.class.getName());
	
	// Hardcoded for now
	setTopologyDBProperties (topologyDBConfigFilename);
	
	cdbHome = new CDBHome (dbHost,dbUser, dbPwd);

	routerHome = cdbHome.getRouterHome ();
	switchHome = cdbHome.getSwitchHome ();
	l3InterfaceHome = cdbHome.getL3InterfaceHome ();
	l2InterfaceHome = cdbHome.getL2InterfaceHome ();	
	ptpHome = cdbHome.getPTPHome ();
	hostSubnetHome = cdbHome.getHostSubnetHome ();
	bwPerClassHome = cdbHome.getBWPerClassHome ();
	linkHome = cdbHome.getLinkHome ();
	serviceClassHome = cdbHome.getServiceClassHome ();
    }

    public void setTopologyDBProperties (String configFile)
    {
	PropertyResourceBundle properties=null;
	 try
	    {
		logger.info ("TopologyDB Configuration Properties from :"+configFile);
		properties = new PropertyResourceBundle(new FileInputStream(configFile));
		
		dbHost = properties.getString("DBServer");
		dbUser = properties.getString("DBLogin");
		dbPwd = properties.getString("DBPassword");
		logger.info (" MySQL Topology DB server:"+dbHost+" user:"+dbUser+" passwd:"+dbPwd);
		
	    }
	 catch( Exception e )
	     {
		    System.out.println( "Unable to read  file: "+configFile+ "hence using system defaults");
		    logger.warn ( "Unable to read  file: "+configFile+ "hence using system defaults");
	     }
    }
    
   public String getToplogyAsString(NetworkTopologyElement pToplogy)
      throws JAXBException 
   {
      StringWriter sw = new StringWriter ();
      JAXBContext context =
	    JAXBContext.newInstance ("mil.darpa.arms.mlrm.BandwidthBroker.topologyDB.generated");
      Marshaller marshaller = context.createMarshaller ();
      marshaller.marshal (pToplogy, sw);
      return sw.toString ();
   }

    public void createTopologyDB(NetworkTopologyElement topo1)
      throws JAXBException 
   {
       int i = 0;
       try {
	   
	   SwitchList sList = topo1.getSwitchList();
	   List swList = sList.getSwitch ();
	   for (i=0; i< swList.size(); i++)
	       {
		   mil.darpa.arms.mlrm.BandwidthBroker.topologyDB.generated.Switch s 
		   = (mil.darpa.arms.mlrm.BandwidthBroker.topologyDB.generated.Switch)swList.get(i);
		   if (s.isIsL3Capable ())
		       {
			   logger.info ("Populating router:"+s.getInternalID());
			   try{
			       String routerID= populateRouter (s);
			       IDTable.put ((String)s.getInternalID (), (String)routerID);
			   }catch (Exception ecx)
			       {
				   logger.warn ("Error populating Router:"+s.getInternalID()+" in topology dB");
				   System.out.println ("Error populating Router:"+s.getInternalID()+" in topology dB");
				   throw new JAXBException(ecx.toString ());
			       }
		       }
		   else
		       {
			   logger.info ("Populating switch:"+s.getInternalID());
			   try{
			       String switchID= populateSwitch (s);
			       IDTable.put ((String)s.getInternalID (), (String)switchID);
			   }catch (Exception ecx)
			       {
				   logger.warn ("Error populating Switch:"+s.getInternalID()+" in topology dB");
				   System.out.println ("Error populating Switch:"+s.getInternalID()+" in topology dB");
				   throw new JAXBException(ecx.toString ());
			       }
		       }
	       }
	   

	   /** PTPs need to be populated too for L3,L2 interfaces */
	   L3InterfaceList l3IntfList = topo1.getL3InterfaceList();
	   List l3IntList = l3IntfList.getL3Interface ();
	   for (i=0; i< l3IntList.size(); i++)
	       {
		   mil.darpa.arms.mlrm.BandwidthBroker.topologyDB.generated.L3Interface l 
		       = (mil.darpa.arms.mlrm.BandwidthBroker.topologyDB.generated.L3Interface)l3IntList.get(i);
		   logger.info ("Populating L3 interface:"+l.getInternalID());
		   try{
		       String l3IntID = populateL3Interface (l);
		       IDTable.put ((String)l.getInternalID (), (String)l3IntID);
		   }catch (Exception ecx)
		       {
			   logger.warn ("Error populating L3Interface:"
					+l.getInternalID()+" for router:"+
					l.getRouterInternalID ()+"in topology dB, make sure the router ID exists");
			   System.out.println ("Error populating L3Interface:"
					       +l.getInternalID()+" for router:"+
					       l.getRouterInternalID ()+"in topology dB, make sure the router ID exists");
			   throw new JAXBException(ecx.toString ());
		       }
		   
	       }
       L2InterfaceList l2IntfList = topo1.getL2InterfaceList();
       List l2IntList = l2IntfList.getL2Interface();
       // Had to add PtpID to L2Interface in arms-initdb.sql and db/L2Interface
       for (i=0; i< l2IntList.size(); i++)
	   {
	       mil.darpa.arms.mlrm.BandwidthBroker.topologyDB.generated.L2Interface l 
		   = (mil.darpa.arms.mlrm.BandwidthBroker.topologyDB.generated.L2Interface)l2IntList.get(i);
	       logger.info ("Populating L2 interface:"+l.getInternalID());
	       try{
		   String l2IntID = populateL2Interface (l);
	       IDTable.put ((String)l.getInternalID (), (String)l2IntID);
	       }catch (Exception ecx)
			   {
                               ecx.printStackTrace ();
			       logger.warn ("Error populating L2Interface:"
					    +l.getInternalID()+" for switch:"+
					    l.getSwitchInternalID ()+"in topology dB, make sure the router ID exists");
			       System.out.println ("Error populating L2Interface:"
					    +l.getInternalID()+" for switch:"+
					    l.getSwitchInternalID ()+"in topology dB, make sure the router ID exists");
			       throw new JAXBException(ecx.toString ());
			   }
	       }
       // Build the Link table in DB
       AttributeValues av = new AttributeValues ();
       for (i=0; i< l2IntList.size(); i++)
	   {
	       mil.darpa.arms.mlrm.BandwidthBroker.topologyDB.generated.L2Interface l 
		   = (mil.darpa.arms.mlrm.BandwidthBroker.topologyDB.generated.L2Interface)l2IntList.get(i);
	       if (l.isIsRemoteL2 ())
		   {
		       // Build a Link with current l2 interface ptpID and the remote l2 interface ptpID
		       logger.info ("Populating Link between l2 interfaces:"+l.getInternalID()+
				    " and "+l.getRemoteInterfaceInternalID ());
		       AttributeValues av1 = new AttributeValues ();
		       av1.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.L2Interface.ID, 
					 (String)IDTable.get(l.getInternalID ()));
		       ObjectList o = l2InterfaceHome.findByAttributes (av1);
		       mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.L2Interface l2Int =null;
		       if (o.size () != 0)
			   l2Int  = ( mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.L2Interface) o.get(0);
		       else
			   {
			       logger.warn ("No L2Interface:"+l.getInternalID ()+
					    " found in DB, hence cannot find its PTP for building Link:");
			       throw new CDBException ();
			   }

		       av1 = l2Int.getAttributes ();
		       av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.Link.attrAPtpID, 
				       av1.getValueForName (
					 mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.L2Interface.attrPTPID));
		      
		       // The other ptpID
		       av1 = new AttributeValues ();
		       av1.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.L2Interface.ID, 
					 (String)IDTable.get(l.getRemoteInterfaceInternalID ()));
		       o = l2InterfaceHome.findByAttributes (av1);
		       l2Int =null;
		       if (o.size () != 0)
			   l2Int  = ( mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.L2Interface) o.get(0);
		       else
			   {
			       logger.warn ("No L2Interface:"+l.getRemoteInterfaceInternalID ()+
					    " found in DB, hence cannot find its PTP for building Link:");
			       throw new CDBException ();
			   }
		       av1 = l2Int.getAttributes ();
		       av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.Link.attrZPtpID, 
				       av1.getValueForName (
					 mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.L2Interface.attrPTPID));
		       Top t= linkHome.create (av);
	
		       Link lk = linkHome.narrow (t);
		       av = lk.getAttributes ();
		       //logger.info (av.print ());
		       //IDTable.put ((String)lk.getID (), (String)lk.getID);
		   }
	   }

       HostSubnetList selist = topo1.getHostSubnetList ();
       List slist = selist.getHostSubnet ();
         for (i=0; i< slist.size(); i++)
	   {
	       mil.darpa.arms.mlrm.BandwidthBroker.topologyDB.generated.HostSubnet l 
		   = (mil.darpa.arms.mlrm.BandwidthBroker.topologyDB.generated.HostSubnet)slist.get(i);
	       logger.info ("Populating HostSubnet:"+l.getSubnetAddress ());
	       try{
		   String hsID =populateHostSubnet (l);
		   IDTable.put ((String)l.getInternalID (), (String)hsID);
	       }catch (Exception ecx)
		   {
		        logger.warn ("Error populating HostSubnet:"+l.getSubnetAddress ()+"!");
				 
			System.out.println ("Error populating HostSubnet:"+l.getSubnetAddress ()+"!");
			throw new JAXBException (ecx.toString ());	 
		   }	       
	   }

	 /** Populate the Service class and BWPerClass fro every interface*/

       l3IntfList = topo1.getL3InterfaceList();
       l3IntList = l3IntfList.getL3Interface ();
       l2IntfList = topo1.getL2InterfaceList();
       l2IntList = l2IntfList.getL2Interface ();
       QoSClassList qclist = topo1.getQoSClassList ();
       List qlist = qclist.getQoSClass ();
         for (i=0; i< qlist.size(); i++)
	   {
	       mil.darpa.arms.mlrm.BandwidthBroker.topologyDB.generated.QoSClass c 
		   = (mil.darpa.arms.mlrm.BandwidthBroker.topologyDB.generated.QoSClass)qlist.get(i);
	       logger.info ("Populating QoSClass:"+c.getQoSClassName ());
	       String cid = new String ("");
	       try{
		    cid = populateQoSClass (c);
		   IDTable.put ((String)c.getInternalID (), (String)cid);
	       }catch (Exception ecx)
		   {
		        logger.warn ("Error populating QoSClass:"+c.getQoSClassName()+"!");
			System.out.println ("Error populating QoSClass:"+c.getQoSClassName ()+"!");
			throw new JAXBException (ecx.toString ());	 
		   }
	       int z=0;

	       for (z=0;z < l3IntList.size (); z++)
		 {
		     mil.darpa.arms.mlrm.BandwidthBroker.topologyDB.generated.L3Interface l3Int =
			 (mil.darpa.arms.mlrm.BandwidthBroker.topologyDB.generated.L3Interface)l3IntList.get(z);
		     try{
			 populateBWPerClass (c,cid,l3Int);
						 
		     }catch (Exception ecx1)
			 {
			     logger.warn ("Error populating BwPerClass:"+c.getQoSClassName()+
					  " for interface "+l3Int.getIPAddress ()+"!");
			     System.out.println ("Error populating BwPerClass:"+c.getQoSClassName()+
					  " for interface "+l3Int.getIPAddress ()+"!");
			     throw new JAXBException (ecx1.toString ());
			 }
		 }
	        for (z=0;z < l2IntList.size (); z++)
		 {
		      mil.darpa.arms.mlrm.BandwidthBroker.topologyDB.generated.L2Interface l2Int = 
			  (mil.darpa.arms.mlrm.BandwidthBroker.topologyDB.generated.L2Interface)l2IntList.get(z);
		     try{
			 populateBWPerClass (c,cid,l2Int);
					 
		     }catch (Exception ecx2)
			 {
			     logger.warn ("Error populating BwPerClass:"+c.getQoSClassName()+
					  " for interface "+l2Int.getInternalID ()+"!");
			     System.out.println ("Error populating BwPerClass:"+c.getQoSClassName()+
					  " for interface "+l2Int.getInternalID ()+"!");
			     throw new JAXBException (ecx2.toString ());
			 }
		 }
	       
	   } 
	 
	 //}

       }catch (Exception e)
	   {
	       logger.warn ("Exception in populating DB:"+e.toString()+
			    "! ");
	       //e.printStackTrace();
	       throw new JAXBException (e.toString ());
	   }

       
   }

    public void populateBWPerClass (mil.darpa.arms.mlrm.BandwidthBroker.topologyDB.generated.QoSClass c,
				String cid, mil.darpa.arms.mlrm.BandwidthBroker.topologyDB.generated.L3Interface z)
	throws CDBException
    {
	// For L3Interfaces, inbound flag set for BWPerClass (inbound for L3, 
	// as traceroute finds the inbound interface in path)
	// If edge, outbound reqd too
	AttributeValues av = new AttributeValues ();
	
	// e.g. 20/100-- percent * 100000 --> rate
	String bwTotal = 
	    (c.getTotalCapacityAllowed ().multiply (z.getBandwidthInbitspersec()).divide (
		  new BigInteger ("100")).toString()); 
	
	logger.info ("total bandwidth allowed:"+bwTotal+" and the util % is "+ c.getTotalCapacityAllowed ().toString ()
		     + " and link bandwidth ="+ z.getBandwidthInbitspersec().toString ());
	av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.BWPerClass.attrBWTotal, 
			 bwTotal);
	av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.BWPerClass.attrSvcClassID, 
			 cid);
	// Get the rigth PTP based on L3 interface name
	AttributeValues av1 = new AttributeValues ();
	av1.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.L3Interface.attrIPAddress,
			  z.getIPAddress ());
	ObjectList o = l3InterfaceHome.findByAttributes (av1);
	mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.L3Interface l3Int =null;
        if (o.size () != 0)
	     l3Int  = (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.L3Interface) o.get(0);
	else
	    {
		logger.warn ("No L3Interface:"+z.getInterfaceID ()+
			     " found in DB, hence cannot find its PTP for building BWPerClass:");
		throw new CDBException ();
	    }
	av1 = l3Int.getAttributes ();
	av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.BWPerClass.attrPtpID, 
			 av1.getValueForName (
					 mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.L3Interface.attrPTPID));
	av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.BWPerClass.attrInbound, 
			 new String ("1"));

	Top t= bwPerClassHome.create (av);
	
       BWPerClass svc = bwPerClassHome.narrow (t);
	av = svc.getAttributes ();
	//logger.info (av.print ());

	// If L3Interface is edge, populate outbound BWPerClass for it
	if (z.isIsItEdge ())
	    {
		logger.info ("Populating outbound BWPerClass for L3Interface:"+z.getInternalID());
	      av = new AttributeValues ();
	
	      // e.g. 20/100-- percent * 100000 --> rate
	      bwTotal 
		  = (c.getTotalCapacityAllowed ().multiply (z.getBandwidthInbitspersec()).divide (
			 new BigInteger ("100")).toString()); 
	      logger.info ("total bandwidth allowed:"+bwTotal+" and the util % is "+
			     c.getTotalCapacityAllowed ().toString ()
			     + " and link bandwidth ="+ z.getBandwidthInbitspersec().toString ());
	      av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.BWPerClass.attrBWTotal, 
			       bwTotal);
	      av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.BWPerClass.attrSvcClassID, 
			       cid);
	      av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.BWPerClass.attrPtpID, 
			       av1.getValueForName (
			      mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.L3Interface.attrPTPID));
	      av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.BWPerClass.attrInbound, 
			       new String ("0"));
	      t= bwPerClassHome.create (av);
	      
	      BWPerClass svc2 = bwPerClassHome.narrow (t);
	      av = svc2.getAttributes ();
	      //logger.info (av.print ());	
	    }
    }

    public void populateBWPerClass (mil.darpa.arms.mlrm.BandwidthBroker.topologyDB.generated.QoSClass c,
				String cid, mil.darpa.arms.mlrm.BandwidthBroker.topologyDB.generated.L2Interface z)
	throws CDBException
    {
	// For L2Interfaces if the remoteL2 flag is not set, it can be assumed to be edge and the outboundbound 
	// BWPerClass is instantiated too.  For all L2Interfaces, the inbound BWPErClass need to be instantiated.
	// When Aint-->BInt, the inbound of BInt is the outbound of the AInt 
	AttributeValues av = new AttributeValues ();
	logger.info ("Populating inbound BWPerClass for L2Interface:"+z.getInternalID());
	// e.g. 20/100-- percent * 100000 --> rate
	String bwTotal 
	    = (c.getTotalCapacityAllowed ().multiply (z.getBandwidthInbitspersec()).divide (
		new BigInteger ("100")).toString()); 
		logger.info ("total bandwidth allowed:"+bwTotal+" and the util % is "+
			     c.getTotalCapacityAllowed ().toString ()
			     + " and link bandwidth ="+ z.getBandwidthInbitspersec().toString ());

	av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.BWPerClass.attrBWTotal, 
			 bwTotal);
	av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.BWPerClass.attrSvcClassID, 
			 cid);
			  
	// Get the rigth PTP based on L2 interface name

	AttributeValues av1 = new AttributeValues ();
	av1.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.L2Interface.attrName, z.getPortID ());
	av1.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.L2Interface.attrSwitchID, 
			  (String) IDTable.get (z.getSwitchInternalID ()));
	ObjectList o = l2InterfaceHome.findByAttributes (av1);
	mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.L2Interface l2Int =null;
        if (o.size () != 0)
	     l2Int  = (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.L2Interface) o.get(0);
	else
	    {
		logger.warn ("No L2Interface:"+z.getPortID ()+
			     " found in DB, hence cannot find its PTP for building BWPerClass:");
		throw new CDBException ();
	    }
	av1 = l2Int.getAttributes ();
	av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.BWPerClass.attrPtpID, 
			 av1.getValueForName (
					      mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.L2Interface.attrPTPID));
	av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.BWPerClass.attrInbound, 
			 new String ("1"));

	Top t= bwPerClassHome.create (av);
	
       BWPerClass svc = bwPerClassHome.narrow (t);
       av = svc.getAttributes ();
	//logger.info (av.print ());


	// Now if the remoteL2 flag is off, that is it is edge, we need outbound BWPerClass too
	if (!z.isIsRemoteL2 ())
	    {
		logger.info ("Populating outbound BWPerClass for L2Interface:"+z.getInternalID());
	      av = new AttributeValues ();
	
	      // e.g. 20/100-- percent * 100000 --> rate
	      bwTotal 
		  = (c.getTotalCapacityAllowed ().multiply (z.getBandwidthInbitspersec()).divide (
			 new BigInteger ("100")).toString()); 
	      logger.info ("total bandwidth allowed:"+bwTotal+" and the util % is "+
			     c.getTotalCapacityAllowed ().toString ()
			     + " and link bandwidth ="+ z.getBandwidthInbitspersec().toString ());
	      av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.BWPerClass.attrBWTotal, 
			       bwTotal);
	      av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.BWPerClass.attrSvcClassID, 
			       cid);
	      av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.BWPerClass.attrPtpID, 
			       av1.getValueForName (
			      mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.L2Interface.attrPTPID));
	      av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.BWPerClass.attrInbound, 
			       new String ("0"));
	      t= bwPerClassHome.create (av);
	      
	      BWPerClass svc2 = bwPerClassHome.narrow (t);
	      av = svc2.getAttributes ();
	      //logger.info (av.print ());
	    }
	
    }

     public String populateQoSClass (mil.darpa.arms.mlrm.BandwidthBroker.topologyDB.generated.QoSClass c)
	throws CDBException
    {
	AttributeValues av = new AttributeValues ();
	av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.ServiceClass.attrName, 
			 c.getQoSClassName());
	av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.ServiceClass.attrDSCP, 
			 c.getDSCP().toString ());
	av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.ServiceClass.attrDropPrecedence, 
			 c.getDropPrecedence().toString ());
	av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.ServiceClass.attrUtilLimit, 
			 c.getTotalCapacityAllowed ().toString ());

	Top t= serviceClassHome.create (av);
	
	ServiceClass svc = serviceClassHome.narrow (t);
	av = svc.getAttributes ();
	//logger.info (av.print ());

	return svc.getID ();
    }
     
     public String populateL3Interface(mil.darpa.arms.mlrm.BandwidthBroker.topologyDB.generated.L3Interface l)
	throws CDBException
    {
	AttributeValues av = new AttributeValues ();
	av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.L3Interface.attrName, 
			 l.getInterfaceID());
	av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.L3Interface.attrVLanID, 
			 new String ("0"));
	av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.L3Interface.attrIPAddress, 
			 l.getIPAddress());
	av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.L3Interface.attrIPMask, 
			 l.getIPSubnetMask ());
	
	// Lookup router based on router name provided
	AttributeValues av1 = new AttributeValues ();
	av1.addNameValue (Router.attrName, l.getRouterInternalID ());
	ObjectList o = routerHome.findByAttributes (av1);
	Router router =null;
        if (o.size () != 0)
	     router  = (Router) o.get(0);
	else
	    {
		logger.warn ("No router:"+l.getRouterInternalID ()+
			     " found in DB, hence cannot add L3Interfce:"+l.getInternalID());
		throw new CDBException ();
	    }
	av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.L3Interface.attrRouterID,
			 router.getID ());
	
	// Add the PTP for this L3Interface
	AttributeValues av2 = new AttributeValues ();
	av2.addNameValue (PTP.attrName,l.getInterfaceID ());
	av2.addNameValue (PTP.attrRate, 
                          (l.getBandwidthInbitspersec ().
                           divide (new BigInteger ("1000")).toString()));
	av2.addNameValue (PTP.attrPhyAddress, new String ("phy"));
	av2.addNameValue (PTP.attrRouterID, router.getID ());
	Top t = ptpHome.create (av2);
	PTP ptp1 = ptpHome.narrow (t);
	av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.L3Interface.attrPTPID, 
			 ptp1.getID ());
	
	t= l3InterfaceHome.create (av);
	mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.L3Interface l3Interface 
	    = l3InterfaceHome.narrow (t);
	av = l3Interface.getAttributes ();
	//logger.info (av.print ());
	return l3Interface.getID ();
    }


     public String populateL2Interface(mil.darpa.arms.mlrm.BandwidthBroker.topologyDB.generated.L2Interface l)
	throws CDBException
    {
	AttributeValues av = new AttributeValues ();
	av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.L2Interface.attrName, 
			 l.getPortID());
	// VlanIDs are not populated into db as it is a list, 
	// later on, the db VlanID will be a list too
	av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.L2Interface.attrVLanID, 
			 l.getVlanIDs ());
		
	// Lookup switch based on swicth name provided
	
	av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.L2Interface.attrSwitchID, 
			 (String)IDTable.get ((String)l.getSwitchInternalID ()));
	
	// Add the PTP for this L2Interface
	AttributeValues av2 = new AttributeValues ();
	av2.addNameValue (PTP.attrName,l.getPortID ());
	av2.addNameValue (PTP.attrRate, 
                          (l.getBandwidthInbitspersec ().
                           divide (new BigInteger ("1000")).toString()));
	av2.addNameValue (PTP.attrPhyAddress, l.getL2Address ());
	av2.addNameValue (PTP.attrSwitchID, (String)IDTable.get ((String)l.getSwitchInternalID ()));
	Top t = ptpHome.create (av2);
	PTP ptp1 = ptpHome.narrow (t);
	av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.L2Interface.attrPTPID, 
			 ptp1.getID ());
	
	t= l2InterfaceHome.create (av);
	mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.L2Interface l2Interface 
	    = l2InterfaceHome.narrow (t);
	av = l2Interface.getAttributes ();
	//logger.info (av.print ());
	return l2Interface.getID ();
    }

     public String populateHostSubnet (
			 mil.darpa.arms.mlrm.BandwidthBroker.topologyDB.generated.HostSubnet l)
	throws CDBException
    {
	
	    AttributeValues av = new AttributeValues ();
	    av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.HostSubnet.attrHostSubnetAddr, 
			     l.getSubnetAddress () );
	    av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.HostSubnet.attrHostSubnetMask, 
			     l.getIPSubnetMask ());
	    if (l.getHostName ()!= null)
		av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.HostSubnet.attrHostname, 
				 l.getHostName ());
	    // Get the edge l3 interface
	    if (l.getEdgeRouterInterfaceInternalID () != null)
		av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.HostSubnet.attrL3InterfaceID, 
			     (String)IDTable.get (l.getEdgeRouterInterfaceInternalID()));
	    av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.HostSubnet.attrPoolName, 
			     l.getPoolName ());
	    if (l.getEdgeRouterinterfaceIPaddress () != null)
		av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.HostSubnet.attrL3Gateway, 
			     l.getEdgeRouterinterfaceIPaddress ());
	    if (l.getEdgeL2SwitchInterfaceInternalID () != null)
		av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.HostSubnet.attrL2InterfaceID, 
				 (String)IDTable.get(l.getEdgeL2SwitchInterfaceInternalID()));

	    Top t= hostSubnetHome.create (av);
	    mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.HostSubnet  hostSubnet = hostSubnetHome.narrow (t);
	    av = hostSubnet.getAttributes ();
	    //logger.info (av.print ());
	    return hostSubnet.getID ();
    }

    public String populateSwitch (mil.darpa.arms.mlrm.BandwidthBroker.topologyDB.generated.Switch s)
	throws CDBException
    {
	AttributeValues av = new AttributeValues ();
	av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.Switch.attrName,  
			 s.getInternalID());
	av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.Switch.attrEquipCode,
			 s.getSwitchID ());
	av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.Switch.attrVendorName, 
			 s.getSwitchType ());
	List c = s.getOperatingSystem ().getContent();
	if (c != null)
	{
	    NameElement ne = (NameElement)c.get(0);
	    StringBuffer ss = new StringBuffer (ne.getValue());
	    VersionElement ve = (VersionElement)c.get(1);
	    ss.append (ve.getValue());
	    av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.Switch.attrOSVersion, 
			 ss.toString ());
	}
	av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.Switch.attrPW,
			 s.getPassword());
	av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.Switch.attrEnablePW, 
			 s.getEnPassword ());
	av.addNameValue (mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.Switch.attrManIP, 
			 s.getManagedElementID ());
	Top t= switchHome.create (av);
	
	mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.Switch switchElement = switchHome.narrow (t);
	av = switchElement.getAttributes ();
	//logger.info (av.print ());
	return switchElement.getID ();
    }

    public String populateRouter (mil.darpa.arms.mlrm.BandwidthBroker.topologyDB.generated.Switch s)
	throws CDBException
    {
	AttributeValues av = new AttributeValues ();
	av.addNameValue (Router.attrName, s.getInternalID());
	av.addNameValue (Router.attrEquipCode, s.getSwitchID ());
	av.addNameValue (Router.attrVendorName, s.getSwitchType ());

	List c = s.getOperatingSystem ().getContent();
	if (c !=null)
	    {
		NameElement ne = (NameElement)c.get(0);
		StringBuffer ss = new StringBuffer (ne.getValue());
		VersionElement ve = (VersionElement)c.get(1);
		ss.append (ve.getValue());
		av.addNameValue (Router.attrOSVersion, ss.toString()); 
	    }
	av.addNameValue (Router.attrPW, s.getPassword());
	av.addNameValue (Router.attrEnablePW, s.getEnPassword ());
	av.addNameValue (Router.attrManIP, s.getManagedElementID ());
	String isItEdge = null;
	if (s.isIsItEdge ())
	    isItEdge = new String ("1");
	else
	    isItEdge = new String ("0");
	
	av.addNameValue (Router.attrIsPE, isItEdge);
	
	Top t= routerHome.create (av);
	
	Router router = routerHome.narrow (t);
	av = router.getAttributes ();
	//logger.info (av.print ());
	return router.getID ();
    }
  
    public void resetDB ()
	throws CDBException
    {
	logger.info ("Dropping all table entries in DB");
	//	cdbHome.resetDB ();//tablesSQL);
	AttributeValues av = null;
	linkHome.remove (av);
	logger.info ("Removed all Link table entries in DB");
	
	bwPerClassHome.remove (av);
	logger.info ("Removed all BwPerClass table entries in DB");

	serviceClassHome.remove (av);
	logger.info ("Removed all ServiceClass table entries in DB");
	
	hostSubnetHome.remove (av);
	logger.info ("Removed all HostSubnet table entries in DB");
	
	ptpHome.remove (av);
	logger.info ("Removed all PTP table entries in DB");
	
	l3InterfaceHome.remove (av);
	logger.info ("Removed all L3Interface table entries in DB");
	
	l2InterfaceHome.remove (av);
	logger.info ("Removed all L2interface table entries in DB");
	
	switchHome.remove (av);
	logger.info ("Removed all Swicth table entries in DB");
	
	routerHome.remove (av);
	logger.info ("Removed all Router table entries in DB");
	

    }
    
    public void commit ()
	throws CDBException
    {
	cdbHome.commit ();
    }

   public static void main (String[] args) 
   {
       try {

	   if (args.length < 3)
	       {
		   System.out.println ("Usage: java mil.darpa.arms.mlrm.BandwidthBroker.topologyDB.TopologyPopulator "+
				       "<input_xml_file_name> <log-prop-file> <DB-config-file>");
		   System.exit (1);
	       }

	File f = new File (args[0]);
	
	TopologyPopulator tp = new TopologyPopulator (new String (args[1]), new String (args[2]));

	InputSource isource = new InputSource (new FileInputStream (f));
	isource.setSystemId (f.toURL ().toString ());
	JAXBContext context =
	    JAXBContext.newInstance ("mil.darpa.arms.mlrm.BandwidthBroker.topologyDB.generated");
	Unmarshaller unmarshaller = context.createUnmarshaller ();
	NetworkTopologyElement topo1 =
	    (NetworkTopologyElement) unmarshaller.unmarshal (isource);
	
	// Empty the DB
	 tp.resetDB ();

	tp.createTopologyDB (topo1);
	System.out.println ("Topology Populated with XML input");
	
	//System.out.println ("Regenerated XML:\n" + tp.getToplogyAsString (topo1));

	tp.commit ();
	
	
	
       } catch (Exception e)
	   {
	       System.out.println ("Failure in Topology DB operation!"+
				   "Check log output: networkprovisioner.topologyDB.log for more information.");
	       //e.printStackTrace ();
	       
	   }
   }
}
