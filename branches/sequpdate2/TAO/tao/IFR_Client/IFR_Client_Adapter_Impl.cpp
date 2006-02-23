// $Id$

#include "IFR_Client_Adapter_Impl.h"
#include "IFR_ExtendedC.h"

#include "tao/AnyTypeCode/NVList.h"
#include "tao/AnyTypeCode/Any_Unknown_IDL_Type.h"

#include "tao/ORB_Core.h"
#include "tao/ORB.h"
#include "tao/Invocation_Adapter.h"
#include "tao/Stub.h"

ACE_RCSID (IFR_Client,
           IFR_Client_Adapter_Impl,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  template<>
  class TAO_IFR_Client_Export Arg_Traits<CORBA::InterfaceDef>
    : public
        Object_Arg_Traits_T<
            CORBA::InterfaceDef_ptr,
            CORBA::InterfaceDef_var,
            CORBA::InterfaceDef_out,
            TAO::Objref_Traits<CORBA::InterfaceDef>,
            TAO::Any_Insert_Policy_Stream <CORBA::InterfaceDef_ptr>
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
    CORBA::Any *any,
    CORBA::InterfaceDef_ptr object_type
  )
{
  (*any) <<= object_type;
}

void
TAO_IFR_Client_Adapter_Impl::dispose (
    CORBA::InterfaceDef_ptr orphan
  )
{
  ::CORBA::release (orphan);
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

#if (TAO_HAS_MINIMUM_CORBA == 0)
void
TAO_IFR_Client_Adapter_Impl::create_operation_list (
    CORBA::ORB_ptr orb,
    CORBA::OperationDef_ptr opDef,
    CORBA::NVList_ptr &result
    ACE_ENV_ARG_DECL
  )
{
  // Create an empty NVList.
  orb->create_list (0,
                    result
                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Get the parameters (if any) from the OperationDef, and for each
  // parameter add a corresponding entry to the result.
  CORBA::ParDescriptionSeq_var params =
    opDef->params (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::ULong paramCount = params->length ();

  for (CORBA::ULong i = 0; i < paramCount; ++i)
    {
      CORBA::Any value;
      TAO::Unknown_IDL_Type *unk = 0;
      ACE_NEW (unk,
               TAO::Unknown_IDL_Type (params[i].type.in ()));
      value.replace (unk);

      // Convert the parameter mode to an arg mode
      CORBA::Flags flags = 0;
      switch(params[i].mode)
        {
        case CORBA::PARAM_IN:
          flags = CORBA::ARG_IN;
          break;
        case CORBA::PARAM_OUT:
          flags = CORBA::ARG_OUT;
          break;
        case CORBA::PARAM_INOUT:
          flags = CORBA::ARG_INOUT;
          break;
        default:
          // Shouldn't happen
          ACE_THROW (CORBA::INTERNAL());
        }

      // Add an argument to the NVList.
      result->add_value (params[i].name.in (),
                         value,
                         flags);
   }
}

#endif /*TAO_HAS_MINIMUM_CORBA*/
// *********************************************************************

// Initialization and registration of dynamic service object.

int
TAO_IFR_Client_Adapter_Impl::Initializer (void)
{
  TAO_ORB_Core::ifr_client_adapter_name ("Concrete_IFR_Client_Adapter");

  return ACE_Service_Config::process_directive (ace_svc_desc_TAO_IFR_Client_Adapter_Impl);
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DEFINE (
    TAO_IFR_Client_Adapter_Impl,
    ACE_TEXT ("Concrete_IFR_Client_Adapter"),
    ACE_SVC_OBJ_T,
    &ACE_SVC_NAME (TAO_IFR_Client_Adapter_Impl),
    ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
    0
  )

ACE_FACTORY_DEFINE (TAO_IFR_Client, TAO_IFR_Client_Adapter_Impl)
