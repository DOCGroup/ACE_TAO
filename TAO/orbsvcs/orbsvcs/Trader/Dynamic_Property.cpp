// $Id$

#include "Dynamic_Property.h"

TAO_Dynamic_Property::TAO_Dynamic_Property(const char* name)
{  
}

TAO_Dynamic_Property::~TAO_Dynamic_Property (void)
{
}

CosTradingDynamic::DynamicProp*
TAO_Dynamic_Property::register_handler(const char* name,
				       CORBA::TypeCode_ptr returned_type,
				       const CORBA::Any& extra_info,
				       TAO_DP_Evaluation_Handler* handler)
{
  string prop_name(name);
  CosTradingDynamic::DynamicProp* dp_struct = 0;
  TAO_Dynamic_Property::HANDLER_MAP::iterator handlers_iter =
    this->handlers_.find(prop_name);

  // Set up the handler to receive evaluations for prop_name
  if (handlers_iter == this->handlers_.end())
    this->handlers_[prop_name] = handler;

  // Create the dp_struct for this dynamic property.
  dp_struct = new CosTradingDynamic::DynamicProp;
  if (dp_struct != 0)
    {
      CORBA::Environment env;
      
      dp_struct->eval_if = this->_this (env);
      TAO_CHECK_ENV_RETURN (env, dp_struct);      
      dp_struct->returned_type =
	CORBA::TypeCode::_duplicate (returned_type);
      dp_struct->extra_info = extra_info;
    }
  
  return dp_struct;
}

TAO_DP_Evaluation_Handler*
TAO_Dynamic_Property::remove_handler(const char* name)
{
  string prop_name(name);
  TAO_DP_Evaluation_Handler* handler = 0;
  HANDLER_MAP::iterator handlers_iter = this->handlers_.find(prop_name);

  if (handlers_iter != this->handlers_.end())
    {
      handler = (*handlers_iter).second;
      this->handlers_.erase(handlers_iter);
    }

  return handler;
}

CORBA::Any*
TAO_Dynamic_Property::evalDP(const char* name,
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
      TAO_DP_Evaluation_Handler* handler = (*handlers_iter).second;
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
