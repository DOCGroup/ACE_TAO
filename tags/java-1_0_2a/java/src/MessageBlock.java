/*************************************************
 *
 * = PACKAGE
 *    ACE.ASX
 *
 * = FILENAME
 *    MessageBlock.java
 *
 *@author Prashant Jain
 *
 *************************************************/
package ACE.ASX;

import ACE.OS.*;

/**
 * <hr>
 * <h2>SYNOPSIS</h2>
 *<blockquote>
 *     Object used to store messages in the ASX framework.
 *</blockquote>
 *
 * <h2>DESCRIPTION</h2>
 *<blockquote>
 *     <tt>MessageBlock</tt> is modeled after the message data structures
 *     used in System V STREAMS. A <tt>MessageBlock</tt> is composed of
 *     one or more <tt>MessageBlock</tt>s that are linked together by <em>PREV</em>
 *     and <em>NEXT</em> pointers. In addition, a <tt>MessageBlock</tt> may also be
 *     linked to a chain of other <tt>MessageBlock</tt>s. This structure
 *     enables efficient manipulation of arbitrarily-large messages
 *     <em>without</em> incurring memory copying overhead.
 *</blockquote>
 *
 *@see MessageQueue
 */
public class MessageBlock
{
  /**
   * Create an empty Message Block
   */
  public MessageBlock ()
    {
      this (0);
    }

  /**
   * Create an empty Message Block.
   * Note that this assumes that type of MessageBlock is MB_DATA.
   *@param size size of the Message Block to create.
   */
  public MessageBlock (int size)
    {
      // Note the explicit cast toString() is needed. For some strange
      // reason, it fails otherwise if size == 0.
      this ((new StringBuffer (size)).toString ());
    }

  /**
   * Create a Message Block. Note that this assumes that type of
   * MessageBlock is MB_DATA. 
   *@param data initial data to create a Message Block with.
   */
  public MessageBlock (String data)
    {
      this (MessageType.MB_DATA,
	    null,
	    data);
    }

  /**
   * Create a Message Block.
   *@param type type of the Message Block (must be one of those
   * specified in class Message Type)
   *@param cont next block of data 
   *@param data initial data to create Message Block with
   */
  public MessageBlock (int type,
		       MessageBlock cont,
		       String data)
    {
      this.flags_ = 0;
      this.priority_ = 0;
      this.next_ = null;
      this.prev_ = null;

      this.init (type, cont, data);
    }

  /**
   * Create a Message Block. Note that this assumes that type of
   * MessageBlock is MB_OBJECT.
   *@param obj initial object to create a Message Block with.
   */
  public MessageBlock (Object obj)
  {
    this (MessageType.MB_OBJECT,
	  null,
	  obj);
  }

  /**
   * Create a Message Block.
   *@param type type of the Message Block (must be one of those
   * specified in class Message Type)
   *@param cont next block of data 
   *@param obj initial object to create Message Block with
   */
  public MessageBlock (int type,
		       MessageBlock cont,
		       Object obj)
  {
    this.init (type, cont, obj);
  }

  /* Initialize the Message Block
   *@param data data to initialize Message Block with
   */
  public void init (String data)
    {
      this.base_ = new StringBuffer (data);
    }

  /**
   * Initialize a Message Block.
   *@param type type of the Message Block (must be one of those
   * specified in class Message Type)
   *@param cont next block of data 
   *@param data data to initialize Message Block with
   */
  public void init (int msgType,
		   MessageBlock msgCont,
		   String data)
    {
      if (data.length () == 0)
	this.base_ = new StringBuffer (0);
      else
	this.base_ = new StringBuffer (data);
      this.type_ = msgType;
      this.cont_ = msgCont;
    }

  /**
   * Initialize a Message Block. Note that this assumes that type of
   * MessageBlock is MB_OBJECT.
   *@param obj initial object to initialize a Message Block with.
   */
  public void init (Object obj)
    {
      this.init (MessageType.MB_OBJECT, null, obj);
    }

  /**
   * Initialize a Message Block.
   *@param type type of the Message Block (must be one of those
   * specified in class Message Type)
   *@param cont next block of data 
   *@param obj object to initialize Message Block with
   */
  public void init (int msgType,
		   MessageBlock msgCont,
		   Object obj)
    {
      this.obj_ = obj;
      this.type_ = msgType;
      this.cont_ = msgCont;
      this.flags_ = 0;
      this.priority_ = 0;
      this.next_ = null;
      this.prev_ = null;
    }

  /**
   * Set message flags. Note that the flags will be set on top of
   * already set flags.
   *@param moreFlags flags to set for the Message Block.
   */
  public long setFlags (long moreFlags)
    {
      // Later we might mask more_flags so that user can't change
      // internal ones: more_flags &= ~(USER_FLAGS -1).
      this.flags_ = ACE.SET_BITS (this.flags_, moreFlags);
      return this.flags_;
    }

  /**
   * Unset message flags.
   *@param lessFlags flags to unset for the Message Block.
   */
  public long clrFlags (long lessFlags)
    {
      // Later we might mask more_flags so that user can't change
      // internal ones: less_flags &= ~(USER_FLAGS -1).
      this.flags_ = ACE.CLR_BITS (this.flags_, lessFlags);
      return this.flags_;
    }

  /**
   * Get the message flags.
   *@return Message flags
   */
  public long flags ()
    {
      return this.flags_;
    }

