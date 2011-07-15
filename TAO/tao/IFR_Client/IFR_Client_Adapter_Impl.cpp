// $Id$

#include "tao/IFR_Client/IFR_Client_Adapter_Impl.h"
#include "tao/IFR_Client/IFR_BasicC.h"

#include "tao/AnyTypeCode/NVList.h"
#include "tao/AnyTypeCode/Any_Unknown_IDL_Type.h"

#include "tao/ORB_Core.h"
#include "tao/ORB.h"
#include "tao/Invocation_Adapter.h"
#include "tao/Stub.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_IFR_Client_Adapter_Impl::~TAO_IFR_Client_Adapter_Impl (void)
{
}

CORBA::Boolean
TAO_IFR_Client_Adapter_Impl::interfacedef_cdr_insert (
    TAO_OutputCDR &cdr,
    CORBA::InterfaceDef_ptr object_type)
{
  return cdr << object_type;
}

void
TAO_IFR_Client_Adapter_Impl::interfacedef_any_insert (
    CORBA::Any *any,
    CORBA::InterfaceDef_ptr object_type)
{
  (*any) <<= object_type;
}

void
TAO_IFR_Client_Adapter_Impl::dispose (CORBA::InterfaceDef_ptr orphan)
{
  ::CORBA::release (orphan);
}

CORBA::InterfaceDef_ptr
TAO_IFR_Client_Adapter_Impl::get_interface (
    CORBA::ORB_ptr orb,
    const char *repo_id)
{
  CORBA::Object_var obj =
    orb->resolve_initial_references ("InterfaceRepository");

  if (CORBA::is_nil (obj.in ()))
    {
      throw ::CORBA::INTF_REPOS ();
    }

  CORBA::Repository_var repo =
    CORBA::Repository::_narrow (obj.in ()
                               );

  if (CORBA::is_nil (repo.in ()))
    {
      throw ::CORBA::INTF_REPOS ();
    }

  CORBA::Contained_var result = repo->lookup_id (repo_id);

  if (CORBA::is_nil (result.in ()))
    {
      return CORBA::InterfaceDef::_nil ();
    }
  else
    {
      return CORBA::InterfaceDef::_narrow (result.in ());
    }
}

CORBA::InterfaceDef_ptr
TAO_IFR_Client_Adapter_Impl::get_interface_remote (
    CORBA::Object_ptr target)
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
      0);

  try
    {
      _tao_call.invoke (0, 0);
    }
  catch (const ::CORBA::OBJECT_NOT_EXIST&)
    {
      return CORBA::InterfaceDef::_nil ();
    }

  return _tao_retval.retn ();
}

#if (TAO_HAS_MINIMUM_CORBA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
void
TAO_IFR_Client_Adapter_Impl::create_operation_list (
    CORBA::ORB_ptr orb,
    CORBA::OperationDef_ptr opDef,
    CORBA::NVList_ptr &result)
{
  // Create an empty NVList.
  orb->create_list (0, result);

  // Get the parameters (if any) from the OperationDef, and for each
  // parameter add a corresponding entry to the result.
  CORBA::ParDescriptionSeq_var params = opDef->params ();

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
          throw ::CORBA::INTERNAL();
        }

      // Add an argument to the NVList.
      result->add_value (params[i].name.in (), value, flags);
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


ACE_STATIC_SVC_DEFINE (
    TAO_IFR_Client_Adapter_Impl,
    ACE_TEXT ("Concrete_IFR_Client_Adapter"),
    ACE_SVC_OBJ_T,
    &ACE_SVC_NAME (TAO_IFR_Client_Adapter_Impl),
    ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
    0
  )

ACE_FACTORY_DEFINE (TAO_IFR_Client, TAO_IFR_Client_Adapter_Impl)
TAO_END_VERSIONED_NAMESPACE_DECL
