/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    widget_test.cpp
 *
 *  $Id$
 *
 *  @author Christopher Kohlhoff <chris@kohlhoff.com>
 */
//=============================================================================

#include "ace/OS.h"
#include "ace/Auto_Ptr.h"
#include "ace/Refcounted_Auto_Ptr.h"
#include "ace/Unbounded_Queue.h"
#include "Widget.h"
#include "Widget_Factory.h"
#include "Widget_Part.h"
#include "Widget_Part_Factory.h"

int main (int argc, char *argv[])
{
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);

  auto_ptr<Widget> w1 (Widget_Factory::create_widget ());
  w1->add_part (Widget_Part_Factory::create_widget_part (w1.get(), "part1", 1));
  w1->add_part (Widget_Part_Factory::create_widget_part (w1.get(), "part2", 2));
  w1->add_part (Widget_Part_Factory::create_widget_part (w1.get(), "part3", 3));

  w1->list_parts ();

  auto_ptr<Widget_Part> p1 (w1->remove_part ());
  p1->print_info ();
  auto_ptr<Widget_Part> p2 (w1->remove_part ());

  w1->list_parts ();

  auto_ptr<Widget> w2 (Widget_Factory::create_widget ());
  w2->add_part (Widget_Part_Factory::create_widget_part (w2.get(), "part4", 4));
  Widget_Part *p3 = Widget_Part_Factory::create_widget_part (w2.get(), "part5", 5);
  w2->add_part (p3);
  p3->remove_from_owner ();

  w2->list_parts ();

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Auto_Basic_Ptr<Widget>;
template class auto_ptr<Widget>;
template class ACE_Auto_Basic_Ptr<Widget_Part>;
template class auto_ptr<Widget_Part>;
template class ACE_Refcounted_Auto_Ptr<Widget_Part, ACE_SYNCH_MUTEX>;
template class ACE_Node<ACE_Refcounted_Auto_Ptr<Widget_Part, ACE_SYNCH_MUTEX> >;
template class ACE_Unbounded_Queue<ACE_Refcounted_Auto_Ptr<Widget_Part, ACE_SYNCH_MUTEX> >;
template class ACE_Unbounded_Queue_Iterator<ACE_Refcounted_Auto_Ptr<Widget_Part, ACE_SYNCH_MUTEX> >;
template class ACE_Refcounted_Auto_Ptr<Widget_Part, ACE_Null_Mutex>;
template class ACE_Node<ACE_Refcounted_Auto_Ptr<Widget_Part, ACE_Null_Mutex> >;
template class ACE_Unbounded_Queue<ACE_Refcounted_Auto_Ptr<Widget_Part, ACE_Null_Mutex> >;
template class ACE_Unbounded_Queue_Iterator<ACE_Refcounted_Auto_Ptr<Widget_Part, ACE_Null_Mutex> >;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Auto_Basic_Ptr<Widget>
#pragma instantiate auto_ptr<Widget>
#pragma instantiate ACE_Auto_Basic_Ptr<Widget_Part>
#pragma instantiate auto_ptr<Widget_Part>
#pragma instantiate ACE_Refcounted_Auto_Ptr<Widget_Part, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Node<ACE_Refcounted_Auto_Ptr<Widget_Part, ACE_SYNCH_MUTEX> >
#pragma instantiate ACE_Unbounded_Queue<ACE_Refcounted_Auto_Ptr<Widget_Part, ACE_SYNCH_MUTEX> >
#pragma instantiate ACE_Unbounded_Queue_Iterator<ACE_Refcounted_Auto_Ptr<Widget_Part, ACE_SYNCH_MUTEX> >
#pragma instantiate ACE_Refcounted_Auto_Ptr<Widget_Part, ACE_Null_Mutex>
#pragma instantiate ACE_Node<ACE_Refcounted_Auto_Ptr<Widget_Part, ACE_Null_Mutex> >
#pragma instantiate ACE_Unbounded_Queue<ACE_Refcounted_Auto_Ptr<Widget_Part, ACE_Null_Mutex> >
#pragma instantiate ACE_Unbounded_Queue_Iterator<ACE_Refcounted_Auto_Ptr<Widget_Part, ACE_Null_Mutex> >
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

