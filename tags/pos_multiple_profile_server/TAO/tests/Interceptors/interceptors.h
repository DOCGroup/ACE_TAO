// $Id$

#include "tao/Interceptor.h"

class Echo_Client_Request_Interceptor : public POA_PortableInterceptor::ClientRequestInterceptor
{
  // = Client-side echo interceptor.  For checking interceptor visually only.
public:
  Echo_Client_Request_Interceptor (void);
  // ctor.

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
};

class Echo_Server_Request_Interceptor : public POA_PortableInterceptor::ServerRequestInterceptor
{
  // = Server-side echo interceptor.  For checking interceptor visually only.
public:
  Echo_Server_Request_Interceptor (void);
  // cotr.

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
};
