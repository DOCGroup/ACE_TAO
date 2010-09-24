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

#include "ace/Auto_Ptr.h"
#include "ace/Refcounted_Auto_Ptr.h"
#include "ace/Unbounded_Queue.h"
#include "ace/Synch_Traits.h"
#include "ace/Thread_Mutex.h"
#include "ace/Null_Mutex.h"
#include "Widget.h"
#include "Widget_Factory.h"
#include "Widget_Part.h"
#include "Widget_Part_Factory.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
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

