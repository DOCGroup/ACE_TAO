/**
 * @file RTServer_Setup.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#ifndef TAO_PERF_RTEC_RTSERVER_SETUP_H
#define TAO_PERF_RTEC_RTSERVER_SETUP_H
#include "ace/pre.h"

#include "RTClient_Setup.h"
#include "RTPOA_Setup.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class RTServer_Setup
 *
 * @brief Simplify the initialization of performance tests.
 *
 */
class TAO_RTEC_Perf_Export RTServer_Setup : public RTClient_Setup
{
public:
  /// Constructor
  RTServer_Setup (int use_rt_corba,
                  CORBA::ORB_ptr orb,
                  const RT_Class &rt_class,
                  int nthreads
                  ACE_ENV_ARG_DECL);

  /// Return the POA configured by the RTPOA_Setup
  PortableServer::POA_ptr poa (void);

private:
  PortableServer::POA_var poa_;

  auto_ptr<RTPOA_Setup> rtpoa_setup_;
};

#if defined(__ACE_INLINE__)
#include "RTServer_Setup.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_PERF_RTEC_RTSERVER_SETUP_H */
