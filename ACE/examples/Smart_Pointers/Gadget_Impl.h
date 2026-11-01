/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    Gadget_Impl.h
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
  Gadget_Impl ();

  /// Destructor.
  ~Gadget_Impl () override;

  Gadget_Impl (Gadget_Impl const &) = delete;
  Gadget_Impl (Gadget_Impl &&) = delete;
  Gadget_Impl &operator= (Gadget_Impl const &) = delete;
  Gadget_Impl &operator= (Gadget_Impl &&) = delete;

  /// Add a new part to the gadget. The gadget takes ownership of the part
  /// object.
  void add_part (Gadget_Part_var part) override;

  /// Remove a random part from the gadget. Ownership of the part is returned
  /// to the caller.
  Gadget_Part_var remove_part () override;

  /// Ask the gadget to print information about the parts that it contains.
  void list_parts () override;

private:
  /// The parts which make up this gadget. The set actually contains instances
  /// of Gadget_Part_var to automatically manage the lifetimes of the
  /// constituent parts.
  ACE_Unbounded_Queue<Gadget_Part_var> parts_;
};

#endif /* GADGET_IMPL_H */
