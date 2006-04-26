/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    gadget_test.cpp
 *
 *  $Id$
 *
 *  @author Christopher Kohlhoff <chris@kohlhoff.com>
 */
//=============================================================================

#include "ace/Auto_Ptr.h"
#include "ace/Refcounted_Auto_Ptr.h"
#include "ace/Unbounded_Queue.h"
#include "Gadget.h"
#include "Gadget_Factory.h"
#include "Gadget_Part.h"
#include "Gadget_Part_Factory.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);

  Gadget_var g1 = Gadget_Factory::create_gadget ();
  g1->add_part (Gadget_Part_Factory::create_gadget_part (g1, "part1", 1));
  g1->add_part (Gadget_Part_Factory::create_gadget_part (g1, "part2", 2));
  g1->add_part (Gadget_Part_Factory::create_gadget_part (g1, "part3", 3));

  g1->list_parts ();

  Gadget_Part_var p1 = g1->remove_part ();
  p1->print_info ();

  // Oops, we forgot to collect the return value! No worries, the temporary
  // Gadget_var returned by the function call will clean it up automatically.
  g1->remove_part ();

  g1->list_parts ();

  Gadget_var g2 = Gadget_Factory::create_gadget ();
  g2->add_part (Gadget_Part_Factory::create_gadget_part (g2, "part4", 4));
  Gadget_Part_var p2 = Gadget_Part_Factory::create_gadget_part (g2, "part5", 5);
  g2->add_part (p2);
  p2->remove_from_owner ();

  g2->list_parts ();

  return 0;
}


