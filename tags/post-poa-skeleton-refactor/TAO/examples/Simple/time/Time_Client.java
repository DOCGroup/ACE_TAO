// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/examples/Simple/time
//
// = FILENAME
//    Time_Client.java
//
// = DESCRIPTION
//    This class implements a Java client interface to the Time example.
//
// = AUTHOR
//   Martin Krumpolec <krumpo@pobox.sk>
//
// ============================================================================

import JACE.Misc.GetOpt;
import org.omg.CosNaming.*;
import org.omg.CORBA.*;
import java.io.*;

public class Time_Client
{
    private ORB orb = null;
    private org.omg.CORBA.Object timeRef = null;
    private boolean shutdown = false;
    private boolean use_naming_service = false;
    private String ior_file = null;
    private String ior = null;

    Time_Client (String args [])
    {
        orb = ORB.init (args, null);
        parse_args (args);
    }

    private void parse_args (String args[])
    {
        GetOpt getopt = new GetOpt (args, "k:f:xn");

        int ch = -1;

        while ((ch = getopt.next ()) != -1)
            {
                switch (ch)
                    {
                    case 'k':
                        ior = getopt.opt_arg () ();
                        break;
                    case 'f':
                        ior_file = getopt.opt_arg () ();
                        try
                            {
                                java.io.FileInputStream file =
                                    new FileInputStream(ior_file);
                                java.io.BufferedReader myInput =
                                    new BufferedReader
                                        (new InputStreamReader (file));
                                ior = myInput.readLine();
                            }
                        catch (Exception e)
                            {
                                System.out.println ("Error: " + e);
                            }
                        break;
                    case 'x':
                        shutdown = true;
                        break;
                    case 'n':
                        use_naming_service = true;
                        break;
                    default:
                        break;
                    }
            }

        return;
    }

    private Time get_time_object ()
    {
        if (ior != null)
            {
                org.omg.CORBA.Object objref = orb.string_to_object (ior);
                return TimeHelper.narrow (objref);
            }
        else
            {
                if (use_naming_service)
                    {
                        // Use multicast to obtain NS handle.
                        NS_Resolve ns_resolve =
                            new NS_Resolve (null);
                        org.omg.CORBA.Object objRef =
                            ns_resolve.resolve_name_service (orb);

                        // Get the root naming context.
                        NamingContext ncRef =
                            NamingContextHelper.narrow (objRef);
                        System.out.println ("got initial NS reference");

                        // Resolve the Object Reference in Naming.
                        NameComponent nc =
                            new NameComponent ("Time", "");
                        NameComponent path[] = {nc};
                        try
                            {
                                return TimeHelper.narrow (ncRef.resolve (path));
                            }
                        catch (Exception e)
                            {
                                System.out.println ("Error: " + e);
                            }
                    }
            }
        return null;
    }

  public static void main (String args[])
  {
    try
      {
        Time_Client client = new Time_Client (args);
        Time timeRef = client.get_time_object ();

        if (timeRef == null)
            {
                System.out.println ("Unable to resolve Time.");
                System.exit (-1);
            }

        // Call the Time server object and print results.
        int time = timeRef.current_time ();

        java.util.Date date =
          new java.util.Date (((long) time) * 1000);

        System.out.println ("string time is " + date);

        if (client.shutdown)
            {
                timeRef.shutdown ();
                System.out.println ("Shutting down server ...");
            }
      }
    catch (Exception e)
      {
        System.out.println ("ERROR : " + e) ;
        e.printStackTrace (System.out);
      }
  }
}
