//$Id: BBServer.java
// This software is governed by Government Purpose Rights
// Contract No: NBCHC030132
// Telcordia Technologies, Inc.
// 1 Telcordia Drive
// Piscataway, NJ 08854-4157

// BBServer.java

// Author : Kirthika Parmeswaran 
// This class is the CORBA server version of the  BandwidthBroker

package mil.darpa.arms.mlrm.BandwidthBroker.bb.server;

import java.io.*;
import java.util.*;

import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import mil.darpa.arms.mlrm.BandwidthBroker.*;
import mil.darpa.arms.mlrm.BandwidthBroker.AdmissionControlPackage.*;
import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;
import org.omg.CosNaming.*;
import org.omg.CosNaming.NamingContextPackage.*;
import mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db.*;
import mil.darpa.arms.mlrm.BandwidthBroker.common.*;
import mil.darpa.arms.mlrm.BandwidthBroker.provisioner.flowprovisioner.server.*;

public class BBServer 
{
    // This section of code is here only to allow BBServer to maintain a reference to the 
    //     log file for the bandwidth broker for those cases where BBServer is NOT the 
    //     main entry point  
    // Added - MRC

    public static Logger bblogger; 

    static
    {
          // Seems not to be used anymore?
          // String bbServerLogFile = new String (
          //                               "." + 
          //                               System.getProperty ("file.separator") + 
	    //			             "proto" +  
          //                               System.getProperty ("file.separator") +
	    // 			             "cfg" + 
          //                               System.getProperty ("file.separator") +
	    //				       "bbserverlog.prop" );

//        PropertyConfigurator.configure(getClass().getResourceAsStream("/bbserverlog.prop"));
    
        java.util.Properties serverProps = new java.util.Properties();
        try {
	    serverProps.load(BBServer.class.getResourceAsStream("/bbserverlog.prop"));
	    Util.ensureLogDirsExist(serverProps);
	    PropertyConfigurator.configure(serverProps);

	    // Create a logger for this type of component
	    //     All subsequent calls with this class name will get this instance
	    bblogger = Logger.getLogger(BBServer.class.getName());

	} catch (Exception e) {
	    System.err.println("Exception trying to read bbserverlog props: "+
			       "(this is expected in non-CCM mode");
	}
    }
    // end add - MRC

    static public void readConfig (String bbConfigFile)
    {
	PropertyResourceBundle properties=null;
        String p = new String ();
	 try
	    {
		try {
		    properties = new PropertyResourceBundle(p.getClass ().getResourceAsStream(bbConfigFile));
		} catch (Exception x) {
		    System.err.println("Exception trying to read bbconfig props: "+
				       "(this is expected in non-CCM mode");
		    // assume we are in non-component mode and got args
		    // from command line
		    properties = new PropertyResourceBundle(new FileInputStream(bbConfigFile));
		    BBServer.bblogger.info ("BB Configuration Properties from :"+bbConfigFile);
		}
		System.err.println("Properties is" + properties.toString());
		corbaName =  properties.getString("CORBAName");
		BBDriver.failAfterX = Integer.parseInt (
				properties.getString("failAfterX"));
		BBDriver.failWhen = Integer.parseInt (
				properties.getString("failWhen"));
		System.out.println ("Fault is to be simulated after "+BBDriver.failAfterX+
				    "requests and is of type="+BBDriver.failWhen);
		corbaName =  properties.getString("CORBAName");

		
	    }
	 catch( Exception e )
	     {
		    System.out.println( "Unable to read  file: "+bbConfigFile+ " hence using system defaults");
		    BBServer.bblogger.warn ( "Unable to read  file: "+bbConfigFile+ " hence using system defaults");
                    corbaName = "AdmissionControl";
	     }
    }

    private static ORB orb;
    private static POA poa;
    private static String corbaName;