  /**
   * Get the type of the message.
   *@return message type
   */
  public int msgType ()
    {
      return this.type_;
    }

  /**
   * Set the type of the message.
   *@param t type of the message
   */
  public void msgType (int t)
    {
      this.type_ = t;
    }

  /**
   * Get the class of the message. Note there are two classes,
   * <normal> messages and <high-priority> messages.
   *@return message class
   */
  public int msgClass ()
    {
      return this.msgType () >= MessageType.MB_PRIORITY
	? MessageType.MB_PRIORITY : MessageType.MB_NORMAL;
    }

  /**
   * Find out if the message is a data message.
   *@return true if message is a data message, false otherwise
   */
  public boolean isDataMsg ()
    {
      int mt = this.msgType ();
      return mt == MessageType.MB_DATA
	|| mt == MessageType.MB_PROTO
	  || mt == MessageType.MB_PCPROTO;
    }

  /**
   * Find out if the message is an object message.
   *@return true if message is an object message, false otherwise
   */
  public boolean isObjMsg ()
    {
      int mt = this.msgType ();
      return mt == MessageType.MB_OBJECT
	|| mt == MessageType.MB_PROTO
	  || mt == MessageType.MB_PCPROTO;
    }

  /**
   * Get the priority of the message.
   *@return message priority
   */
  public long msgPriority ()
    {
      return this.priority_;
    }

  /**
   * Set the priority of the message.
   *@param pri priority of the message
   */
  public void msgPriority (long pri)
    {
      this.priority_ = pri;
    }

  /**
   * Get message data. This assumes that msgType is MB_DATA.
   *@return message data
   */
  public String base () 
    {
      // Create a String object to return
      char temp[] = new char [this.base_.length ()];
      this.base_.getChars (0, this.base_.length (), temp, 0);
      return new String (temp);
    }

  /**
   * Set the message data. This assumes that msgType is MB_DATA.
   *@param data message data
   *@param msgFlags message flags
   */
  public void base (String data,
		    long msgFlags)
    {
      this.base_ = new StringBuffer (data);
      this.flags_ = msgFlags;
    }

  /**
   * Get message object. This assumes that msgType is MB_OBJECT.
   *@return message object
   */
  public Object obj () 
    {
      return this.obj_;
    }

  /**
   * Set the message object. This assumes that msgType is MB_OBJECT.
   *@param object message object
   *@param msgFlags message flags
   */
  public void obj (Object obj,
		   long msgFlags)
    {
      this.obj_ = obj;
      this.flags_ = msgFlags;
    }

  // = The following four methods only make sense if the Message_Block
  // is of type MB_DATA and not MB_OBJECT.

  /**
   * Get length of the message. This method only makes sense if the
   * MessageBlock is of type MB_DATA and not MB_OBJECT.
   *@return length of the message.
   */
  public int length ()
    {
      return this.base_.length ();
    }

  /**
   * Set the length of the message. This method only makes sense if the
   * MessageBlock is of type MB_DATA and not MB_OBJECT.
   *@param n message length
   */
  public void length (int n)
    {
      this.base_.setLength (n);
    }

  /**
   * Get size of the allocated buffer for the message. This method
   * only makes sense if the MessageBlock is of type MB_DATA and not
   * MB_OBJECT. 
   *@return size of the message buffer
   */
  public int size ()
    {
      return this.base_.capacity ();
    }

  /**
   * Set the total size of the buffer. This method will grow the
   * buffer if need be. Also, this method only makes sense if the
   * MessageBlock is of type MB_DATA and not MB_OBJECT.
   *@param n size of message buffer
   */
  public void size (int n)
    {
      this.base_.ensureCapacity (n);
    }


  /**
   * Get the continuation field. The coninuation field is used to
   * chain together composite messages.
   *@return the continuation field
   */
  public MessageBlock cont ()
    {
      return this.cont_;
    }

  /**
   * Set the continuation field. The coninuation field is used to
   * chain together composite messages.
   *@param msgCont continuation field
   */
  void cont (MessageBlock msgCont)
    {
      this.cont_ = msgCont;
    }

  /**
   * Get link to next message. The next message points to the
   * <MessageBlock> directly ahead in the MessageQueue.
   *@return next message block
   */
  MessageBlock next ()
    {
      return this.next_;
    }

  /**
   * Set link to next message. The next message points to the
   * <MessageBlock> directly ahead in the MessageQueue.
   *@param msgBlock next message block
   */
  void next (MessageBlock msgBlock)
    {
      this.next_ = msgBlock;
    }

  /**
   * Get link to previous message. The previous message points to the
   * <MessageBlock> directly before in the MessageQueue.
   *@return previous message block
   */
  MessageBlock prev ()
    {
      return this.prev_;
    }

  /**
   * Set link to previous message. The previous message points to the
   * <MessageBlock> directly before in the MessageQueue.
   *@param msgBlock previous message block
   */
  void prev (MessageBlock msgBlock)
    {
      this.prev_ = msgBlock;
    }

  private int type_;		
  // Type of message.

  private long flags_;
  // Misc flags.

  private long priority_;
  // Priority of message.

  private StringBuffer base_;
  // String data of message block (initialized to null).

  private Object obj_;
  // Object data of message block (initialized to null).

  private MessageBlock cont_;
  // Next message block in the chain.

  private MessageBlock next_;	
  // Next message in the list.

  private MessageBlock prev_;	
  // Previous message in the list.

}

