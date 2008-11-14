/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = FILENAME
//   Multiple.h
//
// = AUTHOR
//   Pradeep Gore <pradeep@cs.wustl.edu>
//
// = DESCRIPTION
//   This is a test class for the Cos Event Service.
//
// ============================================================================

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
  Multiple (void);
  // Constructor.

  virtual ~Multiple (void);
  // Destructor.

  int init (int argc, ACE_TCHAR *argv[]);
  // Calls parse_args, Starts up an ORB, gets hold of the Event Service.
  // Returns 0 on success, -1 on error.

  int runORB (void);
  // Calls the ORB's <run> method.

  void shutdown (void);
  // Closes down the ORB and exits.

 protected:
  virtual int parse_args (int argc, ACE_TCHAR *argv []) = 0;
  // Parse the command line arguments.

  CosEventChannelAdmin::EventChannel_ptr cos_ec_;
  // Reference to a running Event Service.

  const ACE_TCHAR *service_name_;
  // The name with which to locate the Event Service.

 private:
  int init_ORB (int argc, ACE_TCHAR *argv[]);
  // initializes the ORB.
  // Returns 0 on success, -1 on error.

  int init_CosEC (void);
  // initializes the COS EC.
  // Returns 0 on success, -1 on error.

  CORBA::ORB_var orb_;
  // The ORB that we use.

  TAO_Naming_Client naming_client_;
  // An instance of the name client used for resolving the factory
  // objects.
};

#endif /* COSECMULTIPLE_H */
