// $Id$

#include "IFR_Client_Adapter_Impl.h"
#include "IFR_ExtendedC.h"
#include "tao/ORB_Core.h"
#include "tao/Invocation.h"
#include "tao/Stub.h"

ACE_RCSID (IFR_Client,
           IFR_Client_Adapter_Impl,
           "$Id$")

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
    const CORBA::Object_ptr target
    ACE_ENV_ARG_DECL
  )
{
  CORBA::InterfaceDef_ptr _tao_retval = CORBA::InterfaceDef::_nil ();
  CORBA::InterfaceDef_var _tao_safe_retval (_tao_retval);

  ACE_TRY
    {
      // Must catch exceptions, if the server raises a
      // CORBA::OBJECT_NOT_EXIST then we must return 1, instead of
      // propagating the exception.
      TAO_Stub *istub = target->_stubobj ();

      if (istub == 0)
        {
          ACE_THROW_RETURN (CORBA::INTERNAL (
                              CORBA::SystemException::_tao_minor_code (
                                TAO_DEFAULT_MINOR_CODE,
                                EINVAL),
                              CORBA::COMPLETED_NO),
                            _tao_retval);
        }

      TAO_GIOP_Twoway_Invocation _tao_call (istub,
                                            "_interface",
                                            10,
                                            1,
                                            istub->orb_core ());

      for (;;)
        {
          _tao_call.start (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          CORBA::Short flag = TAO_TWOWAY_RESPONSE_FLAG;

          _tao_call.prepare_header (ACE_static_cast (CORBA::Octet, flag)
                                    ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          int _invoke_status =
            _tao_call.invoke (0, 0 ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (_invoke_status == TAO_INVOKE_RESTART)
            {
              _tao_call.restart_flag (1);
              continue;
            }

          ACE_ASSERT (_invoke_status != TAO_INVOKE_EXCEPTION);

          if (_invoke_status != TAO_INVOKE_OK)
            {
              ACE_THROW_RETURN (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE,
                                                CORBA::COMPLETED_YES),
                                CORBA::InterfaceDef::_nil ());
            }

          break;
        }
      TAO_InputCDR &_tao_in = _tao_call.inp_stream ();

      if (!(
            (_tao_in >> _tao_safe_retval.inout ())
            ))
        {
          ACE_THROW_RETURN (CORBA::MARSHAL (), 
                            CORBA::InterfaceDef::_nil ());
        }
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

  return _tao_safe_retval._retn ();
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

