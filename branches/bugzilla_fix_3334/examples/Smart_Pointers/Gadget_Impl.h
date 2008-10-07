/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    Gadget_Impl.h
 *
 *  $Id$
 *
 *  @author Christopher Kohlhoff <chris@kohlhoff.com>
 */
//=============================================================================

#ifndef GADGET_IMPL_H
#define GADGET_IMPL_H

#include "ace/Unbounded_Queue.h"
#include "Gadget.h"
#include "Gadget_Part.h"

/**
 * @class Gadget_Impl
 *
 * @brief An implementation of the Gadget interface.
 */
class Gadget_Impl : public Gadget
{
public:
  /// Constructor.
  Gadget_Impl (void);

  /// Destructor.
  virtual ~Gadget_Impl (void);

  /// Add a new part to the gadget. The gadget takes ownership of the part
  /// object.
  virtual void add_part (Gadget_Part_var part);

  /// Remove a random part from the gadget. Ownership of the part is returned
  /// to the caller.
  virtual Gadget_Part_var remove_part (void);

  /// Ask the gadget to print information about the parts that it contains.
  virtual void list_parts (void);

private:
  /// The parts which make up this gadget. The set actually contains instances
  /// of Gadget_Part_var to automatically manage the lifetimes of the
  /// constituent parts.
  ACE_Unbounded_Queue<Gadget_Part_var> parts_;
};

#endif /* GADGET_IMPL_H */
