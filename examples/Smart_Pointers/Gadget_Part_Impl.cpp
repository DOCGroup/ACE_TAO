/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    Gadget_Part_Impl.cpp
 *
 *  $Id$
 *
 *  @author Christopher Kohlhoff <chris@kohlhoff.com>
 */
//=============================================================================

#include "Gadget_Part_Impl.h"
#include "ace/ACE.h"
#include "ace/Log_Msg.h"
#include "ace/Unbounded_Queue.h"

Gadget_Part_Impl::Gadget_Part_Impl (Gadget_ptr owner,
                                    const char* name,
                                    int size)
  : owner_ (owner),
    name_ (ACE::strnew (name)),
    size_ (size)
{
  ACE_DEBUG ((LM_DEBUG, "Gadget_Part_Impl constructor\n"));
}

Gadget_Part_Impl::~Gadget_Part_Impl (void)
{
  ACE_DEBUG ((LM_DEBUG, "Gadget_Part_Impl destructor\n"));

  delete [] name_;
}

void Gadget_Part_Impl::print_info (void)
{
  ACE_DEBUG ((LM_INFO, "Gadget part: name=%s size=%d\n", name_, size_));
}

void Gadget_Part_Impl::remove_from_owner (void)
{
  // Need to guarantee the existence of the owner for the duration of this call.
  Gadget_var owner = owner_;

  // Weak pointers are automatically set to NULL if the object they refer to
  // is deleted. We can use this fact to check that our owner still exists.
  if (owner == 0)
    return;

  // Take all existing parts from the owner and build up a temporary list. If
  // we find ourselves then we won't add ourselves to the list.
  ACE_Unbounded_Queue<Gadget_Part_var> parts;
  for (;;)
    {
      Gadget_Part_var part = owner->remove_part ();
      if (part == 0)
        break;
      if (part != this)
        parts.enqueue_tail (part);
    }

  // Add the remaining parts back to the gadget.
  while (!parts.is_empty ())
    {
      Gadget_Part_var part;
      parts.dequeue_head (part);
      owner->add_part (part);
    }
}
