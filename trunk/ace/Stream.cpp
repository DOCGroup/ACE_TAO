// Stream.cpp
// $Id$

#if !defined (ACE_STREAM_C)
#define ACE_STREAM_C

#define ACE_BUILD_DLL
//#include "ace/Module.h"
#include "ace/Stream.h"
#include "ace/Stream_Modules.h"

#if !defined (__ACE_INLINE__)
#include "ace/Stream.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_Stream)

// Give some idea of what the heck is going on in a stream!

template <ACE_SYNCH_1> void
ACE_Stream<ACE_SYNCH_2>::dump (void) const
{
  ACE_TRACE ("ACE_Stream<ACE_SYNCH_2>::dump");
  ACE_DEBUG ((LM_DEBUG, "-------- module links --------\n"));

  for (ACE_Module<ACE_SYNCH_2> *mp = this->stream_head_; 
       ; 
       mp = mp->next ())
    {
      ACE_DEBUG ((LM_DEBUG, "module name = %s\n", mp->name ()));
      if (mp == this->stream_tail_)
	break;
    }

  ACE_DEBUG ((LM_DEBUG, "-------- writer links --------\n"));

  ACE_Task<ACE_SYNCH_2> *tp;

  for (tp = this->stream_head_->writer (); 
       ; 
       tp = tp->next ())
    {
      ACE_DEBUG ((LM_DEBUG, "writer queue name = %s\n", tp->name ()));
      tp->dump ();
      ACE_DEBUG ((LM_DEBUG, "-------\n"));
      if (tp == this->stream_tail_->writer ()
	  || (this->linked_us_ 
	      && tp == this->linked_us_->stream_head_->reader ()))
	break;
    }

  ACE_DEBUG ((LM_DEBUG, "-------- reader links --------\n"));
  for (tp = this->stream_tail_->reader (); ; tp = tp->next ())
    {
      ACE_DEBUG ((LM_DEBUG, "reader queue name = %s\n", tp->name ()));
      tp->dump ();
      ACE_DEBUG ((LM_DEBUG, "-------\n"));
      if (tp == this->stream_head_->reader ()
	  || (this->linked_us_ 
	      && tp == this->linked_us_->stream_head_->writer ()))
	break;
    }
}

template <ACE_SYNCH_1> int
ACE_Stream<ACE_SYNCH_2>::push (ACE_Module<ACE_SYNCH_2> *new_top)
{
  ACE_TRACE ("ACE_Stream<ACE_SYNCH_2>::push");
  if (this->push_module  (new_top, 
			  this->stream_head_->next (), 
			  this->stream_head_) == -1)
    return -1;
  else
    return 0;
}

template <ACE_SYNCH_1> int
ACE_Stream_Iterator<ACE_SYNCH_2>::advance (void)
{
  ACE_TRACE ("ACE_Stream_Iterator<ACE_SYNCH_2>::advance");
  this->next_ = this->next_->next ();
  return 0;
}

template <ACE_SYNCH_1> int
ACE_Stream<ACE_SYNCH_2>::put (ACE_Message_Block *mb, ACE_Time_Value *tv)
{
  ACE_TRACE ("ACE_Stream<ACE_SYNCH_2>::put");
  return this->stream_head_->writer ()->put (mb, tv);
}

template <ACE_SYNCH_1> int 
ACE_Stream<ACE_SYNCH_2>::get (ACE_Message_Block *&mb, ACE_Time_Value *tv)
{
  ACE_TRACE ("ACE_Stream<ACE_SYNCH_2>::get");
  return this->stream_head_->reader ()->getq (mb, tv);
}

// Return the "top" ACE_Module in a ACE_Stream, skipping over the
// stream_head.

template <ACE_SYNCH_1> int 
ACE_Stream<ACE_SYNCH_2>::top (ACE_Module<ACE_SYNCH_2> *&m)
{
  ACE_TRACE ("ACE_Stream<ACE_SYNCH_2>::top");
  if (this->stream_head_->next () == this->stream_tail_)
    return -1;
  else
    {
      m = this->stream_head_->next ();
      return 0;
    }
}

// Remove the "top" ACE_Module in a ACE_Stream, skipping over the
// stream_head.

