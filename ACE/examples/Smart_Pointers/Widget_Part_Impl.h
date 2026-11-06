/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    Widget_Part_Impl.h
 *
 *  @author Christopher Kohlhoff <chris@kohlhoff.com>
 */
//=============================================================================

#ifndef WIDGET_PART_IMPL_H
#define WIDGET_PART_IMPL_H

#include "Widget_Part.h"
#include "Widget.h"
#include <string>

/**
 * @class Widget_Part_Impl
 *
 * @brief An implementation of the Widget_Part interface.
 */
class Widget_Part_Impl : public Widget_Part
{
public:
  /// Constructor.
  Widget_Part_Impl (Widget *owner, char const *name, int size);

  /// Destructor.
  ~Widget_Part_Impl () override;

  Widget_Part_Impl (Widget_Part_Impl const &) = delete;
  Widget_Part_Impl (Widget_Part_Impl &&) = delete;
  Widget_Part_Impl &operator= (Widget_Part_Impl const &) = delete;
  Widget_Part_Impl &operator= (Widget_Part_Impl &&) = delete;

  /// Ask the part to print information about itself.
  void print_info () override;

  /// Ask the part to remove itself from the widget that contains it.
  std::unique_ptr<Widget_Part> remove_from_owner () override;

private:
  /// The widget that contains this part.
  Widget * const owner_ {};

  /// The name of this part.
  std::string const name_;

  /// The size of this part.
  int const size_ {};
};

#endif /* WIDGET_PART_IMPL_H */
