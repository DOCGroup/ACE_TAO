//File: Producer.java
//Seth Widoff 8/8/96
//This class attempts at random intervals to enqueue random elements
//into a queue. If the queue is full the thread waits until an element
//has been dequeued and another thread has invoked the notify() method.

package tests.Concurrency.Condition;

import ACE.ASX.TimeValue;
import java.util.Random;

public class Producer implements Runnable
{
  //Maximum pause between enqueues (in milliseconds)
  private static final int MAX_PAUSE = 1000;
  
  private SimpleMessageQueue queue_;
  private boolean stop_requested_ = false;
  private String name_;
  private int iterations_;
  private TimeValue timeout_;

  public Producer(String name, 
		  SimpleMessageQueue queue, 
		  int iterations,
		  TimeValue timeout)
  {
    name_ = "Producer " + name;
    queue_ = queue;
    iterations_ = iterations;
    timeout_ = timeout;
  }

  public void run()
  {
    //Set the random number generator seed to the current time in milliseconds.
    Random random = new Random(System.currentTimeMillis());
    int element = 1;
    
    for (int i = 0; i < iterations_; )
      {
	try
	  {
	    //	    element = random.nextInt();

	    queue_.enqueue((Object)new Integer(element), timeout_);
	    System.out.print("Producer::run() " + name_ + " enqueued " + element);
	    System.out.println(" Queue size: " + queue_.size());
	    
	    Thread.sleep(random.nextLong() % MAX_PAUSE);
	    i++;
	    element++;
	  }
	catch(Exception excp)
	  {
	    System.out.print("Producer::run() Exception: ");
	    System.out.println(excp);
	  }
      }
  }

  public void requestStop()
  {
    stop_requested_ = true;
  }
}