template <ACE_SYNCH_1> int
ACE_Stream<ACE_SYNCH_2>::pop (int flags)
{
  ACE_TRACE ("ACE_Stream<ACE_SYNCH_2>::pop");
  if (this->stream_head_->next () == this->stream_tail_)
    return -1;
  else
    {
      // Skip over the ACE_Stream head.
      ACE_Module<ACE_SYNCH_2> *top_mod = this->stream_head_->next ();
      ACE_Module<ACE_SYNCH_2> *new_top = top_mod->next ();

      this->stream_head_->next (new_top);

      // Close the top ACE_Module.

      top_mod->close (flags);

      // Don't delete the Module unless the flags request this.
      if (flags != ACE_Module<ACE_SYNCH_2>::M_DELETE_NONE)
	delete top_mod;

      this->stream_head_->writer ()->next (new_top->writer ());
      new_top->reader ()->next (this->stream_head_->reader ());
      return 0;
    }
}

// Remove a named ACE_Module from an arbitrary place in the
// ACE_Stream.

template <ACE_SYNCH_1> int
ACE_Stream<ACE_SYNCH_2>::remove (const char *name, 
				 int flags)
{
  ACE_TRACE ("ACE_Stream<ACE_SYNCH_2>::remove");
  ACE_Module<ACE_SYNCH_2> *prev = 0;

  for (ACE_Module<ACE_SYNCH_2> *mod = this->stream_head_; 
       mod != 0; 
       mod = mod->next ())
    if (ACE_OS::strcmp (mod->name (), name) == 0)
      {
	if (prev == 0) // Deleting ACE_Stream Head
	  this->stream_head_->link (mod->next ());
	else
	  prev->link (mod->next ());

	// Don't delete the Module unless the flags request this.
	if (flags != ACE_Module<ACE_SYNCH_2>::M_DELETE_NONE)
	  {
	    // Close down the module and release the memory.
	    mod->close (flags);
	    delete mod;
	  }

	return 0;
      }
    else
      prev = mod;

  return -1;
}

template <ACE_SYNCH_1> ACE_Module<ACE_SYNCH_2> *
ACE_Stream<ACE_SYNCH_2>::find (const char *name)
{
  ACE_TRACE ("ACE_Stream<ACE_SYNCH_2>::find");
  for (ACE_Module<ACE_SYNCH_2> *mod = this->stream_head_; 
       mod != 0; 
       mod = mod->next ())
    if (ACE_OS::strcmp (mod->name (), name) == 0)
	return mod;

  return 0;
}

// Actually push a module onto the stack...

template <ACE_SYNCH_1> int
ACE_Stream<ACE_SYNCH_2>::push_module (ACE_Module<ACE_SYNCH_2> *new_top, 
				      ACE_Module<ACE_SYNCH_2> *current_top, 
				      ACE_Module<ACE_SYNCH_2> *head)
{
  ACE_TRACE ("ACE_Stream<ACE_SYNCH_2>::push_module");
  ACE_Task<ACE_SYNCH_2> *nt_reader = new_top->reader ();
  ACE_Task<ACE_SYNCH_2> *nt_writer = new_top->writer ();
  ACE_Task<ACE_SYNCH_2> *ct_reader = 0;
  ACE_Task<ACE_SYNCH_2> *ct_writer = 0;

  if (current_top)
    {
      ct_reader = current_top->reader ();
      ct_writer = current_top->writer ();
      ct_reader->next (nt_reader);
    }

  nt_writer->next (ct_writer);

  if (head)
    {
      if (head != new_top)
	head->link (new_top);
    }
  else
    nt_reader->next (0);

  new_top->next (current_top);

  if (nt_reader->open (new_top->arg ()) == -1)
    return -1;

  if (nt_writer->open (new_top->arg ()) == -1)
    return -1;
  return 0;
}

#if 0
int
ACE_Stream<ACE_SYNCH_2>::open (void *a, 
			       ACE_Multiplexor &muxer, 
			       ACE_Module<ACE_SYNCH_2> *head)
{
  ACE_TRACE ("ACE_Stream<ACE_SYNCH_2>::open");
  this->stream_head_ = head == 0
    ? new ACE_Module<ACE_SYNCH_2> ("ACE_Stream_Head", 
			       new ACE_Stream_Head<ACE_SYNCH_2>, 
			       new ACE_Stream_Head<ACE_SYNCH_2>, a) : head;
  this->stream_tail_ = 0;
  return muxer.link_from_below (this->stream_head_);
}
#endif 

