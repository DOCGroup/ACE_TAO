/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    Gadget_Impl.cpp
 *
 *  $Id$
 *
 *  @author Christopher Kohlhoff <chris@kohlhoff.com>
 */
//=============================================================================

#include "Gadget_Impl.h"
#include "ace/Log_Msg.h"

Gadget_Impl::Gadget_Impl (void)
{
  ACE_DEBUG ((LM_DEBUG, "Gadget_Impl constructor\n"));
}

Gadget_Impl::~Gadget_Impl (void)
{
  ACE_DEBUG ((LM_DEBUG, "Gadget_Impl destructor\n"));
}

void Gadget_Impl::add_part (Gadget_Part_var part)
{
  parts_.enqueue_tail (part);
}

Gadget_Part_var Gadget_Impl::remove_part (void)
{
  Gadget_Part_var removed_part;
  if (parts_.dequeue_head (removed_part) == -1)
    return Gadget_Part_var();
  return removed_part;
}

void Gadget_Impl::list_parts (void)
{
  ACE_Unbounded_Queue_Iterator<Gadget_Part_var> iter (parts_);
  Gadget_Part_var *current_part;
  while (iter.next (current_part))
    {
      (*current_part)->print_info ();
      iter.advance ();
    }
}
