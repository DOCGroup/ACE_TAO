/**
 * @file RTClient_Setup.h
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#ifndef TAO_PERF_RTEC_RTCLIENT_SETUP_H
#define TAO_PERF_RTEC_RTCLIENT_SETUP_H
#include /**/ "ace/pre.h"

#include "RTCORBA_Setup.h"
#include "PriorityBand_Setup.h"
#include "SyncScope_Setup.h"
#include <memory>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class RT_Class;

/**
 * @class RTClient_Setup
 *
 * @brief Simplify the initialization of performance tests.
 */
class TAO_RTEC_Perf_Export RTClient_Setup
{
public:
  /// Constructor
  RTClient_Setup (int use_rt_corba,
                  CORBA::ORB_ptr orb,
                  const RT_Class &rt_class,
                  int nthreads);

  /// Return non-zero if RTCORBA is enabled
  int use_rt_corba () const;

  /// Return the underlying RTCORBA_Setup pointer
  RTCORBA_Setup *rtcorba_setup ();

private:
  int use_rt_corba_;

  std::unique_ptr<RTCORBA_Setup> rtcorba_setup_;
  SyncScope_Setup syncscope_setup_;
};

#if defined(__ACE_INLINE__)
#include "RTClient_Setup.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_PERF_RTEC_RTCLIENT_SETUP_H */
