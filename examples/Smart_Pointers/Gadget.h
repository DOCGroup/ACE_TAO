/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    Gadget.h
 *
 *  $Id$
 *
 *  @author Christopher Kohlhoff <chris@kohlhoff.com>
 */
//=============================================================================

#ifndef GADGET_H
#define GADGET_H

#include "ace/Bound_Ptr.h"
#include "Gadget_Part.h"

/**
 * @class Gadget
 *
 * @brief An interface for some high-level application object.
 */
class Gadget
{
public:
  /// Destructor.
  virtual ~Gadget (void);

  /// Add a new part to the gadget. The gadget automatically takes shared
  /// responsibility for the ownership of the part object since we are passing
  /// a Gadget_Part_var.
  virtual void add_part (Gadget_Part_var part) = 0;

  /// Remove a random part from the gadget. Responsibility for ownership of the
  /// part is automatically returned to the caller since we are returning a
  /// Gadget_Part_var.
  virtual Gadget_Part_var remove_part (void) = 0;

  /// Ask the gadget to print information about the parts that it contains.
  virtual void list_parts (void) = 0;
};

// The Gadget_var smart pointer has shared (reference counted) ownership
// semantics.
typedef ACE_Strong_Bound_Ptr<Gadget, ACE_SYNCH_MUTEX> Gadget_var;

// The Gadget_ptr smart pointer has no ownership semantics, but supports
// conversion back into a Gadget_var.
typedef ACE_Weak_Bound_Ptr<Gadget, ACE_SYNCH_MUTEX> Gadget_ptr;

#endif /* GADGET_H */
