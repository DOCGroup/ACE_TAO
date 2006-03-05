package interop.wchar;

import java.io.*;

import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import interop.*;

/**
 * Server.java
 *
 * Java implemention of the interoperability tests for wchars.
 *
 * @author Phil Mesnier
 * @version $Id$
 */

public class Server
{
    public static void main(String[] args)
    {
        String iorfile = "IOR";
        boolean verbose = false;
        for (int i = 0; i < args.length; i++)
        {
            if (args[i].equals ("-o"))
                iorfile = args[++i];
            else if (args[i].equals ("-v"))
                verbose = true;
            else
            {
                System.out.println( "Usage: jaco interop.wchar.Server " +
                                    "-o <ior_file> " +
                                    "-v ");
                System.exit( 1 );
            }
        }

        try
        {
            //init ORB
	    ORB orb = ORB.init( args, null );

	    //init POA
	    POA poa =
                POAHelper.narrow( orb.resolve_initial_references( "RootPOA" ));

	    poa.the_POAManager().activate();

            // create a GoodDay object
            WChar_PasserImpl servant = new WChar_PasserImpl(orb, verbose);

            // create the object reference
            org.omg.CORBA.Object obj =
                poa.servant_to_reference( servant );

            PrintWriter pw =
                new PrintWriter( new FileWriter( iorfile ));

            // print stringified object reference to file
            pw.println( orb.object_to_string( obj ));

            pw.flush();
            pw.close();

            // wait for requests
	    orb.run();
        }
        catch( Exception e )
        {
            System.out.println( e );
            e.printStackTrace();
        }
    }
}
