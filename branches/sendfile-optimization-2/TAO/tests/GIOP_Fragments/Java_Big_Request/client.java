// $Id$

// If this server is compiled and run with the JDK ORB, it will
// fragment the GIOP Messages sent when more_data() is called.

import java.io.FileReader;
import java.io.BufferedReader;
import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import Test.Payload_Receiver;
import Test.Payload_ReceiverHelper;

public class client
{
  public static void main (String args[]) {
    try {
      ORB orb = ORB.init (args, null);

      // Get the object reference
      BufferedReader reader = new BufferedReader (
                                        new FileReader ("server.ior"));
      StringBuffer ior = new StringBuffer();
      String line = null;
      while ((line = reader.readLine()) != null) {
        ior.append(line);
      }

      org.omg.CORBA.Object obj = orb.string_to_object (ior.toString ());
      Payload_Receiver receiver = Payload_ReceiverHelper.narrow (obj);

      // Set up the payload
      byte [] seq = new byte [1000000];
      for (int i = 0; i < seq.length; ++i)
        seq [i] = 'A';

      // Invoke the method on the server
      for (int i = 0; i < 20; ++i)
        receiver.more_data (seq);

      receiver.shutdown ();
    }
    catch (Exception e) {
      System.err.println ("ERROR: " + e);
      e.printStackTrace (System.out);
    }
  }
}
