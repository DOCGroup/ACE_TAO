package netsvcs.Time;
/*************************************************
 *
 * = PACKAGE
 *    netsvcs.Time
 *
 * = FILENAME
 *    TSRequestHandler.java
 *
 *@author Prashant Jain, Everett Anderson
 *
 *************************************************/
import java.io.*;
import java.util.*;
import JACE.OS.*;
import JACE.Connection.*;
import JACE.Reactor.*;

/**
 * <hr>
 * <p><h2>SYNOPSIS</h2>
 *
 * <blockquote>Handles giving the client the average difference between
 * the local time and the server times.</blockquote>
 *
 * <p><h2>DESCRIPTION</h2>
 *
 * <blockquote>Whenever the RequestHandler receives input to the port, it
 * sends the current delta (average difference time) in return as a string.
 * Instances of this class are created by TSRequestAcceptor.</blockquote>
 */
public class TSRequestHandler extends SvcHandler
{
  /**
   * Constructor.  Takes in a reference to the Clerk Processor
   * so it can call TSClerkProcessor getDelta().
   */
  public TSRequestHandler (TSClerkProcessor processor)
  {
    this.processor_ = processor;
  }
  
  /**
   *
   * Run this in a separate thread.
   */
  public int open (Object obj)
  {
    new Thread (this).start ();
    return 0;
  }

  /**
   *
   * Called when the thread starts.  This is the main code -- whenever
   * input comes to the socket, it sends out the current delta time
   * as a string.
   */
  public void run ()
  {
    int msgLen;
    try
      {
	while (true)
	  {
	    StringBuffer msg = new StringBuffer ();
	    
	    msgLen = this.peer ().recv (msg);

	    if (msgLen < 0)
	      break;
	    else {
	      
	      // No matter what was sent in, send the average difference back

	      String msgOut = new String("" + this.processor_.getDelta() + '\n');
	      this.peer ().send (msgOut);

	    }
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
	    ACE.ERROR (e);
	  }
      }
  }
  

  // Reference to the Clerk Processor to call getDelta()
  TSClerkProcessor processor_;
}
