// $Id: BBClient.java
// This software is governed by Government Purpose Rights
// Contract No: NBCHC030132
// Telcordia Technologies, Inc.
// 1 Technology Drive
// Piscataway, NJ 08854-4157

// BBClient.java

// Author : Kirthika Parmeswaran 
// This class tests flow requests and deletions 

package mil.darpa.arms.mlrm.BandwidthBroker.bb.clients;

import org.omg.CORBA.*;

import mil.darpa.arms.mlrm.BandwidthBroker.*;
import mil.darpa.arms.mlrm.BandwidthBroker.AdmissionControlPackage.*;
import mil.darpa.arms.mlrm.BandwidthBroker.CommonDef.*;

import org.omg.CosNaming.*;

public class BBCCMClient 
{
	   static String action = "";
	   static int rate = 0;
	   static String flowID = "";
	   static String policy = "";
	   static String srcIP = null, destIP = null;
	   static String qos = "";
	   
	   static QOSRequired findQoS (String qos)
	   {
	      if (qos.equals ("highPriority"))
	         return QOSRequired.highPriority;
	      else if (qos.equals ("mmVoice"))
	         return QOSRequired.mmVoice;
	      else if (qos.equals ("mmVideo"))
	         return QOSRequired.mmVideo;
	      else if (qos.equals ("highReliability"))
	         return QOSRequired.highReliability;
	      return QOSRequired.bestEffort;
	   }
	   static void parseArg (String arg)
	   {
	      if (arg.startsWith ("a"))
	         action = arg.substring (2);
	      else if (arg.startsWith ("r"))
	         rate = Integer.parseInt (arg.substring (2));
	      else if (arg.startsWith ("f"))
	         flowID = arg.substring (2);
	      else if (arg.startsWith ("p"))
	         policy = arg.substring (2);
	      else if (arg.startsWith ("s"))
	         srcIP = arg.substring (2);
	      else if (arg.startsWith ("d"))
	         destIP = arg.substring (2);
	      else if (arg.startsWith ("q"))
	         qos = arg.substring (2);
	   }
	   static void parseArgs (String args [])
	   {
	       for (int i = 0; i < args.length; i++)
	       {
		  parseArg (args [i]);
	       }
	    }