template <ACE_SYNCH_1> int
ACE_Stream<ACE_SYNCH_2>::open (void *a, 
			       ACE_Module<ACE_SYNCH_2> *head, 
			       ACE_Module<ACE_SYNCH_2> *tail)
{
  ACE_TRACE ("ACE_Stream<ACE_SYNCH_2>::open");
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, -1);

  ACE_Task<ACE_SYNCH_2> *h1 = 0, *h2 = 0;
  ACE_Task<ACE_SYNCH_2> *t1 = 0, *t2 = 0;

  if (head == 0)
    {
      h1 = new ACE_Stream_Head<ACE_SYNCH_2>;
      h2 = new ACE_Stream_Head<ACE_SYNCH_2>;
      head = new ACE_Module<ACE_SYNCH_2> ("ACE_Stream_Head", h1, h2, a);
    }

  if (tail == 0)
    {
      t1 = new ACE_Stream_Tail<ACE_SYNCH_2>;
      t2 = new ACE_Stream_Tail<ACE_SYNCH_2>;
      tail = new ACE_Module<ACE_SYNCH_2> ("ACE_Stream_Tail", 
					  t1, t2, a);
    }

  // Make sure *all* the allocation succeeded!
  if (head == 0 && (h1 == 0 || h2 == 0) 
      || tail == 0 && (t1 == 0 || t2 == 0))
    {
      delete h1;
      delete h2;
      delete t1;
      delete t2;
      delete head;
      delete tail;
      errno = ENOMEM;
      return -1;
    }

  this->stream_head_ = head;
  this->stream_tail_ = tail;

  if (this->push_module (this->stream_tail_) == -1)
    return -1;
  else if (this->push_module (this->stream_head_, 
			      this->stream_tail_, 
			      this->stream_head_) == -1)
    return -1;
  else
    return 0;
}

template <ACE_SYNCH_1> int
ACE_Stream<ACE_SYNCH_2>::close (int flags)
{
  ACE_TRACE ("ACE_Stream<ACE_SYNCH_2>::close");
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, -1);

  if (this->stream_head_ != 0 
      && this->stream_tail_ != 0)
    {
      // Don't bother checking return value here.
      this->unlink_i ();

      int result = 0;

      // Remove and cleanup all the intermediate modules.

      while (this->stream_head_->next () != this->stream_tail_)
	if (this->pop (flags) == -1)
	  result = -1;
  
      // Clean up the head and tail of the stream.
      if (this->stream_head_->close (flags) == -1)
	result = -1;
      if (this->stream_tail_->close (flags) == -1)
	result = -1;

      // Cleanup the memory.
      delete this->stream_head_;
      delete this->stream_tail_;

      this->stream_head_ = 0;
      this->stream_tail_ = 0;

      // Tell all threads waiting on the close that we are done.
      this->final_close_.broadcast ();
      return result;
    }
  return 0;
}

template <ACE_SYNCH_1> int 
ACE_Stream<ACE_SYNCH_2>::control (ACE_IO_Cntl_Msg::ACE_IO_Cntl_Cmds cmd, 
				  void *a)
{
  ACE_TRACE ("ACE_Stream<ACE_SYNCH_2>::control");
  ACE_IO_Cntl_Msg ioc (cmd);

  // Create a data block that contains the user-supplied data.
  ACE_Message_Block *db = 
    new ACE_Message_Block (sizeof (int), 
			   ACE_Message_Block::MB_IOCTL, 
			   0, 
			   (char *) a);

  // Create a control block that contains the control field and a
  // pointer to the data block.
  ACE_Message_Block *cb = 
    new ACE_Message_Block (sizeof ioc, 
			   ACE_Message_Block::MB_IOCTL, 
			   db, 
			   (char *) &ioc);

  // Make sure all of the allocation succeeded before continuing.
  if (db == 0 || cb == 0)
    {
      delete cb; 
      delete db; 
      errno = ENOMEM;
      return -1;
    }

  int result = 0;

  if (this->stream_head_->writer ()->put (cb) == -1)
    result = -1;
  else if (this->stream_head_->reader ()->getq (cb) == -1)
    result = -1;
  else
    result = ((ACE_IO_Cntl_Msg *) cb->rd_ptr ())->rval ();

  delete cb; // This also deletes db...
  return result;
}

// Link two streams together at their bottom-most Modules (i.e., the
// one just above the Stream tail).  Note that all of this is premised
// on the fact that the Stream head and Stream tail are non-NULL...
// This must be called with locks held.

