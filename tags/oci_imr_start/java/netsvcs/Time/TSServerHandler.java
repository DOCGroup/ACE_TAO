/*************************************************
 *
 * = PACKAGE
 *    netsvcs.Time
 *
 * = FILENAME
 *    TS_Server_Handler.java
 *
 *@author Prashant Jain, Everett Anderson
 *
 *************************************************/
package netsvcs.Time;

import java.io.*;
import java.util.*;
import JACE.OS.*;
import JACE.Connection.*;
import JACE.Reactor.*;

/**
 * <hr>
 * <p><h2>DESCRIPTION</h2>
 *     
 * <blockquote>Handles requests from a TSClerkHandler and sends
 * back the current local time.</blockquote>
 *
 * @see netsvcs.Time.TSClerkHandler. netsvcs.Time.TSServerAcceptor
 */

public class TSServerHandler extends SvcHandler
{
  // Constructor
  public TSServerHandler ()
  {
  }
  
  // Start this handler in its own thread
  public int open (Object obj)
  {

    new Thread (this).start ();
    return 0;
  }

  // Wait for messages from the Client and send the current local
  // time back as a string.
  public void run ()
  {
    int msgLen;
    try
      {
	while (true)
	  {
	    // Use a new one each time since recv appends
	    StringBuffer msg = new StringBuffer ();

	    // Get the message from the client (blocks)
	    msgLen = this.peer ().recv (msg);

	    // Just keep waiting if there's a problem
	    if (msgLen <= 0)
	      break;

	    // Is the message for the right thing?
	    if (msg.toString().compareTo ("TIME_UPDATE_REQUEST") != 0) {
	      System.err.println("Unknown message: \"" + msg + '\"');
	      this.peer().send("\n");  // send so other side isn't stuck
	      break;
	    }

	    // Get local time
	    long time = System.currentTimeMillis();

	    // Send as a string
	    this.peer ().send ("" + time);

	    ACE.DEBUG("Time: " + new Date(time));
	  }
      }
    catch (NullPointerException e)
      {
	ACE.ERROR ("Connection reset by peer");
      }
    catch (IOException e)
      {
	ACE.ERROR (e);
      }
    finally
      {
	try 
	  {
	    this.peer ().close ();
	  }
	catch (IOException e)
	  {
	  }
      }
  }
}
