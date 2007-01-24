// $Id$

#include "orbsvcs/Naming/Naming_Client.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/Log_Msg.h"

ACE_RCSID (Naming,
           Naming_Client,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CosNaming::NamingContext_ptr
TAO_Naming_Client::operator -> (void) const
{
  return this->naming_context_.ptr ();
}

CosNaming::NamingContext_ptr
TAO_Naming_Client::get_context (void) const
{
  // Must duplicate this pointer so it can be stored in a _var.
  return CosNaming::NamingContext::_duplicate (this->naming_context_.in ());
}

int
TAO_Naming_Client::init (CORBA::ORB_ptr orb,
                         ACE_Time_Value *timeout)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA::Object_var naming_obj =
        orb->resolve_initial_references ("NameService", timeout ACE_ENV_ARG_PARAMETER);

      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the NameService.\n"),
                          -1);
      this->naming_context_ =
        CosNaming::NamingContext::_narrow (naming_obj.in ()
                                           ACE_ENV_ARG_PARAMETER);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_Naming_Client::init");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

TAO_Naming_Client::TAO_Naming_Client (void)
{
  // Do nothing
}

TAO_Naming_Client::~TAO_Naming_Client (void)
{
  // Do nothing
}

TAO_END_VERSIONED_NAMESPACE_DECL
