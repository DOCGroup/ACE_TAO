/*************************************************
 *
 * = PACKAGE
 *    ACE.ASX
 *
 * = FILENAME
 *    Stream.java
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
 *     This class is the primary abstraction for the ASX framework.
 *     It is moduled after System V Stream.
 *</blockquote>
 *
 * <h2>DESCRIPTION</h2>
 *<blockquote>
 *   A Stream consists of a stack of <Modules>, each of which
 *   contains two <Tasks>.
 *</blockquote>
 */

public class Stream
{

  public Stream ()
  {
    this (null, null, null);
  }

  // Create a Stream consisting of <head> and <tail> as the Stream
  // head and Stream tail, respectively.  If these are 0 then the
  // <ACE_Stream_Head> and <ACE_Stream_Tail> are used, respectively.
  // <arg> is the value past in to the open() methods of the tasks.

  public Stream (Object a,
		 Module head,
		 Module tail)
  {
    this.linkedUs_ = null;
    // this.final_close_ = this.lock_;

    if (this.open (a, head, tail) == -1)
      ACE.ERROR ("open" + head.name () + " " +  tail.name ());
  }

  public int push (Module newTop)
  {
    if (this.pushModule  (newTop, 
			  this.streamHead_.next (), 
			  this.streamHead_) == -1)
      return -1;
    else
      return 0;
  }

  public int put (MessageBlock mb, TimeValue tv)
  {
    return this.streamHead_.writer ().put (mb, tv);
  }

  public MessageBlock get (TimeValue tv) throws InterruptedException
  {
    return this.streamHead_.reader ().getq (tv);
  }

// Return the "top" ACE_Module in a ACE_Stream, skipping over the
// stream_head.

  public Module top ()
  {
    if (this.streamHead_.next () == this.streamTail_)
      return null;
    else
      return this.streamHead_.next ();
  }

// Remove the "top" ACE_Module in a ACE_Stream, skipping over the
// stream_head.

  public int pop (long flags)
  {
    if (this.streamHead_.next () == this.streamTail_)
      return -1;
    else
      {
	// Skip over the ACE_Stream head.
	Module top = this.streamHead_.next ();
	Module newTop = top.next ();

	this.streamHead_.next (newTop);

	// Close the top ACE_Module.

	top.close (flags);

	this.streamHead_.writer ().next (newTop.writer ());
	newTop.reader ().next (this.streamHead_.reader ());

	return 0;
      }
  }

// Remove a named ACE_Module from an arbitrary place in the
// ACE_Stream.

  public int remove (String name, long flags)
  {
    Module prev = null;

    for (Module mod = this.streamHead_; 
	 mod != null; mod = mod.next ())
      if (name.compareTo (mod.name ()) == 0)
	{
	  if (prev == null) // Deleting ACE_Stream Head
	    this.streamHead_.link (mod.next ());
	  else
	    prev.link (mod.next ());
	  
	  mod.close (flags);
	  return 0;
	}
      else
	prev = mod;

    return -1;
  }

  public Module find (String name)
  {
    for (Module mod = this.streamHead_; 
	 mod != null; 
	 mod = mod.next ())
      if (name.compareTo (mod.name ()) == 0)
	return mod;

    return null;
  }

// Actually push a module onto the stack...

  private int pushModule (Module newTop, 
			  Module currentTop, 
			  Module head)
  {
    Task ntReader = newTop.reader ();
    Task ntWriter = newTop.writer ();
    Task ctReader = null;
    Task ctWriter = null;

    if (currentTop != null)
      {
      ctReader = currentTop.reader ();
      ctWriter = currentTop.writer ();
      ctReader.next (ntReader);
      }

    ntWriter.next (ctWriter);

    if (head != null)
      {
	if (head != newTop)
	  head.link (newTop);
      }
    else
      ntReader.next (null);

    newTop.next (currentTop);

    if (ntReader.open (newTop.arg ()) == -1)
      return -1;

    if (ntWriter.open (newTop.arg ()) == -1)
      return -1;
    return 0;
  }

  public synchronized int open (Object a, 
				Module head, 
				Module tail)
  {
    Task h1 = null, h2 = null;
    Task t1 = null, t2 = null;

    if (head == null)
      {
	h1 = new StreamHead ();
	h2 = new StreamHead ();
	head = new Module ("ACEStreamHead", h1, h2, a);
      }

    if (tail == null)
      {
	t1 = new StreamTail ();
	t2 = new StreamTail ();
	tail = new Module ("ACEStreamTail", 
			   t1, t2, a);
      }

    // Make sure *all* the allocation succeeded!
    if (h1 == null || h2 == null || head == null
	|| t1 == null || t2 == null || tail == null)
      {
      // Close up!
	head.close (0);
	tail.close (0);
	return -1;
      }

    this.streamHead_ = head;
    this.streamTail_ = tail;

    if (this.pushModule (this.streamTail_,
			 null, null) == -1)
      return -1;
    else if (this.pushModule (this.streamHead_, 
			      this.streamTail_, 
			      this.streamHead_) == -1)
      return -1;
    else
      return 0;
  }

