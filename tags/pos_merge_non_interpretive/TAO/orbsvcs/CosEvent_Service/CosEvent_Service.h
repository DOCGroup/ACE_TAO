/* -*- C++ -*- */
// $Id$
// ============================================================================
//
// = FILENAME
//   CosEvent_Service.h
//
// = AUTHOR
//   Pradeep Gore <pradeep@cs.wustl.edu>
//
// = DESCRIPTION
//   The COS Event Channel service.
//
// ============================================================================

#ifndef COSEVENT_SERVICE_H
#define COSEVENT_SERVICE_H

#include "orbsvcs/CosEvent_Utilities.h"
#include "orbsvcs/CosNamingC.h"

#if defined (_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class CosEvent_Service : public CosEC_ServantBase
{
  // = TITLE
  //   CosEvent_Service
  //
  // = DESCRIPTION
  //   Implementation of the COS Event Service.

 public:
  // = Initialization and termination methods.
  CosEvent_Service (void);
  // Constructor.

  virtual ~CosEvent_Service (void);
  // Destructor.

  int parse_args (int argc, char *argv []);
  // Parses the command line arguments.

  void startup (int argc, char *argv[],
               CORBA::Environment &ACE_TRY_ENV);
  // Initializes the COS Event Service.
  // Returns 0 on success, -1 on error.

  int run (void);
  // run the COS Event Service.
  // Returns 0 on success, -1 on error.

  void shutdown (CORBA::Environment &ACE_TRY_ENV =
                     TAO_default_environment ());
  // Shutdown the COS Event Service.
  // Returns 0 on success, -1 on error.

protected:
  // = Methods from CosEC_ServantBase
  virtual POA_RtecEventChannelAdmin::EventChannel_ptr
  create_rtec (CORBA::Environment &ACE_TRY_ENV);
  // Create a local rtec.

  virtual void activate_rtec (CORBA::Environment &ACE_TRY_ENV);
  // Activates the rtec.

  virtual void deactivate_rtec (CORBA::Environment &ACE_TRY_ENV);
  // Deactivates the rtec.

  void init_ORB (int& argc, char *argv [],
                 CORBA::Environment &ACE_TRY_ENV);
  // initialize the ORB.

  void resolve_naming_service (CORBA::Environment &ACE_TRY_ENV);
  // Resolve the naming service.

  void locate_rtec (CORBA::Environment &ACE_TRY_ENV);
  // Locate a rtec.

  // = Data members
  const char* service_name;
  // The name we use to register with the Naming Service.

  const char* rt_service_name;
  // The name of the Real Time Event Service.

  CORBA::ORB_var orb_;
  // The ORB that we use.

  PortableServer::POA_var poa_;
  // Reference to the root poa.

  CosNaming::NamingContext_var naming_;
  // A naming context.

  CORBA::Boolean remote_Rtec_;
  // Flag to indicate if the RtEC is local/remote,
  // 0 => local, 1 => remote, default is local.
};

#if defined (_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* COSEVENT_SERVICE_H */
