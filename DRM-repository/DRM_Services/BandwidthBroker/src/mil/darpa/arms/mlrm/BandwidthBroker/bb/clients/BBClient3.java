// $Id: BBClient3.java
// This software is governed by Government Purpose Rights
// Contract No: NBCHC030132
// Telcordia Technologies, Inc.
// 1 Technology Drive
// Piscataway, NJ 08854-4157

// BBClient3.java

// Author : Kirthika Parmeswaran 
// This class tests the BBServer's Flow evictor. IF a flow is reserved but not commited within a // specified reservation timeout (1 min, by defualt 3min), it is evicted.

package mil.darpa.arms.mlrm.BandwidthBroker.bb.clients;

import java.io.*;
import org.omg.CORBA.*;
import mil.darpa.arms.mlrm.BandwidthBroker.*;
import mil.darpa.arms.mlrm.BandwidthBroker.AdmissionControlPackage.*;
import mil.darpa.arms.mlrm.BandwidthBroker.CommonDef.*;
import org.omg.CosNaming.*;
import org.omg.CosNaming.NamingContextPackage.*;

public class BBClient3
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
	  String sIP = "172.16.13.15";
	  String smask="255.255.255.0";
	  String dIP = "172.16.14.18";
	  String dmask="255.255.255.0";
	  short pn1 = 1200;
	  short pn2 = 1600;
	  PortInfo sPort = new PortInfo (pn1,pn1); //low,high same
	  PortInfo dPort = new PortInfo (pn2,pn2);
	  BBProtocolType protocol = BBProtocolType.tcp; //tcp
	  int fwdRate = 1000;
	  int flowDuration = 1000;
	  FlowInfo fi = new FlowInfo (new IPAddress (sIP,smask),
				      sPort, 
				      new IPAddress (dIP,dmask), 
				      dPort,
				      protocol, 
				      new BandwidthRate (fwdRate,10), 
				      new BandwidthRate (0,0), 
				      false,
				      flowDuration);
	  
	  QOSRequired qos = QOSRequired.highReliability;
	  
	  SchedulingAction schAction 
	    = SchedulingAction.reserve; //reserve
	  StringHolder fID = new StringHolder();
          IntHolder dscp = new IntHolder ();
	  
	  // Create new flow, flowID is obtained
	  AdmissionControlResult r 
	    = adControl.flowRequest (fi, qos, schAction, fID, dscp);
	  if (r == AdmissionControlResult.DECISION_ADMIT)
	    System.out.println (" Client flow request for flowID-reserve:"+fID.value
				+" result= flow request admitted with dscp = " + dscp.value);
	  else
	    System.out.println (" Client flow request for flowID:"+fID.value
				+" result= flow request denied");
	  
	  // sLeep for a bit, so no flow to commit, 
	  // this tests whether teh flow evictor kicks in
	  Thread.sleep (180000);
	  
	  // now commit flow
	  r  = adControl.flowRequestCommit (fID.value);
	  if (r == AdmissionControlResult.DECISION_ADMIT)
	    System.out.println (" Client flow request for flowID- commit:"+fID.value
				+" result= flow request committed");
	  else
	    System.out.println (" Client flow request for flowID- commit:"+fID.value
				+" result= no flow ID");
	  
        }
      catch( Exception ex ) 
	{
	  System.err.println( ex );
	  ex.printStackTrace();
        }
    }
}

