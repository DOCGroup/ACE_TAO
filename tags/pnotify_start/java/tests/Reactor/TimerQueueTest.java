// ============================================================================
//
// = PACKAGE
//    tests.Reactor
// 
// = FILENAME
//    TimerQueueTest.java
//
// = AUTHOR
//    Prashant Jain
// 
// ============================================================================
package tests.Reactor;

import JACE.OS.*;
import JACE.ASX.*;
import JACE.Reactor.*;

public class TimerQueueTest implements EventHandler
{
  public int handleTimeout (TimeValue tv, Object obj)
  {
    ACE.DEBUG ("handleTimeout: " + tv.toString () + " " + (String) obj);
    return 0;
  }

  public static void main (String args [])
  {
    TimerQueue tq = new TimerQueue (true);
    TimerQueueTest th1 = new TimerQueueTest ();
    int n = 5;

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

    // Set a periodic timer
    int id= tq.scheduleTimer (th1,
			      "periodic",
			      new TimeValue (2),
			      new TimeValue (3));
    
    int i;
    // Set a bunch of single timers
    for (i=1; i <= n; i++)
      {
	tq.scheduleTimer (th1,
			   "A-timer-" + new Integer (i),
			   new TimeValue (i*2));
      }

    TimerQueueTest th2 = new TimerQueueTest ();
    for (i=1; i <= n; i++)
      {
	tq.scheduleTimer (th2,
			  "B-timer-" + new Integer (i),
			  new TimeValue (i*3));
      }
    // Cancel all timers associated with this handler
    tq.cancelTimer (th2);

    try
      {
	Thread.sleep (30000);
      }
    catch (InterruptedException e)
      {
      }
    tq.cancelTimer (id);
    System.exit (0);
  }
}
