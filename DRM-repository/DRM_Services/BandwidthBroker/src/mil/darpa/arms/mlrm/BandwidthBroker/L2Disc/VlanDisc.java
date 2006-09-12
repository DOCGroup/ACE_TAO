//$Id:
/* This file contains functions that discover the VLAN state of passed switches, it is assumed that the 
    switches support Cisco MIBs and that the community string is public */

package mil.darpa.arms.mlrm.BandwidthBroker.L2Disc;

import java.awt.event.*;
import com.adventnet.snmp.studio.api.*;
import java.io.*;
import java.util.*;
import com.adventnet.snmp.beans.*;

import mil.darpa.arms.mlrm.BandwidthBroker.bb.server.*;
import mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.*;

public class VlanDisc    
{
	AgentParams agentParams = null;
	SyncPortTable _PortTable = null;
	SyncIfTable _IfTable = null;
 	SyncDot1dStpPortTable _StpTable = null;
 	SyncDot1dBasePortTable _BasePortTable = null;
	SnmpVlanTable _VlanTable = null;

	NRDB db = null;
	
	/* Masks for getting VLAN State */ 
	int[] mask = {0x80, 0x40, 0x20,0x10, 0x08,0x04,0x02,0x01};
	static boolean DEBUG = true;

	/* STP State constants */
	static int FORWARDING = 5;

	/* List of switches and parameters required for them */
	String[] switches = null;
	String[] swID = null;
	String[] community = null;
	String[] osVersion = null;

	/** Constructor initializes various switch parameters */
	public VlanDisc()
	{
		agentParams = CreateAgentParams.getAgentParams();

		try {

		    db = new NRDB();
		    switches = db.getSwitchMIPs();
		    swID = new String[switches.length];
		    community = new String[switches.length];
		    osVersion = new String[switches.length];

		    // initialize the switch parameters, only done once to
 		    // save database cycles
		    for (int i=0; i<switches.length; i++)
		    {
			swID[i] = db.getSwitchIDFromMIP(switches[i]); 
			community[i] = db.getCommunityFromSwitchID(swID[i]);
			osVersion[i] = db.getOSFromSwitchID(swID[i]);
		    }
		    db.releaseConnection();

		} catch (CDBException cdbe) {
		  cdbe.printStackTrace();
		} 
	}

	/** Obtain and populate the VLAN information against all ports in the 
	    active VLAN table in the database */
	public void getVlanInfo()
	{
			
		try {
		    // first purge the entire activeVlan table, yup, not pretty
		    // but effective and easy
		    db = new NRDB();
		    db.purgeActiveVLAN();
		    db.commit();
		
		    // For each of the switches go through and populate the VLAN table
		    for (int i=0; i<switches.length; i++)
		    {
			// need to check for readCommunity attribute and osVersion
			agentParams.setTargetHost(switches[i]);
			agentParams.setCommunity(community[i]);
			
			
			System.out.println("Target host: " + switches[i] + ", Community: "+ community[i] 
					+ ", OS Version: " + osVersion[i]);

 			_StpTable = new SyncDot1dStpPortTable();
 			_StpTable.setAgentParams(agentParams);
 			_VlanTable = new SnmpVlanTable(switches[i], community[i]);
                    
						
			if (osVersion[i].equals("CatOS")) {
			    _PortTable = new SyncPortTable();
	 		    _PortTable.setAgentParams(agentParams);

			    getVlanInfoCatOS(i);

			   _PortTable.closeResources();
			}
			else { 
			    _IfTable = new SyncIfTable();
	 		    _IfTable.setAgentParams(agentParams);
			    _BasePortTable = new SyncDot1dBasePortTable();
	 		    _BasePortTable.setAgentParams(agentParams);

			    getVlanInfoIOS(i); 

			    _IfTable.closeResources();
			    _BasePortTable.closeResources();
			}
			    
			
			_StpTable.closeResources();
			_VlanTable.closeResources();
		 
		    }
		    db.releaseConnection();
		} catch (CDBException cdbe) {
		  cdbe.printStackTrace();
		}
		     
	}
	
