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
#include "tao/PortableServer/Object_Adapter.h"

/**
 * This constructor takes in orb and ImR_Locator_i pointers to store for later
 * use.  It also grabs a reference to the POACurrent object for use in
 * preinvoke.
 */
ImR_Forwarder::ImR_Forwarder (ImR_Locator_i *imr_impl,
                              CORBA::ORB_ptr orb)
  : imr_impl_ (imr_impl),
    orb_ (orb)
{
  ACE_ASSERT (imr_impl != 0);

  ACE_TRY_NEW_ENV
    {
      CORBA::Object_var tmp =
        orb->resolve_initial_references ("POACurrent" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->poa_current_var_ =
        PortableServer::Current::_narrow (tmp.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // Ignore
    }
  ACE_ENDTRY;

  ACE_ASSERT (!CORBA::is_nil (this->poa_current_var_.in ()));
}


/**
 * We figure out the intended recipient from the POA name.  After activating
 * the server, we throw a forwarding exception to the correct server.
 *
 * The big complicated thing here is that we have to create the forwarding
 * ior based on what we already have.  So we combine the endpoint received
 * from activate_server_i and append the objectid from the request to it.
 *
 * @todo Should we base the name on the entire POA hierarchy?
 * @todo Use ACE_TString for the IOR?  Can it always be a char?
 */
PortableServer::Servant
ImR_Forwarder::preinvoke (const PortableServer::ObjectId &,
                          PortableServer::POA_ptr poa,
                          const char *,
                          PortableServer::ServantLocator::Cookie &
                          ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException, PortableServer::ForwardRequest))
{
  ACE_TString ior;
  CORBA::Object_var forward_obj;

  ACE_TRY
    {
      // Get the POA name.
      CORBA::String_var poa_name = poa->the_name();

      // Activate the server.
      ior =
        this->imr_impl_->activate_server_with_startup (poa_name.in (),
                                                       1
                                                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Add the key
      char *key_str = 0;

      // Unlike POA Current, this implementation cannot be cached.
      TAO_POA_Current *tao_current =
        ACE_dynamic_cast (TAO_POA_Current*, this->poa_current_var_.in ());
      TAO_POA_Current_Impl *impl = tao_current->implementation ();
      TAO::ObjectKey::encode_sequence_to_string (key_str,
                                                 impl->object_key ());

      // Append the key_string to the IOR that is received from the
      // activate_server_with_startup function call.
      ior += key_str;
      CORBA::string_free (key_str);

      // Get the object corresponding to the string.
      forward_obj =
        this->orb_->string_to_object (ior.c_str () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      ACE_PRINT_EXCEPTION (sysex, "Forwarder system exception");
      ACE_RE_THROW;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Forwarder exception");
      ACE_TRY_THROW (CORBA::TRANSIENT (
          CORBA::SystemException::_tao_minor_code (TAO_IMPLREPO_MINOR_CODE, 0),
          CORBA::COMPLETED_NO));
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);

  if (!CORBA::is_nil (forward_obj.in ()))
    ACE_THROW_RETURN (PortableServer::ForwardRequest (forward_obj.in ()), 0);
  else
    ACE_ERROR ((LM_ERROR,
                "Error: Forward_to reference is nil.\n"));

  ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (), 0);
}

/**
 * The postinvoke method just deletes the passed in servant.
 */
void
ImR_Forwarder::postinvoke (const PortableServer::ObjectId &,
                           PortableServer::POA_ptr ,
                           const char * ,
                           PortableServer::ServantLocator::Cookie ,
                           PortableServer::Servant servant
                           ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  delete servant;
  delete imr_impl_;
}
