// ============================================================================
//
// = PACKAGE
//    netsvcs.Time
// 
// = FILENAME
//    Server.java
//
// = AUTHOR
//    Prashant Jain
// 
// ============================================================================
package netsvcs.Time;

// Test driver for the time service server
public class Server
{
  public static void main (String [] args)
    {
      TSServerAcceptor server = new TSServerAcceptor ();
      server.init (args);
    }
}
