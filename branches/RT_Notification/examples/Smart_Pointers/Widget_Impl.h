/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    Widget_Impl.h
 *
 *  $Id$
 *
 *  @author Christopher Kohlhoff <chris@kohlhoff.com>
 */
//=============================================================================

#ifndef WIDGET_IMPL_H
#define WIDGET_IMPL_H

#include "ace/Unbounded_Queue.h"
#include "ace/Refcounted_Auto_Ptr.h"
#include "Widget.h"
#include "Widget_Part.h"

/**
 * @class Widget_Impl
 *
 * @brief An implementation of the Widget interface.
 */
class Widget_Impl : public Widget
{
public:
  /// Constructor.
  Widget_Impl (void);

  /// Destructor.
  virtual ~Widget_Impl (void);

  /// Add a new part to the widget. The widget takes ownership of the part
  /// object.
  virtual void add_part (Widget_Part *part);

  /// Remove a random part from the widget. Ownership of the part is returned
  /// to the caller.
  virtual Widget_Part *remove_part (void);

  /// Ask the widget to print information about the parts that it contains.
  virtual void list_parts (void);

private:
  /// The parts which make up this widget. The set actually contains instances
  /// of ACE_Refcounted_Auto_Ptr to automatically manage the lifetimes of the
  /// constituent parts.
  ///
  /// Some things to note about the choice of ACE_Refcounted_Auto_Ptr:
  /// - We cannot use auto_ptr to manage the objects, since auto_ptr does not
  ///   support the copying and assignment semantics necessary for storage in
  ///   a container.
  /// - The ACE_Strong_Bound_Ptr reference counted pointer could be used to
  ///   store objects in a container, however (for reasons of safety) it
  ///   provides no way to release ownership of the object from the smart
  ///   pointer. We need to be able to release ownership to implement the
  ///   remove_part method.
  /// - ACE_Refcounted_Ptr can both be stored in containers and allows us to
  ///   release ownership of the pointer that it contains.
  ACE_Unbounded_Queue<ACE_Refcounted_Auto_Ptr<Widget_Part, ACE_SYNCH_MUTEX> > parts_;
};

#endif /* WIDGET_IMPL_H */
