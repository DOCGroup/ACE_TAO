// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file     ValueFactory.h
 *
 *  $Id$
 *
 *  @author  Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
 */
//=============================================================================


#ifndef TAO_VALUEFACTORY_H
#define TAO_VALUEFACTORY_H
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Pseudo_Value_VarOut_T.h"

#include "ace/Synch_T.h"

namespace CORBA
{
  class ValueFactoryBase;

  TAO_NAMESPACE_INLINE_FUNCTION void add_ref (ValueFactoryBase *);
  TAO_NAMESPACE_INLINE_FUNCTION void remove_ref (ValueFactoryBase *);

  typedef TAO_Pseudo_Value_Var_T<ValueFactoryBase> ValueFactoryBase_var;

  typedef TAO_Pseudo_Value_Out_T<ValueFactoryBase, ValueFactoryBase_var>
    ValueFactoryBase_out;

  typedef ValueFactoryBase_var ValueFactory_var;

  class TAO_Export ValueFactoryBase
  {
  public:
    ValueFactoryBase ();
    virtual ~ValueFactoryBase ();

    static ValueFactoryBase * _nil (void);

    // non-virtual is non-standard
    void _add_ref (void);
    void _remove_ref (void);

    // private: %!
    /// In a derived class T use return type TAO_OBV_CREATE_RETURN_TYPE (T)
    /// (see at definition below)
    virtual CORBA::ValueBase * create_for_unmarshal (void) = 0;

    // Not pure virtual because this will be overridden only by valuetypes
    // that support an abstract interface.
    virtual CORBA::AbstractBase * create_for_unmarshal_abstract (void);

  private:
    CORBA::ULong _tao_reference_count_;
    TAO_SYNCH_MUTEX _tao_reference_count_lock_;
  }; // CORBA_ValueFactoryBase
}  // End CORBA namespace

// Use this macro for writing code that is independend from
// the compiler support of covariant return types of pointers to
// virtual inherited classes.
// (e.g. in egcs-2.90.29 980515 (egcs-1.0.3 release) its not yet implemented)
// (But it is TAO specific and at the moment the covariant case is not
// elaborated --- its just a suggestion.)

#ifdef TAO_HAS_OBV_COVARIANT_RETURN
#  define TAO_OBV_CREATE_RETURN_TYPE(TYPE) TYPE *
#else /*  TAO_HAS_OBV_COVARIANT_RETURN */
#  define TAO_OBV_CREATE_RETURN_TYPE(TYPE) CORBA::ValueBase *
#endif /* TAO_HAS_OBV_COVARIANT_RETURN */

// (The obtaining of the repository id is currently not yet like the OMG way. %!)
//
// Macro for on the fly registration of a factory (with type Factory).
// The repository id is taken from the static repository id of the
// valuetype via tao_repository_id () of the specialized factory.
// It forgets the pre-registered factory (if any) and the reference
// to the newly created one. (A new reference could be obtained with
// orb->lookup_value_factory (char * repo_id) .)

#define TAO_OBV_REGISTER_FACTORY(FACTORY, VALUETYPE) \
  { CORBA::ValueFactory factory = new FACTORY; \
    CORBA::ValueFactory prev_factory = \
    TAO_ORB_Core_instance ()->orb ()->register_value_factory (\
      VALUETYPE::_tao_obv_static_repository_id (),\
                                   factory); \
    if (prev_factory) prev_factory->_remove_ref (); \
    factory->_remove_ref (); }


#if defined (__ACE_INLINE__)
# include "tao/ValueFactory.i"
#endif /* __ACE_INLINE__) */

#include "ace/post.h"

#endif /* TAO_VALUEFACTORY_H */
