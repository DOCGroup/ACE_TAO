/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    Gadget_Part.h
 *
 *  $Id$
 *
 *  @author Christopher Kohlhoff <chris@kohlhoff.com>
 */
//=============================================================================

#ifndef GADGET_PART_H
#define GADGET_PART_H

#include "ace/Bound_Ptr.h"

/**
 * @class Gadget_Part
 *
 * @brief An interface for some high-level application object.
 */
class Gadget_Part
{
public:
  /// Destructor.
  virtual ~Gadget_Part (void);

  /// Ask the part to print information about itself.
  virtual void print_info (void) = 0;

  /// Ask the part to remove itself from the gadget that contains it.
  virtual void remove_from_owner (void) = 0;
};

// The Gadget_Part_var smart pointer has shared (reference counted) ownership
// semantics.
typedef ACE_Strong_Bound_Ptr<Gadget_Part, ACE_SYNCH_MUTEX> Gadget_Part_var;

// The Gadget_Part_ptr smart pointer has no ownership semantics, but supports
// conversion back into a Gadget_var.
typedef ACE_Weak_Bound_Ptr<Gadget_Part, ACE_SYNCH_MUTEX> Gadget_Part_ptr;

#endif /* GADGET_PART_H */
