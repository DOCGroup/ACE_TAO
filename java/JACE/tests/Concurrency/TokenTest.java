/*************************************************
 *
 * = PACKAGE
 *    tests.Concurrency
 *
 * = FILENAME
 *    TokenTest.java
 *
 *@author Prashant Jain
 *
 *************************************************/
package JACE.tests.Concurrency;

import java.io.*;
import JACE.OS.*;
import JACE.Concurrency.ThreadManager;
import JACE.Concurrency.*;
import JACE.ASX.TimeValue;

public class TokenTest implements Runnable
{
  static class MyToken extends Token
  {
    public void sleepHook ()
    {
      ACE.DEBUG (Thread.currentThread ().getName () + 
		 " blocking, sleepHook called");
    }  
  }

  public void run ()
  {
    String name = Thread.currentThread().getName();
    try
      {
	if (this.token_.acquire () != AbstractLock.FAILURE)
	  ACE.DEBUG (name + " got token");
	else
	  ACE.DEBUG (name + " couldn't get token");

	ACE.DEBUG (name + " calling acquire again (test nesting)");
	if (this.token_.acquire() != AbstractLock.FAILURE)
	  ACE.DEBUG (name + " got token again");
	else
	  ACE.DEBUG (name + " couldn't get token");

	Thread.sleep (1000);

	ACE.DEBUG (name + " gives it up for max 2 sec to first waiter");
	this.token_.renew (1, TimeValue.relativeTimeOfDay(2, 0));

	ACE.DEBUG (name + " releases the token once");
	this.token_.release ();
	ACE.DEBUG (name + " calls release again (nesting level was 2)");
	this.token_.release ();
      }
    catch (InterruptedException e)
      {
	this.token_.release ();
      }
    catch (JACE.ASX.TimeoutException e) 
      {
	ACE.DEBUG (name + " timed out");
      }
  }

  /**
   * Command line: optional number of threads to create (defaults to 2)
   */
  public static void main (String args [])
  {
    ThreadManager tm = new ThreadManager ();
    int n = 2;

    ACE.enableDebugging ();

    try
      {
	if (args.length == 1)
	  {
	    n = Integer.parseInt (args[0]);
	  }
      }
    catch (NumberFormatException e)
      {
	ACE.ERROR ("Illegal argument.");
      }

    tm.spawnN (n,
	       new TokenTest (),
	       false);
  }

  private static MyToken token_ = new MyToken ();
}
