/*************************************************
 *
 * = PACKAGE
 *    ACE.ASX
 *
 * = FILENAME
 *    MessageQueue.java
 *
 *@author Prashant Jain
 *
 *************************************************/
package ACE.ASX;

import java.util.Date;
import ACE.OS.*;
import ACE.Reactor.*;

class NotFullCondition extends TimedWait
{
  public NotFullCondition (MessageQueue mq)
  { 
    super (mq);
    this.mq_ = mq;
  }

  public boolean condition () {
    // Delegate to the appropriate conditional
    // check on the MessageQueue.
    return !this.mq_.isFull ();
  }
  private MessageQueue mq_;
}

class NotEmptyCondition extends TimedWait
{
  public NotEmptyCondition (MessageQueue mq)
  { 
    super (mq);
    this.mq_ = mq;
  }

  public boolean condition () {
    // Delegate to the appropriate conditional
    // check on the MessageQueue.
    return !this.mq_.isEmpty ();
  }
  private MessageQueue mq_;
}


/**
 * <hr>
 * <h2>SYNOPSIS</h2>
 *<blockquote>
 *     A thread-safe message queueing facility, modeled after the
 *     queueing facilities in System V StreamS.
 *</blockquote>
 *
 * <h2>DESCRIPTION</h2>
 *</blockquote>
 *     <tt>MessageQueue</tt> is the central queueing facility for messages
 *     in the ASX framework. All operations are thread-safe, as it is intended
 * to be used for inter-thread communication (<em>e.g.</em>, a producer and
 * consumer thread joined by a <tt>MessageQueue</tt>).  The queue
 * consiste of <tt>MessageBlock</tt>s.
 *</blockquote>
 *
 *@see MessageBlock,TimeValue
 */
public class MessageQueue
{
  /**
   * Default constructor
   */
  public MessageQueue ()
  {
    this (DEFAULT_HWM, DEFAULT_LWM);
  }

  /**
   * Create a Message Queue with high and low water marks.
   *@param hwm High water mark (max number of bytes allowed in the
   * queue) 
   *@param lwm Low water mark (min number of bytes in the queue)
   */
  public MessageQueue (int hwm, int lwm)
  {
    if (this.open (hwm, lwm) == -1)
      ACE.ERROR ("open");
  }

  /**
   * Initialize a Message Queue with high and low water marks.
   *@param hwm High water mark (max number of bytes allowed in the
   * queue) 
   *@param lwm Low water mark (min number of bytes in the queue)
   */
  public synchronized int open (int hwm, int lwm)
  {
    this.highWaterMark_ = hwm;
    this.lowWaterMark_ = lwm;
    this.deactivated_ = false;
    this.currentBytes_	= 0;
    this.currentCount_	= 0;
    this.tail_ = null;
    this.head_ = null;
    return 0;
  }

  // = For enqueue, enqueueHead, enqueueTail, and dequeueHead if
  // timeout is specified, the caller will wait for amount of time in
  // tv.  Calls will return, however, when queue is closed,
  // deactivated, or if the time specified in tv elapses.

  /**
   * Enqueue a <MessageBlock> into the <MessageQueue> in accordance
   * with its <msgPriority> (0 is lowest priority). Note that the
   * call will block (unless the queue has been deactivated).
   *@param newItem item to enqueue onto the Message Queue
   *@return -1 on failure, else the number of items still on the queue.
   */
  public synchronized int enqueue (MessageBlock newItem) throws InterruptedException
  {
    return this.enqueue (newItem, null);
  }

  /**
   * Enqueue a <MessageBlock> into the <MessageQueue> in accordance
   * with its <msgPriority> (0 is lowest priority). Note that the
   * call will return if <timeout> amount of time expires or if the
   * queue has been deactivated.
   *@param newItem item to enqueue onto the Message Queue
   *@param tv amount of time (TimeValue) to wait before returning
   * (unless operation completes before)
   *@return -1 on failure, else the number of items still on the
   * queue. 
   */
  public synchronized int enqueue (MessageBlock newItem, 
				   TimeValue tv) throws InterruptedException
  {
    int result = -1;
    if (this.deactivated_)
      return -1;
    try
      {
	if (tv == null)  // Need to do a blocking wait
	  notFullCondition_.timedWait ();
	else             // Need to do a timed wait
	  notFullCondition_.timedWait (tv);
      }
    catch (TimeoutException e)
      {
	return -1;
      }
	  
    // Check again if queue is still active
    if (this.deactivated_)
      return -1;
    else
      result = this.enqueueInternal (newItem);

    // Tell any blocked threads that the queue has a new item!
    this.notEmptyCondition_.broadcast ();
    return result;
  }

