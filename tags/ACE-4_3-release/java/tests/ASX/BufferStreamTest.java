// ============================================================================
//
// = PACKAGE
//    tests.ASX
// 
// = FILENAME
//    BufferStreamTest.java
//
// = AUTHOR
//    Prashant Jain
// 
// ============================================================================
package tests.ASX;

import java.io.*;
import JACE.OS.*;
import JACE.ASX.*;

// This short program copies stdin to stdout via the use of an ASX
// STREAM.  It illustrates an implementation of the classic "bounded
// buffer" program using an ASX STREAM containing two Modules.  Each
// Module contains two Tasks.  

class CommonTask extends Task
{
  // ACE_Task hooks
  public int open (Object obj)
  {
    if (this.activate (0, 1, false) == -1)
      ACE.ERROR ("spawn");
    return 0;
  }

  public int close (long exitStatus)
  {
    ACE.DEBUG (Thread.currentThread () + " thread is exiting with status "  +
	       exitStatus + " in module " + this.name () + "\n");
    return 0;
  }

  public int put (MessageBlock mb, TimeValue tv) 
  { 
    return 0; 
  }

  public int handleTimeout (TimeValue tv, Object obj)
  {
    return 0;
  }
}

// Define the Producer interface. 

class Producer extends CommonTask
{
  // Read data from stdin and pass to consumer.
  // The Consumer reads data from the stdin stream, creates a message,
  // and then queues the message in the message list, where it is
  // removed by the consumer thread.  A 0-sized message is enqueued when
  // there is no more data to read.  The consumer uses this as a flag to
  // know when to exit.

  public int svc ()
  {
    // Keep reading stdin, until we reach EOF. 

      DataInputStream in = new DataInputStream (System.in);
      String msg = null;
      try
	{
	  while (true)
	    {
	      System.out.print ("Enter input: ");
	      System.out.flush ();
	      msg = in.readLine ();
	      if (msg == null)
		{
		  // Send a shutdown message to the other thread and exit.
		  if (this.putNext (new MessageBlock (0), new TimeValue ()) == -1)
		    ACE.ERROR ("putNext");
		  break;
		}
	      else
		{
		  // Send the message to the other thread.
		  if (this.putNext (new MessageBlock (msg), new TimeValue ()) == -1)
		    ACE.ERROR ("putNext");
		}
	    }
	}
      catch (IOException e)
	{
	}
      return 0;
  }
}

class Consumer extends CommonTask
  // = TITLE
  //    Define the Consumer interface. 
{
  // Enqueue the message on the MessageQueue for subsequent
  // handling in the svc() method.
  public int put (MessageBlock mb, TimeValue tv)
    { 
      try
	{
	  return this.putq (mb, tv);
	}
      catch (InterruptedException e)
	{
	}
      return 0;
    }

  // The consumer dequeues a message from the ACE_Message_Queue, writes
  // the message to the stderr stream, and deletes the message.  The
  // Consumer sends a 0-sized message to inform the consumer to stop
  // reading and exit.

  public int svc ()
    {
      MessageBlock mb = null;

      // Keep looping, reading a message out of the queue, until we
      // timeout or get a message with a length == 0, which signals us to
      // quit.
      try
	{
	  while (true)
	    {
	      // Wait for upto 4 seconds
	      mb = this.getq (new TimeValue (4));
	  
	      if (mb == null)
		break;
	  
	      int length = mb.length ();
	  
	      if (length > 0)
		System.out.println ("\n" + mb.base ());

	      if (length == 0)
		break;
	    }
	}
      catch (InterruptedException e)
	{
	}
      if (mb == null)
	{
	  ACE.ERROR ("timed out waiting for message");
	  System.exit (1);
	}
      return 0;
    }
}

// Spawn off a new thread.

public class BufferStreamTest
{
  public static void main (String args[])
  {
    // Control hierachically-related active objects
    Stream stream = new Stream ();
    Module pm = new Module ("Consumer", new Consumer (), null, null);
    Module cm = new Module ("Producer", new Producer (), null, null);

    // Create Producer and Consumer Modules and push them onto the
    // STREAM.  All processing is performed in the STREAM.

    if (stream.push (pm) == -1)
      {
	ACE.ERROR ("push");
	return;
      }
    else if (stream.push (cm) == -1)
      {
	ACE.ERROR ("push");
	return;
      }
  }
}