	// This function functions in the presence of VLANs
	private void getVlanInfoCatOS(int index) throws CDBException	
	{
     	     Hashtable portInfo = new Hashtable();

		String[] stpIndex = _PortTable.getPortCrossIndex();
		if (stpIndex==null)
		{
			String error = _PortTable.getErrorString();
			System.out.println("Exception in getting Port Cross Index :"+error);
			return;
		}
		int[] modIndex = _PortTable.getPortModuleIndexValue();
		if (modIndex==null)
		{
			String error = _PortTable.getErrorString();
			System.out.println("Exception in getting Module Index :"+error);
			return;
		}
		int[] portIndex = _PortTable.getPortIndexValue();
		if (portIndex!=null)
		{
			for(int i=0; i<stpIndex.length && i<modIndex.length && i< portIndex.length; i++)
			{
			     // create a new Hashtable entry on the stpIndex
			     String portName = new String (modIndex[i] + "/" + portIndex[i]);
			     portInfo.put (stpIndex[i], portName);
			}
		}
		else
		{
			String error = _PortTable.getErrorString();
			System.out.println("Exception in getting Port Index :"+error);
			return;
		}	     

		// we need to find a list of VLAN indices to look at each row
		// in the Stp table
		String[] vlans = _VlanTable.getVlans();
		if (vlans==null)
		{
			System.out.println("Exception in getting VLAN Table :"+ _VlanTable.getErrorString());
			return;
		}
		else if (DEBUG)
		{
		    System.out.println("Number of VLANs is " + vlans.length);
		     for (int i=0; i<vlans.length;i++)
		     {
		          System.out.println(vlans[i]);
		     } 
		     
		}

		// Now head to the Bridge MIB - for each VLAN - set the community
                // get the port state  - and the port index
		for (int i=0; i<vlans.length; i++) {
		    agentParams.setCommunity(community[index] + "@" + vlans[i]);
		    _StpTable.setAgentParams(agentParams);

		    String[] ports = _StpTable.getDot1dStpPort();
		    if (ports==null) {
			String error = _StpTable.getErrorString();
			System.out.println("Exception in getting STP Table :"+error);
			return;
		    }

		    int[] portState = _StpTable.getDot1dStpPortStateValue();
		    if (portState==null) {
			String error = _StpTable.getErrorString();
			System.out.println("Exception in getting STP Table :"+error);
			return;
		    }		

		    // now add the ports to the activeVLAN table
		    for (int j=0; j<portState.length && j<ports.length; j++) {

			if (portState[j] != FORWARDING)
			    continue;

			String portName = (String) portInfo.get(ports[j]);

			if (portName == null)
			    continue;
			
			String ptpID = db.getPtpIDFromName(swID[index], portName);
			if (ptpID == null) 
			    continue;	

			if (DEBUG)	
			    System.out.println("Port: " + portName + ", VlanID: " + vlans[i]);

			db.addVLAN(swID[index], ptpID,	vlans[i]);

		    }
		}

		// commit the changes to the db 
		db.commit();

	}

