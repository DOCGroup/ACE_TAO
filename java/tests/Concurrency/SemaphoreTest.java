/*************************************************
 *
 * = PACKAGE
 *    tests.Concurrency
 *
 * = FILENAME
 *    SemaphoreTest.java
 *
 *@author Prashant Jain
 *
 *************************************************/
package tests.Concurrency;

import java.io.*;
import ACE.OS.*;
import ACE.Concurrency.*;

class SemaphoreWriter extends Thread
{
  SemaphoreWriter (int nIterations, Semaphore s)
    {
      this.nIterations_ = nIterations;
      this.s_ = s;
    }

  public void run ()
    {
      for (int i = 1; i <= this.nIterations_; i++)
	{
	  // Acquire the semaphore (will block until it gets it)
	  try
	    {
	      this.s_.acquire ();
	    }
	  catch (InterruptedException e)
	    {
	      ACE.ERROR (e);
	    }
	  
	  SemaphoreTest.counter++;
	  ACE.DEBUG (Thread.currentThread ().toString () +
		     " acquired semaphore in iteration " + i +
		     ", counter = " + SemaphoreTest.counter);
	  
	  try
	    {
	      Thread.sleep (1);
	    }
	  catch (InterruptedException e)
	    {
	    }

	  // Release the semaphore
	  this.s_.release ();
	  ACE.DEBUG (Thread.currentThread ().toString () +
		     " released semaphore in iteration " + i);
	  try
	    {
	      Thread.sleep (1);
	    }
	  catch (InterruptedException e)
	    {
	    }

	}
    }
  
  int nIterations_;
  Semaphore s_;
}

public class SemaphoreTest
{
  public static void main (String args[])
    {
      int nThreads = 1;
      int count = 1;
      int nIterations = 100;
      int i;
      try
	{
	  if (args.length == 3)
	    {
	      nThreads = Integer.parseInt (args[0]);
	      count = Integer.parseInt (args[1]);
	      nIterations = Integer.parseInt (args[2]);
	    }
	}
      catch (NumberFormatException e)
	{
	  ACE.ERROR ("Illegal argument.");
	}

      // Create a lock
      Semaphore s = new Semaphore (count);

      // Spawn off n_threads
      for (i = 0; i < nThreads; i++)
	new SemaphoreWriter (nIterations, s).start ();            
    }
  public static int counter;
}
