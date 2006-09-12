//$Id$
// This software is governed by Government Purpose Rights
// Contract No: NBCHC030132
// Telcordia Technologies, Inc.
// 1 Technology Drive
// Piscataway, NJ 08854-4157

// NetworkMonitorServer.java

// Author : Kirthika Parmeswaran 
// This class is the CORBA server version of the  BandwidthBroker.NetworkMonitor

package mil.darpa.arms.mlrm.BandwidthBroker.networkmonitor.server;

import java.io.*;

import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import mil.darpa.arms.mlrm.BandwidthBroker.*;
import mil.darpa.arms.mlrm.BandwidthBroker.NetworkMonitorPackage.*;
import mil.darpa.arms.mlrm.BandwidthBroker.ProbeControlPackage.*;
import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;
import org.omg.CosNaming.*;
import org.omg.CosNaming.NamingContextPackage.*;
import mil.darpa.arms.mlrm.BandwidthBroker.bb.server.BBServer;
import mil.darpa.arms.mlrm.BandwidthBroker.common.Util;
import mil.darpa.arms.mlrm.BandwidthBroker.networkmonitor.server.db.*;

public class NetworkMonitorServer 
{
    public static Logger nmlogger = null; 

    private static ORB orb;
    private static POA poa;
    
    // This section of code is here only to allow NetworkMonitor to maintain a reference to the 
    //     log file for those cases where NetworkMonitorServer is NOT the 
    //     main entry point  
    // Added - MRC

    static
    {
    	System.err.println("In Networkmonitorserver static");
        java.util.Properties serverProps = new java.util.Properties();
        try {
	    serverProps.load(NetworkMonitorServer.class.getResourceAsStream("/nmserverlog.prop"));
	    Util.ensureLogDirsExist(serverProps);
	    PropertyConfigurator.configure(serverProps);

	    // Create a logger for this type of component
	    //     All subsequent calls with this class name will get this instance
	    nmlogger = Logger.getLogger(NetworkMonitorServer.class.getName());
	    System.err.println("NM Logger set");
	} catch (Exception e) {
	    System.err.println("Exception trying to read nmserverlog props: "+
			       "(this is expected in non-CCM mode");
	}
    	System.err.println("In Loaded NM Server props");
    }
    // end add - MRC


    public static void main(String[] args) 
    {
        if( args.length < 3 ) 
	{
            System.out.println(
                "Usage: java com.telcordia.research.BandwidthBroker.NetworkMonitorServer"+
		" <log4j_prop> -ORBInitialHost <name service host> "+
		"-ORBInitialPort <name service port>");
            System.exit( 1 );
        }

        try 
        {        
	  
	    PropertyConfigurator.configure(args[0]); // use nmserverlog.prop
	    nmlogger = Logger.getLogger(NetworkMonitorServer.class.getName());
	    
            //initialize ORB
	    orb = ORB.init( args, null );

	    //initialize POA
	    poa = 
                POAHelper.narrow( orb.resolve_initial_references( "RootPOA" ));

	    poa.the_POAManager().activate();

	    // Configuration file name is decided before hand to reduce complexity
	    String nmConfigFile = new String ("."+ 
                                 System.getProperty ("file.separator")+ "cfg"+ 
                                 System.getProperty ("file.separator") + 
                                 "BandwidthBroker" + 
                                 System.getProperty ("file.separator") + 
                                 "nmconfig.prop");
	    
	    NetworkMonitorImpl nmImpl = new NetworkMonitorImpl (nmConfigFile, args);
	    
            // create the object reference
            org.omg.CORBA.Object obj = 
                poa.servant_to_reference(nmImpl );

	    //NamingContextExt nc =
	    //NamingContextExtHelper.narrow(orb.resolve_initial_references("NameService"));
	    
            NameComponent theName [] = {
              new NameComponent("mil", ""),
              new NameComponent("darpa", ""),
              new NameComponent("arms", ""),
              new NameComponent("mlrm", ""),
              new NameComponent("BandwidthBroker", ""),
              new NameComponent("NetworkMonitor", "")

            };

            nsBind (theName, obj);
	    nmlogger.info ("Registered NetworkMonitorServer IOR with Naming");
		
	    System.out.println ("NetworkMonitorServer is ready");
	    nmlogger.info ("NetworkMonitorServer is ready");

            System.out.println(orb.object_to_string(obj));

            // wait for requests
	    orb.run();
        }
        catch( Exception e ) 
        {
            System.out.println( e );
	    e.printStackTrace ();
	    nmlogger.error ("Exception"+e);
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

             
