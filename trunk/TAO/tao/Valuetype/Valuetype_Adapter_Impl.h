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

#include "valuetype_export.h"

#include "tao/Valuetype_Adapter.h"

/**
 * @class TAO_Valuetype_Adapter_Impl
 *
 * @brief TAO_Valuetype_Adapter_Impl.
 *
 * Class that adapts various functions involving the Valuetype,
 * ValueFactory and AbstractInterface classes. This is a concrete class
 * implementating the pure virtual methods of TAO_Valuetype_Adapter
 */
class TAO_Valuetype_Export TAO_Valuetype_Adapter_Impl
  : public TAO_Valuetype_Adapter
{
public:

  virtual ~TAO_Valuetype_Adapter_Impl (void);

  virtual CORBA::Object_ptr abstractbase_to_object (
      CORBA::AbstractBase_ptr
    );

  virtual CORBA::Boolean stream_to_value (TAO_InputCDR &,
                                          CORBA::ValueBase *&);

  virtual CORBA::Boolean stream_to_abstract_base (
      TAO_InputCDR &,
      CORBA::AbstractBase_ptr &
    );

  virtual CORBA::ULong type_info_single (void) const;

  virtual int vf_map_rebind (const char *,
                             CORBA::ValueFactory &);

  virtual int vf_map_unbind (const char *);

  virtual CORBA::ValueFactory vf_map_find (const char *);

  // Used to force the initialization of the ORB code.
  static int Initializer (void);
};

static int
TAO_Requires_Valuetype_Initializer =
  TAO_Valuetype_Adapter_Impl::Initializer ();

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE (TAO_Valuetype_Adapter_Impl)
ACE_FACTORY_DECLARE (TAO_Valuetype, TAO_Valuetype_Adapter_Impl)

#include /**/ "ace/post.h"
#endif /* TAO_VALUETYPE_ADAPTER_IMPL_H */
