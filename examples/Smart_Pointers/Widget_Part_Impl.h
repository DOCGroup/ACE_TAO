/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    Widget_Part_Impl.h
 *
 *  $Id$
 *
 *  @author Christopher Kohlhoff <chris@kohlhoff.com>
 */
//=============================================================================

#ifndef WIDGET_PART_IMPL_H
#define WIDGET_PART_IMPL_H

#include "Widget_Part.h"
#include "Widget.h"

/**
 * @class Widget_Part_Impl
 *
 * @brief An implementation of the Widget_Part interface.
 */
class Widget_Part_Impl : public Widget_Part
{
public:
  /// Constructor.
  Widget_Part_Impl (Widget *owner, const char* name, int size);

  /// Destructor.
  virtual ~Widget_Part_Impl (void);

  /// Ask the part to print information about itself.
  virtual void print_info (void);

  /// Ask the part to remove itself from the widget that contains it.
  virtual void remove_from_owner (void);

private:
  /// The widget that contains this part.
  Widget *owner_;

  /// The name of this part.
  char *name_;

  /// The size of this part.
  int size_;
};

#endif /* WIDGET_PART_IMPL_H */
