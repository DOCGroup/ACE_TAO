// $Id$


import java.util.Enumeration;

public class Queue
{
  // Friendly?
  Queue_Node head_ = null;
  Queue_Node tail_ = null;

  private int length_ = 0;

  public Queue()
    {
    }

  public Enumeration forward_iterator()
    {
      return new Queue_Iterator(this, Queue_Iterator.FORWARD);
    }

  public Enumeration reverse_iterator()
    {
      return new Queue_Iterator(this, Queue_Iterator.REVERSE);
    }

  public void enqueue_tail(Object new_data)
    {
      Queue_Node new_node = new Queue_Node(new_data);

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

      length_++;
    }

  public void enqueue_head(Object new_data)
    {
      Queue_Node new_node = new Queue_Node(new_data);

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

      length_++;
    }

  public Object dequeue_head()
    {
      Object return_value = null;

      if (head_ == null)
	return_value = null;
      else if (tail_ == head_)
	{
	  return_value = head_.data_;
	  tail_ = null;
	  head_ = null;
	}
      else
	{
	  return_value = head_.data_;
	  head_ = head_.next_;
	  head_.prev_ = null;
	}

      length_--;

      return return_value;
    }

  public Object dequeue_tail()
    {
      Object return_value = null;

      if (tail_ == null)
	return_value = null;
      else if (tail_ == head_)
	{
	  return_value = tail_.data_;
	  tail_ = null;
	  head_ = null;
	}
      else
	{
	  return_value = tail_.data_;
	  tail_ = tail_.prev_;
	  tail_.next_ = null;
	}

      length_--;

      return return_value;
    }

  public int length()
    {
      return length_;
    }

  public Object head()
    {
      if (head_ != null)
	return head_.data_;
      else
	return null;
    }

  public Object tail()
    {
      if (tail_ != null)
	return tail_.data_;
      else
	return null;
    }
}

class Queue_Node
{
  public Queue_Node prev_ = null;
  public Queue_Node next_ = null;;
  public Object data_;

  public Queue_Node(Object data)
    {
      data_ = data;
    }
}

class Queue_Iterator implements Enumeration
{
  public static final boolean FORWARD = true;
  public static final boolean REVERSE = false;

  private Queue queue_;
  private Queue_Node queue_ptr_;
  private boolean direction_ = FORWARD;

  public Queue_Iterator(Queue queue)
    {
      queue_ = queue;
      queue_ptr_ = queue.head_;
    }

  public Queue_Iterator(Queue queue, boolean direction)
    {
      queue_ = queue;
      direction_ = direction;

      if (direction_)
	queue_ptr_ = queue_.head_;
      else
	queue_ptr_ = queue_.tail_;
    }

  public Object nextElement()
    {
      Object data = queue_ptr_.data_;

      if (direction_)
	queue_ptr_ = queue_ptr_.next_;
      else
	queue_ptr_ = queue_ptr_.prev_;

      return data;
    }

  public boolean hasMoreElements()
    {
      return queue_ptr_ != null;
    }
}
