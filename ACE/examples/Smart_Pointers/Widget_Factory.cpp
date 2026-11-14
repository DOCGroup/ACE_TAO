/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    Widget_Factory.cpp
 *
 *  @author Christopher Kohlhoff <chris@kohlhoff.com>
 */
//=============================================================================

#include "Widget_Factory.h"
#include "Widget_Impl.h"

std::unique_ptr<Widget> Widget_Factory::create_widget ()
{
  return std::make_unique<Widget_Impl> ();
}
