// ============================================================================
//
// = PACKAGE
//    tests.ASX
// 
// = FILENAME
//    TaskTest.java
//
// = AUTHOR
//    Prashant Jain
// 
// ============================================================================
package tests.ASX;

import java.io.*;
import ACE.OS.*;
import ACE.ASX.*;
import ACE.Reactor.*;

public class TaskTest extends Task
{
  int nThreads_;
  int nIterations_;

  public TaskTest (int nThreads, int nIterations)
    {
      this.nIterations_ = nIterations;
      this.nThreads_ = nThreads;
      if (this.activate (0, nThreads, true) == -1)
	ACE.ERROR ("activate failed");
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
      return 0;
    }

  public int handleTimeout (TimeValue tv, Object obj)
  {
    return 0;
  }
  
  public int svc ()
    {
      for (int i = 1; i <= this.nIterations_; i++)
	{
	  ACE.DEBUG (Thread.currentThread ().toString () + " in iteration " + i);
	  // Allow other threads to run
	  Thread.yield ();
	}
      return 0;
    }

  public static void main (String args[])
    {
      int nThreads = 1;
      int nIterations = 1;
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
      TaskTest tt = new TaskTest (nThreads, nIterations);
    }
}
