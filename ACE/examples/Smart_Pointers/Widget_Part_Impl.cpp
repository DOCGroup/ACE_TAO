/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    Widget_Part_Impl.cpp
 *
 *  @author Christopher Kohlhoff <chris@kohlhoff.com>
 */
//=============================================================================

#include "Widget_Part_Impl.h"
#include "ace/Log_Msg.h"
#include "ace/Unbounded_Queue.h"
#include <memory>
#include <utility>

Widget_Part_Impl::Widget_Part_Impl (Widget *owner, char const *name, int size)
  : owner_ (owner),
    name_ (name),
    size_ (size)
{
  ACE_DEBUG ((LM_DEBUG, "Widget_Part_Impl constructor\n"));
}

Widget_Part_Impl::~Widget_Part_Impl ()
{
  ACE_DEBUG ((LM_DEBUG, "Widget_Part_Impl destructor\n"));
}

void Widget_Part_Impl::print_info ()
{
  ACE_DEBUG ((LM_INFO, "Widget part: name=%s size=%d\n", name_.c_str (), size_));
}

std::unique_ptr<Widget_Part> Widget_Part_Impl::remove_from_owner ()
{
  // Since we only have a raw pointer to refer to the owner, we have no way of
  // checking whether the owner still exists, and if it does guaranteeing that
  // it will continue to exist for the duration of this call. This is not an
  // issue in this limited example program, but in multithreaded applications
  // this function may be called from a different thread to that managing the
  // lifetime of the owner object. See the Gadget example for how
  // ACE_Strong_Bound_Ptr/ACE_Weak_Bound_Ptr can be used to address the problem.

  // The temporary queue holds parts owned by this method. The raw queue
  // entries are wrapped in unique_ptr again as they are returned to the owner.
  ACE_Unbounded_Queue<Widget_Part *> parts;
  std::unique_ptr<Widget_Part> detached_part {};
  for (;;)
    {
      std::unique_ptr<Widget_Part> part = owner_->remove_part ();
      if (!part)
        break;
      if (part.get () == this)
        detached_part = std::move (part);
      else
        {
          if (parts.enqueue_tail (part.get ()) == -1)
            {
              owner_->add_part (std::move (part));
              break;
            }
          part.release ();
        }
    }

  // Add the remaining parts back to the gadget.
  while (!parts.is_empty ())
    {
      Widget_Part *part {};
      parts.dequeue_head (part);
      owner_->add_part (std::unique_ptr<Widget_Part> {part});
    }

  return detached_part;
}
