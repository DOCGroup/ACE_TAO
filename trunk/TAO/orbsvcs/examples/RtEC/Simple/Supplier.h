/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel examples
//
// = FILENAME
//   Supplier
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef SUPPLIER_H
#define SUPPLIER_H

#include "orbsvcs/RtecEventCommS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Supplier : public POA_RtecEventComm::PushSupplier
{
  // = TITLE
  //   Simple supplier object
  //
  // = DESCRIPTION
  //   This class is a supplier of events.
  //   It simply publishes one event type.
  //
public:
  Supplier (void);
  // Constructor

  int run (int argc, ACE_TCHAR* argv[]);
  // Run the test

  // = The RtecEventComm::PushSupplier methods

  virtual void disconnect_push_supplier (void);
  // The skeleton methods.

private:
};

#endif /* SUPPLIER_H */
