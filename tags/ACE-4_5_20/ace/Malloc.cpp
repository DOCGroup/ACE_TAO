// $Id$

#if !defined (ACE_MALLOC_C)
#define ACE_MALLOC_C

#define ACE_BUILD_DLL
#include "ace/Malloc.h"
#include "ace/Object_Manager.h"

#if !defined (__ACE_INLINE__)
#include "ace/Malloc.i"
#endif /* __ACE_INLINE__ */

#include "ace/Synch_T.h"

// Process-wide ACE_Allocator.
ACE_Allocator *ACE_Allocator::allocator_ = 0;

// Controls whether the Allocator is deleted when we shut down (we can
// only delete it safely if we created it!)  This is no longer used;
// see ACE_Allocator::instance (void).
int ACE_Allocator::delete_allocator_ = 0;

void
ACE_Control_Block::dump (void) const
{
  ACE_TRACE ("ACE_Control_Block::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  this->name_head_->dump ();
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT("freep_ = %x"), this->freep_));

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
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT("pointer = %x"), this->pointer_));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT("\nnext_ = %x"), this->next_));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT("\nname_ = %s"), this->name_));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT("\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_Allocator *
ACE_Allocator::instance (void)
{
  //  ACE_TRACE ("ACE_Allocator::instance");

  if (ACE_Allocator::allocator_ == 0)
    {
      // Perform Double-Checked Locking Optimization.
      ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon,
                                *ACE_Static_Object_Lock::instance (), 0));

      if (ACE_Allocator::allocator_ == 0)
        {
          // Have a seat.  We want to avoid ever having to delete the
          // ACE_Allocator instance, to avoid shutdown order
          // dependencies.  ACE_New_Allocator never needs to be
          // destroyed:  its destructor is empty and its instance
          // doesn't have any state.  Therefore, sizeof
          // ACE_New_Allocator is equal to sizeof void *.  It's
          // instance just contains a pointer to its virtual function
          // table.
          //
          // So, we allocate space for the ACE_New_Allocator instance
          // in the data segment.  Because its size is the same as
          // that of a pointer, we allocate it as a pointer so that it
          // doesn't get constructed statically.  We never bother to
          // destroy it.
          static void *allocator_instance = 0;

          // Check this critical assumption.
          // We put it in a variable first to avoid stupid compiler
          // warnings that the condition may always be true/false.
#         if !defined (ACE_NDEBUG)
          int assertion = (sizeof allocator_instance ==
                           sizeof (ACE_New_Allocator));
          ACE_ASSERT (assertion);
#         endif /* !ACE_NDEBUG */

          // Initialize the allocator_instance by using a placement
          // new.  The ACE_NEW_RETURN below doesn't actually allocate
          // a new instance.  It just initializes it in place.
          ACE_NEW_RETURN (ACE_Allocator::allocator_,
                          (&allocator_instance) ACE_New_Allocator,
                          0);
          // If we ever need to cast the address of
          // allocator_instance, then expand the ACE_NEW_RETURN above
          // as follows . . .
          //
          // ACE_Allocator::allocator_ =
          //   (ACE_New_Allocator *)
          //     new (&allocator_instance) ACE_New_Allocator;
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
      // This should never be executed.  See ACE_Allocator::instance (void).
      delete ACE_Allocator::allocator_;
      ACE_Allocator::allocator_ = 0;
      ACE_Allocator::delete_allocator_ = 0;
    }
}

ACE_Allocator::~ACE_Allocator (void)
{
  ACE_TRACE ("ACE_Allocator::~ACE_Allocator");
}

void
ACE_Static_Allocator_Base::dump (void) const
{
  ACE_TRACE ("ACE_Static_Base_Allocator::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\noffset_ = %d"), this->offset_));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\nsize_ = %d\n"), this->size_));
  ACE_HEX_DUMP ((LM_DEBUG, this->buffer_, this->size_));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\n")));

  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

#if defined (ACE_HAS_MALLOC_STATS)
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
  int nblocks = this->nblocks_.value ();
  int ninuse  = this->ninuse_.value ();
  int nchunks = this->nchunks_.value ();

  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT("nblocks = %d"), nblocks));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT("\nninuse = %d"), ninuse));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT("\nnchunks = %d"), nchunks));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT("\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Atomic_Op<ACE_PROCESS_MUTEX, int>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Atomic_Op<ACE_PROCESS_MUTEX, int>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* ACE_HAS_MALLOC_STATS */
#endif /* ACE_MALLOC_C */
