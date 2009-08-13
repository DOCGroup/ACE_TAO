/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    Gadget_Part_Factory.h
 *
 *  $Id$
 *
 *  @author Christopher Kohlhoff <chris@kohlhoff.com>
 */
//=============================================================================

#ifndef GADGET_PART_FACTORY_H
#define GADGET_PART_FACTORY_H

#include "Gadget_Part.h"
#include "Gadget.h"

/**
 * @class Gadget_Part_Factory
 *
 * @brief Used to create Gadget_Part instances.
 */
class Gadget_Part_Factory
{
public:
  /// Create an instance of a gadget. Ownership of the object is automatically
  /// transferred to the caller since we return a Gadget_Part_var. This also
  /// means that the object will be deleted automatically if the caller
  /// "forgets" to collect the return value.
  static Gadget_Part_var create_gadget_part (Gadget_ptr owner,
                                             const char *name,
                                             int size);
};

#endif /* GADGET_PART_FACTORY_H */
