// Module.cpp
// $Id$

#if !defined (ACE_MODULE_C)
#define ACE_MODULE_C

#define ACE_BUILD_DLL
#include "ace/Module.h"
#include "ace/Stream_Modules.h"

#if !defined (__ACE_INLINE__)
#include "ace/Module.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_Module)

template <ACE_SYNCH_1> void
ACE_Module<ACE_SYNCH_2>::dump (void) const
{
  ACE_TRACE ("ACE_Module<ACE_SYNCH_2>::dump");
}

template <ACE_SYNCH_1> void
ACE_Module<ACE_SYNCH_2>::writer (ACE_Task<ACE_SYNCH_2> *q, 
				 int flags /* = M_DELETE_WRITER */) 
{ 
  ACE_TRACE ("ACE_Module<ACE_SYNCH_2>::writer");

  // Close and maybe delete old writer
  this->close_i (1, flags);

  this->q_pair_[1] = q;
  if (q != 0)
    ACE_CLR_BITS (q->flags_, ACE_Task_Flags::ACE_READER);
	
  // Don't allow the caller to change the reader status.
  ACE_SET_BITS (flags_, (flags & M_DELETE_WRITER));
}

template <ACE_SYNCH_1> void
ACE_Module<ACE_SYNCH_2>::reader (ACE_Task<ACE_SYNCH_2> *q, 
				 int flags /* = M_DELETE_READER */)
{
  ACE_TRACE ("ACE_Module<ACE_SYNCH_2>::reader");

  // Close and maybe delete old writer
  this->close_i (0, flags);

  this->q_pair_[0] = q;
  if (q != 0)
    ACE_SET_BITS (q->flags_, ACE_Task_Flags::ACE_READER);
	
  // don't allow the caller to change the reader status
  ACE_SET_BITS (flags_, (flags & M_DELETE_READER));
}

// Link this ACE_Module on top of ACE_Module M.

template <ACE_SYNCH_1> void
ACE_Module<ACE_SYNCH_2>::link (ACE_Module<ACE_SYNCH_2> *m)
{
  ACE_TRACE ("ACE_Module<ACE_SYNCH_2>::link");
  this->next (m);
  this->writer ()->next (m->writer ());
  m->reader ()->next (this->reader ());
}

template <ACE_SYNCH_1> int
ACE_Module<ACE_SYNCH_2>::open (const char *mod_name, 
			       ACE_Task<ACE_SYNCH_2> *writer_q, 
			       ACE_Task<ACE_SYNCH_2> *reader_q, 
			       void *arg,
			       int flags /* = M_DELETE */)
{
  ACE_TRACE ("ACE_Module<ACE_SYNCH_2>::open");
  this->name (mod_name);
  this->arg_ = arg;

  // We may already have readers and/or writers.
  if (this->reader ())
    this->close_i (0, M_DELETE_READER);

  if (this->writer ())
    this->close_i (1, M_DELETE_WRITER);

  if (writer_q == 0) 
    {
      writer_q = new ACE_Thru_Task<ACE_SYNCH_2>;
      ACE_SET_BITS (flags, M_DELETE_WRITER);
    }

  if (reader_q == 0) 
    {
      reader_q = new ACE_Thru_Task<ACE_SYNCH_2>;
      ACE_SET_BITS (flags, M_DELETE_READER);
    }

  this->reader (reader_q);
  this->writer (writer_q);

  // Setup back pointers.
  reader_q->mod_ = this;
  writer_q->mod_ = this;

  // Save the flags
  this->flags_ = flags;

  // Make sure that the memory is allocated before proceding.
  if (writer_q == 0 || reader_q == 0)
    {
      // These calls will delete writer_q and/or reader_q, if
      // necessary.
      this->close_i (0, M_DELETE_READER);
      this->close_i (1, M_DELETE_WRITER);

      // Reset back pointers.
      reader_q->mod_ = 0;
      writer_q->mod_ = 0;

      errno = ENOMEM;
      return -1;
    }

  return 0;
}

// Set and get pointer to sibling ACE_Task in ACE_Module.

template <ACE_SYNCH_1> ACE_Task<ACE_SYNCH_2> *
ACE_Module<ACE_SYNCH_2>::sibling (ACE_Task<ACE_SYNCH_2> *orig)
{
  ACE_TRACE ("ACE_Module<ACE_SYNCH_2>::sibling");
  if (this->q_pair_[0] == orig)
    return this->q_pair_[1];
  else if (this->q_pair_[1] == orig)
    return this->q_pair_[0];
  else
    return 0;
}

template <ACE_SYNCH_1>
ACE_Module<ACE_SYNCH_2>::ACE_Module (void)
  : flags_ (0)
{
  ACE_TRACE ("ACE_Module<ACE_SYNCH_2>::ACE_Module");
  this->name ("<unknown>");
  // Do nothing...
  this->q_pair_[0] = 0;
  this->q_pair_[1] = 0;
}

template <ACE_SYNCH_1>
ACE_Module<ACE_SYNCH_2>::~ACE_Module (void)
{
  ACE_TRACE ("ACE_Module<ACE_SYNCH_2>::~ACE_Module");

  // Only close down if we haven't already done so.
  if (this->reader () || this->writer ())
    this->close ();
}

template <ACE_SYNCH_1>
ACE_Module<ACE_SYNCH_2>::ACE_Module (const char *mod_name,
				     ACE_Task<ACE_SYNCH_2> *writer_q, 
				     ACE_Task<ACE_SYNCH_2> *reader_q, 
				     void *args,
				     int flags /* = M_DELETE */) 
  : flags_ (0)
{
  ACE_TRACE ("ACE_Module<ACE_SYNCH_2>::ACE_Module");

  this->q_pair_[0] = 0;
  this->q_pair_[1] = 0;

  if (this->open (mod_name, writer_q, reader_q, args, flags) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "ACE_Module"));
}
		
template <ACE_SYNCH_1> int
ACE_Module<ACE_SYNCH_2>::close (int flags /* = M_DELETE_NONE */)
{
  ACE_TRACE ("ACE_Module<ACE_SYNCH_2>::close");

  int result = 0;

  ACE_SET_BITS (flags_, flags);

  if (this->close_i (0, flags) == -1)
    result = -1;

  if (this->close_i (1, flags) == -1)
    result = -1;

  return result;
}

template <ACE_SYNCH_1> int
ACE_Module<ACE_SYNCH_2>::close_i (int which,
				  int flags)
{
  ACE_TRACE ("ACE_Module<ACE_SYNCH_2>::close_i");

  if (this->q_pair_[which] == 0)
    return 0;

  // Copy task pointer to prevent problems when ACE_Task::close
  // changes the task pointer
  ACE_Task<ACE_SYNCH_2> *task = this->q_pair_[which];

  // Change so that close doesn't get called again from the task base.

  // Now close the task.
  int result = 0;

  if (task->module_closed () == -1)
    result = -1;

  task->flush ();
  task->next (0);

  // Should we also delete it ?
  if (flags != M_DELETE_NONE 
      && ACE_BIT_ENABLED (flags_, which + 1))
    {
      // Only delete the Tasks if there aren't any more threads
      // running in them.
      if (task->thr_count () == 0)
	delete task;
    }

  // Set the tasks pointer to 0 so that we don't try to close()
  // this object again if the destructor gets called.
  this->q_pair_[which] = 0;

  // Finally remove the delete bit.
  ACE_CLR_BITS (flags_, which + 1);

  return result;
}
#endif /* ACE_MODULE_C */
