// $Id$

#if !defined (ACE_MALLOC_CPP)
#define ACE_MALLOC_CPP

#include "ace/Malloc.h"
#include "ace/Object_Manager.h"

#if !defined (__ACE_INLINE__)
#include "ace/Malloc.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Malloc, "$Id$")

// Process-wide ACE_Allocator.
ACE_Allocator *ACE_Allocator::allocator_ = 0;

// Controls whether the Allocator is deleted when we shut down (we can
// only delete it safely if we created it!)  This is no longer used;
// see ACE_Allocator::instance (void).
int ACE_Allocator::delete_allocator_ = 0;

void
ACE_Control_Block::ACE_Malloc_Header::dump (void) const
{
#if defined (ACE_HAS_DUMP)
  ACE_TRACE ("ACE_Control_Block::ACE_Malloc_Header::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\nnext_block = %@"), (ACE_Malloc_Header *) this->next_block_));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\nsize = %d\n"), this->size_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#endif /* ACE_HAS_DUMP */
}

void
ACE_Control_Block::print_alignment_info (void)
{
#if defined (ACE_HAS_DUMP)
  ACE_TRACE ("ACE_Control_Block::print_alignment_info");
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("Start ---> ACE_Control_Block::print_alignment_info:\n")));
  ACE_DEBUG ((LM_DEBUG,
              ACE_LIB_TEXT ("Sizeof ptr: %d\n")
              ACE_LIB_TEXT ("Sizeof size_t: %d\n")
              ACE_LIB_TEXT ("Sizeof long: %d\n")
              ACE_LIB_TEXT ("Sizeof double: %d\n")
              ACE_LIB_TEXT ("Sizeof ACE_MALLOC_ALIGN: %d\n")
              ACE_LIB_TEXT ("Sizeof ACE_MALLOC_PADDING: %d\n")
              ACE_LIB_TEXT ("Sizeof ACE_MALLOC_HEADER_SIZE: %d\n")
              ACE_LIB_TEXT ("Sizeof ACE_MALLOC_PADDING_SIZE: %d\n")
              ACE_LIB_TEXT ("Sizeof ACE_CONTROL_BLOCK_SIZE: %d\n")
              ACE_LIB_TEXT ("Sizeof ACE_CONTROL_BLOCK_ALIGN_BYTES: %d\n")
              ACE_LIB_TEXT ("Sizeof (MALLOC_HEADER): %d\n")
              ACE_LIB_TEXT ("Sizeof (CONTROL_BLOCK): %d\n"),
              sizeof (char *),
              sizeof (size_t),
              sizeof (long),
              sizeof (double),
              ACE_MALLOC_ALIGN,
              ACE_MALLOC_PADDING,
              ACE_MALLOC_HEADER_SIZE,
              ACE_MALLOC_PADDING_SIZE,
              ACE_CONTROL_BLOCK_SIZE,
              ACE_CONTROL_BLOCK_ALIGN_BYTES,
              sizeof (ACE_Malloc_Header),
              sizeof (ACE_Control_Block)
              ));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("End <--- ACE_Control_Block::print_alignment_info:\n")));
#endif /* ACE_HAS_DUMP */
}

void
ACE_Control_Block::dump (void) const
{
#if defined (ACE_HAS_DUMP)
  ACE_TRACE ("ACE_Control_Block::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("Name Node:\n")));
  for (ACE_Name_Node *nextn = this->name_head_;
       nextn != 0;
       nextn = nextn->next_)
    nextn->dump ();

  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("freep_ = %@"), (ACE_Malloc_Header *) this->freep_));
  this->base_.dump ();

  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\nMalloc Header:\n")));
  for (ACE_Malloc_Header *nexth = ((ACE_Malloc_Header *)this->freep_)->next_block_;
       nexth != 0 && nexth != &this->base_;
       nexth = nexth->next_block_)
    nexth->dump ();

  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#endif /* ACE_HAS_DUMP */
}

ACE_Control_Block::ACE_Name_Node::ACE_Name_Node (void)
{
  ACE_TRACE ("ACE_Control_Block::ACE_Name_Node::ACE_Name_Node");
}

ACE_Control_Block::ACE_Name_Node::ACE_Name_Node (const char *name,
                                                 char *name_ptr,
                                                 char *pointer,
                                                 ACE_Name_Node *next)
  : name_ (name_ptr),
    pointer_ (pointer),
    next_ (next),
    prev_ (0)
{
  ACE_TRACE ("ACE_Control_Block::ACE_Name_Node::ACE_Name_Node");
  char *n = this->name_;
  ACE_OS::strcpy (n, name);
  if (next != 0)
    next->prev_ = this;
}

ACE_Control_Block::ACE_Name_Node::ACE_Name_Node (const ACE_Name_Node &)
{
  ACE_TRACE ("ACE_Control_Block::ACE_Name_Node::ACE_Name_Node");
#if !defined (ACE_PSOS)
  ACE_ASSERT (!"not implemented!");
#endif /* ! ACE_PSOS */
}

const char *
ACE_Control_Block::ACE_Name_Node::name (void) const
{
  const char *c = this->name_;
  return c;
}

void
ACE_Control_Block::ACE_Name_Node::name (const char *)
{
#if !defined (ACE_PSOS)
  ACE_ASSERT (0 == "not implemented!");
#endif /* ! ACE_PSOS */
}

ACE_Control_Block::ACE_Malloc_Header::ACE_Malloc_Header (void)
  : next_block_ (0),
    size_ (0)
{
}

void
ACE_Control_Block::ACE_Name_Node::dump (void) const
{
#if defined (ACE_HAS_DUMP)
  ACE_TRACE ("ACE_Control_Block::ACE_Name_Node::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("pointer = %@"), (const char *) this->pointer_));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\nnext_ = %@"), (ACE_Name_Node *) this->next_));
  ACE_DEBUG ((LM_DEBUG,
              ACE_LIB_TEXT ("\nname_ = (%@, %s)"),
              (const char *) this->name_,
              (const char *) this->name_));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#endif /* ACE_HAS_DUMP */
}


#if defined (ACE_HAS_MALLOC_STATS)
ACE_Malloc_Stats::ACE_Malloc_Stats (void)
  : nchunks_ (0),
    nblocks_ (0),
    ninuse_ (0)
{
  ACE_TRACE ("ACE_Malloc_Stats::ACE_Malloc_Stats");
}

void
ACE_Malloc_Stats::dump (void) const
{
#if defined (ACE_HAS_DUMP)
  ACE_TRACE ("ACE_Malloc_Stats::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  int nblocks = this->nblocks_.value ();
  int ninuse  = this->ninuse_.value ();
  int nchunks = this->nchunks_.value ();

  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT("nblocks = %d"), nblocks));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT("\nninuse = %d"), ninuse));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT("\nnchunks = %d"), nchunks));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT("\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#endif /* ACE_HAS_DUMP */
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Atomic_Op<ACE_PROCESS_MUTEX, int>;
template class ACE_Atomic_Op_Ex<ACE_PROCESS_MUTEX, int>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Atomic_Op<ACE_PROCESS_MUTEX, int>
#pragma instantiate ACE_Atomic_Op_Ex<ACE_PROCESS_MUTEX, int>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
#endif /*ACE_HAS_MALLOC_STATS*/
#endif /* ACE_MALLOC_CPP */
