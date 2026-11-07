/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Supplier.h
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
 * It simply publishes one event type.
 */
class Supplier : public POA_RtecEventComm::PushSupplier
{
public:
  /// Constructor
  Supplier () = default;
  Supplier (Supplier const &) = delete;
  Supplier (Supplier &&) = delete;
  Supplier &operator= (Supplier const &) = delete;
  Supplier &operator= (Supplier &&) = delete;

  /// Run the test
  int run (int argc, ACE_TCHAR* argv[]);

  // = The RtecEventComm::PushSupplier methods

  /// The skeleton methods.
  void disconnect_push_supplier () override;

private:
};

#endif /* SUPPLIER_H */