  /**
     * Enqueue a <MessageBlock> at the end of the <MessageQueue>. Note
     * that the call will block (unless the queue has been deactivated).
     *@param newItem item to enqueue onto the Message Queue
     *@return -1 on failure, else the number of items still on the queue.
     */
  public synchronized int enqueueTail (MessageBlock newItem) throws InterruptedException
  {
    return this.enqueueTail (newItem, null);
  }

  /**
     * Enqueue a <MessageBlock> at the end of the <MessageQueue>. Note
     * that the call will return if <timeout> amount of time expires or
     * if the queue has been deactivated.
     *@param newItem item to enqueue onto the Message Queue
     *@param tv amount of time (TimeValue) to wait before returning
     * (unless operation completes before)
     *@return -1 on failure, else the number of items still on the queue.
     */
  public synchronized int enqueueTail (MessageBlock newItem,
				       TimeValue tv) throws InterruptedException
  {
    int result = -1;
    if (this.deactivated_)
      return -1;
    try
      {
	if (tv == null)  // Need to do a blocking wait
	  notFullCondition_.timedWait ();
	else             // Need to do a timed wait
	  notFullCondition_.timedWait (tv);
      }
    catch (TimeoutException e)
      {
	return -1;
      }
	  
    // Check again if queue is still active
    if (this.deactivated_)
      return -1;
    else
      result = this.enqueueTailInternal (newItem);

    // Tell any blocked threads that the queue has a new item!
    this.notEmptyCondition_.broadcast ();
    return result;
  }

  /**
     * Enqueue a <MessageBlock> at the head of the <MessageQueue>. Note
     * that the call will block (unless the queue has been deactivated).
     *@param newItem item to enqueue onto the Message Queue
     *@return -1 on failure, else the number of items still on the queue.
     */
  public synchronized int enqueueHead (MessageBlock newItem) throws InterruptedException
  {
    return this.enqueueHead (newItem, null);
  }

  /**
     * Enqueue a <MessageBlock> at the head of the <MessageQueue>. Note
     * that the call will return if <timeout> amount of time expires or
     * if the queue has been deactivated.
     *@param newItem item to enqueue onto the Message Queue
     *@param tv amount of time (TimeValue) to wait before returning
     * (unless operation completes before)
     *@return -1 on failure, else the number of items still on the queue.
     */
  public synchronized int enqueueHead (MessageBlock newItem,
				       TimeValue tv) throws InterruptedException
  {
    int result = -1;
    if (this.deactivated_)
      return -1;
    try
      {
	if (tv == null)  // Need to do a blocking wait
	  notFullCondition_.timedWait ();
	else             // Need to do a timed wait
	  notFullCondition_.timedWait (tv);
      }
    catch (TimeoutException e)
      {
	return -1;
      }
    
    // Check again if queue is still active
    if (this.deactivated_)
      return -1;
    else
      result = this.enqueueHeadInternal (newItem);

    // Tell any blocked threads that the queue has a new item!
    this.notEmptyCondition_.broadcast ();
    return result;
  }

  /**
     * Dequeue and return the <MessageBlock> at the head of the
     * <MessageQueue>. Note that the call will block (unless the queue
     * has been deactivated). 
     *@return null on failure, else the <MessageBlock> at the head of queue.
     */
  public synchronized MessageBlock dequeueHead () throws InterruptedException
  {
    return this.dequeueHead (null);
  }

