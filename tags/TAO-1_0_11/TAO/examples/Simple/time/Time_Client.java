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

import org.omg.CosNaming.*;
import org.omg.CORBA.*;

public class Time_Client 
{
  public static void main (String args[])
  {
    try
      {
        // Create and initialize the ORB.
        ORB orb = ORB.init (args, null);

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
        Time timeRef =
          TimeHelper.narrow (ncRef.resolve (path));

        // Call the Time server object and print results.
        int time = timeRef.time ();

        java.util.Date date =
          new java.util.Date (((long) time) * 1000);

        System.out.println ("string time is " + date);
      } 
    catch (Exception e) 
      {
        System.out.println ("ERROR : " + e) ;
        e.printStackTrace (System.out);
      }
  }
}
