// $Id$

#include "interceptors.h"

ACE_RCSID(Interceptors, interceptors, "$Id$")

const CORBA::ULong request_ctx_id = 0xdead;
const CORBA::ULong reply_ctx_id = 0xbeef;
const char *request_msg = "The Echo_Request_Interceptor request message";
const char *reply_msg = "The Echo_Request_Interceptor reply message";

Echo_Client_Request_Interceptor::Echo_Client_Request_Interceptor (CORBA::ORB_ptr orb)
  : myname_ ("Echo_Client_Interceptor"),
    orb_ (CORBA::ORB::_duplicate (orb))
{
}

char *
Echo_Client_Request_Interceptor::name (CORBA::Environment &)
{
  return CORBA::string_dup (this->myname_);
}

void
Echo_Client_Request_Interceptor::preinvoke (CORBA::ULong rid,
                                            CORBA::Boolean,
                                            CORBA::Object_ptr objref,
                                            const char *op,
                                            IOP::ServiceContextList &sc,
                                            CORBA::NVList_ptr &,
                                            PortableInterceptor::Cookies &,
                                            CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::String_var ior = this->orb_->object_to_string (objref,
                                                        ACE_TRY_ENV);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "Echo_Client_Request_Interceptor::preinvoke from \"%s\" request_id: %d on object: %s\n",
              op,
              rid,
              ior));

  CORBA::ULong length = sc.length ();
  sc.length (length + 1);

  sc[length].context_id = request_ctx_id;

  CORBA::ULong string_len = ACE_OS::strlen (request_msg) + 1;
  CORBA::Octet *buf = 0;
  ACE_NEW (buf,
           CORBA::Octet [string_len]);

  ACE_OS::strcpy (ACE_reinterpret_cast (char *, buf), request_msg);
  sc[length].context_data.replace (string_len, string_len, buf, 1);
}

void
Echo_Client_Request_Interceptor::postinvoke (CORBA::ULong rid,
                                             CORBA::Boolean,
                                             CORBA::Object_ptr objref,
                                             const char * op,
                                             IOP::ServiceContextList &sc,
                                             CORBA::NVList_ptr &,
                                             PortableInterceptor::Cookies &,
                                             CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::String_var ior = this->orb_->object_to_string (objref,
                                                        ACE_TRY_ENV);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "Echo_Client_Request_Interceptor::postinvoke from \"%s\" request_id: %d on object: %s\n",
              op,
              rid,
              ior));

  for (CORBA::ULong size = 0; size < sc.length (); ++size)
    if (sc[size].context_id == reply_ctx_id)
      {
        const char *buf = ACE_reinterpret_cast (const char *, sc[size].context_data.get_buffer ());
        ACE_DEBUG ((LM_DEBUG,
                    "  Received service context: %s\n",
                    buf));
      }
}

void
Echo_Client_Request_Interceptor::exception_occurred (CORBA::ULong rid,
                                                     CORBA::Boolean,
                                                     CORBA::Object_ptr objref,
                                                     const char * op,
                                                     PortableInterceptor::Cookies &,
                                                     CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::String_var ior = this->orb_->object_to_string (objref,
                                                        ACE_TRY_ENV);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "Echo_Client_Request_Interceptor::exception_occurred from \"%s\" request_id: %d on object: %s\n",
              op,
              rid,
              ior));
}

Echo_Server_Request_Interceptor::Echo_Server_Request_Interceptor (CORBA::ORB_ptr orb)
  : myname_ ("Echo_Server_Interceptor"),
    orb_ (CORBA::ORB::_duplicate (orb))
{
}

char *
Echo_Server_Request_Interceptor::name (CORBA::Environment &)
{
  return CORBA::string_dup (this->myname_);
}

void
Echo_Server_Request_Interceptor::preinvoke (CORBA::ULong rid,
                                            CORBA::Boolean,
                                            CORBA::Object_ptr objref,
                                            const char *op,
                                            IOP::ServiceContextList &sc,
                                            CORBA::NVList_ptr &,
                                            PortableInterceptor::Cookies &,
                                            CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::String_var ior = this->orb_->object_to_string (objref,
                                                        ACE_TRY_ENV);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "Echo_Server_Request_Interceptor::preinvoke from \"%s\" request_id: %d on object: %s\n",
              op,
              rid,
              ior));

  for (CORBA::ULong size = 0; size < sc.length (); ++size)
    if (sc[size].context_id == request_ctx_id)
      {
        const char *buf = ACE_reinterpret_cast (const char *, sc[size].context_data.get_buffer ());
        ACE_DEBUG ((LM_DEBUG,
                    "  Received service context: %s\n",
                    buf));
      }
}

void
Echo_Server_Request_Interceptor::postinvoke (CORBA::ULong rid,
                                             CORBA::Boolean,
                                             CORBA::Object_ptr objref,
                                             const char * op,
                                             IOP::ServiceContextList &sc,
                                             CORBA::NVList_ptr &,
                                             PortableInterceptor::Cookies &,
                                             CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::String_var ior = this->orb_->object_to_string (objref,
                                                        ACE_TRY_ENV);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "Echo_Server_Request_Interceptor::postinvoke from \"%s\" request_id: %d on object: %s\n",
              op,
              rid,
              ior));

  CORBA::ULong length = sc.length ();
  sc.length (length + 1);

  sc[length].context_id = reply_ctx_id;

  CORBA::ULong string_len = ACE_OS::strlen (reply_msg) + 1;
  CORBA::Octet *buf = 0;
  ACE_NEW (buf,
           CORBA::Octet [string_len]);

  ACE_OS::strcpy (ACE_reinterpret_cast (char *, buf), reply_msg);
  sc[length].context_data.replace (string_len, string_len, buf, 1);
}

void
Echo_Server_Request_Interceptor::exception_occurred (CORBA::ULong rid,
                                                     CORBA::Boolean,
                                                     CORBA::Object_ptr objref,
                                                     const char * op,
                                                     PortableInterceptor::Cookies &,
                                                     CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::String_var ior = this->orb_->object_to_string (objref,
                                                        ACE_TRY_ENV);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "Echo_Server_Request_Interceptor::exception_occurred from \"%s\" request_id: %d on object: %s\n",
              op,
              rid,
              ior));
}