template <ACE_SYNCH_1> int
ACE_Stream<ACE_SYNCH_2>::link_i (ACE_Stream<ACE_SYNCH_2> &us)
{
  ACE_TRACE ("ACE_Stream<ACE_SYNCH_2>::link_i");
  this->linked_us_ = &us;
  // Make sure the other side is also linked to us!
  us.linked_us_ = this;

  ACE_Module<ACE_SYNCH_2> *my_tail = this->stream_head_;

  if (my_tail == 0)
    return -1;

  // Locate the module just above our Stream tail. 
  while (my_tail->next () != this->stream_tail_)
    my_tail = my_tail->next ();

  ACE_Module<ACE_SYNCH_2> *other_tail = us.stream_head_;

  if (other_tail == 0)
    return -1;

  // Locate the module just above the other Stream's tail.
  while (other_tail->next () != us.stream_tail_)
    other_tail = other_tail->next ();

  // Reattach the pointers so that the two streams are linked!
  my_tail->writer ()->next (other_tail->reader ());
  other_tail->writer ()->next (my_tail->reader ());
  return 0;
}

template <ACE_SYNCH_1> int
ACE_Stream<ACE_SYNCH_2>::link (ACE_Stream<ACE_SYNCH_2> &us)
{
  ACE_TRACE ("ACE_Stream<ACE_SYNCH_2>::link");
  
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, -1);

  return this->link_i (us);
}  

// Must be called with locks held...

template <ACE_SYNCH_1> int
ACE_Stream<ACE_SYNCH_2>::unlink_i (void)
{
  ACE_TRACE ("ACE_Stream<ACE_SYNCH_2>::unlink_i");

  // Only try to unlink if we are in fact still linked!

  if (this->linked_us_ != 0)
    {
      ACE_Module<ACE_SYNCH_2> *my_tail = this->stream_head_;

      // Only relink if we still exist!
      if (my_tail)
	{
	  // Find the module that's just before our stream tail.
	  while (my_tail->next () != this->stream_tail_)
	    my_tail = my_tail->next ();

	  // Restore the writer's next() link to our tail.
	  my_tail->writer ()->next (this->stream_tail_->writer ());
	}

      ACE_Module<ACE_SYNCH_2> *other_tail = 
	this->linked_us_->stream_head_;

      // Only fiddle with the other side if it in fact still remains.
      if (other_tail != 0)
	{
	  while (other_tail->next () != this->linked_us_->stream_tail_)
	    other_tail = other_tail->next ();

	  other_tail->writer ()->next (this->linked_us_->stream_tail_->writer ());

	}

      // Make sure the other side is also aware that it's been unlinked!
      this->linked_us_->linked_us_ = 0;

      this->linked_us_ = 0;
      return 0;
    }
  else
    return -1;
}

template <ACE_SYNCH_1> int
ACE_Stream<ACE_SYNCH_2>::unlink (void)
{
  ACE_TRACE ("ACE_Stream<ACE_SYNCH_2>::unlink");
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, -1);
  return this->unlink_i ();
}

template <ACE_SYNCH_1>
ACE_Stream<ACE_SYNCH_2>::ACE_Stream (void * a, 
				     ACE_Module<ACE_SYNCH_2> *head, 
				     ACE_Module<ACE_SYNCH_2> *tail)
  : linked_us_ (0),
    final_close_ (this->lock_)
{
  ACE_TRACE ("ACE_Stream<ACE_SYNCH_2>::ACE_Stream");
  if (this->open (a, head, tail) == -1)
    ACE_ERROR ((LM_ERROR, "ACE_Stream<ACE_SYNCH_2>::open (%s, %s)\n", 
	       head->name (), tail->name ()));
}

#if 0
ACE_Stream<ACE_SYNCH_2>::ACE_Stream (void *a, 
				     ACE_Multiplexor &muxer, 
				     ACE_Module<ACE_SYNCH_2> *head)
  : linked_us_ (0),
    final_close_ (this->lock_)
{
  ACE_TRACE ("ACE_Stream<ACE_SYNCH_2>::ACE_Stream");
  if (this->open (a, muxer, head) == -1)
    ACE_ERROR ((LM_ERROR, "ACE_Stream<ACE_SYNCH_2>::open (%s, %s)\n", 
		head->name ()));
}
#endif 

template <ACE_SYNCH_1>
ACE_Stream<ACE_SYNCH_2>::~ACE_Stream (void)
{
  ACE_TRACE ("ACE_Stream<ACE_SYNCH_2>::~ACE_Stream");

  if (this->stream_head_ != 0)
    this->close ();
}

template <ACE_SYNCH_1>
ACE_Stream_Iterator<ACE_SYNCH_2>::ACE_Stream_Iterator (const ACE_Stream<ACE_SYNCH_2> &sr)
  : next_ (sr.stream_head_)
{
  ACE_TRACE ("ACE_Stream_Iterator<ACE_SYNCH_2>::ACE_Stream_Iterator");
}

#endif /* ACE_STREAM_C */
