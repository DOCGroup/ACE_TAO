package JACE.tests.Concurrency;

import JACE.OS.*;
import JACE.ASX.*;
import JACE.Concurrency.*;

/** 
 * Performs a test with 5 threads.  This should test what happens during
 * renews, also testing what happens when timeout exceptions are generated
 * during operations.
 *
 * Sample output:
 *
 * Thread 1 calling acquire
 * Thread 1 got lock
 * Thread 1 doing work
 * Thread 2 calling acquire
 * Thread 1 finished doing work
 * Thread 1 calling renew -1 1
 * Thread 2 got lock
 * Thread 2 doing work
 * Thread 3 calling acquire
 * Thread 4 calling acquire
 * Thread 5 calling acquire with 1 sec timeout
 * Thread 1 timed out in renew
 * Thread 1 failed renewing
 * Thread 1 calling release
 * Thread 1 failed release
 * Thread 5 timed out in acquire
 * Thread 5 failed acquire
 * Thread 5 calling release
 * Thread 5 failed release
 * Thread 2 finished doing work
 * Thread 2 calling renew 1
 * Thread 3 got lock
 * Thread 3 doing work
 * Thread 3 finished doing work
 * Thread 3 calling release
 * Thread 3 released
 * Thread 2 got the lock back
 * Thread 2 calling release
 * Thread 2 released
 * Thread 4 got lock
 * Thread 4 doing work
 * Thread 4 finished doing work
 * Thread 4 calling release
 * Thread 4 released
 */
public class TimingTest
{
  static class Worker extends Thread
  {
    AbstractLock lock;
    String name;
    int order;

    public Worker (String name, int order, AbstractLock lock)
    {
      this.name = name;
      this.order = order;
      this.lock = lock;
    }

    public void run()
    {
      switch (this.order) 
	{
	case 1:
	  methodA();
	  break;
	case 2:
	  methodB();
	  break;
	case 3:
	  methodC();
	  break;
	case 4:
	  methodD();
	  break;
	}
    }

    public void methodA()
    {
      // acquires
      // launches a class 2 worker
      // works
      // renew -1 with a 1 sec timeout (intended to fail)
      // release

      try {
	if (acquire() == AbstractLock.FAILURE)
	  return;
	
	new Worker("Thread 2", 2, lock).start();
	
	work(3000);
	
	renew(-1, 1);
      } finally {
	release();
      }
    }

    public void methodB()
    {
      // acquire
      // launch two new class 3 workers and one class 4 worker 
      // work
      // renew, yielding to one waiter
      // release
      try {
	if (acquire() == AbstractLock.FAILURE)
	  return;
	
	new Worker ("Thread 3", 3, lock).start ();
	new Worker ("Thread 4", 3, lock).start ();
	new Worker ("Thread 5", 4, lock).start ();
	
	work(3000);
	
	renew(1);
      } finally {
	release();
      }
    }

    public void methodC()
    {
      // acquire
      // work
      // release

      try {
	if (acquire() == AbstractLock.FAILURE)
	  return;
	
	work(1000);
      } finally {
	release();
      }

    }

    public void methodD()
    {
      // acquire with a short timeout 
      // (shouldn't get past the timeout)
      try {
	if (acquire (1) == AbstractLock.FAILURE)
	  return;
	
	ACE.DEBUG (name + " probably should have timed out in acquire");
	work (1000);
      } finally {
	release ();
      }
    }

    public int acquire (int relativeTimeout)
    {
      int result = AbstractLock.FAILURE;
      ACE.DEBUG(name + " calling acquire with " + relativeTimeout +
		" sec timeout");

      try {
	result = lock.acquire (TimeValue.relativeTimeOfDay (relativeTimeout, 0));
      } catch (JACE.ASX.TimeoutException e) {
	ACE.DEBUG(name + " timed out in acquire");
      } catch (Exception e) {
	ACE.DEBUG (name + " got exception: " + e);
      }

      if (result != AbstractLock.FAILURE)
	ACE.DEBUG(name + " got lock");
      else
	ACE.DEBUG(name + " failed acquire");

      return result;
    }

    public int acquire()
    {
      int result = AbstractLock.FAILURE;

      ACE.DEBUG(name + " calling acquire");
      try {
	result = lock.acquire();
      } catch (Exception e) {
	ACE.DEBUG(name + " got exception: " + e);
      }
      if (result != AbstractLock.FAILURE)
	ACE.DEBUG(name + " got lock");
      else
	ACE.DEBUG(name + " failed acquire");

      return result;
    }

    public void release()
    {
      ACE.DEBUG(name + " calling release");
      int result = AbstractLock.FAILURE;
      try { 
	result = lock.release();
      } catch (Exception e) {
	ACE.DEBUG (name + " got exception: " + e);
      }

      if (result != AbstractLock.FAILURE)
	ACE.DEBUG(name + " released");
      else
	ACE.DEBUG(name + " failed release");
    }

    public void renew(int pos)
    {
      int result = AbstractLock.FAILURE;
      ACE.DEBUG(name + " calling renew " + pos);
      try {
	result = lock.renew(pos);
      } catch (Exception e) {
	ACE.DEBUG (name + " got exception: " + e);
      }

      if (result != AbstractLock.FAILURE)
	ACE.DEBUG(name + " got the lock back");
      else
	ACE.DEBUG(name + " failed renewing");
    }

    public void renew(int pos, int relativeTimeout)
    {
      int result = AbstractLock.FAILURE;
      ACE.DEBUG(name + " calling renew " + pos + " " + 
		relativeTimeout);
      try {
	result = lock.renew(pos,
			    TimeValue.relativeTimeOfDay(relativeTimeout,
							0));
      } catch (JACE.ASX.TimeoutException e) {
	ACE.DEBUG(name + " timed out in renew");
      } catch (Exception e) {
	ACE.DEBUG (name + " got exception: " + e);
      }

      if (result != AbstractLock.FAILURE)
	ACE.DEBUG(name + " got the lock back");
      else
	ACE.DEBUG(name + " failed renewing");
    }

    public void work(int ms)
    {
      ACE.DEBUG(name + " doing work");
      try {
	Thread.sleep(ms);
      } catch (InterruptedException e) { 
	ACE.DEBUG(name + " was interrupted");
      }
      ACE.DEBUG(name + " finished doing work");
    }
  }
 
  /**
   * Command line:
   *
   * Optional class name (which implements AbstractLock) to test
   */
  public static void main(String args[]) { 
    AbstractLock lock;

    ACE.enableDebugging ();

    try {

      if (args.length > 0) {
	ACE.DEBUG("Using class " + args[0] + " as the Lock");

	lock = (AbstractLock)(Class.forName (args[0]).newInstance ());
      } else
	lock = new Token ();

      new Worker("Thread 1", 1, lock).start();

    } catch (Exception e) {
      ACE.ERROR("Error: " + e);
    }
  }
} 
