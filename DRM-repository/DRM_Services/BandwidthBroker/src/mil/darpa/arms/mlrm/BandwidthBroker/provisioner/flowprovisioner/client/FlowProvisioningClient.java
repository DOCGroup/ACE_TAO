// $Id: FlowProvisioningClient.java
//The DARPA ARMS Program
// This software/interface/output
// is governed by Government Purpose Rights
// Contract No: NBCHC030132
// Telcordia Technologies, Inc.
// 1 Technology Drive
// Piscataway, NJ 08854-4157 

// FlowProvisioningClient.java
// Author: Ravi Vaidyanathan <vravi@research.telcordia.com>
// Initial draft: 11/26/03
// Modifications for new IDLs and wrapper: 1/26/04
package mil.darpa.arms.mlrm.BandwidthBroker.provisioner.flowprovisioner.client;
import mil.darpa.arms.mlrm.BandwidthBroker.*;
import mil.darpa.arms.mlrm.BandwidthBroker.FlowProvisionerPackage.*;
//import BandwidthBroker.AdmissionControlPackage.*;
import mil.darpa.arms.mlrm.BandwidthBroker.CommonDef.*;
import org.omg.CosNaming.*;
import org.omg.CosNaming.NamingContextPackage.*;
import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import org.omg.PortableServer.POA;
import java.io.*;
import java.lang.*;

import java.util.Properties;

// client class to initiate calls to the Server
public class FlowProvisioningClient {

  static FlowProvisioner flowP;

