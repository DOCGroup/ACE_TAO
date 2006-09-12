// $Id$
//
// The DARPA ARMS Program
//
// The software supporting this interface is governed by Government Purpose 
// Rights
//
// Contract No: NBCHC030132
// Telcordia Technologies, Inc.
// 1 Telcordia Drive
// Piscataway, NJ 08854-4157

// Comments:  

// Authors: Kirthika Parmeswaran <kirthika@research.telcordia.com>,
//          Balakrishnan Dasarathy <das@research.telcordia.com>,
//	        Murray Conarty <murray.conarty@prismtechnologies.com>,
//          Shree Gadgil <sgadgil@research.telcordia.com>
//		    Ravi Vaidyanathan <sgadgil@research.telcordia.com>
//          Rick Porter <sgadgil@research.telcordia.com>

//
// Date:   August 28, 2005

package mil.darpa.arms.mlrm.BandwidthBroker.FaultMonitorServer;


import java.util.*;

import org.objectweb.openccm.corba.TheORB;
import org.omg.CORBA.ORB;
import org.omg.CosNaming.NamingContextExt;
import org.omg.CosNaming.NamingContextExtHelper;

import mil.darpa.arms.mlrm.*;
import mil.darpa.arms.mlrm.BandwidthBroker.*;
import mil.darpa.arms.mlrm.BandwidthBroker.FaultMonitorPackage.*;
import mil.darpa.arms.mlrm.BandwidthBroker.bb.server.*;
import mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.*;


// This class implements the functions of FaultMonitor
public class FaultMonitorLocalImpl implements FaultMonitorCallback {
  
  private ORB orb = TheORB.getORB();
  private NamingContextExt ncRef = null; 


  // DB pointer
  NRDB db = null;

  // SwitchDisc class
  SwitchDisc sd = null;

  // set the ORB
//  public void setORB(ORB orb_val) {
//    orb = orb_val; 
//  }

  // Debug print function
  private static void _printSwitchInfo(switchInfo sw) {

    if (sw != null) {
      System.out.print("Switch name: " + sw.name); 
      System.out.println(", Status " + sw.status.value());
      System.out.println("Ports: ");
      
      for (int i=0; i<sw.ports.length; i++) {

        System.out.println(sw.ports[i].portName + ", Status: " + sw.ports[i].operStatus.value());
      }
      return;
   } 

   System.out.println("_printSwitch: null values");

  }


  /** Fetch switch Status from the DB and package it in switchInfoHolder */
  private switchInfo _getSwitchStatusFromDB (String swName) 
		throws CDBException 
  {
	System.out.println("In _getSwitchStatusFromDB, switch: " + swName);

	switchInfo info = new switchInfo();

	// for each port, we need to report the operational/admin status
	Hashtable ports = db.getSwitchPortStatus(swName);

	System.out.println("the ports we got are: " + ports.toString());

	// now fill out the switchInfoHolder 
	info.name = swName;
	String status = db.getSwitchStatus(swName);

	// Switch status
	if (status.equalsIgnoreCase("OPERATIONAL"))
	  info.status = switchStatus.OPERATIONAL;
	else info.status = switchStatus.FAIL;  

	// port Status fields
	ArrayList ps = new ArrayList();

	for(Enumeration e = ports.keys(); e.hasMoreElements();) {
	  String portName = (String) e.nextElement();
	  portStatus adminStatus, opnStatus;
	  NEeventType ev;
	  
	  String arr[] = ((String) ports.get(portName)).split(":");

	  if (arr[0].equalsIgnoreCase("UP"))
	    adminStatus = portStatus.UP;
	  else 
	    adminStatus = portStatus.DOWN;

	  if (arr[1].equalsIgnoreCase("UP"))
	    opnStatus = portStatus.UP;
	  else 
	    opnStatus = portStatus.DOWN;

	  if (arr.length > 2) {

	  if (arr[2].equalsIgnoreCase("portUP"))
	    ev = NEeventType.portUP;
	  else if (arr[2].equalsIgnoreCase("portDOWN"))
	    ev = NEeventType.portDOWN;
	  else if (arr[2].equalsIgnoreCase("portFLAP"))
	    ev = NEeventType.portFLAP;
	  else ev = NEeventType.NULL_EVENT;
	  }
	  else ev = NEeventType.NULL_EVENT;
           

	  portInfo p = new portInfo(portName, adminStatus, opnStatus, portType.PHYSICAL, ev);

	  ps.add(p);

	}

	info.ports = (portInfo[]) ps.toArray(new portInfo[]{}); 

	System.out.println("leaving _getSwitchStatusFromDB");
        //_printSwitchInfo(info);
	return info;

  }