  /**
     * Dequeue and return the <MessageBlock> at the head of the
     * <MessageQueue>. Note that the call will return if <timeout>
     * amount of time expires or if the queue has been deactivated.
     *@return null on failure, else the <MessageBlock> at the head of queue.
     */
  public synchronized MessageBlock dequeueHead (TimeValue tv) throws InterruptedException
  {
    MessageBlock result = null;
    if (this.deactivated_)
      return null;
    try
      {
	if (tv == null)  // Need to do a blocking wait
	  notEmptyCondition_.timedWait ();
	else             // Need to do a timed wait
	  notEmptyCondition_.timedWait (tv);
      }
    catch (TimeoutException e)
      {
	return null;
      }
	  
    // Check again if queue is still active
    if (this.deactivated_)
      return null;
    else
      result = this.dequeueHeadInternal ();

    // Tell any blocked threads that the queue has room for an item!
    this.notFullCondition_.broadcast ();
    return result;
  }

  /**
     * Check if queue is full.
     *@return true if queue is full, else false.
     */
  public synchronized boolean isFull ()
  {
    return this.isFullInternal ();
  }

  /**
     * Check if queue is empty.
     *@return true if queue is empty, else false.
     */
  public synchronized boolean isEmpty ()
  {
    return this.isEmptyInternal ();
  }

  /**
     * Get total number of bytes on the queue.
     *@return total number number of bytes on the queue
     */
  public int messageBytes ()
  {
    return this.currentBytes_;
  }

  /**
     * Get total number of messages on the queue.
     *@return total number number of messages on the queue
     */
  public int messageCount ()
  {
    return this.currentCount_;
  }

  // = Flow control routines 

    /**
     * Get high watermark.
     *@return high watermark
     */
  public int highWaterMark ()
  {
    return this.highWaterMark_;
  }

  /**
     * Set high watermark.
     *@param hwm high watermark
     */
  public void highWaterMark (int hwm)
  {
    this.highWaterMark_ = hwm;
  }

  /**
     * Get low watermark.
     *@return low watermark
     */
  public int lowWaterMark ()
  {
    return this.lowWaterMark_;
  }

  /**
     * Set low watermark.
     *@param lwm low watermark
     */
  public void lowWaterMark (int lwm)
  {
    this.lowWaterMark_ = lwm;
  }

  // = Activation control methods.

    /**
     * Deactivate the queue and wakeup all threads waiting on the queue
     * so they can continue.  No messages are removed from the queue,
     * however.  Any other operations called until the queue is
     * activated again will immediately return -1.  
     *@return WAS_INACTIVE if queue was inactive before the call and
     * WAS_ACTIVE if queue was active before the call.
     */
  public synchronized int deactivate ()
  {
    return this.deactivateInternal ();
  }


  /**
     * Reactivate the queue so that threads can enqueue and dequeue
     * messages again.  
     *@return WAS_INACTIVE if queue was inactive before the call and
     * WAS_ACTIVE if queue was active before the call.
     */
  public synchronized int activate ()
  {
    return this.activateInternal ();
  }

  protected boolean isEmptyInternal ()
  {
    // Not sure about this one!!!!
    return this.currentBytes_ <= this.lowWaterMark_ && this.currentCount_ <= 0;
  }

  protected boolean isFullInternal ()
  {
    return this.currentBytes_ > this.highWaterMark_;
  }

  protected int deactivateInternal ()
  {
    int currentStatus = 
      this.deactivated_ ? WAS_INACTIVE : WAS_ACTIVE;
      
    this.notFullCondition_.broadcast ();
    this.notEmptyCondition_.broadcast ();
      
    this.deactivated_ = true;
    return currentStatus;
  }

  protected int activateInternal ()
  {
    int currentStatus = 
      this.deactivated_ ? WAS_INACTIVE : WAS_ACTIVE;
    this.deactivated_ = false;

    return currentStatus;
  }

  protected int enqueueTailInternal (MessageBlock newItem)
  {
    if (newItem == null)
      return -1;

    // List was empty, so build a new one.
    if (this.tail_ == null)
      {
	this.head_     = newItem;
	this.tail_     = newItem;
	newItem.next (null);
	newItem.prev (null);
      }
    // Link at the end.
    else 
      {
	newItem.next (null);
	this.tail_.next (newItem);
	newItem.prev (this.tail_);
	this.tail_ = newItem;
      }

    if (newItem.msgType() != MessageType.MB_OBJECT)
      {
	// Make sure to count *all* the bytes in a composite message!!! 
	for (MessageBlock temp = newItem; 
	     temp != null; 
	     temp = temp.cont ())
	  this.currentBytes_ += temp.size ();
      }

    this.currentCount_++;
    return this.currentCount_;
  }

