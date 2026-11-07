// -*- C++ -*-

//=============================================================================
/**
 *  @file    Widget_Impl.h
 *
 *  @author Christopher Kohlhoff <chris@kohlhoff.com>
 */
//=============================================================================

#ifndef WIDGET_IMPL_H
#define WIDGET_IMPL_H

#include "Widget.h"
#include "Widget_Part.h"

#include "ace/Unbounded_Queue.h"

/**
 * @class Widget_Impl
 *
 * @brief An implementation of the Widget interface.
 */
class Widget_Impl : public Widget
{
public:
  /// Constructor.
  Widget_Impl ();

  /// Destructor.
  ~Widget_Impl () override;

  Widget_Impl (Widget_Impl const &) = delete;
  Widget_Impl (Widget_Impl &&) = delete;
  Widget_Impl &operator= (Widget_Impl const &) = delete;
  Widget_Impl &operator= (Widget_Impl &&) = delete;

  /// Add a new part to the widget. The widget takes ownership of the part
  /// object.
  void add_part (std::unique_ptr<Widget_Part> part) override;

  /// Remove a random part from the widget. Ownership of the part is returned
  /// to the caller.
  std::unique_ptr<Widget_Part> remove_part () override;

  /// Ask the widget to print information about the parts that it contains.
  void list_parts () override;

private:
  /// The queue owns its raw part pointers. add_part() transfers ownership
  /// from a unique_ptr, remove_part() returns it, and the destructor cleans
  /// up any parts that remain in the queue.
  ACE_Unbounded_Queue<Widget_Part *> parts_;
};

#endif /* WIDGET_IMPL_H */
