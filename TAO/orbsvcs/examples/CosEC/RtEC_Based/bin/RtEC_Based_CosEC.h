/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   RtEC_Based_CosEC.h
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 *
 * The COS Event Channel service.
 */
//=============================================================================


#ifndef RTEC_BASED_COSEC_H
#define RTEC_BASED_COSEC_H

#include "CosEvent_Utilities.h"
#include "orbsvcs/CosNamingC.h"

#if defined (_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
 * @class RtEC_Based_CosEC
 *
 * @brief RtEC_Based_CosEC
 *
 * Implementation of the COS Event Service.
 */
class RtEC_Based_CosEC : public CosEC_ServantBase
{
 public:
  /// Constructor.
  RtEC_Based_CosEC ();

  /// Destructor.
  virtual ~RtEC_Based_CosEC ();

  /// Parses the command line arguments.
  int parse_args (int argc, ACE_TCHAR *argv []);

  /// Initializes the COS Event Service.
  /// Returns 0 on success, -1 on error.
  void startup (int argc, ACE_TCHAR *argv[]);

  /// run the COS Event Service.
  /// Returns 0 on success, -1 on error.
  int run ();

  /// Shutdown the COS Event Service.
  /// Returns 0 on success, -1 on error.
  void shutdown ();

protected:
  // = Methods from CosEC_ServantBase
  /// Create a local rtec.
  virtual POA_RtecEventChannelAdmin::EventChannel_ptr
  create_rtec ();

  /// Activates the rtec.
  virtual void activate_rtec ();

  /// Deactivates the rtec.
  virtual void deactivate_rtec ();

  /// initialize the ORB.
  void init_ORB (int& argc, ACE_TCHAR *argv []);

  /// Resolve the naming service.
  void resolve_naming_service ();

  /// Locate a rtec.
  void locate_rtec ();

  // = Data members
  /// The name we use to register with the Naming Service.
  const ACE_TCHAR *service_name;

  /// The name of the Real Time Event Service.
  const ACE_TCHAR *rt_service_name;

  /// The ORB that we use.
  CORBA::ORB_var orb_;

  /// Reference to the root poa.
  PortableServer::POA_var poa_;

  /// A naming context.
  CosNaming::NamingContext_var naming_;

  /// Flag to indicate if the RtEC is local/remote,
  /// 0 => local, 1 => remote, default is local.
  CORBA::Boolean remote_rtec_;
};

#if defined (_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* RTEC_BASED_COSEC_H */
