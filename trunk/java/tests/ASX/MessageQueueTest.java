// ============================================================================
//
// = PACKAGE
//    tests.ASX
// 
// = FILENAME
//    MessageQueueTest.java
//
// = AUTHOR
//    Prashant Jain
// 
// ============================================================================
package tests.ASX;

import java.io.*;
import ACE.OS.*;
import ACE.ASX.*;

public class MessageQueueTest
{
  public static void main (String args[])
    {
      try
	{
	  MessageBlock conMb;
	  MessageQueue msgQueue = new MessageQueue ();
	  MessageBlock mb1 = new MessageBlock ("hello");
	  MessageBlock mb2 = new MessageBlock ("world");
	  mb1.msgPriority (5);
	  mb2.msgPriority (7);

	  // Enqueue in priority order.
	  if (msgQueue.enqueue (mb1) == -1)
	    ACE.ERROR ("put_next");

	  if (msgQueue.enqueue (mb2) == -1)
	    ACE.ERROR ("put_next");

	  // Now try to dequeue
	  if ((conMb = msgQueue.dequeueHead ()) == null)
	    ACE.ERROR ("dequeueHead");
	  else
	    ACE.DEBUG ("Consumer: removed item " + conMb.base () + " of priority " + conMb.msgPriority ());
	}
      catch (InterruptedException e)
	{
	}
    }
}
  
