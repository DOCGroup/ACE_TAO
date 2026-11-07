/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    Widget.h
 *
 *  @author Christopher Kohlhoff <chris@kohlhoff.com>
 */
//=============================================================================

#ifndef WIDGET_H
#define WIDGET_H

#include <memory>

#include "Widget_Part.h"

/**
 * @class Widget
 *
 * @brief An interface for some high-level application object.
 */
class Widget
{
public:
  Widget () = default;

  /// Destructor.
  virtual ~Widget ();

  Widget (Widget const &) = delete;
  Widget (Widget &&) = delete;
  Widget &operator= (Widget const &) = delete;
  Widget &operator= (Widget &&) = delete;

  /// Add a new part to the widget. The widget takes ownership of the part
  /// object.
  virtual void add_part (std::unique_ptr<Widget_Part> part) = 0;

  /// Remove a random part from the widget. Ownership of the part is returned
  /// to the caller.
  virtual std::unique_ptr<Widget_Part> remove_part () = 0;

  /// Ask the widget to print information about the parts that it contains.
  virtual void list_parts () = 0;
};

#endif /* WIDGET_H */
