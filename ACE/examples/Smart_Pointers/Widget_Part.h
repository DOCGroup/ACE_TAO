/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    Widget_Part.h
 *
 *  @author Christopher Kohlhoff <chris@kohlhoff.com>
 */
//=============================================================================

#ifndef WIDGET_PART_H
#define WIDGET_PART_H

/**
 * @class Widget_Part
 *
 * @brief An interface for some high-level application object.
 */
class Widget_Part
{
public:
  /// Destructor.
  virtual ~Widget_Part ();

  /// Ask the part to print information about itself.
  virtual void print_info () = 0;

  /// Ask the part to remove itself from the widget that contains it.
  virtual void remove_from_owner () = 0;
};

#endif /* WIDGET_PART_H */
