package tests.Concurrency.Condition;

import ACE.ASX.TimeoutException;
import ACE.ASX.TimeValue;
import ACE.Concurrency.*;

public class SimpleMessageQueue
{
  private int num_items_ = 0;
  private int head_ = 0, tail_ = 0;
  private Object[] queue_;

  private Mutex lock_ = new Mutex ();
  private Condition notFull_ = new Condition (lock_);
  private Condition notEmpty_ = new Condition (lock_);
  
  public SimpleMessageQueue(int size)
  {
    queue_ = new Object[size];
  }
 
   public void enqueue(Object element, TimeValue timeout)
    throws TimeoutException, InterruptedException
  {
    try 
      {
	lock_.acquire ();    
	while (this.isFull ())
	  notFull_.Wait (timeout);
	
	if (tail_ == queue_.length)
	  tail_ = 0;
	queue_[tail_] = element;
	tail_++;

	num_items_++;
	notEmpty_.signal ();
      }
    finally
      {
	lock_.release ();
      }
  }

  public Object dequeue (TimeValue timeout) 
       throws TimeoutException, InterruptedException
  {
    Object return_value = null;
	
    try 
      {
	lock_.acquire ();
	while (this.isEmpty ())
	  notEmpty_.Wait (timeout);
	
	return_value = queue_[head_];
	head_++;
	if (head_ == queue_.length)
	  head_ = 0;

	num_items_--;    
	notFull_.signal ();
      }
    finally 
      {
	lock_.release ();
      }
    return return_value;
  }

  public boolean isEmpty()
  {
    return num_items_ == 0;
  }

  public boolean isFull()
  {
    return num_items_ == queue_.length;
  }

  public int size()
  {
    return num_items_;
  }
}

