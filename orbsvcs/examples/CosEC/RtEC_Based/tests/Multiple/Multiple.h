/* -*- C++ -*- */

//=============================================================================
/**
 *  @file   Multiple.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 *
 * This is a test class for the Cos Event Service.
 *
 *
 */
//=============================================================================


#ifndef COSECMULTIPLE_H
#define COSECMULTIPLE_H

#include "orbsvcs/Naming/Naming_Client.h"
#include "orbsvcs/Naming/Naming_Server.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/CosEventChannelAdminC.h"

class Multiple
{
 public:
  // = TITLE
  //   class Multiple
  //
  // = DESCRIPTION
  //   Base class for suppliers and consumers to send/receive events
  //   via the Cos Event Service.
 public:
  // = Initialization and termination methods.
  /// Constructor.
  Multiple (void);

  /// Destructor.
  virtual ~Multiple (void);

  /// Calls parse_args, Starts up an ORB, gets hold of the Event Service.
  /// Returns 0 on success, -1 on error.
  int init (int argc, ACE_TCHAR *argv[]);

  /// Calls the ORB's <run> method.
  int runORB (void);

  /// Closes down the ORB and exits.
  void shutdown (void);

 protected:
  /// Parse the command line arguments.
  virtual int parse_args (int argc, ACE_TCHAR *argv []) = 0;

  /// Reference to a running Event Service.
  CosEventChannelAdmin::EventChannel_ptr cos_ec_;

  /// The name with which to locate the Event Service.
  const ACE_TCHAR *service_name_;

 private:
  /// initializes the ORB.
  /// Returns 0 on success, -1 on error.
  int init_ORB (int argc, ACE_TCHAR *argv[]);

  /// initializes the COS EC.
  /// Returns 0 on success, -1 on error.
  int init_CosEC (void);

  /// The ORB that we use.
  CORBA::ORB_var orb_;

  /// An instance of the name client used for resolving the factory
  /// objects.
  TAO_Naming_Client naming_client_;
};

#endif /* COSECMULTIPLE_H */
