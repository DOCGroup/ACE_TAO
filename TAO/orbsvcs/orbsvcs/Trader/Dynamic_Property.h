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

#if defined (OS_NO_NAMESPACE)
#define queue foobar
#endif /* OS_NO_NAMESPACE */

#include "orbsvcs/CosTradingS.h"

#if defined (OS_NO_NAMESPACE)
#undef queue
#endif /* OS_NO_NAMESPACE */


class TAO_DP_Evaluation_Handler
{
public:

  virtual CORBA::Any* evalDP(const CORBA::Any& extra_info,
			     CORBA::Environment& _env)
    TAO_THROW_SPEC ((CosTradingDynamic::DPEvalFailure)) = 0;
  
};

class TAO_Dynamic_Property :
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

  TAO_Dynamic_Property(const char* name = "Dynamic Property");
  
  CosTradingDynamic::DynamicProp*
    register_handler(const char* name,
		     CORBA::TypeCode_ptr returned_type,
		     const CORBA::Any& extra_info,
		     TAO_DP_Evaluation_Handler* handler);
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
  
private:

  typedef map
    <
    string,
    TAO_DP_Evaluation_Handler*,
    less<string>
    > HANDLER_MAP;

  HANDLER_MAP handlers_;

};

#endif /* TAO_DYNAMIC_PROPERTY_H*/
