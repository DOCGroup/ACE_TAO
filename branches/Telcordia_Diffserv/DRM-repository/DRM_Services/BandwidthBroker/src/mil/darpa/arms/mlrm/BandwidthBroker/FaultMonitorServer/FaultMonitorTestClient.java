// $Id: FaultMonitorTestClient.java
//The DARPA ARMS Program
// This software/interface/output
// is governed by Government Purpose Rights
// Contract No: NBCHC030132
// Telcordia Technologies, Inc.
// 1 Telcordia Drive
// Piscataway, NJ 08854-4157 

// FaultMonitorTestClient.java
// Author: Ravi Vaidyanathan <vravi@research.telcordia.com>

package mil.darpa.arms.mlrm.BandwidthBroker.FaultMonitorServer;


import mil.darpa.arms.mlrm.*;
import mil.darpa.arms.mlrm.BandwidthBroker.*;
import mil.darpa.arms.mlrm.BandwidthBroker.FaultMonitorPackage.*;
import org.omg.CosNaming.*;
import org.omg.CosNaming.NamingContextPackage.*;
import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import org.omg.PortableServer.POA;

// client class to initiate calls to the Server
public class FaultMonitorTestClient {

  static FaultMonitor fm;

  // Debug print function
  private static void _printSwitchInfoHolder(switchInfoHolder sw) {

    if (sw.value != null) {
      System.out.print("Switch name: " + sw.value.name); 
      System.out.println(", Status " + sw.value.status.value());
      System.out.println("Ports: ");
      
      for (int i=0; i<sw.value.ports.length; i++) {

        System.out.println(sw.value.ports[i].portName + ", Status: " + sw.value.ports[i].operStatus.value());
      }
      return;
   } 
  }

  public static void main(String args[]) {
    try{

      // create and initialize the ORB
      ORB orb = ORB.init(args, null);

/*
      // get the root naming context
      org.omg.CORBA.Object objRef =
          orb.resolve_initial_references("NameService");
      // Use NamingContextExt which is part of the Interoperable
      // Naming Service (INS) specification.
      NamingContextExt ncRef = NamingContextExtHelper.narrow(objRef);

*/


      // get reference to rootpoa & activate the POAManager
      POA rootpoa = POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
      rootpoa.the_POAManager().activate();

      // create servant and register it with the ORB
      FaultMonitorClientImpl fmClient = new FaultMonitorClientImpl();
      fmClient.setORB(orb); 

      // get object reference from the servant
      org.omg.CORBA.Object ref = rootpoa.servant_to_reference(fmClient);
      FaultMonitorClient cref = FaultMonitorClientHelper.narrow(ref);
	  
      // get the root naming context
      org.omg.CORBA.Object objRef =
          orb.resolve_initial_references("NameService");
      // Use NamingContextExt which is part of the Interoperable
      // Naming Service (INS) specification.
      NamingContextExt ncRef = NamingContextExtHelper.narrow(objRef);

      // bind the Object Reference in Naming
      String name = "FaultMonitorTestClient";
      NameComponent path[] = ncRef.to_name( name );
      ncRef.rebind(path, cref);
      
      // resolve the Object Reference in Naming
      String name2 = "FaultMonitor";
      fm = FaultMonitorHelper.narrow(ncRef.resolve_str(name2));

      switchInfoHolder sw = new switchInfoHolder ();

      // test the call
      System.out.println("Testing the synchronous call on switch: daffodil");
      fm.getSwitchStatus("daffodil", sw);

      _printSwitchInfoHolder(sw);

      // now test the subscribe and unsubscribe events
      System.out.println("Subscribing for asynchronous events on: dandelion");
      fm.subscribeToFaultEvent(name, "oreo",
		new String[] {"dandelion"}, NEeventType.ALL);

      // wait for invocations from clients
      System.out.println("FaultMonitorTestClient waiting for NetworkFault events");
      orb.run();


     /*
      switchInfoHolder sw = new switchInfoHolder ();

      System.out.println("Getting switch status for daisy");
      // test the getstatus call
      fm.getSwitchStatus("daisy", sw);

      //_printSwitchInfoHolder(sw);

      System.out.println("Subscribing to fault on dandelion");

      // now test the subscribe and unsubscribe events
      fm.subscribeToFaultEvent("FaultMonitorTest", "oreo",
		new String[] {"dandelion"}, NEeventType.ALL);


      System.out.println("Sleeping for 10 secs...");
      Thread.sleep(10000);
      fm.unsubscribeToFaultEvent("FaultMonitorTest", "oreo"); 
     */


    }
    catch (Exception e) {
       System.err.println("ERROR: " + e);
       e.printStackTrace(System.out);
     }
	  
     System.out.println("FaultMonitorTestClient Exiting ...");
	
  }

} // end class FlowProvisioningClient
