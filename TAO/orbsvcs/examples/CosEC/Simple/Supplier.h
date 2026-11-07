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

#include "orbsvcs/CosEventCommS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class Supplier
 *
 * @brief Simple supplier object
 *
 * This class is a supplier of events.
 */
class Supplier : public POA_CosEventComm::PushSupplier
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

  // = The CosEventComm::PushSupplier methods

  /// The skeleton methods.
  void disconnect_push_supplier () override;

private:
};

#endif /* SUPPLIER_H */
