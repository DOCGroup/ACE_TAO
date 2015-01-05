/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    Widget_Impl.cpp
 *
 *  $Id$
 *
 *  @author Christopher Kohlhoff <chris@kohlhoff.com>
 */
//=============================================================================

#include "Widget_Impl.h"
#include "ace/Log_Msg.h"

Widget_Impl::Widget_Impl (void)
{
  ACE_DEBUG ((LM_DEBUG, "Widget_Impl constructor\n"));
}

Widget_Impl::~Widget_Impl (void)
{
  ACE_DEBUG ((LM_DEBUG, "Widget_Impl destructor\n"));
}

void Widget_Impl::add_part (Widget_Part *part)
{
  // Take ownership of the part object using a ACE_Refcounted_Auto_Ptr.
  ACE_Refcounted_Auto_Ptr<Widget_Part, ACE_SYNCH_MUTEX> new_part (part);

  parts_.enqueue_tail (new_part);
}

Widget_Part *Widget_Impl::remove_part (void)
{
  ACE_Refcounted_Auto_Ptr<Widget_Part, ACE_SYNCH_MUTEX> removed_part;
  if (parts_.dequeue_head (removed_part) == -1)
    return 0;

  // Ownership of the part object is released and transferred to the caller.
  return removed_part.release();
}

void Widget_Impl::list_parts (void)
{
  ACE_Unbounded_Queue_Iterator<ACE_Refcounted_Auto_Ptr<Widget_Part, ACE_SYNCH_MUTEX> > iter (parts_);
  ACE_Refcounted_Auto_Ptr<Widget_Part, ACE_SYNCH_MUTEX> *current_part;
  while (iter.next (current_part))
    {
      (*current_part)->print_info ();
      iter.advance ();
    }
}
