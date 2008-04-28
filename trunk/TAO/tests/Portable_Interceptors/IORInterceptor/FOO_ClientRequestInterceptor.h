// -*- C++ -*-

//=============================================================================
/**
 * @file FOO_ClientRequestInterceptor.h
 *
 * $Id$
 *
 * Implementation header for the "FOO" client request test
 * interceptor.
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef FOO_CLIENT_REQUEST_INTERCEPTOR_H
#define FOO_CLIENT_REQUEST_INTERCEPTOR_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PI/PI.h"
#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
 * @class FOO_ClientRequestInterceptor
 *
 * @brief Simple concrete client request interceptor.
 *
 * This is a test client request interceptor that extracts a tagged
 * component that should have been embedded in the server IOR by an
 * IOR interceptor.  It displays the contents of that tagged
 * component.
 */
class FOO_ClientRequestInterceptor
  : public virtual PortableInterceptor::ClientRequestInterceptor,
    public virtual ::CORBA::LocalObject
{
public:

  /// Constructor
  FOO_ClientRequestInterceptor (IOP::Codec_ptr codec);

  /**
   * @name Methods Required by the Client Request Interceptor
   * Interface
   *
   * These are methods that must be implemented since they are pure
   * virtual in the abstract base class.  They are the canonical
   * methods required for all client request interceptors.
   */
  //@{
  /// Return the name of this ClientRequestInterceptor.
  virtual char * name (void);

  virtual void destroy (void);

  virtual void send_request (
      PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void send_poll (
      PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void receive_reply (
      PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void receive_exception (
      PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void receive_other (
      PortableInterceptor::ClientRequestInfo_ptr ri);
  //@}

private:

  /// The CDR encapsulation coder/decoder used to retrieve data
  /// from the CDR encapsulation in the tagged component.
  IOP::Codec_var codec_;

};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif  /* FOO_CLIENT_REQUEST_INTERCEPTOR_H */
