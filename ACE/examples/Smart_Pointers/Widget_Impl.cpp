/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    Widget_Impl.cpp
 *
 *  @author Christopher Kohlhoff <chris@kohlhoff.com>
 */
//=============================================================================

#include "Widget_Impl.h"
#include "ace/Log_Msg.h"
#include <memory>

Widget_Impl::Widget_Impl ()
{
  ACE_DEBUG ((LM_DEBUG, "Widget_Impl constructor\n"));
}

Widget_Impl::~Widget_Impl ()
{
  ACE_DEBUG ((LM_DEBUG, "Widget_Impl destructor\n"));
  Widget_Part *part {};
  while (parts_.dequeue_head (part) == 0)
    {
      std::unique_ptr<Widget_Part> owned_part {part};
    }
}

void Widget_Impl::add_part (std::unique_ptr<Widget_Part> part)
{
  if (parts_.enqueue_tail (part.get ()) == 0)
    part.release ();
}

std::unique_ptr<Widget_Part> Widget_Impl::remove_part ()
{
  Widget_Part *part {};
  if (parts_.dequeue_head (part) == -1)
    return {};

  // Ownership of the part object is released and transferred to the caller.
  return std::unique_ptr<Widget_Part> {part};
}

void Widget_Impl::list_parts ()
{
  ACE_Unbounded_Queue_Iterator<Widget_Part *> iter (parts_);
  Widget_Part **current_part {};
  while (iter.next (current_part))
    {
      (*current_part)->print_info ();
      iter.advance ();
    }
}
