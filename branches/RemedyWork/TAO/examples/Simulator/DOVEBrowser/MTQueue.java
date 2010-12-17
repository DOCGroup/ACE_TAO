// $Id$

// This is a queue class for use by multiple threads, with at least one
// thread enqueueing objects and another dequeueing them.  The dequeue_*
// methods block the thread if there are no more objects in the queue,
// while the try_dequeue_* methods do not block but rather return a
// null reference if there is nothing in the queue.
public class MTQueue
{
  // Initially, the head and tail of the queue are null
  MTQueue_Node head_ = null;
  MTQueue_Node tail_ = null;

  // Constructor - does nothing.
  public MTQueue ()
    {
    }

  // Places a passed Object at the end of the queue.
  public synchronized void enqueue_tail (Object new_data)
    {
      // Create a new node to hold the object.
      MTQueue_Node new_node = new MTQueue_Node(new_data);

      // Insert the node into the queue.
      if (tail_ == null)
	{
	  tail_ = new_node;
	  head_ = new_node;
	}
      else
	{
	  new_node.prev_ = tail_;
	  tail_.next_ = new_node;
	  tail_ = new_node;
	}

      // Wake up any waiting threads
      notifyAll ();
    }

  // Places a passed Object at the front of the queue.
  public synchronized void enqueue_head(Object new_data)
    {
      // Create a new node to hold the object.
      MTQueue_Node new_node = new MTQueue_Node(new_data);

      // Insert the node into the queue.
      if (head_ == null)
	{
	  tail_ = new_node;
	  head_ = new_node;
	}
      else
	{
	  new_node.next_ = head_;
	  head_.prev_ = new_node;
	  head_ = new_node;
	}

      // Wake up any waiting threads
      notifyAll ();
    }

  // Try to remove an object from the head of the queue - nonblocking.
  public synchronized Object try_dequeue_head()
  {
      // Start with a null reference.
      Object return_value = null;

      // If there's anything there, dequeue it.
      if (head_ != null)
        {
          return_value = dequeue_head ();
        }

      // Return what we found, if anything.
      return return_value;
  }

  // Remove an object from the head of the queue - blocking.
  public synchronized Object dequeue_head()
    {
      // Start with a null reference.
      Object return_value = null;

      // Wait until there's something to dequeue.
      while (head_ == null)
        {
          try
            {
               wait ();
            }
          catch (InterruptedException e)
            {
              return return_value;
            }
        }

      // Dequeue the object at the head of the queue.  Make sure
      // to null out references within dequeued nodes to prevent
      // out of memory errors.
      if (tail_ == head_)
	{
	  return_value = head_.data_;
          head_.next_ = null;
          head_.prev_ = null;
          head_.data_ = null;
	  tail_ = null;
	  head_ = null;
	}
      else
	{
	  return_value = head_.data_;
	  head_ = head_.next_;
          head_.prev_.next_ = null;
          head_.prev_.prev_ = null;
          head_.prev_.data_ = null;
	  head_.prev_ = null;
	}

      // Return the object we dequeued.
      return return_value;
    }

  // Try to remove an object from the tail of the queue - nonblocking.
  public synchronized Object try_dequeue_tail ()
  {
      // Start with a null reference.
      Object return_value = null;

      // If there's anything there, dequeue it.
      if (tail_ != null)
        {
          return_value = dequeue_tail ();
        }

      // Return what we found, if anything.
      return return_value;
  }

  // Remove an object from the tail of the queue - blocking.
  public synchronized Object dequeue_tail ()
    {
      // Start with a null reference.
      Object return_value = null;

      // Wait until there's something to dequeue.
      while (tail_ == null)
        {
          try
            {
               wait ();
            }
          catch (InterruptedException e)
            {
              return return_value;
            }
        }

      // Dequeue the object at the back of the queue.  Make sure
      // to null out references within dequeued nodes to prevent
      // out of memory errors.
      if (tail_ == head_)
	{
	  return_value = tail_.data_;
          tail_.data_ = null;
          tail_.next_ = null;
          tail_.prev_ = null;
	  tail_ = null;
	  head_ = null;
	}
      else
	{
	  return_value = tail_.data_;
	  tail_ = tail_.prev_;
          tail_.next_.data_ = null;
          tail_.next_.next_ = null;
          tail_.next_.prev_ = null;
	  tail_.next_ = null;
	}

      // Return the object we dequeued.
      return return_value;
    }
}

class MTQueue_Node
{
  public MTQueue_Node prev_ = null;
  public MTQueue_Node next_ = null;;
  public Object data_;

  public MTQueue_Node(Object data)
    {
      data_ = data;
    }
}


