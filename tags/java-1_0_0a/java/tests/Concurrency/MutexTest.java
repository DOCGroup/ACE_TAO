/*************************************************
 *
 * = PACKAGE
 *    tests.Concurrency
 *
 * = FILENAME
 *    MutexTest.java
 *
 *@author Prashant Jain
 *
 *************************************************/
package tests.Concurrency;

import java.io.*;
import ACE.OS.*;
import ACE.Concurrency.*;

class MutexReader extends Thread
{
  MutexReader (int nIterations, Mutex mutex)
    {
      this.nIterations_ = nIterations;
      this.mutex_ = mutex;
    }

  public void run ()
    {
      for (int i = 1; i <= this.nIterations_; i++)
	{
	  // Acquire the mutex (will block until it gets it)
	  try
	    {
	      this.mutex_.acquire ();
	    }
	  catch (InterruptedException e)
	    {
	      ACE.ERROR (e);
	    }

	  MutexTest.count++;
	  ACE.DEBUG (Thread.currentThread ().toString () +
		     " reader acquired mutex in iteration " + i +
		     ", count = " + MutexTest.count);

	  try
	    {
	      Thread.sleep (1);
	    }
	  catch (InterruptedException e)
	    {
	    }
	  // Release the mutex
	  this.mutex_.release ();
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
  Mutex mutex_;
}

class MutexWriter extends Thread
{
  MutexWriter (int nIterations, Mutex mutex)
    {
      this.nIterations_ = nIterations;
      this.mutex_ = mutex;
    }

  public void run ()
    {
      for (int i = 1; i <= this.nIterations_; i++)
	{
	  // Acquire the mutex (will block until it gets it)
	  try
	    {
	      this.mutex_.acquire ();
	    }
	  catch (InterruptedException e)
	    {
	      ACE.ERROR (e);
	    }
	  
	  MutexTest.count++;
	  ACE.DEBUG (Thread.currentThread ().toString () +
		     " writer acquired mutex in iteration " + i +
		     ", count = " + MutexTest.count);
	  
	  try
	    {
	      Thread.sleep (1);
	    }
	  catch (InterruptedException e)
	    {
	    }

	  // Release the mutex
	  this.mutex_.release ();
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
  Mutex mutex_;
}

public class MutexTest
{
  public static void main (String args[])
    {
      int nReaders = 1;
      int nWriters = 1;
      int nIterations = 100;
      int i;
      try
	{
	  if (args.length == 3)
	    {
	      nReaders = Integer.parseInt (args[0]);
	      nWriters = Integer.parseInt (args[1]);
	      nIterations = Integer.parseInt (args[2]);
	    }
	}
      catch (NumberFormatException e)
	{
	  ACE.ERROR ("Illegal argument.");
	}

      // Create a lock
      Mutex mutex = new Mutex ();

      // Now spawn off the readers and writers
      for (i = 0; i < nReaders; i++)
	new MutexReader (nIterations, mutex).start ();
      
      for (i = 0; i < nWriters; i++)
	new MutexWriter (nIterations, mutex).start ();            
    }
  public static int count;
}
