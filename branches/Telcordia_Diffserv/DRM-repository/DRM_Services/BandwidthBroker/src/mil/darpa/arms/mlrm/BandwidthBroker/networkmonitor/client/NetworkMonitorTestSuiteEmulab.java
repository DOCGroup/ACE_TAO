//$Id$
/*
 * NetworkMonitorTestSuite.java - the probe monitor test suite
 *
 * The DARPA ARMS PROGRAM
 * This software is governed by Government Purpose Rights
 * Contract No: NBCHC030132
 * Telcordia Technologies, Inc.
 * One Telcordia Drive
 * Piscataway, NJ 08854-4157
 *
 * Author: Kirthika Parmeswaran
 */

package mil.darpa.arms.mlrm.BandwidthBroker.networkmonitor.client;
import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import mil.darpa.arms.mlrm.BandwidthBroker.*;
import mil.darpa.arms.mlrm.BandwidthBroker.NetworkMonitorPackage.*;
import mil.darpa.arms.mlrm.BandwidthBroker.CommonDef.*;
import org.omg.CosNaming.*;
import org.omg.CosNaming.NamingContextPackage.*;
import java.io.*;
/** This class tests the NetworkMonitor server portion of the
 * BandwidthBroker framework. 

 * The test_latency_betn_hosts () and test_latency_betn_pools () use the same
 * source-destination measurement hosts + TOS and hence have the same
 * probeID. The test_latency_betn_flowendpoints () uses flowtoken to find
 * latency. The TOS is different in this case, hence the probeID is different.
 * Since, the first time around, the probes are started and the test
 * has to wait for resuts, the test may take 3*measurementPeriod
 * (i.e. 3min in this case) to complete. Keep the server running, and
 * retry the client, it will be faster the second time.

 * It is required that before running this
 * test, the probe server and Imp software be running on the probe
 * hosts.  The scripts bin/runProbeServer.sh and bin/RunProbe.sh show how to
 * do this respectively, first start the probe server,then the probes.  
 * The nmconfig.prop file mentions the names of the probe
 * hosts. These hosts must also be populated into the BB Mysql DB as
 * part of the HostSubnet table.  The test also uses a <flowToken>
 * which is the flow record ID in the BB Mysql DB.  To populate the
 * BBMySQL DB for running this test, it is necessary to populate
 * db/nm-data.sql in addition to populating the other sql inputs. */


public class NetworkMonitorTestSuiteEmulab
{
    
    private static ORB orb;
    private static POA poa;

