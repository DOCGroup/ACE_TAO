//$Id:
/* This file contains functions that discover the state of switches and ports, it is assumed that the 
    switches support standard IF MIBs and that the community string is public */

package mil.darpa.arms.mlrm.BandwidthBroker.FaultMonitorServer;

import java.awt.event.*;
import com.adventnet.snmp.studio.api.*;
import java.io.*;
import java.util.*;
import java.net.*;
import com.adventnet.snmp.beans.*;

import mil.darpa.arms.mlrm.BandwidthBroker.bb.server.*;
import mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.*;
import com.telcordia.jIOS.*;
import mil.darpa.arms.mlrm.BandwidthBroker.FaultMonitorPackage.*;

public class SwitchDisc implements TrapListener   
{
	AgentParams agentParams = null;
	SyncIfTable _IfTable = null;
	SyncIfXTable _IfXTable = null;

	NRDB db = null;
	//FaultMonitorImpl fm = null;
	FaultMonitorCallback fm;

	TrapManager tm = null;
	static int PORT = 40000;
	

	// SNMP OIDs for decoding
	static String ifBaseOID  = ".1.3.6.1.2.1.2.2.1"; 
	static String ifIndexOID = ".1.3.6.1.2.1.2.2.1.1"; 
	static String ifAdmOID   = ".1.3.6.1.2.1.2.2.1.7"; 
	static String ifOperOID  = ".1.3.6.1.2.1.2.2.1.8"; 

	// Trap types
	static int LINK_DOWN = 2;
	static int LINK_UP = 3;

	static String[] linkStatus = {"", "UP", "DOWN"};
	static String[] trapStatus = {"", "", "DOWN", "UP"};
	
	
	/* List of switches and parameters required for them */
	String[] switches = null;
	String[] swID = null;
	String[] community = null;
	boolean[] switchIndexed = null;

	/* debugging */
	static boolean DEBUG = false;


	/* IP address of the local host */
	String ipAddress = null;

	/** Constructor initializes various switch parameters */
	public SwitchDisc(NRDB _db, FaultMonitorCallback _fm)
	{
		agentParams = CreateAgentParams.getAgentParams();

		try {

		    db = _db;
            fm = _fm;
		    switches = db.getSwitchMIPs();
		    swID = new String[switches.length];
		    community = new String[switches.length];
		    switchIndexed = new boolean[switches.length];

		    // initialize the switch parameters, only done once to
 		    // save database cycles
		    for (int i=0; i<switches.length; i++)
		    {
			swID[i] = db.getSwitchIDFromMIP(switches[i]); 
			community[i] = db.getCommunityFromSwitchID(swID[i]);
			switchIndexed[i] = false;
		    }

		} catch (CDBException cdbe) {
		  cdbe.printStackTrace();
		} 

	
	  try {
	    ipAddress = InetAddress.getLocalHost().getHostAddress();
	  } catch (UnknownHostException uhe) {
	
	    uhe.printStackTrace();

	  }
	}

	/** Get the switch index */
	private int _getSwitchIndex(String switchID) {

	  int index=-1;

 	  for (int i=0; i< swID.length; i++)  {
	    if (swID[i].equals(switchID)) {
	      index = i;
	      break;
	    }
	  }

	  return index;
	}
	

	/** Map the port names to ifIndices */
        private void _indexSwitch(String switchID) throws CDBException
	{

	  // first get all the port names 
	  String[] ports = db.getSwitchPtps(switchID);

          // get all the IfNames 
	  String os = db.getOSFromSwitchID(switchID);

	  String[] ifNames;
          String[] ifIndices;

          // For IOS, it is better to get the names for the IfTable description instead
	  if (os.equals("IOS")) {
             ifNames = _IfTable.getIfDescr();
             ifIndices = _IfTable.getIfIndex();
	  } else {
             ifNames = _IfXTable.getIfName();
             ifIndices = _IfXTable.getIfIndex();
          }

	  // debug print
	  for (int j=0; j<ports.length; j++) {
          for (int i=0; i<ifNames.length && i<ifIndices.length ; i++) {

	    if (ports[j].equals(ifNames[i])) {
	      db.updateIfIndex(switchID, ports[j], ifIndices[i]);
	      System.out.println("Index: " + ifIndices[i] + "\t Port: " + ports[j]);
	      break;
	    }
	  }
	  }

	  db.commit();

	}

