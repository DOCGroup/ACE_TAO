/*************************************************
 *
 * = PACKAGE
 *    tests.Concurrency
 *
 * = FILENAME
 *    RWMutexTest.java
 *
 *@author Ross Dargahi (rossd@krinfo.com)
 *
 *************************************************/
package tests.Concurrency;

import ACE.OS.*;
import ACE.Concurrency.*;

class TestThread extends Thread
{
  TestThread(String  name,
	     boolean writer,
	     RWMutex  lock)
  {
    super (name);
    mWriter = writer;
    mLock = lock;
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
		ACE.DEBUG (getName() + ": Acquired Read Lock");
		
		int sleepTime = i * 100;
		sleep (sleepTime);

		mLock.release ();
		ACE.DEBUG (getName () + ": Released Read Lock");
	      }
	    else
	      {
		mLock.acquireWrite ();
		ACE.DEBUG (getName () + ": Acquired Write Lock");
					
		int sleepTime = i * 100;
		sleep (sleepTime);

		mLock.release ();
		ACE.DEBUG (getName () + ": Released Write Lock");
	      }
	  }
	catch (InterruptedException ex)
	  {
	    ACE.ERROR ("InterruptedException");
	  }
      }
  }

  RWMutex  mLock;
  boolean mWriter;
}

public class RWMutexTest
{
  public static void main(String [] args)
  {
    RWMutex lock = new RWMutex();
    
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

