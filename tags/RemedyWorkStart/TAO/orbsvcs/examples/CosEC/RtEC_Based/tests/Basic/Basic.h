/* -*- C++ -*- */

//=============================================================================
/**
 *  @file   Basic.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 *
 * This is a simple test class for the standard Cos Event Channel.
 *
 *
 */
//=============================================================================


#ifndef COSECBASIC_H
#define COSECBASIC_H

#include "Consumer.h"
#include "Supplier.h"

/**
 * @class Basic
 *
 * @brief class Basic
 *
 * Creates a CORBA Standard Event Channel (COSEC) implemented with
 * TAO's Real-time Event Channel (RtEC) and sends an event across.
 */
class Basic
{
public:
  // = Initialization and termination methods.
  /// Constructor.
  Basic (void);

  /// Destructor.
  ~Basic (void);

  /// Starts up an ORB and the CosEC.
  /// Returns 0 on success, -1 on error.
  void init (int argc, ACE_TCHAR *argv[]);

  /// Connects a consumer and a supplier to the CosEC and sends 1 event
  /// across.
  void run (void);

  /// Closes down the CosEC.
  void shutdown (void);

private:
  /// initializes the ORB.
  /// Returns 0 on success, -1 on error.
  void init_ORB (int argc, ACE_TCHAR *argv[]);

  /// initializes the COS EC.
  /// Returns 0 on success, -1 on error.
  void init_CosEC (void);

  /// The ORB that we use.
  CORBA::ORB_var orb_;

  /// The root poa.
  PortableServer::POA_var root_poa_;

  /// Reference to the CosEC returned after activating it in the ORB.
  CosEventChannelAdmin::EventChannel_var cos_ec_;

  /// The Cos Consumer that will receive the event.
  Consumer consumer_;

  /// The Cos Supplier that will supply the event.
  Supplier supplier_;
};

#endif /* COSECBASIC_H */
