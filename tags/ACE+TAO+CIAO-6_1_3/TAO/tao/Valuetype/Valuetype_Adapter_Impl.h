// -*- C++ -*-

//=============================================================================
/**
 *  @file    Valuetype_Adapter_Impl.h
 *
 *  $Id$
 *
 *  @author  Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================


#ifndef TAO_VALUETYPE_ADAPTER_IMPL_H
#define TAO_VALUETYPE_ADAPTER_IMPL_H

#include /**/ "ace/pre.h"

#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Valuetype/valuetype_export.h"
#include "tao/Valuetype/ValueFactory_Map.h"
#include "tao/Valuetype_Adapter.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Valuetype_Adapter_Impl
 *
 * @brief TAO_Valuetype_Adapter_Impl.
 *
 * Class that adapts various functions involving the Valuetype,
 * ValueFactory and AbstractInterface classes. This is a concrete class
 * implementing the pure virtual methods of TAO_Valuetype_Adapter
 */
class TAO_Valuetype_Export TAO_Valuetype_Adapter_Impl
  : public TAO_Valuetype_Adapter
{
public:

  virtual ~TAO_Valuetype_Adapter_Impl (void);

  virtual CORBA::Object_ptr abstractbase_to_object (
      CORBA::AbstractBase_ptr);

  virtual CORBA::Boolean stream_to_value (TAO_InputCDR &, CORBA::ValueBase *&);

  virtual CORBA::Boolean stream_to_abstract_base (
      TAO_InputCDR &,
      CORBA::AbstractBase_ptr &);

  virtual CORBA::Long type_info_single (void) const;

  virtual CORBA::Boolean is_type_info_implied (CORBA::Long) const;
  virtual CORBA::Boolean is_type_info_single (CORBA::Long) const;
  virtual CORBA::Boolean is_type_info_list (CORBA::Long) const;
  virtual CORBA::Boolean is_value_chunked (CORBA::Long) const;

  virtual int vf_map_rebind (const char *, CORBA::ValueFactory &);

  virtual int vf_map_unbind (const char *);

  virtual CORBA::ValueFactory vf_map_find (const char *);

  virtual CORBA::TypeCode_ptr derived_type (CORBA::ValueBase *);

private:
  TAO_ValueFactory_Map map_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_VALUETYPE_ADAPTER_IMPL_H */
