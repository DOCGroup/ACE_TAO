// ============================================================================
//
// = PACKAGE
//    netsvcs.Time
// 
// = FILENAME
//    Clerk.java
//
// = AUTHOR
//    Prashant Jain
// 
// ============================================================================
package netsvcs.Time;

// Test driver for the time server clerk
public class Clerk
{
  public static void main (String [] args)
    {
      TSClerkProcessor clerk = new TSClerkProcessor ();
      clerk.init (args);
    }
}
