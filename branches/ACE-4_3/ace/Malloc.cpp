// Malloc.cpp
// $Id$

#if !defined (ACE_MALLOC_C)
#define ACE_MALLOC_C

#define ACE_BUILD_DLL
#include "ace/Malloc.h"

#if !defined (__ACE_INLINE__)
#include "ace/Malloc.i"
#endif /* __ACE_INLINE__ */

#include "ace/Synch_T.h"

// Process-wide ACE_Allocator.
ACE_Allocator *ACE_Allocator::allocator_ = 0;

// Controls whether the Allocator is deleted when we shut down (we can
// only delete it safely if we created it!)
int ACE_Allocator::delete_allocator_ = 0;

void
ACE_Control_Block::dump (void) const
{
  ACE_TRACE ("ACE_Control_Block::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  this->name_head_->dump ();
  ACE_DEBUG ((LM_DEBUG, "freep_ = %x", this->freep_));

  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_Name_Node::ACE_Name_Node (void)
{
  ACE_TRACE ("ACE_Name_Node::ACE_Name_Node");
}

ACE_Name_Node::ACE_Name_Node (const char *name,
			      void *ptr,
			      ACE_Name_Node *next)
  : pointer_ (ptr),
    next_ (next)
{
  ACE_TRACE ("ACE_Name_Node::ACE_Name_Node");
  ACE_OS::strcpy (this->name_, name);
}

void
ACE_Name_Node::dump (void) const
{
  ACE_TRACE ("ACE_Name_Node");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "pointer = %x", this->pointer_));
  ACE_DEBUG ((LM_DEBUG, "\nnext_ = %x", this->next_));
  ACE_DEBUG ((LM_DEBUG, "\nname_ = %s", this->name_));
  ACE_DEBUG ((LM_DEBUG, "\n"));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_Allocator *
ACE_Allocator::instance (void)
{
  ACE_TRACE ("ACE_Allocator::instance");

  if (ACE_Allocator::allocator_ == 0)
    {
      // Perform Double-Checked Locking Optimization.
      ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon,
				*ACE_Static_Object_Lock::instance (), 0));

      if (ACE_Allocator::allocator_ == 0)
	{
	  ACE_NEW_RETURN (ACE_Allocator::allocator_,
			  ACE_New_Allocator,
			  0);
	  ACE_Allocator::delete_allocator_ = 1;
	}
    }
  return ACE_Allocator::allocator_;
}

ACE_Allocator *
ACE_Allocator::instance (ACE_Allocator *r)
{
  ACE_TRACE ("ACE_Allocator::instance");
  ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon,
			    *ACE_Static_Object_Lock::instance (), 0));
  ACE_Allocator *t = ACE_Allocator::allocator_;

  // We can't safely delete it since we don't know who created it!
  ACE_Allocator::delete_allocator_ = 0;

  ACE_Allocator::allocator_ = r;
  return t;
}

void
ACE_Allocator::close_singleton (void)
{
  ACE_TRACE ("ACE_Allocator::close_singleton");

  ACE_MT (ACE_GUARD (ACE_Recursive_Thread_Mutex, ace_mon,
		     *ACE_Static_Object_Lock::instance ()));

  if (ACE_Allocator::delete_allocator_)
    {
      delete ACE_Allocator::allocator_;
      ACE_Allocator::allocator_ = 0;
      ACE_Allocator::delete_allocator_ = 0;
    }
}

ACE_Allocator::~ACE_Allocator (void)
{
  ACE_TRACE ("ACE_Allocator::~ACE_Allocator");
}

#if defined (ACE_MALLOC_STATS)
ACE_Malloc_Stats::ACE_Malloc_Stats (void)
  : nblocks_ (0),
    nchunks_ (0),
    ninuse_ (0)
{
  ACE_TRACE ("ACE_Malloc_Stats::ACE_Malloc_Stats");
}

void
ACE_Malloc_Stats::dump (void) const
{
  ACE_TRACE ("ACE_Malloc_Stats::print");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  int nblocks = this->nblocks_;
  int ninuse  = this->ninuse_;
  int nchunks = this->nchunks_;

  ACE_DEBUG ((LM_DEBUG, "nblocks = %d", nblocks));
  ACE_DEBUG ((LM_DEBUG, "\nninuse = %d", ninuse));
  ACE_DEBUG ((LM_DEBUG, "\nnchunks = %d", nchunks));
  ACE_DEBUG ((LM_DEBUG, "\n"));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Atomic_Op<ACE_PROCESS_MUTEX, int>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Atomic_Op<ACE_PROCESS_MUTEX, int>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


#endif /* ACE_MALLOC_STATS */
#endif /* ACE_MALLOC_C */
