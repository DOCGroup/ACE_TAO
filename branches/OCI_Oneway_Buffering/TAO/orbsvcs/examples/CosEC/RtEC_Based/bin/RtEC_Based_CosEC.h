/* -*- C++ -*- */
// $Id$
// ============================================================================
//
// = FILENAME
//   RtEC_Based_CosEC.h
//
// = AUTHOR
//   Pradeep Gore <pradeep@cs.wustl.edu>
//
// = DESCRIPTION
//   The COS Event Channel service.
//
// ============================================================================

#ifndef RTEC_BASED_COSEC_H
#define RTEC_BASED_COSEC_H

#include "CosEvent_Utilities.h"
#include "orbsvcs/CosNamingC.h"

#if defined (_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class RtEC_Based_CosEC : public CosEC_ServantBase
{
  // = TITLE
  //   RtEC_Based_CosEC
  //
  // = DESCRIPTION
  //   Implementation of the COS Event Service.

 public:
  // = Initialization and termination methods.
  RtEC_Based_CosEC (void);
  // Constructor.

  virtual ~RtEC_Based_CosEC (void);
  // Destructor.

  int parse_args (int argc, char *argv []);
  // Parses the command line arguments.

  void startup (int argc, char *argv[]
               ACE_ENV_ARG_DECL);
  // Initializes the COS Event Service.
  // Returns 0 on success, -1 on error.

  int run (void);
  // run the COS Event Service.
  // Returns 0 on success, -1 on error.

  void shutdown (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
  // Shutdown the COS Event Service.
  // Returns 0 on success, -1 on error.

protected:
  // = Methods from CosEC_ServantBase
  virtual POA_RtecEventChannelAdmin::EventChannel_ptr
  create_rtec (ACE_ENV_SINGLE_ARG_DECL);
  // Create a local rtec.

  virtual void activate_rtec (ACE_ENV_SINGLE_ARG_DECL);
  // Activates the rtec.

  virtual void deactivate_rtec (ACE_ENV_SINGLE_ARG_DECL);
  // Deactivates the rtec.

  void init_ORB (int& argc, char *argv []
                 ACE_ENV_ARG_DECL);
  // initialize the ORB.

  void resolve_naming_service (ACE_ENV_SINGLE_ARG_DECL);
  // Resolve the naming service.

  void locate_rtec (ACE_ENV_SINGLE_ARG_DECL);
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

  CORBA::Boolean remote_rtec_;
  // Flag to indicate if the RtEC is local/remote,
  // 0 => local, 1 => remote, default is local.
};

#if defined (_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* RTEC_BASED_COSEC_H */
