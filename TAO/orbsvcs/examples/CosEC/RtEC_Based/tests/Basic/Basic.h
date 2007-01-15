/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = FILENAME
//   Basic.h
//
// = AUTHOR
//   Pradeep Gore <pradeep@cs.wustl.edu>
//
// = DESCRIPTION
//   This is a simple test class for the standard Cos Event Channel.
//
// ============================================================================

#ifndef COSECBASIC_H
#define COSECBASIC_H

#include "Consumer.h"
#include "Supplier.h"

class Basic
{
  // = TITLE
  //   class Basic
  //
  // = DESCRIPTION
  //   Creates a CORBA Standard Event Channel (COSEC) implemented with
  //   TAO's Real-time Event Channel (RtEC) and sends an event across.
public:
  // = Initialization and termination methods.
  Basic (void);
  // Constructor.

  ~Basic (void);
  // Destructor.

  void init (int argc, char *argv[] ACE_ENV_ARG_DECL);
  // Starts up an ORB and the CosEC.
  // Returns 0 on success, -1 on error.

  void run (ACE_ENV_SINGLE_ARG_DECL);
  // Connects a consumer and a supplier to the CosEC and sends 1 event
  // across.

  void shutdown (ACE_ENV_SINGLE_ARG_DECL);
  // Closes down the CosEC.

private:
  void init_ORB (int argc, char *argv[] ACE_ENV_ARG_DECL);
  // initializes the ORB.
  // Returns 0 on success, -1 on error.

  void init_CosEC (ACE_ENV_SINGLE_ARG_DECL);
  // initializes the COS EC.
  // Returns 0 on success, -1 on error.

  CORBA::ORB_var orb_;
  // The ORB that we use.

  PortableServer::POA_var root_poa_;
  // The root poa.

  CosEventChannelAdmin::EventChannel_var cos_ec_;
  // Reference to the CosEC returned after activating it in the ORB.

  Consumer consumer_;
  // The Cos Consumer that will receive the event.

  Supplier supplier_;
  // The Cos Supplier that will supply the event.
};

#endif /* COSECBASIC_H */
