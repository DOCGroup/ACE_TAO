/*************************************************
 *
 * = PACKAGE
 *    JACE.tests.Concurrency
 *
 * = FILENAME
 *    RWMutexTest.java
 *
 *@author Ross Dargahi (rossd@krinfo.com)
 *
 *************************************************/
package JACE.tests.Concurrency;

import JACE.OS.*;
import JACE.Concurrency.*;

public class RWMutexTest
{
  static class TestThread extends Thread
  {
    TestThread(String  name,
	       boolean writer,
	       AbstractLock lock)
    {
      super (name);
      mWriter = writer;
      mLock = lock;
    }

    public void output (String msg)
    {
      synchronized (iosynch) {
	ACE.DEBUG (msg);
      }
    }

    public void run()
    {
      for (int i = 0; i < 10; i++)
	{
	  try
	    {
	      if (!mWriter)
		{
		  mLock.acquireRead();
		  output (getName() + ": Acquired Read Lock");
		
		  int sleepTime = 10;
		  sleep (sleepTime);

		  mLock.release ();
		  output (getName () + ": Released Read Lock");
		}
	      else
		{
		  mLock.acquireWrite ();
		  output (getName () + ": Acquired Write Lock");
		
		  int sleepTime = 10;
		  sleep (sleepTime);

		  mLock.release ();
		  output (getName () + ": Released Write Lock");
		}
	    }
	  catch (InterruptedException ex)
	    {
	      ACE.ERROR ("InterruptedException");
	    }
	  catch (LockException ex) 
	    {
	      ACE.ERROR ("LockException: " + ex.getMessage ());
	    }
	}
    }

    AbstractLock  mLock;
    boolean mWriter;

    static Object iosynch = new Object ();
  }

  /**
   * Command line arguments:
   *
   * Optional class name to use for the tests (must implement the
   * AbstractLock interface).  Followed by an optional number of
   * iterations.
   */
  public static void main(String [] args)
    throws ClassNotFoundException,
	   IllegalAccessException,
	   InstantiationException,
	   InterruptedException,
	   NumberFormatException
  {
    AbstractLock lock;
    int iterations = 1;

    ACE.enableDebugging ();

    if (args.length > 0) {
      ACE.DEBUG("Using class " + args[0] + " as the Lock");
      
      lock = (AbstractLock)(Class.forName (args[0]).newInstance ());

      if (args.length > 1) 
	iterations = Integer.parseInt (args[1]);
	
    } else
      lock = new RWMutex ();
 
    for (int i = 0; i < iterations; i++) {
  
      ACE.DEBUG("Iteration " + (i + 1));

      TestThread t1 = new TestThread ("1", false, lock);
      TestThread t2 = new TestThread ("2", false, lock);
      TestThread t3 = new TestThread ("3", false, lock);
      TestThread t4 = new TestThread ("4", true, lock);
      TestThread t5 = new TestThread ("5", false, lock);
      TestThread t6 = new TestThread ("6", false, lock);
      TestThread t7 = new TestThread ("7", false, lock);
      TestThread t8 = new TestThread ("8", true, lock);

      t1.start ();
      t2.start ();
      t3.start ();
      t4.start ();
      t5.start ();
      t6.start ();
      t7.start ();
      t8.start ();
    }
  }
}