	/** Obtain and populate the switch information against all ports in the DB */
	public void getSwitchInfo(String switchID)
	{

	  try {

	    int index = _getSwitchIndex(switchID);

	    // set the TargetHost and Communiity attributes
	    agentParams.setTargetHost(switches[index]);
	    agentParams.setCommunity(community[index]);

	    _IfXTable = new SyncIfXTable();
	    _IfTable = new SyncIfTable();
	    _IfTable.setAgentParams(agentParams); 
	    _IfXTable.setAgentParams(agentParams); 

            // get all the ifIndex values, if the switch has been initialized
            // with IF Indices, if not, first do the If Index initilization
            if (switchIndexed[index] == false) {
	      _indexSwitch(switchID);
	      switchIndexed[index] = true;
	    }

            String[] ifIndices = db.getSwitchIfIndices(switchID);

	    String[] snmpIndices = _IfTable.getIfIndex();
	    //String[] snmpOpStatus = _IfTable.getIfOperStatus();
	    //String[] snmpAdStatus = _IfTable.getIfAdminStatus();
	    int[] snmpOpStatus = _IfTable.getIfOperStatusValue();
	    int[] snmpAdStatus = _IfTable.getIfAdminStatusValue();

	    
	    for (int i=0; i<ifIndices.length; i++) {
	      for (int j=0; j<snmpIndices.length; j++) {
		if (ifIndices[i].equals(snmpIndices[j])) {
		  db.updateIfIndexStatus(switchID, ifIndices[i], 
			linkStatus[snmpOpStatus[j]], linkStatus[snmpAdStatus[j]]);
		  System.out.println("IfIndex " + snmpIndices[j] + " OP " + linkStatus[snmpOpStatus[j]] + ", Ad " + linkStatus[snmpAdStatus[j]]);
		  break;
		}
	      }
            } 

	    db.commit();

	    // clean up resources
	    _IfTable.closeResources();
	    _IfXTable.closeResources();
	  } catch (CDBException cdbe) {
	    cdbe.printStackTrace();
	  }
		     
	}

	/** Subscribe to traps on the switch that you are interested in */
	public void subscribeToTrap(String switchID) {


	  int index = _getSwitchIndex(switchID);

	  System.out.println(" Index is: " + index + " mip is: " + switches[index]);

	  if (tm == null) {
	    tm = new TrapManager(PORT);
	    tm.addTrapListener(this);
	  }

	  try {
	    // make sure the switch is indexed
            if (switchIndexed[index] == false) {

	      // set the TargetHost and Communiity attributes
	      agentParams.setTargetHost(switches[index]);
	      agentParams.setCommunity(community[index]);
	
	      _IfXTable = new SyncIfXTable();
	      _IfTable = new SyncIfTable();
	      _IfTable.setAgentParams(agentParams); 
	      _IfXTable.setAgentParams(agentParams); 

	      _indexSwitch(switchID);
	    
	      // clean up resources
	      _IfTable.closeResources();
	      _IfXTable.closeResources();

	      switchIndexed[index] = true;	 

	    } 

            // get the switch pwd:enPwd:userName for issuing the CLI command
	    String pwds = db.getSwitchPwds(switchID);

            System.out.println("Passwords are " + pwds);

            // get the local host and the switch MIP and add the appropriate snmp command
	    // Use the CiscoUtils class to implement this functionality

	    // get the local IP address and the pwd, enPwd of the switch
	    String cmd = "snmp-server host " + ipAddress + " public udp-port " + PORT;
            String cmd2 = "snmp-server enable traps snmp linkdown linkup ";
	    System.out.println("Command is " + cmd);
      	    CiscoUtils cu = new CiscoUtils ("IOS");

            // check the login format 
            String[] access = pwds.split(":");

            if (access.length == 2)
      	      cu.cisLogin(switches[index], access[0], access[1]);
      	    else cu.cisLogin(switches[index], access[2], access[0], access[1]);
            cu.cisConf(cmd + "\r" + cmd2); 

	  } catch (CDBException cdbe) {
	    cdbe.printStackTrace();
	  }

	 
          System.out.println("Leaving SD: subscribeToTrap");


	}

