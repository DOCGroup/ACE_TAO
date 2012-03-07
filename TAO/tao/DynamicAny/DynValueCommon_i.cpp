// -*- C++ -*-
// $Id$

#include "tao/DynamicAny/DynValueCommon_i.h"
//#include "tao/DynamicAny/DynAnyUtils_T.h"

//#include "tao/AnyTypeCode/Marshal.h"
#include "tao/AnyTypeCode/Any_Unknown_IDL_Type.h"
//#include "tao/AnyTypeCode/AnyTypeCode_methods.h"
//#include "tao/CDR.h"
//#include "tao/Valuetype/ValueBase.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_DynValueCommon_i::TAO_DynValueCommon_i (CORBA::Boolean allow_truncation)
  : TAO_DynCommon (allow_truncation)
  , TAO_DynAny_i (allow_truncation)
  , is_null_ (true)
{
}

TAO_DynValueCommon_i::~TAO_DynValueCommon_i (void)
{
}

CORBA::Boolean
TAO_DynValueCommon_i::is_null (void)
{
  return this->is_null_;
}

void
TAO_DynValueCommon_i::set_to_null (void)
{
  this->is_null_        = true;
  this->component_count_  = 0u;
  this->current_position_ = -1;
}

void
TAO_DynValueCommon_i::init_common (void)
{
  this->ref_to_component_ = false;
  this->container_is_destroying_ = false;
  this->has_components_ = true;
  this->destroyed_ = false;
  this->set_to_value ();
}

TAO_DynValueCommon_i *
TAO_DynValueCommon_i::_narrow (CORBA::Object_ptr _tao_objref)
{
  return (CORBA::is_nil (_tao_objref)) ?
         0 :
         dynamic_cast<TAO_DynValueCommon_i *> (_tao_objref);
}

TAO_END_VERSIONED_NAMESPACE_DECL
