/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    Widget_Part_Factory.cpp
 *
 *  @author Christopher Kohlhoff <chris@kohlhoff.com>
 */
//=============================================================================

#include "Widget_Part_Factory.h"
#include "Widget_Part_Impl.h"

std::unique_ptr<Widget_Part> Widget_Part_Factory::create_widget_part (
  Widget *owner,
  char const *name,
  int size)
{
  return std::make_unique<Widget_Part_Impl> (owner, name, size);
}
