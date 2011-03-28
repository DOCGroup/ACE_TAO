/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Supplier
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef SUPPLIER_H
#define SUPPLIER_H

#include "orbsvcs/RtecEventCommS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class Supplier
 *
 * @brief Simple supplier object
 *
 * This class is a supplier of events.
 * It simply register for two event typesone event type
 * The class is just a helper to simplify common tasks in EC
 * tests, such as subscribing for a range of events, disconnecting
 * from the EC, informing the driver of shutdown messages, etc.
 * There are several ways to connect and disconnect this class,
 * and it is up to the driver program to use the right one.
 */
class Supplier : public POA_RtecEventComm::PushSupplier
{
public:
  /// Constructor
  Supplier (void);

  // = The RtecEventComm::PushSupplier methods

  /// The skeleton methods.
  virtual void disconnect_push_supplier (void);

private:
};

#endif /* SUPPLIER_H */