  protected int enqueueHeadInternal (MessageBlock newItem)
  {
    if (newItem == null)
      return -1;

    newItem.prev (null);
    newItem.next (this.head_);

    if (this.head_ != null)
      this.head_.prev (newItem);
    else
      this.tail_ = newItem;

    this.head_ = newItem;

    if (newItem.msgType() != MessageType.MB_OBJECT)
      {
	// Make sure to count *all* the bytes in a composite message!!!
	for (MessageBlock temp = newItem; 
	     temp != null; 
	     temp = temp.cont ())
	  this.currentBytes_ += temp.size ();
      }

    this.currentCount_++;

    return this.currentCount_;
  }

  protected int enqueueInternal (MessageBlock newItem)
  {
    if (newItem == null)
      return -1;

    if (this.head_ == null)
      // Check for simple case of an empty queue, where all we need to
      // do is insert <newItem> into the head.
      return this.enqueueHeadInternal (newItem);
    else
      {
	MessageBlock temp;

	// Figure out where the new item goes relative to its priority.

	for (temp = this.head_;
	     temp != null;
	     temp = temp.next ())
	  {
	    if (temp.msgPriority () <= newItem.msgPriority ())
	      // Break out when we've located an item that has lower
	      // priority that <newItem>.
	      break;
	  }

	if (temp == null)
	  // Check for simple case of inserting at the end of the queue,
	  // where all we need to do is insert <newItem> after the
	  // current tail.
	  return this.enqueueTailInternal (newItem);
	else if (temp.prev () == null)
	  // Check for simple case of inserting at the beginning of the
	  // queue, where all we need to do is insert <newItem> before
	  // the current head.
	  return this.enqueueHeadInternal (newItem);
	else
	  {
	    // Insert the message right before the item of equal or lower
	    // priority.
	    newItem.next (temp);
	    newItem.prev (temp.prev ());
	    temp.prev ().next (newItem);
	    temp.prev (newItem);
	  }
      }

    if (newItem.msgType() != MessageType.MB_OBJECT)
      {    
	// Make sure to count *all* the bytes in a composite message!!!
	for (MessageBlock temp = newItem; 
	     temp != null; 
	     temp = temp.cont ())
	  this.currentBytes_ += temp.size ();
      }

    this.currentCount_++;
    return this.currentCount_;
  }

  protected MessageBlock dequeueHeadInternal ()
  {
    MessageBlock firstItem = this.head_;
    this.head_ = this.head_.next ();

    if (this.head_ == null)
      this.tail_ = null;

    if (firstItem.msgType() != MessageType.MB_OBJECT)
      {
	// Make sure to subtract off all of the bytes associated with this
	// message.
	for (MessageBlock temp = firstItem; 
	     temp != null; 
	     temp = temp.cont ())
	  this.currentBytes_ -= temp.size ();
      }

    this.currentCount_--;
    return firstItem;
  }

  
  /** Default high watermark (16 K). */
  public final static int DEFAULT_HWM = 16 * 1024;

  /** Default low watermark. */
  public final static int DEFAULT_LWM = 0;

  /** Message queue was active before activate() or deactivate(). */
  public final static int WAS_ACTIVE = 1;
  
  /** Message queue was inactive before activate() or deactivate(). */
  public final static int WAS_INACTIVE = 2;

  private int highWaterMark_;
  // Greatest number of bytes before blocking.

  private int lowWaterMark_;
  // Lowest number of bytes before unblocking occurs.

  private boolean deactivated_;
  // Indicates that the queue is inactive.

  private int currentBytes_;
  // Current number of bytes in the queue.

  private int currentCount_;
  // Current number of messages in the queue.

  private MessageBlock head_;
  // Head of Message_Block list.

  private MessageBlock tail_;
  // Tail of Message_Block list.

  // The Delegated Notification mechanisms.
  private NotFullCondition notFullCondition_ = new NotFullCondition (this);
  private NotEmptyCondition notEmptyCondition_ = new NotEmptyCondition (this);

}
