//File: Consumer.java
//Seth Widoff 8/8/96
//This class attempts at random intervals to dequeue random elements
//from a queue. If the queue is empty the thread waits until an element
//has been enqueued and another thread has invoked the notify() method.
package JACE.tests.Concurrency.Condition;

import java.lang.Math;
import JACE.ASX.TimeValue;
import java.util.Random;

public class Consumer implements Runnable
{
  //Maximum pause between dequeues (in milliseconds)
  private static final int MAX_PAUSE = 1000;

  private SimpleMessageQueue queue_;
  private boolean stop_requested_ = false;
  private String name_;
  private int iterations_;
  private TimeValue timeout_;
  
  public Consumer(String name, 
		  SimpleMessageQueue queue, 
		  int iterations,
		  TimeValue timeout)
  {
    name_ = "Consumer " + name;
    queue_ = queue;
    iterations_ = iterations;
    timeout_ = timeout;
  }

  public void run()
  {
    //Set the random number generator seed to the current time in
    //milliseconds.

    Random random = new Random(System.currentTimeMillis());
    Integer element;

    for (int i = 0; i < iterations_; )
      {
	try
	  {
	    TimeValue absTv = TimeValue.relativeTimeOfDay (timeout_);
	    element = (Integer)queue_.dequeue(absTv);
	    if (element != null)
	      {
		
		System.out.println("Consumer::run() " + name_ + " dequeued " 
				 + element.toString() +
				 " Queue size: " + queue_.size());
		
		Thread.sleep(Math.abs (random.nextLong()) % MAX_PAUSE);
	      }
	    else
	      {
		System.out.println ("Null");
	      }
	    i++;
	  }
	catch(Exception excp)
	  {
	    System.out.print ("Consumer::run() Exception: ");
	    System.out.println(excp);
	  }
      }
  }

  public void requestStop()
  {
    stop_requested_ = true;
  }
}