    public static void main (String [] args)
    {
	NetworkMonitorClientEmulab nmClient = new NetworkMonitorClientEmulab ();
	NetworkMonitorEventClientEmulab nmAsyncClient = new NetworkMonitorEventClientEmulab ();
       	NetworkMonitor netMon=null;
	org.omg.CORBA.Object obj =null;

	if( args.length < 3 ) 
	    {
		System.out.println( "Usage: java "+
				    " com.telcordia.research.BandwidthBroker.networkmonitor.client.NetworkMonitorTestSuiteEmulab"+ "<clientname>"+
				    " -ORBInitialHost <name service host> "+
				    "-ORBInitialPort <name service port>" );
		System.exit( 1 );
	    }
	String clientName = args[0];//"NetworkMonitorEventClient"; //default
	System.out.println ("Notification client:"+clientName);
      // initialize the ORB.


	try 
	    {
		orb = ORB.init( args, null );
		//initialize POA
         	poa =
                POAHelper.narrow( orb.resolve_initial_references( "RootPOA" ));

        	poa.the_POAManager().activate();
		
		NamingContextExt rootContext = null;
		NamingContextExt root;
		
		try
		    {
			// Resolve NameService
			obj =
			    orb.resolve_initial_references ("NameService");
			rootContext = NamingContextExtHelper.narrow (obj);
		    }
		catch (org.omg.CORBA.ORBPackage.InvalidName ex)
		    {
			System.err.println ("Unknown Service");
		    }
		if (rootContext == null)
		    {
			System.err.println ("Failed to resolve NameService");
			System.exit (1);
		    }
		else
		    {
			System.out.println ("Resolved NameService");
		    }
		// register as the push_condition servant
		// create the object reference
		obj =
		    poa.servant_to_reference(nmAsyncClient);
		 NameComponent myName [] = {
		     new NameComponent("mil", ""),
		     new NameComponent("darpa", ""),
		     new NameComponent("arms", ""),
		     new NameComponent("mlrm", ""),
		     new NameComponent("BandwidthBroker", ""),
		     new NameComponent(clientName, "")
		 };
		 
		 nsBind (myName, obj);
		
		 nmAsyncClient.nmEventClient = rootContext.to_string (myName);
		 System.out.println ("Registered"+nmAsyncClient.nmEventClient+" client IOR with Naming");
	
		 // Lookup the NetworkMonitor object)
		NameComponent theName [] = {
              new NameComponent("mil", ""),
              new NameComponent("darpa", ""),
              new NameComponent("arms", ""),
              new NameComponent("mlrm", ""),
              new NameComponent("BandwidthBroker", ""),
              new NameComponent("NetworkMonitor", "")

            };
		obj = rootContext.resolve_str (rootContext.to_string (theName));
		
		// and narrow it to NetworkMonitor
	        netMon = NetworkMonitorHelper.narrow( obj );
	    }
	catch (Exception e)
	    {
		System.err.println ("ERROR:"+
				    "Exception in network monitor client setup execution!"+e.toString ()); 
		e.printStackTrace ();
		System.exit (-1);
	    }


		boolean runnow=true;
		int whichcase=0;

		while (runnow)
		    {
			try
			    {
				System.out.println ("Performance/Network Monitor Test Suite (press the number of the testcase):");
				System.out.println ("1. Query: Delay between pools\n"+
						    "2. Query: Delay between flow endpoints\n"+
						    "3. Query: Delay between hosts \n"+
						    "4. Global threshold based Notification: Delay between pools\n"+
						    "5. Local threahold based Notification: Delay between flow endpoints\n"+
						    "6. Periodic Notification: Delay between hosts\n"+
						    "7. Exit");
				BufferedReader br = new BufferedReader (new InputStreamReader (System.in));	
		 	       try{	
				whichcase = Integer.parseInt(br.readLine ());
			        }	
				catch (Exception e)
				{
				  whichcase =0;
                                 }		
				
				switch (whichcase)
				    {
				    case 3: 	
					{
					    System.out.println ("\n\nTesting DELAY BETWEEN HOSTS begins ...");
					    System.out.println ("Hosts:1. rondomon08 and rondomon09,\n"+
								"      2. alpha and beta\n"+
								"  QoS:1. HighPriority\n"+
								"      2. HighReliability\n"+
								"Probe measurement frequency : 60 seconds\n"+
								"Period over which result is averaged : 190 seconds"
								);
					    System.out.println (nmClient.test_delay_betn_hosts (netMon));
					    System.out.println ("Testing DELAY BETWEEN HOSTS ends.\n");
					    break;
					}
				    case 2: 	
					{
					    System.out.println ("\n\nTesting DELAY BETWEEN FLOW ENDPOINTS begins ...");
					    System.out.println ("Flow Token: 1 \n"+
								"QoS: HighReliability\n"+
								"Probe measurement frequency : 60 seconds\n"+
								"Period over which result is averaged : 190 seconds"
							);
					    System.out.println ("DELAY="+nmClient.test_delay_betn_flowendpoints (netMon));
					    System.out.println ("Testing DELAY BETWEEN FLOWENDPOINTS ends.\n");
					    break;
					}
				    case 1: 	
					{
					    System.out.println ("\n\nTesting DELAY BETWEEN POOLS begins ...");
					    System.out.println ("Pools: Pool1C and Pool1D\n"+
								"  QoS: HighPriority\n"+
								"Probe measurement frequency : 60 seconds\n"+
								"Period over which result is averaged : 60 seconds"
								);
					    System.out.println ("DELAY="+nmClient.test_delay_betn_pools (netMon));
					    System.out.println ("Testing DELAY BETWEEN POOLS ends.\n");
					    break;
					}
				    case 6: 	
					{
					    System.out.println ("\n\nTesting PERIODIC NOTIFICATION for DELAY BETWEEN HOSTS begins ...");
					    //  System.out.println ("netmon="+netMon+" clientname=*"+nmAsyncClient.nmEventClient+"*");
					    System.out.println ("Event Type: Periodic update every 60 seconds (after first event)\n"+
								"Hosts: rondomon08 and rondomon09\n"+
								"QoS: HighPriority\n"+
								"Probe measurement frequency : 60 seconds\n"+
								"Period over which result is averaged : 190 seconds\n"
								);
					    System.out.print ("\nProbe setup in progress...");
					    nmAsyncClient.test_delay_betn_hosts_periodic_event (netMon);
					    System.out.println ("now complete.\n\nWaiting 300 seconds for event notifications ...\n");
					    
				    	try{
					    Thread.sleep (300000);
					} catch (Exception ee)
					    {
						System.out.println ("Thread sleep interrupted in client execution");
						ee.printStackTrace ();
						System.exit (-1);
					    }
					System.out.println ("\n\nUNSUBSCRIBE EVENT for PERIODIC DELAY EVENT between HOSTS ..."); 
					nmAsyncClient.test_delay_betn_hosts_periodic_event_unsubscribe (netMon);

					System.out.println ("Testing PERIODIC NOTIFICATION for DELAY BETWEEN HOSTS ends.\n");
				    break;
					}
				    case 5: 	
					{
					    System.out.println ("\n\nTesting LOCAL THRESHOLD BASED NOTIFICATION for DELAY BETWEEN FLOWENDPOINTS begins ...");
					    System.out.println ("Event Type: Notification if delay is over 2 milliseconds"+
								" for this particular flow token (overrides global threshold of"+
								" 120 seconds specified for a sequence of event requests)\n"+
								"Flow Token: 1\n"+
								"QoS: HighReliability\n"+
								"Probe measurement frequency : 60 seconds\n"+
								"Period over which result is averaged : 190 seconds\n"
								);
					    System.out.print ("\nProbe setup in progress...");
					    nmAsyncClient.test_delay_betn_flowendpoints_local_threshold_event (netMon);
					    
					    System.out.println ("now complete. \n\nWaiting 300 seconds for event notifications ...\n");
					    
					    try{
						Thread.sleep (300000);
					    } catch (Exception ee)
						{
						    System.out.println ("Thread sleep interrupted in client execution");
						    ee.printStackTrace ();
						    System.exit (-1);
						}
					    System.out.println ("\n\nUNSUBSCRIBE EVENT for DELAY BETWEEN FLOWENDPOINTS with local threshold ..."); 
					    nmAsyncClient.test_delay_betn_flowendpoints_local_threshold_event_unsubscribe (netMon);
					    System.out.println ("Testing LOCAL THRESHOLD BASED NOTIFICATION for DELAY BETWEEN FLOWENDPOINTS ends.\n");
					    break;
					}
				    case 4: 	
					{
					    System.out.println ("\n\nTesting GLOBAL THRESHOLD BASED NOTIFICATION for DELAY BETWEEN POOLS begins ...");
					    System.out.println ("Event Type: Notification if delay is over 2 milliseconds,"+
						       " (the global threshold for this event request)\n"+
								"Pools: Pool1C and Pool1D \n"+
								"QoS: HighPriority\n"+
								"Probe measurement frequency : 60 seconds\n"+
								"Period over which result is averaged : 60 seconds\n");
					    
					    System.out.print ("\nProbe setup in progress...");
					    nmAsyncClient.test_delay_betn_pools_global_threshold_event (netMon);
					    System.out.println ("now completed. \n\nWaiting 300 seconds for event notifications ...\n");
					    
					    try{
						Thread.sleep (300000);
					    } catch (Exception ee)
						{
						    System.out.println ("Thread sleep interrupted in client execution");
						    ee.printStackTrace ();
						    System.exit (-1);
						}
					    System.out.println ("\n\nUNSUBSCRIBE EVENT for DELAY BETWEEN POOLS with global threshold ..."); 
					    nmAsyncClient.test_delay_betn_pools_global_threshold_event_unsubscribe (netMon);
					    
					    System.out.println ("\n\nTesting GLOBAL THRESHOLD BASED NOTIFICATION for DELAY BETWEEN POOLS ends. \n");
					    
					    break;
					}
					
				    case 7: 
					{
					    runnow=false;
					    
					    System.out.println ("Exiting Performance/Network Monitor Test Suite");
					    System.exit (1);
					    break;
					}
				    default:
					{
					    System.out.println ("Wrong input:"+whichcase+", so try again");
					    break;
					}
				    }// end of switch
				
			    }// end of try
			catch (Exception e)
			    {
				System.err.println ("Exception in network monitor client execution!"+e.toString ()); 
				e.printStackTrace ();
			    }
			
		    }// end of whil  

    }


