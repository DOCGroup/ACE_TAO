// ============================================================================
//
// = PACKAGE
//    tests.ASX
// 
// = FILENAME
//    PriorityBufferTest.java
//
// = AUTHOR
//    Prashant Jain
// 
// ============================================================================
package tests.ASX;

import java.io.*;
import JACE.OS.*;
import JACE.ASX.*;

class consumer extends Thread
{
  public void run ()
    {
      MessageBlock mb = null;
      long curPriority = 0;
      int length = 0;

      try
	{
	  // Keep looping, reading a message out of the queue, until we
	  // get a message with a length == 0, which signals us to quit.
	  for (;;)
	    {
	      if ((mb = PriorityBufferTest.msgQueue.dequeueHead ()) == null)
		break;
	      
	      length = mb.length ();
	      curPriority = mb.msgPriority ();
	      
	      if (length > 0)
		ACE.DEBUG ("Consumer: removed item \"" + mb.base () + "\" of priority: " + curPriority);
	      
	      if (length == 0)
		break;
	    }
	}
      catch (InterruptedException e)
	{
	}
    }
}

class producer extends Thread
{
  producer (int delay)
    {
      this.delay_ = delay;
    }

  public void run ()
    {
      try 
	{
	  long count = 0;
	  for (char c = 'a'; c <= 'z'; c++)
	    {
	      count++;
	      // Allocate a new message
	      MessageBlock mb = new MessageBlock (new Character (c).toString ());
	      // Set the priority
	      mb.msgPriority (count);
	  
	      // Enqueue in priority order.
	      if (PriorityBufferTest.msgQueue.enqueue (mb) == -1)
		ACE.ERROR ("put_next");
	      else
		{
		  ACE.DEBUG ("Producer: inserted item \"" + mb.base () + "\" of priority: " + count);
		  if (this.delay_ > 0)
		    this.sleep (this.delay_);
		}
	    }

	  // Now send a 0-sized shutdown message to the other thread
	  if (PriorityBufferTest.msgQueue.enqueueTail (new MessageBlock (0)) == -1)
	    ACE.ERROR ("put_next");
	}
      catch (InterruptedException e)
	{
	}
    }

  private int delay_;
}

public class PriorityBufferTest
{
  public static MessageQueue msgQueue = new MessageQueue ();

  public static void main (String args[])
    {
      int delay = 0;
      if (args.length == 1)
	{
	  try
	    {
	      delay = Integer.parseInt (args[0]);
	    }
	  catch (NumberFormatException e)
	    {
	      ACE.ERROR ("Illegal argument.");
	    }
	}
      new producer (delay).start ();
      new consumer ().start ();      
    }
}