  public static void main(String args[]) {
    try{
      // create and initialize the ORB
      ORB orb = ORB.init(args, null);

      // get the root naming context
      org.omg.CORBA.Object objRef =
          orb.resolve_initial_references("NameService");
      // Use NamingContextExt which is part of the Interoperable
      // Naming Service (INS) specification.
      NamingContextExt ncRef = NamingContextExtHelper.narrow(objRef);

      // resolve the Object Reference in Naming
      String name = "FlowProvisioning";
      flowP = FlowProvisionerHelper.narrow(ncRef.resolve_str(name));

      PortInfo src = new PortInfo ((short)10,(short)100);
      PortInfo dst = new PortInfo ((short)10,(short)100);
    
      FlowInfo flow = new FlowInfo (ProtocolType.tcp,
				    new IPAddress ("10.1.1.0","255.255.255.0"),
				    src,
				    new IPAddress ("10.1.2.0","255.255.255.0"),
				    dst, -1 ); // Drop dscp input filtering

      while (true) {

        System.out.print("\n\t(1) Add-IOS\n\t" +
			 "(2) Del-IOS\n\t" +
			 "(3) Add-CatOS\n\t" +
			 "(4) Del-CatOS\n\t" +
			 "(5) Add QoS Policy\n\t" +
			 "(6) Add to OEP  Lab\n\t" +
			 "(7) Del from OEP Lab\n\t" +
			 "(8) Add-Linux\n\t" +
			 "(9) Del-Linux\n\t" +
			 "(10) Exit\nEnter Command (1-10):");
        java.io.BufferedReader in = new java.io.BufferedReader(new InputStreamReader(System.in));

        String choice = in.readLine();
        int c = Integer.parseInt(choice);

        switch (c) {
	  // NOTE: Flow ID's must now be small integers
	case 1:
      
		System.out.println("Adding flow TEST to dandelion...");
      		flowP.addFlow("dandelion","Fa4/1", "1", 
			TrustState.none, 
			(short) 10, flow, 32000,  64000, 128000, 
			ExceedAction.MARKDOWN,
 			ExceedAction.DROP);

                /*
		System.out.println("Adding flow TEST2 to SLC...");
      		flowP.addFlow("SLC","Ethernet1/1", "2", 
			TrustState.none, 
			(short) 20, flow, 25000,  50000, 100000, 
			ExceedAction.MARKDOWN,
 			ExceedAction.DROP);
                */
		break;
     
	  case 2:

		System.out.println("Deleting flows TEST from dandelion...");
      		flowP.delFlow("dandelion","Fa4/1", "1"); 
      		//flowP.delFlow("SLC","Ethernet1/1", "2"); 
      		break;

	  case 3:
		System.out.println("Adding flow TEST to 192.4.249.161...");
	        flowP.addFlow("192.4.249.161","1/1", "3", 
			TrustState.none, 
			(short) 10, flow, 2000, 1000, 4000, 
			ExceedAction.MARKDOWN,
 			ExceedAction.DROP);

		System.out.println("Adding flow TEST2 to 192.4.249.161...");
      		flowP.addFlow("192.4.249.161","1/1", "4", 
			TrustState.none, 
			(short) 10, flow, 2000, 1000, 4000, 
			ExceedAction.MARKDOWN,
 			ExceedAction.DROP);
		break;

	  case 4:
		System.out.println("Deleting flows TEST/TEST2 from 192.4.249.161");
		flowP.delFlow("192.4.249.161","1/1", "3");
      		flowP.delFlow("192.4.249.161","1/1", "4");
		break;
	 
	  case 5:
     		System.out.println("Adding policy to LA");
                String [] intfs = {"Ethernet1/1", "Ethernet1/2"};
                flowP.addQoSPolicy("LA", intfs, "STANDARD_OPS");
     		System.out.println("Adding policy to BOS");
                String [] intfs2 = {"Ethernet1/1", "Ethernet1/2"};
                flowP.addQoSPolicy("BOS", intfs2, "BATTLE_MODE");
		break;

	  case 6:
     		System.out.println("Adding flow to Porstmouth lab");
		flowP.addFlow("192.168.85.1","FastEthernet4/1", "6", 
			TrustState.none, 
			(short) 10, flow, 20000,  40000, 80000, 
			ExceedAction.MARKDOWN,
 			ExceedAction.DROP);	

		flowP.addFlow("192.168.85.1","Vlan61", "7", 
			TrustState.none, 
			(short) 10, flow, 20000,  40000, 80000, 
			ExceedAction.MARKDOWN,
 			ExceedAction.DROP);	
		break;

	 case 7:
     		System.out.println("Deleting flow from Portsmouth lab");
		flowP.delFlow("192.168.85.1","FastEthernet4/1", "6"); 
		flowP.delFlow("192.168.85.1","Vlan61", "7"); 
		break;

	case 8:  // Linux Add
	  System.out.println("Adding flow: " + "8");
	  flowP.addFlow("192.4.8.246","eth1", "8", 
			TrustState.none, 
			(short) 4, flow, 20000, 40000, 80000, 
			ExceedAction.MARKDOWN,
 			ExceedAction.DROP);
	  System.out.println("Adding flow: " + "9");
	  flowP.addFlow("192.4.8.246","eth1", "9", 
			TrustState.none,
			(short) 5, flow, 10000, 20000, 50000, 
			ExceedAction.MARKDOWN,
 			ExceedAction.DROP);
	  break;

	case 9:  // Linux Delete
	  System.out.println("Deletinging second Linux flow: " + "8");
	  // Stateful
	  flowP.delFlowSL("192.4.8.246",
			"eth1", 
			"8", 
			TrustState.none, 
			(short) 4, 
			flow, 
			20000, 
			40000, 
			80000, 
			ExceedAction.MARKDOWN,
 			ExceedAction.DROP);
	  System.out.println("Deletinging second Linux flow: " + "9");
	  // Stateless
	  flowP.delFlow("192.4.8.246","eth1", "9");
	  break;

	  case 10:
     		System.out.println("FlowProvisioningClient Exiting ...");
		return;

          default:
		System.out.println("Unrecognized command, expecting (1-5)");
                break;

        }
      }

    }
    catch (Exception e) {
       System.err.println("ERROR: " + e);
       e.printStackTrace(System.out);
     }
	  
     System.out.println("FlowProvisioningClient Exiting ...");
	
  }

} // end class FlowProvisioningClient
