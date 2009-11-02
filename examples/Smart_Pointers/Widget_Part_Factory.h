/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    Widget_Part_Factory.h
 *
 *  $Id$
 *
 *  @author Christopher Kohlhoff <chris@kohlhoff.com>
 */
//=============================================================================

#ifndef WIDGET_PART_FACTORY_H
#define WIDGET_PART_FACTORY_H

#include "Widget_Part.h"
#include "Widget.h"

/**
 * @class Widget_Part_Factory
 *
 * @brief Used to create Widget_Part instances.
 */
class Widget_Part_Factory
{
public:
  /// Create an instance of a widget part. Ownership of the newly created
  /// object is transferred to the caller.
  static Widget_Part *create_widget_part (Widget *owner, const char *name, int size);
};

#endif /* WIDGET_PART_FACTORY_H */
