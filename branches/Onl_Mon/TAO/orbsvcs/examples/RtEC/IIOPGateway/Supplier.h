/* -*- C++ -*- */
/**
 *  @file   Supplier.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 *  IIOP Gateway
 */
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
 * This class is a supplier of events. It simply publishes one event type.
 */
class Supplier : public POA_RtecEventComm::PushSupplier
{
public:
  /// Constructor
  Supplier (void);

  /// Run the test
  int run (int argc, char* argv[]);

  // = The RtecEventComm::PushSupplier methods

  /// The skeleton methods.
  virtual void disconnect_push_supplier (void);

private:
  int parse_args (int argc, char *argv[]);
};

#endif /* SUPPLIER_H */
