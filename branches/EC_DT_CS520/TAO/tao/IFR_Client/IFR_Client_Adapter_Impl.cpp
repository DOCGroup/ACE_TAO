// $Id$

#include "IFR_Client_Adapter_Impl.h"
#include "IFR_ExtendedC.h"
#include "tao/ORB_Core.h"
#include "tao/Invocation_Adapter.h"
#include "tao/Stub.h"

ACE_RCSID (IFR_Client,
           IFR_Client_Adapter_Impl,
           "$Id$")

namespace TAO
{
  ACE_TEMPLATE_SPECIALIZATION
  class TAO_IFR_Client_Export Arg_Traits<CORBA::InterfaceDef>
    : public
        Object_Arg_Traits_T<
            CORBA::InterfaceDef_ptr,
            CORBA::InterfaceDef_var,
            CORBA::InterfaceDef_out,
            TAO::Objref_Traits<CORBA::InterfaceDef>
          >
  {
  };
}

TAO_IFR_Client_Adapter_Impl::~TAO_IFR_Client_Adapter_Impl (void)
{
}

CORBA::Boolean
TAO_IFR_Client_Adapter_Impl::interfacedef_cdr_insert (
    TAO_OutputCDR &cdr,
    CORBA::InterfaceDef_ptr object_type
  )
{
  return cdr << object_type;
}

void
TAO_IFR_Client_Adapter_Impl::interfacedef_any_insert (
    CORBA::Any &any,
    CORBA::InterfaceDef_ptr object_type
  )
{
  any <<= object_type;
}

void
TAO_IFR_Client_Adapter_Impl::dispose (
    CORBA::InterfaceDef_ptr orphan
  )
{
  CORBA::release (orphan);
}

CORBA::InterfaceDef_ptr
TAO_IFR_Client_Adapter_Impl::get_interface (
    CORBA::ORB_ptr orb,
    const char *repo_id
    ACE_ENV_ARG_DECL
  )
{
  CORBA::Object_var obj =
    orb->resolve_initial_references ("InterfaceRepository"
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::InterfaceDef::_nil ());

  if (CORBA::is_nil (obj.in ()))
    {
      ACE_THROW_RETURN (CORBA::INTF_REPOS (),
                        CORBA::InterfaceDef::_nil ());
    }

  CORBA::Repository_var repo =
    CORBA::Repository::_narrow (obj.in ()
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::InterfaceDef::_nil ());

  if (CORBA::is_nil (repo.in ()))
    {
      ACE_THROW_RETURN (CORBA::INTF_REPOS (),
                        CORBA::InterfaceDef::_nil ());
    }

  CORBA::Contained_var result = repo->lookup_id (repo_id
                                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::InterfaceDef::_nil ());

  if (CORBA::is_nil (result.in ()))
    {
      return CORBA::InterfaceDef::_nil ();
    }
  else
    {
      return CORBA::InterfaceDef::_narrow (result.in ()
                                           ACE_ENV_ARG_PARAMETER);
    }
}

CORBA::InterfaceDef_ptr
TAO_IFR_Client_Adapter_Impl::get_interface_remote (
    CORBA::Object_ptr target
    ACE_ENV_ARG_DECL
  )
{
  TAO::Arg_Traits<CORBA::InterfaceDef>::ret_val _tao_retval;

  TAO::Argument *_tao_signature [] =
    {
      &_tao_retval
    };

  TAO::Invocation_Adapter _tao_call (
      target,
      _tao_signature,
      1,
      "_interface",
      10,
      0
    );

  ACE_TRY
    {
      _tao_call.invoke (0, 0 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK (_tao_retval.excp ());
    }
  ACE_CATCH (CORBA::OBJECT_NOT_EXIST, ex)
    {
      return CORBA::InterfaceDef::_nil ();
    }
  ACE_CATCHANY
    {
      ACE_RE_THROW;
    }
  ACE_ENDTRY;

  return _tao_retval.retn ();
}

// *********************************************************************

// Initialization and registration of dynamic service object.

int
TAO_IFR_Client_Adapter_Impl::Initializer (void)
{
  TAO_ORB_Core::ifr_client_adapter_name ("Concrete_IFR_Client_Adapter");

  return ACE_Service_Config::process_directive (ace_svc_desc_TAO_IFR_Client_Adapter_Impl);
}

ACE_STATIC_SVC_DEFINE (
    TAO_IFR_Client_Adapter_Impl,
    ACE_TEXT ("Concrete_IFR_Client_Adapter"),
    ACE_SVC_OBJ_T,
    &ACE_SVC_NAME (TAO_IFR_Client_Adapter_Impl),
    ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
    0
  )

ACE_FACTORY_DEFINE (TAO_IFR_Client, TAO_IFR_Client_Adapter_Impl)