  /** Fetch the Switch Status from SNMP */
  public void getSwitchStatus (String swName, switchInfoHolder info) 
		throws FaultMonitorException 
  {

    System.out.println("getSwitchStatus on switch: " + swName);
    // first check to see if the switch is monitored, if so we can get it's 
    // status from the DB
    try {

      if (db == null) db = new NRDB();

      if (sd == null) sd = new SwitchDisc(db, this);

      String isMonitored = db.isSwitchMonitored(swName);

      if (isMonitored.equals("0")) {
	sd.getSwitchInfo(db.getSwitchIDFromName(swName));
      }

      // now just pick up from the DB anyway
      info.value = _getSwitchStatusFromDB(swName);
    }
    catch (CDBException cdbe) {

      cdbe.printStackTrace();
      throw new FaultMonitorException();

    }


  }

  /** Subscribe to a Fault Monitor Event */
  public void subscribeToFaultEvent (String eventSinkName, 
	String cookie, String[] switches, 
	NEeventType type) throws FaultMonitorException 
  {

    // stuff the registration in the DB table FaultSubscriptions
    try {
    
      if (db == null) db = new NRDB();

      if (sd == null) sd = new SwitchDisc(db, this);

      // check to see if the switches are monitored, if not initiate monitoring
      for (int i=0; i<switches.length; i++) {
	System.out.println("In subscribeToFault, switch: " + switches[i]);
        db.updateFaultSubscriptions(eventSinkName, cookie, switches[i], type.value()); 
        if ((db.isSwitchMonitored(switches[i])).equals("0")) {

          // initiate monitoring first 
	  sd.subscribeToTrap(db.getSwitchIDFromName(switches[i]));

          // now update the DB
          db.updateSwitchMonitoringStatus(switches[i], String.valueOf(1));
        }

      }

      db.commit();
      System.out.println("Leaving subscribeToFault");
      
    }
    catch (CDBException cdbe) {

      cdbe.printStackTrace();
      throw new FaultMonitorException();

    }

  }


  /** A fault occured and we were informed of it
      - Get the list of interested parties on this fault
      - Resolve and notify them of the fault event */
  public void notifySubscribersOfFault(String swName, String portName, 
	String status) throws FaultMonitorException
  {

    System.out.println("notifySubscribers: " + swName + ", " + status);

    try {

      // get list of people interested in this switch
      // in the form eventSinkName:cookie
      String[] eventSinks = db.getFaultSinks(swName, status);

      // package the fault notification appropriately

      // get the root naming context
      if (ncRef == null) {
        org.omg.CORBA.Object objRef =
          orb.resolve_initial_references("NameService");

        // Use NamingContextExt which is part of the Interoperable
        // Naming Service (INS) specification.
        ncRef = NamingContextExtHelper.narrow(objRef);
      }


      portInfo [] ports = new portInfo [1] ;
      if (status.equals("UP"))
        ports[0] = new portInfo(portName, portStatus.UP, portStatus.UP, portType.PHYSICAL, NEeventType.portUP);
      else 
        ports[0] = new portInfo(portName, portStatus.DOWN, portStatus.DOWN, portType.PHYSICAL, NEeventType.portDOWN);

      switchInfo sw = new switchInfo(swName, switchStatus.OPERATIONAL, ports);

      // loop through and call the clients
      for (int i=0; i<eventSinks.length; i++) {

        String[] arr = eventSinks[i].split(":");

        System.out.println("About to notify subscriber: " + arr[0]);
        // resolve the eventSink
	    FaultMonitorClient fmClient = FaultMonitorClientHelper.narrow(ncRef.resolve_str(arr[0])); 

        // invoke the eventSink
	fmClient.push_NetworkFault(sw);
      }
    } catch (Exception e) {
      e.printStackTrace();
      throw new FaultMonitorException();
    }

  }

  /**  Unsubscribe a component for fault monitor events */
  public void unsubscribeToFaultEvent (String eventSinkName, 
	String cookie) throws FaultMonitorException
  {


    try {
      // delete the subscription in the DB
      String[] swIDs = db.removeFaultSubscriptions(eventSinkName, cookie);

      // Was that the last subscription for this switch?
      for (int i=0; i< swIDs.length; i++) {
        if (!db.isSwitchSubscribed(swIDs[i])) {
          // no more listeners on this switch, unsubscribe to the switch 
          // from the DB and via SNMP
          db.updateSwitchMonitoringStatus(db.getSwitchNameFromID(swIDs[i]), String.valueOf(0)); 
	  sd.unsubscribeFromTrap(swIDs[i]);
        }
      }

      db.commit();
    } catch (CDBException cdbe) {
      cdbe.printStackTrace();
      throw new FaultMonitorException();
    }
  }

} // end class FaultMonitorLocalImpl
