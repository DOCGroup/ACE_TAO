// ============================================================================
//
// = PACKAGE
//    tests.ASX
// 
// = FILENAME
//    ThreadPoolTest.java
//
// = AUTHOR
//    Prashant Jain
// 
// ============================================================================
package JACE.tests.ASX;

import java.io.*;
import JACE.OS.*;
import JACE.ASX.*;
import JACE.Reactor.*;

public class ThreadPoolTest extends Task
{
  int nThreads_;
  int nIterations_;
  
  public static int MAX_MB_SIZE = 1024;

  public ThreadPoolTest (int nThreads, int nIterations)
    {
      this.nIterations_ = nIterations;
      this.nThreads_ = nThreads;
      if (this.activate (0, nThreads, true) == -1)
	ACE.ERROR ("activate failed");
    }

  public int handleTimeout (TimeValue tv, Object obj)
  {
    return 0;
  }

  public int open (Object obj)
    {
      return 0;
    }

  public int close (long flags)
    {
      return 0;
    }

  public int put (MessageBlock mb, TimeValue tv)
    {
      try
	{
	  return this.putq (mb, tv);
	}
      catch (InterruptedException e)
	{
	}
      return 0;
    }
  
  public int svc ()
    {
      int count = 1;

      // Keep looping, reading a message out of the queue, until we get a
      // message with a length == 0, which signals us to quit.
      try
	{
	  for (;; count++)
	    {       
	      MessageBlock mb = this.getq (null);
	      if (mb == null)
		{
		  ACE.ERROR (Thread.currentThread ().toString () + " in iteration " + count + ", got result -1, exiting");
		  break;
		}
	      int length = mb.length ();
	      
	      if (length > 0)
		ACE.DEBUG (Thread.currentThread ().toString () +
			   " in iteration " + count + ", length = " + 
			   length + ", text = \"" + mb.base () + "\"");
	      
	      if (length == 0)
		{
		  ACE.DEBUG (Thread.currentThread ().toString () +
			     " in iteration " + count + 
			     ", got NULL message, exiting");
		  break;
		}
	      Thread.yield ();
	    }
	}
      catch (InterruptedException e)
	{
	}
      return 0;
    }

  public static void produce (ThreadPoolTest threadPool, int nIterations)
    {
      int count = 0;
      for (int n = 0;;)
	{
	  // Allocate a new message.
	  MessageBlock mb = new MessageBlock (new Integer (count).toString ());
	  
	  if (count == nIterations)
	    n = 1; // Indicate that we need to shut down.
	  else
	    count++;

	  if (count == 0 || (count % 20 == 0))
	    {
	      try
		{
		  Thread.sleep (1);
		}
	      catch (InterruptedException e)
		{
		}
	    }
	  if (n != 1)
	    {
	      ACE.DEBUG ("Producing...");
	      // Pass the message to the Thread_Pool.
	      if (threadPool.put (mb, null) == -1)
		ACE.ERROR ("put");
	    }
	  else
	    {
	      // Send a shutdown message to the waiting threads and exit.
	      ACE.DEBUG ("start loop, dump of task");

	      for (int i = threadPool.thrCount (); i > 0; i--)
		{
		  ACE.DEBUG (Thread.currentThread ().toString () + 
			     "EOF, enqueueing NULL block for thread " + i);

		  // Enqueue a NULL message to flag each consumer to
		  // shutdown.
		  if (threadPool.put (new MessageBlock (0), null) == -1)
		    ACE.ERROR ("put");
		}

	      break;
	    }
	}
    }

  public static void main (String args[])
    {
      int nThreads = 1;
      int nIterations = 100;

      ACE.enableDebugging ();

      try
	{
	  if (args.length == 2)
	    {
	      nThreads = Integer.parseInt (args[0]);
	      nIterations = Integer.parseInt (args[1]);
	    }
	  else if (args.length == 1)
	    {
	      nThreads = Integer.parseInt (args[0]);
	    }
	}
      catch (NumberFormatException e)
	{
	  ACE.ERROR ("Illegal argument.");
	}
      ACE.DEBUG ("Threads = " + nThreads + " Iterations = " + nIterations);

      // Create the worker tasks.
      ThreadPoolTest threadPool = new ThreadPoolTest (nThreads,
						      nIterations);

      // Create work for the worker tasks to process in their own threads.
      produce (threadPool, nIterations);
      ACE.DEBUG ("exiting...");
    }
}


