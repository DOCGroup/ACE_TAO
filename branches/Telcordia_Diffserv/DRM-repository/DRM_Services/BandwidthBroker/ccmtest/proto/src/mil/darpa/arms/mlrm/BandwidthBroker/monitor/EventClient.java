
package mil.darpa.arms.mlrm.BandwidthBroker.monitor;

import mil.darpa.arms.mlrm.*;
import mil.darpa.arms.mlrm.BandwidthBroker.*;
import mil.darpa.arms.mlrm.BandwidthBroker.cif.*;
//import mil.darpa.arms.mlrm.BandwidthBroker.CommonDef.*;
//import  mil.darpa.arms.mlrm.BandwidthBroker.AdmissionControlPackage.*;
//import mil.darpa.arms.mlrm.BandwidthBroker.FlowProvisionerPackage.*;
import org.omg.CosNaming.*;
import org.omg.CosNaming.NamingContextPackage.*;
import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import org.omg.PortableServer.POA;
import org.omg.CosPropertyService.*;
import java.io.*;
import java.lang.*;

import java.util.*;


public class EventClient
{

  //   static FlowProvisioner flowP;
  //  static AdmissionControl admCtrl;

    public static void main(String args[]) {

        try
        {

            // create and initialize the ORB
            System.out.println("Initializing the ORB...");
            org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init(args,null);

            // Obtain the Name Service.
            System.out.println("Obtaining the Name Service...");
            org.omg.CORBA.Object obj = orb.resolve_initial_references("NameService");
            if (obj == null)
                System.out.println("NameService Error");
            org.omg.CosNaming.NamingContext nc = org.omg.CosNaming.NamingContextHelper.narrow(obj);


   //         System.out.println("Obtaining FP...");
            // FlowProvisioner server was created during deployment and bound to the name service
            // We can get that reference from the name service 
            // or get a reference to the home and create a new instance
            // In this case, we'll stick to using just one instance - so we get it directly from the NS
            org.omg.CosNaming.NameComponent[] ncomp = new org.omg.CosNaming.NameComponent[2];
     //       ncomp[0] = new org.omg.CosNaming.NameComponent("OpenCCM", "");
      //      ncomp[1] = new org.omg.CosNaming.NameComponent("FlowProvisioner", "");

       //     org.omg.CORBA.Object fpObj  = nc.resolve(ncomp);
 //           FPServer fp = FPServerHelper.narrow(fpObj);
//
  //          flowP = fp.provide_fp_clients();


            // Now get the BW Server.
   //         System.out.println("Obtaining BW...");
//
 //           ncomp[0] = new org.omg.CosNaming.NameComponent("OpenCCM", "");
  //          ncomp[1] = new org.omg.CosNaming.NameComponent("BandwidthBroker", "");

   //         org.omg.CORBA.Object bwObj  = nc.resolve(ncomp);
    //        BWServer bw = BWServerHelper.narrow(bwObj);

     //       admCtrl = bw.provide_ac_clients();



            // ClientHome reference is available with the NS 
            // Unlike the BW and the FP servers, we didnt create an instances 
            // of the client component. We will create those here
           System.out.println("Obtaining ClientHome...");
            ncomp[0] = new org.omg.CosNaming.NameComponent("OpenCCM", "");
            ncomp[1] = new org.omg.CosNaming.NameComponent("BandwidthBrokerClientHome", "");
            org.omg.CORBA.Object clObj  = nc.resolve(ncomp);
            ClientHome cHome = ClientHomeHelper.narrow(clObj);


            ncomp[0] = new org.omg.CosNaming.NameComponent("OpenCCM", "");
            ncomp[1] = new org.omg.CosNaming.NameComponent("BBClient", "");
            clObj  = nc.resolve(ncomp);


            // Connect the client receptacle to the FP faucet
            // ClientHome cHome = ClientHomeHelper.narrow(clObj);
            // Client c1 = cHome.create();

            Client c1 = ClientHelper.narrow (clObj);
            c1.the_name("Event");

      //      c1.connect_fp_server(flowP);
       //     c1.connect_ac_server(admCtrl);
            c1.configuration_complete();

//            BWNotificationConsumer cons = c1.get_consumer_from_BWComponent(); 
//            mil.darpa.arms.mlrm.BandwidthBroker.cif.BWNotificationEvent evt = new mil.darpa.arms.mlrm.BandwidthBroker.cif.BWNotificationEvent("Event");
 //           for( int i = 0; i < 10; i ++)
  //              cons.push_BWNotification(evt);

              ConditionConsumer cons = c1.get_consumer_from_BWComponent ();

	      while (true)
	      {
		 try
		 {
		 String rc = monitorFlows ();

		 if (rc != null)
		 {
	            BWConditionEvent cdn = 
		       new BWConditionEvent (rc, orb.create_any ());
                    cons.push_Condition (cdn);
		 }
		 Thread.sleep (15000);
		 }
		 catch (Exception e)
		 {
		    e.printStackTrace ();
		    break;
		 }

	      }


        } 
        catch( Exception ex)
        {
            ex.printStackTrace();
        }
    }

    private static String parsetc = " tc -s filter show dev eth0 root > filter-data ; tc -s filter show dev eth1 root >> filter-data ; tc -s filter show dev eth2 root >> filter-data ; tc -s filter show dev eth3 root >> filter-data ; tc -s filter show dev eth4 root >> filter-data ; awk '/handle/ {h = $10; split (h, hh, \"x\"); printf (\"%s \", hh[2])} /overlimits/ {b = $9 ; split (b, v, \")\"); print v[1]} ' filter-data; rm filter-data";

    private static Hashtable lastReadings = new Hashtable ();

    private static String monitorFlows () throws Exception
    {
       String rc = null;
       String outFile = "output";
       LinuxUtils._issueCmd (parsetc, "switch1", outFile);
       BufferedReader br = new BufferedReader (new FileReader (outFile));
       String flowID;
       Long overlimits, oldOV;

       while (true)
       {
          String line = br.readLine ();
	  if (line == null)
	     break;
	  StringTokenizer st = new StringTokenizer (line);
	  if (st.hasMoreTokens ())
          {
	     flowID = "" + Integer.parseInt (st.nextToken (), 16);
             {
	        if (st.hasMoreTokens ())
		{
	           overlimits = new Long (st.nextToken ());

	           oldOV = (Long) lastReadings.get (flowID);
	           if (oldOV != null)
	           {
	              if (overlimits.compareTo (oldOV) > 0) 
	              {
	                if (rc == null)
	                   rc = "";
	                rc = rc + flowID + ":" + 
	                 (overlimits.longValue () - oldOV.longValue ()) + " ";
	              }
	           }
	           lastReadings.put (flowID, overlimits);
	        }
	     }
	  }
       }
       System.out.println (rc);
       return rc;
    }


}

        
