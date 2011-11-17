/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    Widget_Factory.cpp
 *
 *  $Id$
 *
 *  @author Christopher Kohlhoff <chris@kohlhoff.com>
 */
//=============================================================================

#include "Widget_Factory.h"
#include "Widget_Impl.h"

Widget *Widget_Factory::create_widget (void)
{
  return new Widget_Impl;
}