    public static void main(String[] args) 
    {
        if( args.length < 3 ) 
	{
            System.out.println(
                "Usage: java com.telcordia.research.BandwidthBroker.BBServer"+
		" <log4j_prop> -ORBInitialHost <name service host> "+
		"-ORBInitialPort <name service port>");
            System.exit( 1 );
        }

        try 
        {        
	  
            System.out.println ("Using " + args [0] + "as log.prop file.");
	    PropertyConfigurator.configure(args[0]); // use bbserverlog.prop
	    bblogger = Logger.getLogger(BBServer.class.getName());
	    
            //initialize ORB
	    orb = ORB.init( args, null );

	    // BBN code for persistent IORs
            //initialize POA
            POA rootPOA =
                POAHelper.narrow( orb.resolve_initial_references( "RootPOA" ));

            //poa.the_POAManager().activate();

            /* create policies */
            org.omg.CORBA.Policy [] policies = new org.omg.CORBA.Policy[2];
            policies[0] =
              rootPOA.create_id_assignment_policy(IdAssignmentPolicyValue.USER_ID);
            policies[1] =
              rootPOA.create_lifespan_policy(LifespanPolicyValue.PERSISTENT);

            /* create child POA */
            poa = rootPOA.create_POA("BBServerPOA", rootPOA.the_POAManager(), policies);

            /* activate child POA */
            poa.the_POAManager().activate();
            // BBN code end



	    
	    // removed Telcordia code
	    // initialize POA
	    //poa = 
            //    POAHelper.narrow( orb.resolve_initial_references( "RootPOA" ));

	    //poa.the_POAManager().activate();
            // Telcordia code end

            // create an AdmissionControl object
	    // Configuration file name is decided before hand to reduce complexity
	    String bbConfigFile = new String ("cfg"+ 
                                 System.getProperty ("file.separator") + 
                                 "BandwidthBroker" + 
                                 System.getProperty ("file.separator") + 
                                 "bbconfig.prop");
	    
            readConfig (bbConfigFile);
	    System.out.println ("Registering as " + corbaName + "\n");
	    String sqlTraceFile = new String ("data"+ 
                                 System.getProperty ("file.separator") + 
                                 "logs" + 
                                 System.getProperty ("file.separator") + 
                                 "bb.sqlTrace");
            DbUtil.sqlTraceFile = sqlTraceFile;

	    AdmissionControlImpl adControlImpl = new AdmissionControlImpl (bbConfigFile, args);
	    
	     poa.activate_object_with_id ("AdmissionControl".getBytes(), adControlImpl);
	     
            // create the object reference
            org.omg.CORBA.Object obj = 
                poa.servant_to_reference( adControlImpl );

	    //NamingContextExt nc =
	    //NamingContextExtHelper.narrow(orb.resolve_initial_references("NameService"));
	    
            NameComponent theName [] = {
              new NameComponent("mil", ""),
              new NameComponent("darpa", ""),
              new NameComponent("arms", ""),
              new NameComponent("mlrm", ""),
              new NameComponent("BandwidthBroker", ""),
              new NameComponent(corbaName, "")

            };

	    //nc.bind (nc.to_name("AdmissionControl"), obj );
	    //nc.bind (theName, obj );
            nsBind (theName, obj);
	    bblogger.info ("Registered BBServer IOR with Naming");
		
	    System.out.println ("BBServer is ready and registered as " + corbaName);
	    bblogger.info ("BbServer is ready");

            System.out.println(orb.object_to_string(obj));

            // wait for requests
	    orb.run();
        }
        catch( Exception e ) 
        {
            System.out.println( e );
	    e.printStackTrace ();
	    bblogger.error ("Exception"+e);
        }
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

    public static void nsBind(NameComponent[] name,
                              Servant servant)
    {
        try {
            org.omg.CORBA.Object ref = poa.servant_to_reference(servant);
            nsBind(name, ref);
        } 
        catch (Exception ex) {
        }
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


