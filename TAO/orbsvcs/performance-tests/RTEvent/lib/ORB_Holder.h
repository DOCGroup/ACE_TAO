/**
 * @file ORB_Holder.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#ifndef TAO_PERF_RTEC_ORB_HOLDER_H
#define TAO_PERF_RTEC_ORB_HOLDER_H

#include "tao/ORB.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ORB_Holder
 *
 * @brief Implement a helper class to initialize and destroy an ORB.
 *
 */
class ORB_Holder
{
public:
  /// Constructor
  ORB_Holder (int &argc, char *argv[],
              const char *orb_id = 0
              ACE_ENV_ARG_DECL);

  /// Destructor
  /**
   * @todo This method could benefit from the error logging described
   * in Servant_var.cpp
   */
  ~ORB_Holder (void);

  /// Access the underlying ORB, using the canonical CORBA memory
  /// management model
  CORBA::ORB_ptr orb (void);

  /// Implicit conversion to CORBA::ORB_ptr
  /**
   * It behaves as the .in() methods in the T_var classes.
   */
  operator CORBA::ORB_ptr () const;

  /// Smart pointer behavior
  CORBA::ORB_ptr operator-> () const;

private:
  /// The ORB
  CORBA::ORB_var orb_;
};

#if defined(__ACE_INLINE__)
#include "ORB_Holder.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_PERF_RTEC_ORB_HOLDER_H */
