/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    Gadget_Part_Impl.h
 *
 *  $Id$
 *
 *  @author Christopher Kohlhoff <chris@kohlhoff.com>
 */
//=============================================================================

#ifndef GADGET_PART_IMPL_H
#define GADGET_PART_IMPL_H

#include "Gadget_Part.h"
#include "Gadget.h"

/**
 * @class Gadget_Part_Impl
 *
 * @brief An implementation of the Gadget_Part interface.
 */
class Gadget_Part_Impl : public Gadget_Part
{
public:
  /// Constructor.
  Gadget_Part_Impl (Gadget_ptr owner, const char* name, int size);

  /// Destructor.
  virtual ~Gadget_Part_Impl (void);

  /// Ask the part to print information about itself.
  virtual void print_info (void);

  /// Ask the part to remove itself from the gadget that contains it.
  virtual void remove_from_owner (void);

private:
  /// The gadget that contains this part.
  ///
  /// Some things to note about the choice of ACE_Weak_Bound_Ptr (from the
  /// typedef for Gadget_ptr):
  /// - We cannot use an ACE_Strong_Bound_Ptr (Gadget_var) since that would
  ///   result in circular ownership.
  /// - If we use a raw pointer we have no circular ownership problems, but we
  ///   are unable to guarantee the lifetime of the owner object for the
  ///   duration of the remove_from_owner call. This may not be a problem in
  ///   this limited example, but in multithreaded programs remove_from_owner
  ///   may be called from a different thread to the thread which manages the
  ///   owner's lifetime.
  /// - ACE_Weak_Bound_Ptr (Gadget_ptr) has no ownership semantics, so we have
  ///   no circular ownership problems. Weak pointers can also be converted
  ///   back into strong ones, so it is possible to guarantee the lifetime of
  ///   the owner object for the duration of the remove_from_owner call.
  Gadget_ptr owner_;

  /// The name of this part.
  char *name_;

  /// The size of this part.
  int size_;
};

#endif /* GADGET_PART_IMPL_H */
