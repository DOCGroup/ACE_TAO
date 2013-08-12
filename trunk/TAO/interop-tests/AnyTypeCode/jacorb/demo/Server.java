package demo;

import Demo.*;
import java.io.*;

public class Server
{
  public static void main( String[] args )
    throws Exception
  {
    org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init (args, null);

    org.omg.PortableServer.POA poa =
      org.omg.PortableServer.POAHelper.narrow (orb.resolve_initial_references ("RootPOA"));

    poa.the_POAManager ().activate ();

    org.omg.CORBA.Object o = poa.servant_to_reference (new ATC_TestImpl ());

    String ior_output_file = "test.ior";

    if ( args.length == 1 )
      {
        ior_output_file = args[0];
      }
    PrintWriter ps =
      new PrintWriter(new FileOutputStream(new File( ior_output_file )));
    ps.println (orb.object_to_string (o));
    ps.close ();

    orb.run ();
  }
}
