/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = FILENAME
//   CosEC_Basic.h
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

#include "CosECConsumer.h"
#include "CosECSupplier.h"

class CosEC_Basic
{
  // = TITLE
  //   class CosEC_Basic
  //
  // = DESCRIPTION
  //   Creates a CORBA Standard Event Channel (COSEC) implemented with
  //   TAO's Real-time Event Channel (RtEC) and sends an event across.
public:
  // = Initialization and termination methods.
  CosEC_Basic (void);
  // Constructor.

  ~CosEC_Basic (void);
  // Destructor.

  void init (int argc, char *argv[],CORBA::Environment &ACE_TRY_ENV);
  // Starts up an ORB and the CosEC.
  // Returns 0 on success, -1 on error.

  void run (CORBA::Environment &ACE_TRY_ENV);
  // Connects a consumer and a supplier to the CosEC and sends 1 event
  // across.

  void shutdown (CORBA::Environment &ACE_TRY_ENV);
  // Closes down the CosEC.

private:
  void init_ORB (int argc, char *argv[], CORBA::Environment &ACE_TRY_ENV);
  // initializes the ORB.
  // Returns 0 on success, -1 on error.

  void init_CosEC (CORBA::Environment &ACE_TRY_ENV);
  // initializes the COS EC.
  // Returns 0 on success, -1 on error.

  CORBA::ORB_var orb_;
  // The ORB that we use.

  PortableServer::POA_var root_poa_;
  // The root poa.

  CosEventChannelAdmin::EventChannel_var cos_ec_;
  // Reference to the CosEC returned after activating it in the ORB.

  CosECConsumer consumer_;
  // The Cos Consumer that will receive the event.

  CosECSupplier supplier_;
  // The Cos Supplier that will supply the event.
};

#endif /* COSECBASIC_H */
