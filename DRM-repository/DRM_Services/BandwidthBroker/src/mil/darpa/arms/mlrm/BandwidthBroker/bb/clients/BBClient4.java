// $Id: BBClient4.java,
// This software is governed by Government Purpose Rights
// Contract No: NBCHC030132
// Telcordia Technologies, Inc.
// 1 Technology Drive
// Piscataway, NJ 08854-4157

// BBClient3.java

// Author : Kirthika Parmeswaran 
// This class tests the BBServer's bandwidth allocation policy change function
package mil.darpa.arms.mlrm.BandwidthBroker.bb.clients;

import java.io.*;
import org.omg.CORBA.*;
import mil.darpa.arms.mlrm.BandwidthBroker.*;
import mil.darpa.arms.mlrm.BandwidthBroker.AdmissionControlPackage.*;
import mil.darpa.arms.mlrm.BandwidthBroker.CommonDef.*;
import org.omg.CosNaming.*;
import org.omg.CosNaming.NamingContextPackage.*;

public class BBClient4
{
    public static void main( String args[] ) 
    {
             if( args.length < 1 ) 
	{
	  System.out.println( "Usage: java "+
			      " com.telcordia.research.BandwidthBroker.BBClient"+
			      " -ORBInitialHost <name service host> "+
			      "-ORBInitialPort <name service port>" );
	  System.exit( 1 );
        }
      // initialize the ORB.
      ORB orb = ORB.init( args, null );
      org.omg.CORBA.Object obj =null;
      try 
	{
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
	  // Lookup the BBServer (AdmissionControl object)
	  obj = rootContext.resolve_str ("AdmissionControl");
	  
      	  // and narrow it to AdmissionControl
	  // if this fails, a BAD_PARAM will be thrown
	  AdmissionControl adControl = AdmissionControlHelper.narrow( obj );
	  // invoke the operation and print the result
	  
	  // Bandwidth per service class (high_priority,
	  // high_reliability, high_throughput,best-effort) fractions for modes such as policy1
	  // policy2 are 10,20,40,30 and 20,30,40,10 respectively.

	  AdmissionControlResult r  ;
	  r  = adControl.changeBandwidthAllocationPolicyTo (BandwidthPolicy.policy1);
	  if (r == AdmissionControlResult.DECISION_ADMIT)
	    System.out.println (" Changed to bandwidth allocation policy:"+
				new Integer (BandwidthPolicy.policy1.value ()).toString());
	  else
	    System.out.println (" Failure in changing to bandwidth allocation policy:"+
				new Integer (BandwidthPolicy.policy1.value ()).toString());
	  

           
           testBWQuery (adControl);
        }
      catch( Exception ex ) 
	{
	  System.err.println( ex );
	  ex.printStackTrace();
        }
    }

    public static void testBWQuery (AdmissionControl adControl)
    {
       BWBetnSubnetsSeqHolder bsh = new BWBetnSubnetsSeqHolder ();

       bsh.value = new BWBetnSubnets [2];

       bsh.value [0]  = new BWBetnSubnets ();
       bsh.value [0].srcSubnet = new IPAddress ("172.16.14.0", "255.255.255.0");
       bsh.value [0].destSubnet = new IPAddress ("172.16.13.0", "255.255.255.0");
       bsh.value [0].bwPerClassSeq = new BWPerQoS [1];
       bsh.value [0].bwPerClassSeq [0] = new BWPerQoS (new BandwidthRate (5000, 0),
                                                    QOSRequired.highReliability);

       bsh.value [1]  = new BWBetnSubnets ();
       bsh.value [1].srcSubnet = new IPAddress ("172.16.13.0", "255.255.255.0");
       bsh.value [1].destSubnet = new IPAddress ("172.16.14.0", "255.255.255.0");
       bsh.value [1].bwPerClassSeq = new BWPerQoS [1];
       bsh.value [1].bwPerClassSeq [0] = new BWPerQoS (new BandwidthRate (5000, 0),
                                                    QOSRequired.highReliability);


       try
       {
	  AdmissionControlResult r =  
          adControl.subnetbandwidthQuery (bsh, BWQueryType.available);

	  if (r == AdmissionControlResult.DECISION_ADMIT)
	  {
          //for (int i = 0; i < bsh.value.length; i++)
          {
             System.out.print ("Bandwidth query returned ");
             System.out.print (bsh.value [0].bwPerClassSeq [0].availableBW.requiredBW);
             System.out.println ("," + bsh.value [1].bwPerClassSeq [0].availableBW.requiredBW);
          }
          }
          else
             System.out.println ("Bandwidth query failed.");
       }
       catch (Exception e)
       {
          System.out.println ("exception during bandwidth query.");
          e.printStackTrace ();
       }

    }
}

