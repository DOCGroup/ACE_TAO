/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    Widget_Part_Factory.cpp
 *
 *  $Id$
 *
 *  @author Christopher Kohlhoff <chris@kohlhoff.com>
 */
//=============================================================================

#include "Widget_Part_Factory.h"
#include "Widget_Part_Impl.h"

Widget_Part *Widget_Part_Factory::create_widget_part (Widget *owner,
                                                      const char* name,
                                                      int size)
{
  return new Widget_Part_Impl (owner, name, size);
}