    private static NamingContext findOrMakeContext(NamingContext parent,
                                                   NameComponent cname)
        throws NotFound,
               CannotProceed,
               InvalidName
    {
        NameComponent[] name = { cname };
        try {
            parent.bind_new_context(name);
        } catch (AlreadyBound ab) {
            // ignore this error
        }
        org.omg.CORBA.Object ref = parent.resolve(name);
        return NamingContextHelper.narrow(ref);
    }

    private static void bind(NamingContext root,
                             NameComponent[] name,
                             org.omg.CORBA.Object ref)
        throws NotFound,
               CannotProceed,
               InvalidName,
               AlreadyBound
    {
        int context_length = name.length-1;
        NamingContext context = root;
        for (int i=0; i<context_length; i++) {
            context = findOrMakeContext(context, name[i]);
        }
        NameComponent[] tname = { name[context_length] };
        context.rebind(tname, ref);
    }

    public static void nsBind (NameComponent[] name,
			org.omg.CORBA.Object ref)
    {
        try {
            // Depends on -ORBInitRef having been set properly.
            org.omg.CORBA.Object o =
                orb.resolve_initial_references("NameService");
            NamingContextExt root = NamingContextExtHelper.narrow(o);
            bind(root, name, ref);
        } catch (Exception ex) 
        {
        }
    }

}
