// $Id$

#include "tao/corba.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class Echo_Client_Request_Interceptor
  :  public virtual PortableInterceptor::ClientRequestInterceptor,
     public virtual CORBA::LocalObject
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

  virtual char * name (CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());
  // Canonical name of the interceptor.

  virtual void preinvoke (CORBA::ULong request_id,
                          CORBA::Boolean response_expected,
                          CORBA::Object_ptr objref,
                          const char * operation_name,
                          IOP::ServiceContextList & sc,
                          CORBA::NVList_ptr &args,
                          PortableInterceptor::Cookies & ck,
                          CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());

  virtual void postinvoke (CORBA::ULong request_id,
                           CORBA::Boolean response_expected,
                           CORBA::Object_ptr objref,
                           const char * operation_name,
                           IOP::ServiceContextList & sc,
                           CORBA::NVList_ptr &args,
                           PortableInterceptor::Cookies & ck,
                           CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());

  virtual void exception_occurred (CORBA::ULong request_id,
                                   CORBA::Boolean response_expected,
                                   CORBA::Object_ptr objref,
                                   const char * operation_name,
                                   PortableInterceptor::Cookies & ck,
                                   CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());

private:
  const char *myname_;

  CORBA::ORB_var orb_;
};

class Echo_Server_Request_Interceptor
  : public virtual PortableInterceptor::ServerRequestInterceptor,
    public virtual CORBA::LocalObject
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

  virtual char * name (CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());
  // Canonical name of the interceptor.

  virtual void preinvoke (CORBA::ULong request_id,
                          CORBA::Boolean response_expected,
                          CORBA::Object_ptr objref,
                          const char * operation_name,
                          IOP::ServiceContextList & sc,
                          CORBA::NVList_ptr &args,
                          PortableInterceptor::Cookies & ck,
                          CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());

  virtual void postinvoke (CORBA::ULong request_id,
                           CORBA::Boolean response_expected,
                           CORBA::Object_ptr objref,
                           const char * operation_name,
                           IOP::ServiceContextList & sc,
                           CORBA::NVList_ptr &args,
                           PortableInterceptor::Cookies & ck,
                           CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());

  virtual void exception_occurred (CORBA::ULong request_id,
                                   CORBA::Boolean response_expected,
                                   CORBA::Object_ptr objref,
                                   const char * operation_name,
                                   PortableInterceptor::Cookies & ck,
                                   CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());

private:
  const char *myname_;

  CORBA::ORB_var orb_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */
