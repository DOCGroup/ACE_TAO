/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = FILENAME
//   CosEC_Multiple.h
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

#include "orbsvcs/Naming/Naming_Utils.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/CosEventChannelAdminC.h"

class CosEC_Multiple
{
 public:
  // = TITLE
  //   class CosEC_Multiple
  //
  // = DESCRIPTION
  //   Base class for suppliers and consumers to send/receive events
  //   via the Cos Event Service.
 public:
  // = Initialization and termination methods.
  CosEC_Multiple (void);
  // Constructor.

  virtual ~CosEC_Multiple (void);
  // Destructor.

  int init (int argc, char *argv[]);
  // Calls parse_args, Starts up an ORB, gets hold of the Event Service.
  // Returns 0 on success, -1 on error.

  int runORB (void);
  // Calls the ORB's <run> method.

  void shutdown (void);
  // Closes down the ORB and exits.

 protected:
  virtual int parse_args (int argc, char *argv []) = 0;
  // Parse the command line arguments.

  CosEventChannelAdmin::EventChannel_ptr cos_ec_;
  // Reference to a running Event Service.

  const char* service_name;
// The name with which to locate the Event Service.

 private:
  int init_ORB (int argc, char *argv[]);
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
