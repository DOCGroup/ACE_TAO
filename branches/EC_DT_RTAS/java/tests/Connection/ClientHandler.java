// ============================================================================
//
// = PACKAGE
//    tests.Connection
// 
// = FILENAME
//    ClientHandler.java
//
// = AUTHOR
//    Prashant Jain
// 
// ============================================================================
package tests.Connection;

import java.io.*;
import java.net.*;
import JACE.OS.*;
import JACE.Connection.*;

public class ClientHandler extends SvcHandler
{
  public ClientHandler ()
    {
    }
  
  public int open (Object obj)
  {
    new Thread (this).start ();
    return 0;
  }

  public void run ()
    {
      BufferedReader in = new BufferedReader(new InputStreamReader(System.in)); 

      String msg;
      StringBuffer ack = new StringBuffer ();
      int ack_len;
      try
	{
	  while (true)
	    {
	      System.out.print ("Enter input: ");
	      System.out.flush ();
	      msg = in.readLine ();
	      if (msg == null)
		break;
	      this.peer ().send (new StringBuffer (msg));
	      System.out.println ("Waiting for ack...");
	      ack_len = this.peer ().recv (ack);
	      if (ack_len == 0)
		break;
	      else
		System.out.println (ack);
	    }
	}
      catch (NullPointerException e)
	{
	  ACE.ERROR ("connection reset by peer");
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
