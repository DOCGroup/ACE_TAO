// $Id$

#include "Dynamic_Property.h"

TAO_DP_Dispatcher::TAO_DP_Dispatcher(const char* name)
{  
}

TAO_DP_Dispatcher::~TAO_DP_Dispatcher (void)
{
  for (HANDLER_MAP::iterator handler_iter = this->handlers_.begin ();
       handler_iter != this->handlers_.end ();
       handler_iter++)
    {
      CORBA::Boolean delete_me = (*handler_iter).second.second;

      if (delete_me)
	delete (*handler_iter).second.first;
    }
}

void
TAO_DP_Dispatcher::
register_handler(const char* name,
		 TAO_DP_Evaluation_Handler* handler,
		 CORBA::Boolean release_on_delete)
{
  string prop_name(name);
  TAO_DP_Dispatcher::HANDLER_MAP::iterator handlers_iter =
    this->handlers_.find(prop_name);

  // Set up the handler to receive evaluations for prop_name
  if (handlers_iter == this->handlers_.end())
    this->handlers_[prop_name] = make_pair (handler, release_on_delete);
}

#if defined TAO_HAS_DYNAMIC_PROPERTY_BUG
CosTradingDynamic::DynamicProp*
TAO_DP_Dispatcher::
construct_dynamic_prop (const char* name,
			CORBA::TypeCode_ptr returned_type,
			const CORBA::Any& extra_info,
			CORBA::ORB_ptr orb)
#else
CosTradingDynamic::DynamicProp*
TAO_DP_Dispatcher::
construct_dynamic_prop (const char* name,
			CORBA::TypeCode_ptr returned_type,
			const CORBA::Any& extra_info)
#endif /* TAO_HAS_DYNAMIC_PROPERTY_BUG */
{
  CosTradingDynamic::DynamicProp* dp_struct = 0;

  ACE_NEW_RETURN (dp_struct, CosTradingDynamic::DynamicProp, 0);

  TAO_TRY
    {
      CosTradingDynamic::DynamicPropEval* dp_eval = this->_this (TAO_TRY_ENV);

#if defined TAO_HAS_DYNAMIC_PROPERTY_BUG
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
TAO_DP_Dispatcher::remove_handler(const char* name)
{
  string prop_name(name);
  TAO_DP_Evaluation_Handler* handler = 0;
  HANDLER_MAP::iterator handlers_iter = this->handlers_.find(prop_name);

  if (handlers_iter != this->handlers_.end())
    {
      handler = (*handlers_iter).second.first;
      this->handlers_.erase(handlers_iter);
    }

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
  string prop_name(name);
  CORBA::Any* result = 0;
  HANDLER_MAP::iterator handlers_iter = this->handlers_.find(prop_name);

  if (handlers_iter != this->handlers_.end())
    {
      TAO_DP_Evaluation_Handler* handler = (*handlers_iter).second.first;
      result = handler->evalDP (extra_info, returned_type, _env);
      TAO_CHECK_ENV_RETURN (_env, result);
      
      if (! returned_type->equal(result->type(), _env))
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
