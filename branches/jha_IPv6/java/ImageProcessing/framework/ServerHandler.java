package imaging.framework;

import java.io.*;
import java.net.*;
import JACE.OS.*;
import JACE.Connection.*;

public class ServerHandler extends SvcHandler
{
  public int open (Object obj)
  {
    new Thread (this).start ();
    return 0;
  }

  public void run ()
    {
      int msg_len;
      try
	{
	  while (true)
	    {
	      StringBuffer msg = new StringBuffer ();
	      msg_len = this.peer ().recv (msg);
	      if (msg_len == 0)
		break;
	      System.out.println (msg);
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