	// This function functions in the presence of VLANs
	private void getVlanInfoIOS(int index) throws CDBException	
	{	
    	     Hashtable portInfo = new Hashtable();

		// For devices running IOS, the port names are available in the 
                // IF-MIB, get these names and index them on the ifIndex
		String[] ifIndex = _IfTable.getIfIndex();
		if (ifIndex==null)
		{
			String error = _IfTable.getErrorString();
			System.out.println("Exception in getting Interface Index :"+error);
			return;
		} 

		// Now get the IfDescr - this is the portName
		String[] ifDescr = _IfTable.getIfDescr();
		if (ifDescr==null)
		{
			String error = _IfTable.getErrorString();
			System.out.println("Exception in getting Interface Index :"+error);
			return;
		}

		for(int i=0; i<ifIndex.length && i<ifDescr.length; i++)
		{
		     // create a new Hashtable entry on the ifIndex
		     portInfo.put (ifIndex[i], ifDescr[i]);
		}

		// we need to find a list of VLAN indices to look at each row
		// in the Stp table
		String[] vlans = _VlanTable.getVlans();
		if (vlans==null)
		{
			System.out.println("Exception in getting VLAN Table :"+ _VlanTable.getErrorString());
			return;
		}
		else if (DEBUG)
		{
		    System.out.println("Number of VLANs is " + vlans.length);
		     for (int i=0; i<vlans.length;i++)
		     {
		          System.out.println(vlans[i]);
		     } 
		     
		}

		// Now head to the Bridge MIB - for each VLAN - set the community
                // get the port state  - and the port index - note that to match with the
		// ifIndex we actually go to the BasePortTable in the Bridge MIB
		for (int i=0; i<vlans.length; i++) {
		    agentParams.setCommunity(community[index] + "@" + vlans[i]);
		    _BasePortTable.setAgentParams(agentParams);
		    _StpTable.setAgentParams(agentParams);

		    String[] ports = _BasePortTable.getDot1dBasePortIfIndex();
		    if (ports==null) {
			String error = _BasePortTable.getErrorString();
			System.out.println("Exception in getting Base Port Table :"+error);
			return;
		    }

		    int[] portState = _StpTable.getDot1dStpPortStateValue();
		    if (portState==null) {
			String error = _StpTable.getErrorString();
			System.out.println("Exception in getting STP Table :"+error);
			return;
		    }		

		    // now add the ports to the activeVLAN table
		    for (int j=0; j<portState.length && j<ports.length; j++) {

			if (portState[j] != FORWARDING)
			    continue;

			String portName = (String) portInfo.get(ports[j]);

			if (portName == null)
			    continue;
			
			String ptpID = db.getPtpIDFromName(swID[index], portName);
			if (ptpID == null) 
			    continue;	

			if (DEBUG)	
			    System.out.println("Port: " + portName + ", VlanID: " + vlans[i]);

			db.addVLAN(swID[index], ptpID,	vlans[i]);

		    }
		}

		// commit the changes to the db 
		db.commit();

	}




	// This function takes a port cross index and a list of vlans and states and
	// returns the vlan membership of the port
	private ArrayList getPortVlans(int index, String[] allVlans, String[] vlanPortState)
	{
		// Start with an empty arraylist
		ArrayList vlans = null;
		int token = index/8;
		int remainder = index%8;
	   
		if (remainder == 0) 
		{
			// use the previous token 
			token--;
			remainder = 8;
		}
	        
	 
	     for (int i=0; i<allVlans.length && i<vlanPortState.length; i++)
	     {
	          if ((vlanPortState[i] != null) && (vlanPortState[i].length() > 0))
	          {
	               String state = getNthToken(vlanPortState[i], token);
	               if (state != null) 
	               {
		               int x = Integer.parseInt(state, 16);
		               int y = (x & mask[remainder -1]);
		               if (y >0)
		               {
		                    if (vlans == null)
		                    	vlans = new ArrayList();
		                    vlans.add(allVlans[i]);
		                    if (DEBUG) {
				     System.out.println("Index: " +index+ " active for vlan: "+ allVlans[i]);
		                    }
		               }
	               }
	               
	          }
	          
	     }
	     return vlans;
	     
	}
	
	
	private String getNthToken(String str, int token)
	{
	     StringTokenizer st = new StringTokenizer (str);
	     String temp = null;
	     
	     if (st.countTokens() <=  token)
	     	return null;
	     	
	    for (int i=0; i<=token; i++)
	    {
	         temp = st.nextToken();
	    }
	    
	    return temp;
	    
	     
	}


	public static void main(String[] args)
	{
		System.out.println("About to start");
		VlanDisc disc = new VlanDisc();
		System.out.println("Fetching Vlan Info (SNMP)");
		disc.getVlanInfo();

		System.out.println("Invoking Vlan Tree computation");
		TreeBuilder builder = new TreeBuilder();
		builder.buildAllTrees();
		builder.getPath("1", "2", "100");
		System.out.println("Exiting");
	}
 
   	
	

}






