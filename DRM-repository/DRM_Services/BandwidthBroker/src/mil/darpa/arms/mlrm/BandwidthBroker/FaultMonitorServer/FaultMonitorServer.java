//$Id: FaultMonitorServer.java
// Author: Ravi Vaidyanathan <vravi@research.telcordia.com>

package mil.darpa.arms.mlrm.BandwidthBroker.FaultMonitorServer;

import mil.darpa.arms.mlrm.BandwidthBroker.*;
import mil.darpa.arms.mlrm.BandwidthBroker.FaultMonitorPackage.*;
import org.omg.CosNaming.*;
import org.omg.CosNaming.NamingContextPackage.*;
import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import org.omg.PortableServer.POA;


// Main server class that initializes various functions
public class FaultMonitorServer {

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

   System.out.println("_printSwitch: null values");

  }

  public static void main(String args[]) {
    try{

      // create and initialize the ORB
      ORB orb = ORB.init(args, null);

      // get reference to rootpoa & activate the POAManager
      POA rootpoa = POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
      rootpoa.the_POAManager().activate();

      // create servant and register it with the ORB
      FaultMonitorImpl fault = new FaultMonitorImpl();
      fault.setORB(orb); 

      // get object reference from the servant
      org.omg.CORBA.Object ref = rootpoa.servant_to_reference(fault);
      FaultMonitor fref = FaultMonitorHelper.narrow(ref);
	  
      // get the root naming context
      org.omg.CORBA.Object objRef =
          orb.resolve_initial_references("NameService");
      // Use NamingContextExt which is part of the Interoperable
      // Naming Service (INS) specification.
      NamingContextExt ncRef = NamingContextExtHelper.narrow(objRef);

      // bind the Object Reference in Naming
      String name = "FaultMonitor";
      NameComponent path[] = ncRef.to_name( name );
      ncRef.rebind(path, fref);
      
      System.out.println("FaultMonitorServer ready and waiting ...");

/*
      switchInfoHolder sw = null;

      // test the call
      fref.getSwitchStatus("daisy", sw);

      _printSwitchInfoHolder(sw);
*/

      // wait for invocations from clients
      orb.run();
    } 
	
    catch (Exception e) {
       System.err.println("ERROR: " + e);
       e.printStackTrace(System.out);
     }
	  
     System.out.println("FaultMonitorServer Exiting ...");
	
  }

} // end class FaultMonitorServer






