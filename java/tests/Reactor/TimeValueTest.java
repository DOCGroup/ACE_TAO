/*************************************************
 *
 * = PACKAGE
 *    ACE.Reactor
 *
 * = FILENAME
 *    TimeValueTest.java
 *
 *@author Prashant Jain
 *
 *************************************************/
package tests.Reactor;

import ACE.OS.*;
import ACE.ASX.*;

public class TimeValueTest
{
  public void ASSERT (boolean expression, int i)
  {
    if (!expression)
      ACE.DEBUG ("ASSERT failed for " + i);
  }

  public void runTest ()
  {
    TimeValue tv1 = new TimeValue ();
    TimeValue tv2 = new TimeValue (2);
    TimeValue tv3 = new TimeValue (100);  
    TimeValue tv4 = new TimeValue (1, 1000000000);
    TimeValue tv5 = new TimeValue (2);
    TimeValue tv6 = new TimeValue (1, -1000000000);

    this.ASSERT (tv1.equals (new TimeValue (0)), 1);
    this.ASSERT (tv2.lessThan (tv3), 2);
    this.ASSERT (tv2.lessThanEqual (tv2), 3);
    this.ASSERT (tv2.greaterThanEqual (tv4), 4);
    this.ASSERT (tv5.greaterThanEqual (tv6), 5);
    this.ASSERT (tv2.equals (new TimeValue (1, 1000000000)), 6);
    this.ASSERT (tv5.equals (tv4), 7);
    this.ASSERT (tv2.equals (tv4), 8);
    this.ASSERT (tv1.notEquals (tv2), 9);
    this.ASSERT (tv6.equals (tv1), 10);
  }

  public static void main (String [] args)
  {
    new TimeValueTest ().runTest ();
  }
}
