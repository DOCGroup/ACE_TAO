/**
 * @file ORB_Shutdown.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */
#ifndef TAO_PERF_RTEC_ORB_SHUTDOWN_H
#define TAO_PERF_RTEC_ORB_SHUTDOWN_H
#include /**/ "ace/pre.h"

#include "rtec_perf_export.h"
#include "tao/ORB.h"

/**
 * @class ORB_Shutdown
 *
 * @brief Helper functor to call the shutdown() operation in the ORB.
 */
class ORB_Shutdown
{
public:
  void operator() (CORBA::ORB_ptr orb
                   ACE_ENV_ARG_DECL);
};

#if defined(__ACE_INLINE__)
#include "ORB_Shutdown.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_PERF_RTEC_ORB_SHUTDOWN_H */
