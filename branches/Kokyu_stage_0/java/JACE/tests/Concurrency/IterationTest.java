/*************************************************
 *
 * = PACKAGE
 *    JACE.tests.Concurrency
 *
 * = FILENAME
 *    IterationTest.java
 *
 *@author Prashant Jain
 *@author Everett Anderson
 *
 *************************************************/
package JACE.tests.Concurrency;

import java.io.*;
import JACE.OS.*;
import JACE.Concurrency.*;

/**
 * Test which uses an AbstractLock to prevent threads from mangling
 * shared data.  This currently tests the four local concurrency
 * mechanisms -- Token, Mutex, RWMutex, and Semaphore.
 */
public class IterationTest
{
  static class LockReader extends Thread
  {
    LockReader (int nIterations, AbstractLock lock)
    {
      this.nIterations_ = nIterations;
      this.lock_ = lock;
    }

    public void run ()
    {
      for (int i = 1; i <= this.nIterations_; i++)
	{
	  // Acquire the lock (will block until it gets it)
	  try
	    {
	      this.lock_.acquire ();
	    }
	  catch (InterruptedException e)
	    {
	      ACE.ERROR (e);
	    }
	  catch (LockException e)
	    {
	      ACE.ERROR (e);
	    }

	  IterationTest.count++;
	  ACE.DEBUG (Thread.currentThread ().toString () +
		     " reader acquired lock in iteration " + i +
		     ", count = " + IterationTest.count);

	  try
	    {
	      Thread.sleep (1);
	    }
	  catch (InterruptedException e)
	    {
	    }
	  // Release the lock
	  try 
	    {
	      this.lock_.release ();
	    }
	  catch (LockException e)
	    {
	      ACE.ERROR (e);
	    }
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
    AbstractLock lock_;
  }

  static class LockWriter extends Thread
  {
    LockWriter (int nIterations, AbstractLock lock)
    {
      this.nIterations_ = nIterations;
      this.lock_ = lock;
    }

    public void run ()
    {
      for (int i = 1; i <= this.nIterations_; i++)
	{
	  // Acquire the lock (will block until it gets it)
	  try
	    {
	      this.lock_.acquire ();
	    }
	  catch (InterruptedException e)
	    {
	      ACE.ERROR (e);
	    }
	  catch (LockException e)
	    {
	      ACE.ERROR (e);
	    }
	  
	  
	  IterationTest.count++;
	  ACE.DEBUG (Thread.currentThread ().toString () +
		     " writer acquired lock in iteration " + i +
		     ", count = " + IterationTest.count);
	  
	  try
	    {
	      Thread.sleep (1);
	    }
	  catch (InterruptedException e)
	    {
	    }

	  // Release the lock
	  try 
	    {
	      this.lock_.release ();
	    }
	  catch (LockException e) 
	    {
	      ACE.ERROR (e);
	    }

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
    AbstractLock lock_;
  }

  final static int TOKEN = 0;
  final static int MUTEX = 1;
  final static int RWMUTEX = 2;
  final static int SEMAPHORE = 3;

  /**
   * Command line arguments:
   * 
   * (number of readers) (number of writers) (number of iterations)
   */
  public static void main (String args[]) throws InterruptedException
  {
    int nReaders = 1;
    int nWriters = 1;
    int nIterations = 100;
    int i;

    ACE.enableDebugging ();

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

    AbstractLock lock = null;
    Thread threads[] = new Thread [nReaders + nWriters];

    for (int state = TOKEN; state <= SEMAPHORE; state++) {

      count = 0;

      switch (state) 
	{
	case TOKEN:
	  ACE.DEBUG ("--------\nTesting Token:");
	  lock = new Token ();
	  break;
	case MUTEX:
	  ACE.DEBUG ("--------\nTesting Mutex:");
	  lock = new Mutex ();
	  break;
	case RWMUTEX:
	  ACE.DEBUG ("--------\nTesting RWMutex:");
	  lock = new RWMutex ();
	  break;
	case SEMAPHORE:
	  ACE.DEBUG ("--------\nTesting Semaphore:");
	  lock = new Semaphore ();
	  break;
	}

      int numThreads = 0;

      // Spawn off the readers and writers
      for (i = 0; i < nReaders; i++) {
	threads[numThreads] = new LockReader (nIterations, lock);
	threads[numThreads].start ();
	numThreads++;
      }
      
      for (i = 0; i < nWriters; i++) {
	threads[numThreads] = new LockWriter (nIterations, lock); 
	threads[numThreads].start ();
	numThreads++;
      }

      for (i = 0; i < numThreads; i++)
	threads[i].join ();

    }
  }

  public static int count;
}
