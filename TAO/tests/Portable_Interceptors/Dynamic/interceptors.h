// $Id$

#ifndef TAO_INTERCEPTORS_H
#define TAO_INTERCEPTORS_H

#include "tao/corba.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

// Forward Declarations

class PortableInterceptor::ClientRequestInfo;
class PortableInterceptor::ServerRequestInfo;

class Echo_Client_Request_Interceptor
:  public PortableInterceptor::ClientRequestInterceptor//,
   //  public CORBA::LocalObject
{
  // = Client-side echo interceptor.  For checking interceptor visually only.
public:
  Echo_Client_Request_Interceptor (CORBA::ORB_ptr orb);
  // ctor.
  virtual ~Echo_Client_Request_Interceptor ();
  // dtor.

  virtual void _add_ref (void);
  // Increment the reference count.

  virtual void _remove_ref (void);
  // Decrement the reference count.

  virtual char * name (CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  // Canonical name of the interceptor.
  
  virtual void send_request (PortableInterceptor::ClientRequestInfo_ptr ri,
                             CORBA::Environment &ACE_TRY_ENVV = 
                             TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri,
                              CORBA::Environment &ACE_TRY_ENV = 
                              TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri,
                                  CORBA::Environment &ACE_TRY_ENV = 
                                  TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException, 
                    PortableInterceptor::ForwardRequest));

private:
  const char *myname_;

  CORBA::ORB_var orb_;
  
};

class Echo_Server_Request_Interceptor
: public PortableInterceptor::ServerRequestInterceptor//,
//    public CORBA::LocalObject
{
  // = Server-side echo interceptor.  For checking interceptor visually only.
public:
  Echo_Server_Request_Interceptor (CORBA::ORB_ptr orb);
  // cotr.
  ~Echo_Server_Request_Interceptor ();
  // dotr.

  virtual void _add_ref (void);
  // Increment the reference count.

  virtual void _remove_ref (void);
  // Decrement the reference count.

  virtual char * name (CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Canonical name of the interceptor.
  
  virtual void receive_request (PortableInterceptor::ServerRequestInfo_ptr ri,
                                CORBA::Environment &ACE_TRY_ENV = 
                                TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void send_reply (PortableInterceptor::ServerRequestInfo_ptr ri,
                           CORBA::Environment &ACE_TRY_ENV = 
                           TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void send_exception (PortableInterceptor::ServerRequestInfo_ptr ri,
                                CORBA::Environment &ACE_TRY_ENV = 
                                TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

private:
  const char *myname_;

  CORBA::ORB_var orb_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */
#endif /* TAO_INTERCEPTORS_H */