	/** Unsubscribe from traps on the switch */
	public void unsubscribeFromTrap(String switchID) {
	  int index = _getSwitchIndex(switchID);

	  // get the local IP address and the pwd, enPwd of the switch
	  String cmd = "no snmp-server host " + ipAddress + " public udp-port " + PORT;
          String cmd2 = "no snmp-server enable traps snmp linkdown linkup ";
	  System.out.println("Command is " + cmd);
      	  CiscoUtils cu = new CiscoUtils ("IOS");
      	  cu.cisLogin(switches[index], "ngnlab", "mtv");
          cu.cisConf(cmd + "\r" + cmd2);
	}

	/** Interface TrapListener implementation */
	public void receivedTrap(TrapEvent event) {

          /* Which box was the trap on? */
	  String mip = event.getRemoteHost();
	  String obj = event.getObjectID(0);

	  /* What kind of a trap was it? */
	  int trap = event.getTrapType();

 	  if (DEBUG)
	    System.out.println("Rcvd trap: " + trap + "  from: " + mip + " on object: " + obj);

	  if (trap != LINK_DOWN && trap != LINK_UP) {
	    System.out.println("Rcvd trap: " + trap + "  from: " + mip + " on object: " + obj);
	    System.out.println("Unknown trap type");
	    return;
	  }


	  // get the IF index
	  String[] arr = obj.split("\\.");
	  if (arr == null) {
 	    if (DEBUG)
	      System.out.println("Couldn't extract ifIndex");
	    return;
	  } 

	  String ifIndex = arr[arr.length-1];

	  // We know the trap type and the IF Index, we are done ;-)
	  try {

	    int index = switches.length;

	    // find the index 
	    for (int i=0; i<switches.length; i++) {
		if (switches[i].equals(mip)) {
		  index = i;
		  break;
		}
	    }

	    if (index == switches.length) {
		System.out.println("Trap " + trap + " received from unknown MIP " + mip + " on obj: " + obj);
		return;
	    }
	
	    String ifName = db.getPortName(swID[index], ifIndex);
	    System.out.println("Rcvd trap: " + trap + "  from: " + mip + " on object: " + obj + " which represents port name: " + ifName);

	    // update the DB status  - note we currently do not really
	    // have the adminStatus, we need to get it from the MIB - for 
	    // now we don't care too much about the adminStatus - just set it to the 
	    // operational status

            // make sure we do not report duplicate traps and flaps etc.
            String status = db.getIfIndexStatus(swID[index], ifIndex);

            System.out.println("Got the following switch status: " + status);


            if (!trapStatus[trap].equalsIgnoreCase(status)) {
	      db.updateIfIndexStatus(swID[index], ifIndex, trapStatus[trap], 
				trapStatus[trap]);
	
	      db.commit();
	      // Notify the interested subscribers on this trap
              fm.notifySubscribersOfFault(db.getSwitchNameFromID(swID[index]), ifName, trapStatus[trap]);
            }
            else System.out.println("Duplicate trap, status is " + status + " and trap is " + trapStatus[trap]);

           


	  } 
	  catch (CDBException cdbe) 
	  {
	    cdbe.printStackTrace();
	  } 
	  catch (FaultMonitorException fe)
	  {
	     fe.printStackTrace ();
	  }

	  System.out.println("Continues waiting to receive traps .......");

	} 
	
/** 
	public static void main(String[] args)
	{
		System.out.println("About to start");
		try {
		  NRDB _db = new NRDB();
		  SwitchDisc disc = new SwitchDisc(_db);
		  System.out.println("Fetching Switch Info (SNMP)");
		  disc.getSwitchInfo("1");
		  disc.getSwitchInfo("5");

		  System.out.println("Registering for Traps");
		  disc.subscribeToTrap("5");

		  System.out.println("De-Registering from Traps");
		  disc.unsubscribeFromTrap("5");
                } catch (CDBException cdbe) {
                  cdbe.printStackTrace();
                }
		
	}
*/
 
   	
	

}






