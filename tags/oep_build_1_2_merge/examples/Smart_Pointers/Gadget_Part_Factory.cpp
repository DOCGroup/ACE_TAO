/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    Gadget_Part_Factory.cpp
 *
 *  $Id$
 *
 *  @author Christopher Kohlhoff <chris@kohlhoff.com>
 */
//=============================================================================

#include "Gadget_Part_Factory.h"
#include "Gadget_Part_Impl.h"

Gadget_Part_var Gadget_Part_Factory::create_gadget_part (Gadget_ptr owner,
                                                         const char* name,
                                                         int size)
{
  return Gadget_Part_var (new Gadget_Part_Impl (owner, name, size));
}
