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
package tests.Concurrency;

import java.io.*;
import ACE.OS.*;
import ACE.Concurrency.*;

class MyToken extends Token
{
  public void sleepHook ()
  {
    ACE.DEBUG (Thread.currentThread () + " blocking, sleepHook called");
  }  
}

public class TokenTest implements Runnable
{
  public void run ()
  {
    try
      {
	this.token_.acquire ();
	ACE.DEBUG (Thread.currentThread () + " acquired token");
	this.token_.acquire ();
	ACE.DEBUG (Thread.currentThread () + " acquired token");
	Thread.sleep (100);

	this.token_.renew (0);

	this.token_.release ();
	ACE.DEBUG (Thread.currentThread () + " released token");
	this.token_.release ();
	ACE.DEBUG (Thread.currentThread () + " released token");
      }
    catch (InterruptedException e)
      {
	this.token_.release ();
      }
  }

  public static void main (String args [])
  {
    ThreadManager tm = new ThreadManager ();
    int n = 1;
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

  private MyToken token_ = new MyToken ();
}
