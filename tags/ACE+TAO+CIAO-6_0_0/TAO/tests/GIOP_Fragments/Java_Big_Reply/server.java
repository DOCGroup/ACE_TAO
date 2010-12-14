// $Id$

// If this server is compiled and run with the JDK ORB, it will
// fragment the GIOP Messages sent when get_big_reply() is called.

import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import Test.Big_ReplyPOA;

class Big_ReplyImpl extends Big_ReplyPOA
{
  private org.omg.CORBA.ORB orb_;

  public byte[] get_big_reply () {
    byte [] seq = new byte [1000000];
    for (int i = 0; i < seq.length; i++)
      seq [i] = 'A';
    return seq;
  }

  public void setORB (org.omg.CORBA.ORB orb_val) {
    orb_ = orb_val;
  }

  public void ping () {
  }

  public void shutdown () {
    orb_.shutdown (false);
  }
}


public class server
{
  public static void main (String args[]) {
    try {
      ORB orb = ORB.init (args, null);
      POA poa = org.omg.PortableServer.POAHelper.narrow (
                       orb.resolve_initial_references ("RootPOA"));

      Big_ReplyImpl servant = new Big_ReplyImpl ();
      servant.setORB (orb);
      poa.activate_object (servant);

      String filename = new String ("server.ior");
      String ior = orb.object_to_string (servant._this ());
      java.io.FileWriter file = new java.io.FileWriter (filename);
      file.write (ior);
      file.flush ();
      file.close ();

      poa.the_POAManager ().activate ();
      System.out.println ("Ready...");
      orb.run ();
    }
    catch (Exception e) {
      System.err.println ("ERROR: " + e);
      e.printStackTrace (System.out);
    }
  }
}
