/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    Gadget_Factory.cpp
 *
 *  @author Christopher Kohlhoff <chris@kohlhoff.com>
 */
//=============================================================================

#include "Gadget_Factory.h"
#include "Gadget_Impl.h"

Gadget_var Gadget_Factory::create_gadget ()
{
  return Gadget_var (new Gadget_Impl);
}
