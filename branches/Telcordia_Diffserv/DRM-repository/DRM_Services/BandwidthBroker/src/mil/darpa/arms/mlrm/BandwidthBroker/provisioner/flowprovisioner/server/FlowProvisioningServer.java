//$Id: FlowProvisioningServer.java
//The DARPA ARMS Program
// This software/interface/output
// is governed by Government Purpose Rights
// Contract No: NBCHC030132
// Telcordia Technologies, Inc.
// 1 Technology Drive
// Piscataway, NJ 08854-4157 

// FlowProvisioningServer.java
// Author: Ravi Vaidyanathan <vravi@research.telcordia.com>
// Initial draft: 11/26/03
// Revisions for new IDLs: 1/14/04
// Revisions for Java Cisco wrapper: 1/19/04
package mil.darpa.arms.mlrm.BandwidthBroker.provisioner.flowprovisioner.server;

import mil.darpa.arms.mlrm.BandwidthBroker.*;
import mil.darpa.arms.mlrm.BandwidthBroker.FlowProvisionerPackage.*;
import mil.darpa.arms.mlrm.BandwidthBroker.CommonDef.*;
import org.omg.CosNaming.*;
import org.omg.CosNaming.NamingContextPackage.*;
import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import org.omg.PortableServer.POA;
import com.telcordia.jIOS.*;
import java.util.*;
import org.apache.log4j.PropertyConfigurator;


// Author: Ravi Vaidyanathan

// Main server class that initializes various functions
public class FlowProvisioningServer {

    static String log4jProp="";
    static String routerAuthProp="";
    static String nameString="";
    static boolean loopbackFlag=false;
    static boolean faultActivatedFlag=false;
  public static void main(String args[]) {
    try{
	if( args.length < 1 ) 
	{
	  System.out.println( "Usage: java "+
			      "mil.darpa.arms.mlrm.BandwidthBroker.provisioner.flowprovisioner.server.FlowProvisioningServer "+
			      " p= <log4j property file> "+
			      "l=yes/no (yes for loopback)"+ 
			      "r= <router authentication file>" +
			      "f= <flowprovisioner name>" +
			      "w= <failwhen=0 for no fault else 1>" +
			      "a= <failAfterX=0 for no fault else #request>"+
			      "-ORBInitRef NameService=corbaloc::$ONS_HOST:$ONS_PORT/NameService");
	  System.exit( 1 );
        }
      parseArgs (args);
      System.out.println ("The arguments used for Flow Provisioner are: log4j file="+log4jProp+
			  "  loopback="+loopbackFlag+"  routerAuth file="+routerAuthProp+ 
			  "  flowprovisioner name="+nameString);

      PropertyConfigurator.configure(log4jProp); // use ioslog.prop

      // create and initialize the ORB
      ORB orb = ORB.init(args, null);

      // get reference to rootpoa & activate the POAManager
      POA rootpoa = POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
      rootpoa.the_POAManager().activate();

      // create servant and register it with the ORB
      FlowProvisioningImpl flowP = new FlowProvisioningImpl();
      flowP.setORB(orb); 

      // get object reference from the servant
      org.omg.CORBA.Object ref = rootpoa.servant_to_reference(flowP);
      FlowProvisioner fref = FlowProvisionerHelper.narrow(ref);
	  
      // get the root naming context
      org.omg.CORBA.Object objRef =
          orb.resolve_initial_references("NameService");
      // Use NamingContextExt which is part of the Interoperable
      // Naming Service (INS) specification.
      NamingContextExt ncRef = NamingContextExtHelper.narrow(objRef);

      String flowProvisionerNameArg=new String ("FlowProvisioning");
      // String denoting the name the Flow Provisioning
      // server will use to bind to the Naming Service.  This is required so that each
      // FlowProvisioner from a certain pool could still use the same
      // centralized Naming Service all components talk to
      if (nameString.compareTo("") !=0)
	  flowProvisionerNameArg=nameString; //e.g.pool1fP 

      // bind the Object Reference in Naming
      String name = flowProvisionerNameArg;
      NameComponent path[] = ncRef.to_name( name );
      ncRef.rebind(path, fref);
      
      // do some init functions
      flowP.initAuth(routerAuthProp);

      // check for loopback
      //for (int i=0; i<args.length; i++) 
      //{
      if (loopbackFlag)
	  flowP.setLoopback();

      System.out.println("FlowProvisioningServer ready and waiting ...");

      // wait for invocations from clients
      orb.run();
    } 
	
    catch (Exception e) {
       System.err.println("ERROR: " + e);
       e.printStackTrace(System.out);
     }
	  
     System.out.println("FlowProvisioningServer Exiting ...");
	
  }

   static void parseArg (String arg)
   {
      if (arg.startsWith ("p="))
         log4jProp = arg.substring (2);
      else if (arg.startsWith ("r="))
         routerAuthProp = arg.substring (2);
      else if (arg.startsWith ("f="))
         nameString = arg.substring (2);
      else if (arg.startsWith ("l="))
      {
          if (arg.substring (2).startsWith("yes"))
	     loopbackFlag = true;
          else
             loopbackFlag = false;
      }
      else if (arg.startsWith ("e="))
      {
          if (arg.substring (2).startsWith("yes"))
	     faultActivatedFlag = true;
          else
             faultActivatedFlag = false;
      }
      else if (arg.startsWith ("w="))
      {
          try{
	      FlowProvisioningImpl.failWhenFP
		  = Integer.parseInt(arg.substring (2));
	  }catch (Exception e)
	      {
		  System.out.println ("Error reading failWhenFP value for fault testing, resorting to default of 0");
		  FlowProvisioningImpl.failWhenFP=0;
	      }
      }
      else if (arg.startsWith ("a="))
      {
          try{
	      FlowProvisioningImpl.failAfterXFP
		  = Integer.parseInt(arg.substring (2));
	       }catch (Exception e)
	      {
		  System.out.println ("Error reading failAfterXFP value for fault testing, resorting to default of 0");
		  FlowProvisioningImpl.failWhenFP=0;
	      }
      }
      
      
   }
   static void parseArgs (String args [])
   {
       for (int i = 0; i < args.length; i++)
       {
	  parseArg (args [i]);
       }
    }

} // end class FlowProvisioningServer






