// $Id$

#include "interceptors.h"

ACE_RCSID(Benchmark, interceptors, "$Id$")

const CORBA::ULong request_ctx_id = 0xdead;
const CORBA::ULong reply_ctx_id = 0xbeef;

Vault_Client_Request_Interceptor::Vault_Client_Request_Interceptor (CORBA::ORB_ptr orb)
  : myname_ ("Vault_Client_Interceptor"),
    orb_ (CORBA::ORB::_duplicate (orb))
{
}

Vault_Client_Request_Interceptor::~Vault_Client_Request_Interceptor ()
{
}

void
Vault_Client_Request_Interceptor::_add_ref (void)
{
  this->_incr_refcnt ();
  // This is a TAO specific implementation.
}

void
Vault_Client_Request_Interceptor::_remove_ref (void)
{
  this->_decr_refcnt ();
  // This is a TAO specific implementation.
}

char *
Vault_Client_Request_Interceptor::name (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->myname_);
}
 
void 
Vault_Client_Request_Interceptor::send_request (PortableInterceptor::ClientRequestInfo_ptr ri,
                                               CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest))
{
  if (ACE_OS::strcmp (ri->operation (), "authenticate") == 0)
    {
      // MAke the context to send the context to the target
      IOP::ServiceContext sc;
      sc.context_id = request_ctx_id;

      const char passwd [20] = "root123";
      CORBA::ULong string_len = ACE_OS::strlen (passwd) + 1;
      CORBA::Octet *buf = 0;
      ACE_NEW (buf,
               CORBA::Octet [string_len]);
      ACE_OS::strcpy (ACE_reinterpret_cast (char *, buf), passwd);
  
      sc.context_data.replace (string_len, string_len, buf, 1);

      // Add this context to the service context list.
      ri->add_request_service_context (sc, 0);
    }
  
  if (ACE_OS::strcmp (ri->operation (), "update_records") == 0)
    {
      Dynamic::ParameterList paramlist = *(ri->arguments ());
      Test_Interceptors::Secure_Vault::Record *record;
      CORBA::Long id;
      paramlist[0].argument >>= id;
      paramlist[1].argument >>= record;
    }
  
}

void 
Vault_Client_Request_Interceptor::receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri,
                                                CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (ACE_OS::strcmp (ri->operation (), "update_records") == 0)
    {
      CORBA::Long result;
      CORBA_Any result_any = *(ri->result ());
      result_any >>= result;
    }

}

void 
Vault_Client_Request_Interceptor::receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri,
                                                    CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException, 
                     PortableInterceptor::ForwardRequest))
{
  // As of now, there is no way to extract an exception from an Any in TAO.
  CORBA::Exception *e = ri->_received_exception ();

  ACE_ASSERT (ACE_OS::strcmp (ri->received_exception_id (), e->_id ())== 0);

}


Vault_Server_Request_Interceptor::Vault_Server_Request_Interceptor (CORBA::ORB_ptr orb)
  : myname_ ("Vault_Server_Interceptor"),
    orb_ (CORBA::ORB::_duplicate (orb))
{
}

Vault_Server_Request_Interceptor::~Vault_Server_Request_Interceptor ()
{
}

void
Vault_Server_Request_Interceptor::_add_ref (void)
{
  this->_incr_refcnt ();
  // This is a TAO specific implementation.
}

void
Vault_Server_Request_Interceptor::_remove_ref (void)
{
  this->_decr_refcnt ();
  // This is a TAO specific implementation.
}

char *
Vault_Server_Request_Interceptor::name (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->myname_);
}
  
void 
Vault_Server_Request_Interceptor::receive_request (PortableInterceptor::ServerRequestInfo_ptr ri,
                                                  CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  if (ACE_OS::strcmp (ri->operation (), "authenticate") == 0)
    {

     IOP::ServiceId id = request_ctx_id;
     IOP::ServiceContext *sc = ri->get_request_service_context (id);
      
     if (sc == 0)
       ACE_THROW (CORBA::NO_MEMORY ());
     
     const char *buf = ACE_reinterpret_cast (const char *, sc->context_data.get_buffer ());
     if (ACE_OS::strcmp (buf, "root123") !=0)
       ACE_THROW (Test_Interceptors::Invalid ());     
    }
  
  if (ACE_OS::strcmp (ri->operation (), "update_records") == 0)
    {
      Dynamic::ParameterList paramlist = *(ri->arguments ());
      Test_Interceptors::Secure_Vault::Record *record;
      CORBA::Long id;
      paramlist[0].argument >>= id;
      paramlist[1].argument >>= record;
    }

   
}

void 
Vault_Server_Request_Interceptor::send_reply (PortableInterceptor::ServerRequestInfo_ptr ri,
                                             CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  if (ACE_OS::strcmp (ri->operation (), "update_records") == 0)
    {
      CORBA::Long result;
      CORBA_Any result_any = *(ri->result ());
      result_any >>= result;
    }
}

void 
Vault_Server_Request_Interceptor::send_exception (PortableInterceptor::ServerRequestInfo_ptr ri,
                                                 CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  CORBA::Exception *e = ri->_sending_exception ();
  
}