    public static void main( String args[] ) 
    {
    	/*
      if( args.length < 1 ) 
	{
	  System.out.println( "Usage: java "+
			      " com.telcordia.research.BandwidthBroker.BBCCMClient"+
			      " -ORBInitialHost <name service host> "+
			      "-ORBInitialPort <name service port>" );
	  System.exit( 1 );
        }
        */

      parseArgs (args);
       
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
	  // Lookup the BBComponent (provides AdmissionControl functionality)
	  obj = rootContext.resolve_str ("mil/darpa/arms/mlrm/BandwidthBroker/BandwidthBrokerComponent");
	  
	  // and narrow it to AdmissionControl
	  // if this fails, a BAD_PARAM will be thrown
	  BandwidthBrokerComponent bbComponent = BandwidthBrokerComponentHelper.narrow( obj );
	  AdmissionControl adControl = bbComponent.provide_theController();
	  
	  // invoke the operation and print the result
	  //String srcIP = "192.168.81.100";
	  String smask="255.255.255.0";
	  //String destIP = "192.168.82.101";
	  String dmask="255.255.255.0";

      srcIP = "mako"; destIP = "javelin"; 
	  //smask = ""; dmask = "";
	  
	  // invoke the operation and print the result
	  //short pn1 = 20;
	  short pn1 = -1;
	  short pn2 = -1;
	  PortInfo sPort = new PortInfo (pn1,pn1); //low,high same
	  PortInfo dPort = new PortInfo (pn2,pn2);
	  BBProtocolType protocol = BBProtocolType.tcp; //tcp
	  int fwdRate = 1000;
	  int flowDuration = 1000;
	  AdmissionControlResult r =null;
	  if (action.equals ("add"))
	  {
	     fwdRate = rate;
	     FlowInfo uniFI = new FlowInfo (new IPAddress (srcIP,smask),
					 sPort, 
					 new IPAddress (destIP,dmask), 
					 dPort,
					 protocol, 
					 new BandwidthRate (fwdRate,10), 
					 new BandwidthRate (0,0), 
					 false,
					 flowDuration);
		  
		  QOSRequired qosr = findQoS (qos);
		  SchedulingAction schAction 
		    = SchedulingAction.reserve_and_commit; //reserve and actiavte the flow
		  StringHolder fID = new StringHolder();
	          IntHolder dscp = new IntHolder ();
		  
		  // Create new flow, flowID is obtained
		   r = adControl.flowRequest (uniFI, qosr, schAction, fID, dscp);
		  if (r == AdmissionControlResult.DECISION_ADMIT)
		    System.out.println (" Client flow request for flowID:"+fID.value
					+" result= flow request admitted with dscp = " +
	                                dscp.value);
		  else
		    System.out.println (" Client flow request for flowID:"+fID.value
		    +" result= flow request denied");
		  
		  }
		  else if (action.equals ("del"))
		  {
		  
		     // Now delete the flow using the same flowID
		     r = adControl.flowDelete (flowID);
		     if (r == AdmissionControlResult.DECISION_DELETE)
		       System.out.println (" Client flow delete request for flowID:"+
	                                   flowID + 
					   " result= flow deleted");
		     else
		       System.out.println (" Client flow delete request for flowID:" +
	                                   flowID +
		                        " result= no flow ID");
		  }
		  else if (action.equals ("mod"))
		  {
		  
		     // Now delete the flow using the same flowID
		     r = adControl.unidirectionalFlowUpdate(flowID, 0, rate);
		     if (r == AdmissionControlResult.DECISION_ADMIT)
		       System.out.println (" Client flow modify request for flowID: "+
	                                   flowID + " new rate: " + rate +  
					   " result= flow modified");
		     else
		       System.out.println (" Client flow delete request for flowID:" +
	                                   flowID +" new rate: " + rate +  
		                        " result= modify denied" );
	          }
		  else if (action.equals ("policy"))
		  {
	  
		      r = null;
		      if (policy.equals ("1"))
		      {
		         r  = adControl.changeBandwidthAllocationPolicyTo (BandwidthPolicy.policy1);
		      }
		      else if (policy.equals ("2"))
		      {
		         r  = adControl.changeBandwidthAllocationPolicyTo (BandwidthPolicy.policy2);
		      }
		      if (r != null)
		      {
		      if (r == AdmissionControlResult.DECISION_ADMIT)
		    System.out.println (" Changed to bandwidth allocation policy:"+
					new Integer (BandwidthPolicy.policy2.value ()).toString());
		  else
		    System.out.println (" Failure in changing to bandwidth allocation policy:"+
					new Integer (BandwidthPolicy.policy2.value ()).toString());
		      }
		   }
		   else if (action.equals ("query"))
	           {
	              testBWQuery (adControl);
	           }
		   else if (action.equals ("standby"))
	           {
		       r = adControl.standby ();
		       System.out.println ("Released all the flows. Bandwidth Broker is now in standby mode.");
	           }
		   else if (action.equals ("resume"))
	           {
		       r = adControl.resume ();
		       System.out.println ("Bandwidth Broker operations have been resumed.");
	           }
		  

	           
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
	         bsh.value [0].srcSubnet = new IPAddress ("192.168.81.100", "255.255.255.0");
	         bsh.value [0].destSubnet = new IPAddress ("192.168.81.101", "255.255.255.0");
	         bsh.value [0].bwPerClassSeq = new BWPerQoS [1];
	         bsh.value [0].bwPerClassSeq [0] = new BWPerQoS (new BandwidthRate (5000, 0),
	                                                      QOSRequired.highReliability);
	                                                      //QOSRequired.raw);

	         bsh.value [1]  = new BWBetnSubnets ();
	         bsh.value [1].srcSubnet = new IPAddress ("192.168.81.101", "255.255.255.0");
	         bsh.value [1].destSubnet = new IPAddress ("192.168.81.101", "255.255.255.0");
	         bsh.value [1].bwPerClassSeq = new BWPerQoS [1];
	         bsh.value [1].bwPerClassSeq [0] = new BWPerQoS (new BandwidthRate (5000, 0),
	                                                      QOSRequired.highReliability);
	                                                      //QOSRequired.raw);


	         try
	         {
	  	  AdmissionControlResult r =  
	            adControl.subnetbandwidthQuery (bsh, BWQueryType.provisioned);

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