  public synchronized int close (long flags)
  {
    if (this.streamHead_ != null
	&& this.streamTail_ != null)
      {
	// Don't bother checking return value here.
	this.unlinkInternal ();

	int result = 0;

	// Remove and cleanup all the intermediate modules.

	while (this.streamHead_.next () != this.streamTail_)
	  {
	    if (this.pop (flags) == -1)
	      result = -1;
	  }
  
	// Clean up the head and tail of the stream.
	if (this.streamHead_.close (flags) == -1)
	  result = -1;
	if (this.streamTail_.close (flags) == -1)
	  result = -1;

	this.streamHead_ = null;
	this.streamTail_ = null;

	// Tell all threads waiting on the close that we are done.
	//      this.final_close_.broadcast ();
	return result;
      }
    return 0;
  }

  public int control (int cmd, Object a) throws InterruptedException
  {
    IOCntlMsg ioc = new IOCntlMsg (cmd);

    // Create a data block that contains the user-supplied data.
    MessageBlock db = 
      new MessageBlock (MessageType.MB_IOCTL, 
			null, 
			a);

    // Create a control block that contains the control field and a
    // pointer to the data block.
    MessageBlock cb = 
      new MessageBlock (MessageType.MB_IOCTL, 
			db, 
			(Object) ioc);

    int result = 0;

    if (this.streamHead_.writer ().put (cb, new TimeValue ()) == -1)
      result = -1;
    else if ((cb = this.streamHead_.reader ().getq (new TimeValue ())) == null)
      result = -1;
    else
      result = ((IOCntlMsg ) cb.obj ()).rval ();

    return result;
  }

// Link two streams together at their bottom-most Modules (i.e., the
// one just above the Stream tail).  Note that all of this is premised
// on the fact that the Stream head and Stream tail are non-NULL...
// This must be called with locks held.

  private int linkInternal (Stream us)
  {
    this.linkedUs_ = us;
    // Make sure the other side is also linked to us!
    us.linkedUs_ = this;

    Module myTail = this.streamHead_;

    if (myTail == null)
      return -1;

    // Locate the module just above our Stream tail. 
    while (myTail.next () != this.streamTail_)
      myTail = myTail.next ();

    Module otherTail = us.streamHead_;

    if (otherTail == null)
      return -1;

  // Locate the module just above the other Stream's tail.
    while (otherTail.next () != us.streamTail_)
      otherTail = otherTail.next ();

    // Reattach the pointers so that the two streams are linked!
    myTail.writer ().next (otherTail.reader ());
    otherTail.writer ().next (myTail.reader ());
    return 0;
  }

  public synchronized int link (Stream us)
  {
    return this.linkInternal (us);
  }  

// Must be called with locks held...

  private int unlinkInternal ()
  {
  // Only try to unlink if we are in fact still linked!
    
    if (this.linkedUs_ != null)
    {
      Module myTail = this.streamHead_;

      // Only relink if we still exist!
      if (myTail != null)
	{
	  // Find the module that's just before our stream tail.
	  while (myTail.next () != this.streamTail_)
	    myTail = myTail.next ();

	  // Restore the writer's next() link to our tail.
	  myTail.writer ().next (this.streamTail_.writer ());
	}

      Module otherTail = this.linkedUs_.streamHead_;

      // Only fiddle with the other side if it in fact still remains.
      if (otherTail != null)
	{
	  while (otherTail.next () != this.linkedUs_.streamTail_)
	    otherTail = otherTail.next ();

	  otherTail.writer ().next (this.linkedUs_.streamTail_.writer ());
	  
	}

      // Make sure the other side is also aware that it's been unlinked!
      this.linkedUs_.linkedUs_ = null;

      this.linkedUs_ = null;
      return 0;
    }
    else
      return -1;
  }

  public synchronized int unlink ()
  {
    return this.unlinkInternal ();
  }

  public void dump ()
  {
    ACE.DEBUG ("-------- module links --------");

    for (Module mp = this.streamHead_; ; mp = mp.next ())
      {
	ACE.DEBUG ("module name = " + mp.name ());
	if (mp == this.streamTail_)
	  break;
      }
    
    ACE.DEBUG ("-------- writer links --------");
    
    Task tp;
    
    for (tp = this.streamHead_.writer (); ; tp = tp.next ())
      {
	ACE.DEBUG ("writer queue name = " + tp.name ());
	tp.dump ();
	ACE.DEBUG ("-------\n");
	if (tp == this.streamTail_.writer ()
	    || (this.linkedUs_ != null && tp == this.linkedUs_.streamHead_.reader ()))
	  break;
      }

    ACE.DEBUG ("-------- reader links --------\n");
    for (tp = this.streamTail_.reader (); ; tp = tp.next ())
      {
	ACE.DEBUG ("reader queue name = " + tp.name ());
	tp.dump ();
	ACE.DEBUG ("-------\n");
	if (tp == this.streamHead_.reader ()
	    || (this.linkedUs_ != null && tp == this.linkedUs_.streamHead_.writer ()))
	  break;
      }
  }

  Module streamHead_ = null;
  // Pointer to the head of the stream.

  Module streamTail_ = null;
  // Pointer to the tail of the stream.

  Stream linkedUs_ = null;
  // Pointer to an adjoining linked stream.

  // = Synchronization objects used for thread-safe streams.
  //  ACE_SYNCH_MUTEX lock_;
  // Protect the stream against race conditions.
  
  //  ACE_SYNCH_CONDITION final_close_;
  // Use to tell all threads waiting on the close that we are done.

}


