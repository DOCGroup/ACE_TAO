// -*- C++ -*-

//=============================================================================
/**
 * @file Request_Interceptor.h
 *
 * $Id$
 *
 * Implementation header for the request interceptor base class for
 * the request interceptor flow test.
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef REQUEST_INTERCEPTOR_H
#define REQUEST_INTERCEPTOR_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Atomic_Op.h"

#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"
#include "tao/CORBA_String.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
 * @class Request_Interceptor
 *
 * @brief Abstract base class for request interceptors used in this
 * test.
 */
class Request_Interceptor
  : public virtual PortableInterceptor::Interceptor
{
public:

  /// Constructor.
  Request_Interceptor (const char *name);

  /**
   * @name Methods Required by the Client Request Interceptor
   * Interface
   *
   * These are methods that must be implemented since they are pure
   * virtual in the abstract base class.  They are the canonical
   * methods required for all client request interceptors.
   */
  //@{
  /// Return the name of this Requestinterceptor.
  virtual char * name (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void destroy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));
  //@}

protected:

  /// The name of this interceptor.
  CORBA::String_var name_;

  /// The number of times a starting interception point has been
  /// called.
  ACE_Atomic_Op<TAO_SYNCH_MUTEX, CORBA::ULong>
    starting_interception_point_count_;

  /// The number of times an ending interception point has been
  /// called.
  ACE_Atomic_Op<TAO_SYNCH_MUTEX, CORBA::ULong>
    ending_interception_point_count_;

};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif  /* REQUEST_INTERCEPTOR_H */
