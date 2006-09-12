//$Id: BBTester.java,
// This software is governed by Government Purpose Rights
// Contract No: NBCHC030132
// Telcordia Technologies, Inc.
// 1 Technology Drive
// Piscataway, NJ 08854-4157

// BBClient.java

// Author : Kirthika Parmeswaran 
// This class tests flow requests and deletions 

package mil.darpa.arms.mlrm.BandwidthBroker.bb.clients;

import java.io.*;
import org.omg.CORBA.*;
import mil.darpa.arms.mlrm.BandwidthBroker.*;
import mil.darpa.arms.mlrm.BandwidthBroker.AdmissionControlPackage.*;
import mil.darpa.arms.mlrm.BandwidthBroker.CommonDef.*;
import org.omg.CosNaming.*;
import org.omg.CosNaming.NamingContextPackage.*;

public class BBTester 
{
    public static void main( String args[] ) 
    {
      if( args.length < 5 ) 
	{
	  System.out.println( "Usage: java "+
			      " com.telcordia.research.BandwidthBroker.BBClient"+
			      " -ORBInitialHost <name service host> "+
			      "-ORBInitialPort <name service port> "+
			      "<operationType (1=reserve&commitrequest, 2=reserve-request, "+
			      "3=commit-request, 4=deleteflow)>"+
			      " flowID (0 for reserve requests)"+
			      " fwdRate (bps)"+
			      " serviceClass (1=highPriority, 2=mmVoice, "+
			      "3=highReliability, 4=bestEffort)" );
	  System.exit( 1 );
        }
      // initialize the ORB.
      // String nargs = new String[2];
      //nargs[0]=args[0];
      
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
	  
	  // Get the parameters
	  String operationType = args[4];
	  String flowIdentifier = args[5];
	  if (operationType == null || flowIdentifier == null)
	      {
		  System.out.println ("Error in input!");
		  System.exit (1);
	      }

	  String fr = null;
	  String sc = null;
	  if (args.length == 8)
	    {
	      fr = args[6];
	      sc = args[7];
	    }
	  int frate = 0;
	  if (fr != null)
	    frate = Integer.parseInt (fr);
	  int sclass = 0;
	  if ( sc != null)
	    sclass = Integer.parseInt (sc);

	  BBTester bbT = new BBTester ();

	  switch (Integer.parseInt (operationType))
	      {
	      case 1: // reserve & commit flow request
		  {
		      System.out.println ("flowID ="+ bbT.createNewFlow (adControl, frate, sclass, 1));
		      break;
		  }
	      case 2: // reserve new flow
		  {
		      System.out.println ("flowID ="+  bbT.createNewFlow (adControl,frate, sclass, 2));
		      break;
		  }
	      case 3: // commit flow
		  {
		      if ( bbT.commitFlow (adControl,flowIdentifier))
			  System.out.println ("Success");
		      else
			  System.out.println ("Failure");
		      break;
		  }
	      case 4: // delete flow
		  {
		      if ( bbT.deleteFlow (adControl,flowIdentifier))
			  System.out.println ("Success");
		      else
			  System.out.println ("Failure");
		      break;
		  }
	      default:
		  {
		      System.out.println ("Error in input operationType");
		      System.exit (1);
		  }
	      }
	
        }
      catch( Exception ex ) 
	{
            System.err.println( ex );
	    ex.printStackTrace();
        }
    }

    public String createNewFlow (AdmissionControl adControl,int fRate, 
				 int sclass, int reserveType)
      throws AdmissionControlException
    {
	// invoke the operation and print the result
	  String sIP = "172.16.14.5";
	  String smask="255.255.255.0";
	  String dIP = "172.16.13.2";
	  String dmask="255.255.255.0";
	  short pn1 = 1200;
	  short pn2 = 1600;
	  PortInfo sPort = new PortInfo (pn1,pn1); //low,high same
	  PortInfo dPort = new PortInfo (pn2,pn2);
	  BBProtocolType protocol = BBProtocolType.tcp; //tcp
	  int fwdRate = fRate;
	  int flowDuration = 1000;
	  FlowInfo uniFI = new FlowInfo (new IPAddress (sIP,smask),
					 sPort, 
					 new IPAddress (dIP,dmask), 
					 dPort,
					 protocol, 
					 new BandwidthRate (fwdRate,10), 
					 new BandwidthRate (0,0), 
					 false,
					 flowDuration);

	  QOSRequired qos = QOSRequired.highReliability;
	  switch (sclass)
	      {
	      case 1: // high priority
		  {
		      qos = QOSRequired.highPriority;
		      break;
		  }
	      case 2: // high reliability
		  {
		      qos = QOSRequired.highReliability;
		      break;
		  }
	      case 3: // high thruput
		  {
		      qos = QOSRequired.mmVoice;
		      break;
		  }
	      default:
	     
		  
	      }
	  
	  
	  SchedulingAction schAction = SchedulingAction.reserve_and_commit;
	  switch (reserveType)
	      {
	      case 1: // resereve and commit
		  {
		      schAction = SchedulingAction.reserve_and_commit;
		      break;
		  }
	      default:
	      case 2: // resereve 
		  {
		      schAction = SchedulingAction.reserve;
		      break;
		  }
	      }
	  StringHolder fID = new StringHolder();
          IntHolder dscp = new IntHolder ();
	  
	  AdmissionControlResult r =null;
	  // Create new flow, flowID is obtained
	   r = adControl.flowRequest (uniFI, qos, schAction, fID, dscp);
	   if (r == AdmissionControlResult.DECISION_ADMIT)
	       System.out.println (" Client flow request for flowID:"+fID.value
				   +" result= flow request admitted with dscp = " + dscp.value);
	   else
	       System.out.println (" Client flow request for flowID:"+fID.value
	    +" result= flow request denied");

	   return fID.value;
	  
    }

    public boolean deleteFlow (AdmissionControl adControl,String flowID)
      throws AdmissionControlException
    {
	
	  AdmissionControlResult r = adControl.flowDelete (flowID);
	  if (r == AdmissionControlResult.DECISION_DELETE)
	    {
		System.out.println (" Client flow delete request for flowID:"+flowID
				+" result= flow deleted");
		return true;
	    }
	  else
	      {
		  System.out.println (" Client flow delete request for flowID:"+flowID
				      +" result= no flow ID");
		  return false;
	      }
    }

    public boolean commitFlow (AdmissionControl adControl,String flowID)
      throws AdmissionControlException
    {
	 AdmissionControlResult r  = adControl.flowRequestCommit (flowID);
	      if (r == AdmissionControlResult.DECISION_ADMIT)
		  {
		      System.out.println (" Client flow request for flowID- commit:"+flowID
				      +" result= flow request committed");
		      return true;
		  }
	      else
		  {
		      System.out.println (" Client flow request for flowID- commit:"+flowID
					  +" result= no flow ID");
		      return false;
		  }
    }
}
