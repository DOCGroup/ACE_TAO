/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = FILENAME
//   Updates.h
//
// = DESCRIPTION
//   Test to check if <offer_change> and <subscription_change> messages are
//   received correctly.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef NOTIFY_TESTS_UPDATES_H
#define NOTIFY_TESTS_UPDATES_H

#include "Notify_Test_Client.h"

class Notify_Test_Updates_Client : public Notify_Test_Client
{
 public:
  // Initialization and termination code
  Notify_Test_Updates_Client (void);
  virtual ~Notify_Test_Updates_Client ();

  virtual void init_concrete (int argc, char *argv [], CORBA::Environment &ACE_TRY_ENV);
  // initialization.

  virtual void run_test (CORBA::Environment &ACE_TRY_ENV);
  // Run the test.

};

#endif /* NOTIFY_TESTS_UPDATES_H */
