/* -*- C++ -*- */

// ========================================================================
// $Id$
// 
// = LIBRARY 
//    orbsvcs
//  
// = FILENAME
//    Dynamic_Property.h
// 
// = AUTHOR 
//    Seth Widoff <sbw1@cs.wustl.edu> 
//   
// ========================================================================


#ifndef TAO_DYNAMIC_PROPERTY_H
#define TAO_DYNAMIC_PROPERTY_H

#include <map>
#include <string>

#include "orbsvcs/CosTradingS.h"

class TAO_DP_Evaluation_Handler
{
public:

  virtual CORBA::Any* evalDP(const CORBA::Any& extra_info,
			     CORBA::TypeCode_ptr returned_type,
			     CORBA::Environment& _env)
    TAO_THROW_SPEC ((CosTradingDynamic::DPEvalFailure)) = 0;
  
};

class TAO_DP_Dispatcher :
  public POA_CosTradingDynamic::DynamicPropEval
// = TITLE
//   This class exhibits one strategy for handling many dynamic
//   properties from a single process. Dynamic_Property, when
//   registered with the orb, will receive and dispatch to registered
//   handlers, incoming evalDP requests. This pushes the
//   demultiplexing of dynamic property evaluation up from the orb
//   level and into the application. 
{
public:

  TAO_DP_Dispatcher(const char* name = "Dynamic Property");

  ~TAO_DP_Dispatcher (void);
  
  void register_handler (const char* name,
			 TAO_DP_Evaluation_Handler* handler,
			 CORBA::Boolean release_on_delete = CORBA::B_FALSE);
  // Registers a handler with the Dynamic_Property
  // demultiplexer. Returns a constructed dynamic property struct
  // upon success, which the caller must then deallocate when
  // finished, or 0 on failure (i.e., a property with the same name
  // is already registered).
  
  TAO_DP_Evaluation_Handler* remove_handler(const char* name);
  
  virtual CORBA::Any* evalDP(const char* name,
			     CORBA::TypeCode_ptr returned_type,
			     const CORBA::Any& extra_info,
			     CORBA::Environment& _env)
    TAO_THROW_SPEC ((CORBA::SystemException,
		     CosTradingDynamic::DPEvalFailure));
  // Point of demultiplexing.  

  CosTradingDynamic::DynamicProp*
    construct_dynamic_prop (const char* name,			    
			    CORBA::TypeCode_ptr returned_type,
			    const CORBA::Any& extra_info);
  
private:

  typedef map
    <
    string,
    pair <TAO_DP_Evaluation_Handler*, CORBA::Boolean>,
    less<string>
    > HANDLER_MAP;

  HANDLER_MAP handlers_;

};

#endif /* TAO_DYNAMIC_PROPERTY_H*/
