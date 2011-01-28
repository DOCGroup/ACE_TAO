/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    Widget_Part_Impl.cpp
 *
 *  $Id$
 *
 *  @author Christopher Kohlhoff <chris@kohlhoff.com>
 */
//=============================================================================

#include "Widget_Part_Impl.h"
#include "ace/ACE.h"
#include "ace/Log_Msg.h"
#include "ace/Refcounted_Auto_Ptr.h"
#include "ace/Unbounded_Queue.h"
#include "ace/Null_Mutex.h"

Widget_Part_Impl::Widget_Part_Impl (Widget *owner, const char* name, int size)
  : owner_ (owner),
    name_ (ACE::strnew (name)),
    size_ (size)
{
  ACE_DEBUG ((LM_DEBUG, "Widget_Part_Impl constructor\n"));
}

Widget_Part_Impl::~Widget_Part_Impl (void)
{
  ACE_DEBUG ((LM_DEBUG, "Widget_Part_Impl destructor\n"));

  delete [] name_;
}

void Widget_Part_Impl::print_info (void)
{
  ACE_DEBUG ((LM_INFO, "Widget part: name=%s size=%d\n", name_, size_));
}

void Widget_Part_Impl::remove_from_owner (void)
{
  // Since we only have a raw pointer to refer to the owner, we have no way of
  // checking whether the owner still exists, and if it does guaranteeing that
  // it will continue to exist for the duration of this call. This is not an
  // issue in this limited example program, but in multithreaded applications
  // this function may be called from a different thread to that managing the
  // lifetime of the owner object. See the Gadget example for how
  // ACE_Strong_Bound_Ptr/ACE_Weak_Bound_Ptr can be used to address the problem.

  // Take all existing parts from the owner and build up a temporary queue. If
  // we find ourselves then we won't add ourselves to the queue. We will
  // actually store ACE_Refcounted_Auto_Ptr instances in the queue, and since we
  // know that there is only one thread involved we can use ACE_Null_Mutex to
  // eliminate the locking overhead.
  ACE_Unbounded_Queue<ACE_Refcounted_Auto_Ptr<Widget_Part, ACE_Null_Mutex> > parts;
  for (;;)
    {
      ACE_Refcounted_Auto_Ptr<Widget_Part, ACE_Null_Mutex> part (owner_->remove_part ());
      if (part.null ())
        break;
      if (part.get () == this)
        // Someone else will be responsible for our lifetime.
        part.release();
      else
        parts.enqueue_tail (part);
    }

  // Add the remaining parts back to the gadget.
  while (!parts.is_empty ())
    {
      ACE_Refcounted_Auto_Ptr<Widget_Part, ACE_Null_Mutex> part;
      parts.dequeue_head (part);
      owner_->add_part (part.release ());
    }
}
