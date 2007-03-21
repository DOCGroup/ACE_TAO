//=============================================================================
/**
*  @file   Forwarder.cpp
*
*  $Id$
*
*  @brief  Definition of ImR_Forwarder
*
*  @author Darrell Brunsch <brunsch@cs.wustl.edu>
*  @author Priyanka Gontla <pgontla@doc.ece.uci.edu>
*/
//=============================================================================

#include "Forwarder.h"
#include "ImR_Locator_i.h"

#include "tao/ORB.h"
#include "tao/Object_KeyC.h"
#include "tao/ORB_Constants.h"

#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/POA_Current_Impl.h"
#include "tao/PortableServer/POA_Current.h"

/**
* This constructor takes in orb and ImR_Locator_i pointers to store for later
* use.  It also grabs a reference to the POACurrent object for use in
* preinvoke.
*/
ImR_Forwarder::ImR_Forwarder (ImR_Locator_i& imr_impl)
  : locator_ (imr_impl)
{
}

void
ImR_Forwarder::init (CORBA::ORB_ptr orb)
{
  ACE_ASSERT (! CORBA::is_nil(orb));
  this->orb_ = orb;
  try
    {
      CORBA::Object_var tmp =
        orb->resolve_initial_references ("POACurrent");

      this->poa_current_var_ =
        PortableServer::Current::_narrow (tmp.in ());
    }
  catch (const CORBA::Exception&)
    {
      ACE_DEBUG ((LM_DEBUG, "ImR_Forwarder::init() Exception ignored.\n"));
    }
  ACE_ASSERT (!CORBA::is_nil (this->poa_current_var_.in ()));
}

/**
* We figure out the intended recipient from the POA name.  After activating
* the server, we throw a forwarding exception to the correct server.
*
* The big complicated thing here is that we have to create the forwarding
* ior based on what we already have.  So we combine the endpoint received
* from activate_server_i and append the objectid from the request to it.
*/
PortableServer::Servant
ImR_Forwarder::preinvoke (const PortableServer::ObjectId &,
                          PortableServer::POA_ptr poa,
                          const char *,
                          PortableServer::ServantLocator::Cookie &)
{
  ACE_ASSERT (! CORBA::is_nil(poa));
  CORBA::Object_var forward_obj;

  try
    {
      CORBA::String_var server_name = poa->the_name();

      if (locator_.debug() > 1)
        ACE_DEBUG ((LM_DEBUG, "ImR: Activating server <%s>.\n", server_name.in()));

      // The activator stores a partial ior with each server. We can
      // just tack on the current ObjectKey to get a valid ior for
      // the desired server.
      CORBA::String_var pior = locator_.activate_server_by_name (server_name.in (), false);

      ACE_CString ior = pior.in ();

      // Check that the returned ior is the expected partial ior with
      // missing ObjectKey.
      if (ior.find ("corbaloc:") != 0 || ior[ior.length () - 1] != '/')
        {
          ACE_ERROR ((LM_ERROR, "ImR_Forwarder::preinvoke () Invalid corbaloc ior.\n\t<%s>\n", ior.c_str()));
          throw CORBA::OBJECT_NOT_EXIST (
            CORBA::SystemException::_tao_minor_code (
              TAO_IMPLREPO_MINOR_CODE,
              0),
            CORBA::COMPLETED_NO);
        }

      CORBA::String_var key_str;
      // Unlike POA Current, this implementation cannot be cached.
      TAO::Portable_Server::POA_Current* tao_current =
        dynamic_cast <TAO::Portable_Server::POA_Current*> (this->poa_current_var_.in ());

      ACE_ASSERT(tao_current != 0);
      TAO::Portable_Server::POA_Current_Impl* impl = tao_current->implementation ();
      TAO::ObjectKey::encode_sequence_to_string (key_str.out (), impl->object_key ());

      ior += key_str.in();

      if (locator_.debug() > 0)
        ACE_DEBUG ((LM_DEBUG, "ImR: Forwarding invocation on <%s> to <%s>\n", server_name.in(), ior.c_str()));

      forward_obj =
        this->orb_->string_to_object (ior.c_str ());
    }
  catch (const ImplementationRepository::CannotActivate&)
    {
      throw CORBA::TRANSIENT (
        CORBA::SystemException::_tao_minor_code (
          TAO_IMPLREPO_MINOR_CODE,
          0),
        CORBA::COMPLETED_NO);
    }
  catch (const ImplementationRepository::NotFound&)
    {
      throw CORBA::TRANSIENT (
        CORBA::SystemException::_tao_minor_code (
          TAO_IMPLREPO_MINOR_CODE,
          0),
        CORBA::COMPLETED_NO);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Forwarder");
      throw CORBA::TRANSIENT (
        CORBA::SystemException::_tao_minor_code (
          TAO_IMPLREPO_MINOR_CODE,
          0),
        CORBA::COMPLETED_NO);
    }

  if (!CORBA::is_nil (forward_obj.in ()))
    throw PortableServer::ForwardRequest (forward_obj.in ());

  ACE_ERROR ((LM_ERROR, "Error: Forward_to reference is nil.\n"));
  throw CORBA::OBJECT_NOT_EXIST (
    CORBA::SystemException::_tao_minor_code (
      TAO_IMPLREPO_MINOR_CODE,
      0),
    CORBA::COMPLETED_NO);
}

void
ImR_Forwarder::postinvoke (const PortableServer::ObjectId &,
                           PortableServer::POA_ptr,
                           const char *,
                           PortableServer::ServantLocator::Cookie,
                           PortableServer::Servant)
{
}

