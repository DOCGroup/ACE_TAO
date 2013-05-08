// $Id$

#include "orbsvcs/Naming/Naming_Client.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Log_Macros.h"

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
TAO_Naming_Client::init (CORBA::ORB_ptr orb, ACE_Time_Value *timeout)
{
  try
    {
      CORBA::Object_var naming_obj =
        orb->resolve_initial_references ("NameService", timeout);

      if (CORBA::is_nil (naming_obj.in ()))
        ORBSVCS_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the NameService.\n"),
                          -1);
      this->naming_context_ =
        CosNaming::NamingContext::_narrow (naming_obj.in ());
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("TAO_Naming_Client::init");
      return -1;
    }

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
