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
ACE_Module<ACE_SYNCH_2>::writer (ACE_Task<ACE_SYNCH_2> *q)
{ 
  ACE_TRACE ("ACE_Module<ACE_SYNCH_2>::writer");
  this->q_pair_[1] = q;
  if (q != 0)
    ACE_CLR_BITS (q->flags_, ACE_Task_Flags::ACE_READER);
}

template <ACE_SYNCH_1> void
ACE_Module<ACE_SYNCH_2>::reader (ACE_Task<ACE_SYNCH_2> *q) 
{
  ACE_TRACE ("ACE_Module<ACE_SYNCH_2>::reader");
  this->q_pair_[0] = q;
  if (q != 0)
    ACE_SET_BITS (q->flags_, ACE_Task_Flags::ACE_READER);
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
ACE_Module<ACE_SYNCH_2>::open (char *mod_name, 
			       ACE_Task<ACE_SYNCH_2> *writer_q, 
			       ACE_Task<ACE_SYNCH_2> *reader_q, 
			       void *arg)
{
  ACE_TRACE ("ACE_Module<ACE_SYNCH_2>::open");
  this->name (mod_name);
  this->arg_ = arg;

  if (writer_q == 0)
    writer_q = new ACE_Thru_Task<ACE_SYNCH_2>;
  if (reader_q == 0)
    reader_q = new ACE_Thru_Task<ACE_SYNCH_2>;

  // Make sure that the memory is allocated before proceding.
  if (writer_q == 0 || reader_q == 0)
    {
      delete writer_q;
      delete reader_q;
      errno = ENOMEM;
      return -1;
    }

  this->reader (reader_q);
  this->writer (writer_q);

  // Setup back pointers.
  reader_q->mod_ = this;
  writer_q->mod_ = this;
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

template <ACE_SYNCH_1> ACE_INLINE
ACE_Module<ACE_SYNCH_2>::ACE_Module (void)
{
  ACE_TRACE ("ACE_Module<ACE_SYNCH_2>::ACE_Module");
  this->name ("<unknown>");
  // Do nothing...
}

// Should never be called... 
template <ACE_SYNCH_1> ACE_INLINE
ACE_Module<ACE_SYNCH_2>::~ACE_Module (void)
{
  ACE_TRACE ("ACE_Module<ACE_SYNCH_2>::~ACE_Module");
  ACE_ERROR ((LM_ERROR, "destructor for %s should never be called!\n", 
	     this->name ()));
}

template <ACE_SYNCH_1> ACE_INLINE
ACE_Module<ACE_SYNCH_2>::ACE_Module (char *mod_name, 
				     ACE_Task<ACE_SYNCH_2> *writer_q, 
				     ACE_Task<ACE_SYNCH_2> *reader_q, 
				     void *flags)
{
  ACE_TRACE ("ACE_Module<ACE_SYNCH_2>::ACE_Module");
  if (this->open (mod_name, writer_q, reader_q, flags) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "ACE_Module"));
}
		
template <ACE_SYNCH_1> int
ACE_Module<ACE_SYNCH_2>::close (u_long flags)
{
  ACE_TRACE ("ACE_Module<ACE_SYNCH_2>::close");
  ACE_Task<ACE_SYNCH_2> *reader_q = this->reader ();
  ACE_Task<ACE_SYNCH_2> *writer_q = this->writer ();
  int result = 0;

  if (reader_q != 0)
    {
      if (reader_q->close () == -1)
	result = -1;
      reader_q->flush ();
      reader_q->next (0);
    }

  if (writer_q != 0)
    {
      if (writer_q->close () == -1)
	result = -1;
      writer_q->flush ();
      writer_q->next (0);
    }

  if (ACE_BIT_ENABLED (flags, ACE_Module<ACE_SYNCH_2>::M_DELETE))
    {
      // Only delete the Tasks if there aren't any more threads
      // running in them.
      if (reader_q->thr_count () == 0)
	delete reader_q;
      this->reader (0);

      if (writer_q->thr_count () == 0)
	delete writer_q;
      this->writer (0);

      delete (void *) this; // Darn well better be allocated dynamically!!!
    }
  return result;
}

#endif /* ACE_MODULE_C */
