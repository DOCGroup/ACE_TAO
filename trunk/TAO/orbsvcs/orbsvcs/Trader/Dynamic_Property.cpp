// $Id$

#include "Dynamic_Property.h"

TAO_DP_Dispatcher::TAO_DP_Dispatcher(const char* name)
{  
}

TAO_DP_Dispatcher::~TAO_DP_Dispatcher (void)
{
  for (Handler_Map::iterator handler_iter (this->handlers_);
       ! handler_iter.done ();
       handler_iter++)
    {
      Handler_Info& handler_info = (*handler_iter).int_id_;

      if (handler_info.free_on_delete_)
	delete handler_info.handle_;	  
    }
}

int
TAO_DP_Dispatcher::
register_handler(const char* name,
		 TAO_DP_Evaluation_Handler* handler,
		 CORBA::Boolean release_on_delete)
{
  TAO_String_Hash_Key prop_name(name);

  // Set up the handler to receive evaluations for prop_name
  Handler_Info handler_info;  
  handler_info.handle_ = handler;
  handler_info.free_on_delete_ = release_on_delete;

  return (this->handlers_.bind (prop_name, handler_info) != 0) ? -1 : 0;
}

CosTradingDynamic::DynamicProp*
TAO_DP_Dispatcher::
construct_dynamic_prop (const char* name,
			CORBA::TypeCode_ptr returned_type,
			const CORBA::Any& extra_info)
{
  CosTradingDynamic::DynamicProp* dp_struct = 0;

  ACE_NEW_RETURN (dp_struct, CosTradingDynamic::DynamicProp, 0);

  TAO_TRY
    {
      CosTradingDynamic::DynamicPropEval* dp_eval = this->_this (TAO_TRY_ENV);

#if defined TAO_HAS_OBJECT_IN_STRUCT_MARSHAL_BUG
      CORBA::ORB_ptr orb = TAO_ORB_Core_instance ()-> orb ();
      dp_struct->eval_if = orb->object_to_string (dp_eval, TAO_TRY_ENV);
#else
      dp_struct->eval_if = dp_eval;
#endif /* TAO_HAS_DYNAMIC_PROPERTY_BUG */

      TAO_CHECK_ENV;
      
      dp_struct->returned_type =
	CORBA::TypeCode::_duplicate (returned_type);
      dp_struct->extra_info = extra_info;
    }
  TAO_CATCHANY
    {
      return 0;
    }
  TAO_ENDTRY;

  return dp_struct;
}


TAO_DP_Evaluation_Handler*
TAO_DP_Dispatcher::remove_handler (const char* name)
{
  Handler_Info handler_info;
  TAO_String_Hash_Key prop_name (name);
  TAO_DP_Evaluation_Handler* handler = 0;

  if (this->handlers_.unbind (prop_name, handler_info) == 0)
    handler = handler_info.handle_;

  return handler;
}

CORBA::Any*
TAO_DP_Dispatcher::evalDP(const char* name,
			  CORBA::TypeCode_ptr returned_type,
			  const CORBA::Any& extra_info,
			  CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException, 
		   CosTradingDynamic::DPEvalFailure))
{
  CORBA::Any* result = 0;
  Handler_Info handler_info;
  TAO_String_Hash_Key prop_name(name);
  
  if (this->handlers_.find (prop_name, handler_info) == 0)
    {
      TAO_DP_Evaluation_Handler* handler = handler_info.handle_;
      
      result = handler->evalDP (extra_info, returned_type, _env);      
      TAO_CHECK_ENV_RETURN (_env, result);

      
      if (result == 0)
	{
	  TAO_THROW_RETURN (CosTradingDynamic::DPEvalFailure
			    (name, returned_type, extra_info),
			    result);
	}
      
      if (! returned_type->equal (result->type (), _env))
	{
	  TAO_THROW_RETURN (CosTradingDynamic::DPEvalFailure
			    (name, returned_type, extra_info),
			    result);
	}
    }
  else
    {
      TAO_THROW_RETURN (CosTradingDynamic::DPEvalFailure
			(name, returned_type, extra_info),
			result);
    }

  return result;
}
